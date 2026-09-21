// Run with python3 tests/test-vulkan-device-features.py.
#include <vulkan/vulkan.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#define LOG_FUNC(...) ((void) 0)
#define LOG_INFO(...) ((void) 0)
#define LOG_DEBUG(...) ((void) 0)
#define LOG_WARN(...) ((void) 0)

// These extension names normally come from vulkan_win32.h in the Windows build.
#define VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME "VK_KHR_external_memory_win32"
#define VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME "VK_KHR_external_semaphore_win32"

struct State
{
    bool creatingD3DDevice = false;
    bool vkAntiLagSupported = false;
    static State& Instance()
    {
        static State state;
        return state;
    }
};
struct Config
{
    struct Option
    {
        bool value_or_default() const { return false; }
    } VulkanExtensionSpoofing;
    static Config* Instance()
    {
        static Config config;
        return &config;
    }
};
namespace KernelBaseProxy
{
static auto GetModuleHandleA_()
{
    return +[](const char*) -> void* { return nullptr; };
}
static auto GetProcAddress_()
{
    return +[](void*, const char*) -> void* { return nullptr; };
}
} // namespace KernelBaseProxy
static void* vulkanModule = nullptr;
static std::vector<VkExtensionProperties> driverExtensions;
static bool bdaSupported = true;
static bool descriptorSupported = true;
static bool enumerationFails = false;
static unsigned incompleteReads = 0;
static uint32_t driverApi = VK_API_VERSION_1_3;

VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice, const char*, uint32_t* count,
                                                                    VkExtensionProperties* properties)
{
    if (enumerationFails)
        return VK_ERROR_INITIALIZATION_FAILED;
    if (!properties)
    {
        *count = static_cast<uint32_t>(driverExtensions.size());
        return VK_SUCCESS;
    }
    if (incompleteReads)
    {
        --incompleteReads;
        return VK_INCOMPLETE;
    }
    const auto n = std::min(*count, static_cast<uint32_t>(driverExtensions.size()));
    std::copy_n(driverExtensions.begin(), n, properties);
    *count = n;
    return n < driverExtensions.size() ? VK_INCOMPLETE : VK_SUCCESS;
}
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(VkPhysicalDevice, VkPhysicalDeviceProperties* properties)
{
    properties->apiVersion = driverApi;
}
static PFN_vkEnumerateDeviceExtensionProperties o_vkEnumerateDeviceExtensionProperties =
    vkEnumerateDeviceExtensionProperties;
static PFN_vkGetPhysicalDeviceProperties o_vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;

static void VKAPI_CALL GetFeatures(VkPhysicalDevice, VkPhysicalDeviceFeatures2* features)
{
    features->features.shaderInt16 = VK_TRUE;
    for (auto* node = static_cast<VkBaseOutStructure*>(features->pNext); node; node = node->pNext)
    {
        switch (node->sType)
        {
        case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES:
            reinterpret_cast<VkPhysicalDeviceBufferDeviceAddressFeatures*>(node)->bufferDeviceAddress = bdaSupported;
            break;
        case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT:
            reinterpret_cast<VkPhysicalDeviceDescriptorBufferFeaturesEXT*>(node)->descriptorBuffer =
                descriptorSupported;
            break;
        case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES:
            reinterpret_cast<VkPhysicalDeviceVulkan12Features*>(node)->bufferDeviceAddress = bdaSupported;
            break;
        case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_KHR:
            reinterpret_cast<VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR*>(node)->computeDerivativeGroupLinear =
                VK_TRUE;
            break;
        default:
            break; // All other optional features remain unsupported.
        }
    }
}

#include "device-features-under-test.h"

static void ResetDriver()
{
    driverExtensions = {
        { VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, 2 },
        { VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, 1 },
        { VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, 1 },
        { VK_KHR_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME, 1 },
    };
    bdaSupported = descriptorSupported = true;
    enumerationFails = false;
    incompleteReads = 0;
    driverApi = VK_API_VERSION_1_3;
}

static void CheckNegotiation(const void* chain, uint32_t api, bool expectedUpgrade, bool expectedDescriptor,
                             bool featureQuery = true)
{
    const char* extensions[] = { VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME };
    VkPhysicalDeviceFeatures originalFeatures {};
    originalFeatures.robustBufferAccess = VK_TRUE;
    VkDeviceCreateInfo original { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    original.pNext = chain;
    original.enabledExtensionCount = 1;
    original.ppEnabledExtensionNames = extensions;
    if (!FindFeatureStruct<VkPhysicalDeviceFeatures2>(chain, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2))
        original.pEnabledFeatures = &originalFeatures;
    auto info = original;
    {
        VulkanDeviceFeatureState state(&info, featureQuery ? GetFeatures : nullptr);
        assert(VulkanSpoofing::hkvkCreateDevice({}, &info, nullptr, nullptr, featureQuery ? &state : nullptr, api) ==
               VK_SUCCESS);
        assert(HasExtension(&info, VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) == expectedUpgrade);
        assert(HasExtension(&info, VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) != expectedUpgrade);
        const auto* bda = FindFeatureStruct<VkPhysicalDeviceBufferDeviceAddressFeatures>(
            info.pNext, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES);
        const auto* descriptors = FindFeatureStruct<VkPhysicalDeviceDescriptorBufferFeaturesEXT>(
            info.pNext, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT);
        assert((bda && bda->bufferDeviceAddress) == expectedUpgrade);
        assert((descriptors && descriptors->descriptorBuffer) == expectedDescriptor);
        if (featureQuery && !enumerationFails)
        {
            const auto* derivatives = FindFeatureStruct<VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR>(
                info.pNext, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_KHR);
            assert(derivatives && derivatives->computeDerivativeGroupLinear);
        }
    }
    assert(original.pNext == chain && original.enabledExtensionCount == 1);
    assert(original.ppEnabledExtensionNames == extensions);
    assert(originalFeatures.robustBufferAccess && !originalFeatures.shaderInt16);
}

int main()
{
    ResetDriver();
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, true, true);
    CheckNegotiation(nullptr, VK_API_VERSION_1_0, true, true);
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, false, false, false);
    incompleteReads = 1;
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, true, true);
    assert(incompleteReads == 0);

    // Even an all-false EXT feature struct must preserve the application's EXT route.
    for (VkBool32 enabled : { VK_FALSE, VK_TRUE })
    {
        VkPhysicalDeviceBufferDeviceAddressFeaturesEXT ext {
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT
        };
        ext.bufferDeviceAddress = enabled;
        ext.bufferDeviceAddressCaptureReplay = enabled;
        ext.bufferDeviceAddressMultiDevice = enabled;
        const auto original = ext;
        CheckNegotiation(&ext, VK_API_VERSION_1_1, false, false);
        assert(std::memcmp(&ext, &original, sizeof(ext)) == 0);
    }

    bdaSupported = false;
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, false, false);
    ResetDriver();
    descriptorSupported = false;
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, true, false);
    ResetDriver();
    driverExtensions.erase(driverExtensions.begin() + 1); // No modern BDA extension.
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, false, false);
    ResetDriver();
    enumerationFails = true;
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, false, false);
    ResetDriver();

    // The upstream upgrade intentionally applies only to the pre-1.2 path.
    CheckNegotiation(nullptr, VK_API_VERSION_1_2, false, false);
    driverApi = VK_API_VERSION_1_1;
    CheckNegotiation(nullptr, VK_API_VERSION_1_2, true, true);
    ResetDriver();

    // RDR2's unowned uniform-layout node must survive; changes behind it are restored.
    VkPhysicalDeviceFeatures2 features { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
    features.features.robustBufferAccess = VK_TRUE;
    VkPhysicalDeviceUniformBufferStandardLayoutFeatures uniform {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES
    };
    uniform.uniformBufferStandardLayout = VK_TRUE;
    uniform.pNext = &features;
    const auto originalUniform = uniform;
    const auto originalFeatures = features;
    CheckNegotiation(&uniform, VK_API_VERSION_1_1, true, true);
    assert(std::memcmp(&uniform, &originalUniform, sizeof(uniform)) == 0);
    assert(std::memcmp(&features, &originalFeatures, sizeof(features)) == 0);

    State::Instance().creatingD3DDevice = true;
    CheckNegotiation(nullptr, VK_API_VERSION_1_1, false, false, false);
    std::cout << "Vulkan negotiation tests passed: EXT/KHR guards, API limits, unsupported features, enumeration "
                 "retry/failure, caller restoration, D3D bypass\n";
}
