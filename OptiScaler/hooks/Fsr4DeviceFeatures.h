#pragma once
#include <vulkan/vulkan.h>
#include <algorithm>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// Compatibility with the Vulkan 1.4.310 headers bundled with OptiScaler.
#ifndef VK_VALVE_shader_mixed_float_dot_product
#define VK_VALVE_SHADER_MIXED_FLOAT_DOT_PRODUCT_EXTENSION_NAME "VK_VALVE_shader_mixed_float_dot_product"
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MIXED_FLOAT_DOT_PRODUCT_FEATURES_VALVE                                \
    static_cast<VkStructureType>(1000673000)
struct VkPhysicalDeviceShaderMixedFloatDotProductFeaturesVALVE
{
    VkStructureType sType;
    void* pNext;
    VkBool32 shaderMixedFloatDotProductFloat16AccFloat32;
    VkBool32 shaderMixedFloatDotProductFloat16AccFloat16;
    VkBool32 shaderMixedFloatDotProductBFloat16Acc;
    VkBool32 shaderMixedFloatDotProductFloat8AccFloat32;
};
#endif

namespace fsr4vk
{
// Owns copies: never modifies the application's const pNext chain. Unknown
// structures are rejected, not silently dropped or guessed. Keep alive through
// vkCreateDevice. Only used by the explicitly opted-in experimental path.
class DeviceFeatures
{
    std::vector<std::shared_ptr<void>> nodes;
    std::vector<const char*> extensions;
    VkPhysicalDeviceFeatures core {};
    VkDeviceCreateInfo info {};
    template <class T> T* copy(const T& value)
    {
        auto node = std::make_shared<T>(value);
        auto* ptr = node.get();
        nodes.push_back(std::move(node));
        return ptr;
    }
    VkBaseOutStructure* clone(const VkBaseInStructure* node)
    {
#define FSR4_COPY(type, tag)                                                                                           \
    case tag:                                                                                                          \
        return reinterpret_cast<VkBaseOutStructure*>(copy(*reinterpret_cast<const type*>(node)))
        switch (static_cast<int>(node->sType))
        {
            FSR4_COPY(VkPhysicalDeviceFeatures2, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2);
            FSR4_COPY(VkPhysicalDeviceVulkan11Features, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES);
            FSR4_COPY(VkPhysicalDeviceVulkan12Features, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES);
            FSR4_COPY(VkPhysicalDeviceVulkan13Features, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES);
            FSR4_COPY(VkPhysicalDeviceVulkan14Features, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderFloatControls2FeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT_CONTROLS_2_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceShaderFloat16Int8Features,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES);
            FSR4_COPY(VkPhysicalDevice8BitStorageFeatures, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES);
            FSR4_COPY(VkPhysicalDeviceDescriptorIndexingFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES);
            FSR4_COPY(VkPhysicalDeviceBufferDeviceAddressFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderIntegerDotProductFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES);
            FSR4_COPY(VkPhysicalDeviceSynchronization2Features,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES);
            FSR4_COPY(VkPhysicalDeviceTimelineSemaphoreFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES);
            FSR4_COPY(VkPhysicalDevice16BitStorageFeatures, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES);
            FSR4_COPY(VkPhysicalDeviceMemoryPriorityFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDescriptorBufferFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceShaderMixedFloatDotProductFeaturesVALVE,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MIXED_FLOAT_DOT_PRODUCT_FEATURES_VALVE);
        default:
            throw std::runtime_error("unhandled device pNext sType=" + std::to_string(node->sType));
        }
#undef FSR4_COPY
    }
    template <class T> T* find(VkStructureType tag)
    {
        for (auto* n = static_cast<const VkBaseInStructure*>(info.pNext); n; n = n->pNext)
            if (n->sType == tag)
                return const_cast<T*>(reinterpret_cast<const T*>(n));
        return nullptr;
    }
    template <class T> T* ensure(VkStructureType tag)
    {
        if (auto* found = find<T>(tag))
            return found;
        T value {};
        value.sType = tag;
        auto* node = copy(value);
        node->pNext = const_cast<void*>(info.pNext);
        info.pNext = node;
        return node;
    }

  public:
    DeviceFeatures(VkPhysicalDevice physical, const VkDeviceCreateInfo& source, PFN_vkGetPhysicalDeviceFeatures2 query,
                   PFN_vkEnumerateDeviceExtensionProperties enumerate)
        : info(source)
    {
        if (!query || !enumerate)
            throw std::runtime_error("missing Vulkan feature-query functions");
        VkPhysicalDeviceShaderMixedFloatDotProductFeaturesVALVE mixed {};
        mixed.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MIXED_FLOAT_DOT_PRODUCT_FEATURES_VALVE;
        VkPhysicalDeviceShaderFloatControls2FeaturesKHR floatControls {
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT_CONTROLS_2_FEATURES_KHR
        };
        mixed.pNext = &floatControls;
        VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR derivatives {
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_KHR
        };
        VkPhysicalDeviceDescriptorBufferFeaturesEXT descriptors {
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT
        };
        VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT mutableType {
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_EXT
        };
        VkPhysicalDeviceVulkan13Features v13 { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
        VkPhysicalDeviceVulkan12Features v12 { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
        VkPhysicalDeviceFeatures2 available { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
        available.pNext = &v12;
        v12.pNext = &v13;
        v13.pNext = &mutableType;
        mutableType.pNext = &descriptors;
        descriptors.pNext = &derivatives;
        derivatives.pNext = &mixed;
        query(physical, &available);
        if (!available.features.shaderInt16 || !available.features.shaderStorageImageReadWithoutFormat ||
            !available.features.shaderStorageImageWriteWithoutFormat || !v12.shaderFloat16 || !v12.shaderInt8 ||
            !v12.storageBuffer8BitAccess || !v12.runtimeDescriptorArray ||
            !v12.descriptorBindingVariableDescriptorCount || !v12.bufferDeviceAddress || !v13.synchronization2 ||
            !v13.shaderIntegerDotProduct || !mutableType.mutableDescriptorType || !descriptors.descriptorBuffer ||
            !derivatives.computeDerivativeGroupLinear || !mixed.shaderMixedFloatDotProductFloat16AccFloat32 ||
            !floatControls.shaderFloatControls2)
            throw std::runtime_error("required FSR4 device feature unavailable");
        uint32_t count = 0;
        if (enumerate(physical, nullptr, &count, nullptr) != VK_SUCCESS)
            throw std::runtime_error("extension count failed");
        std::vector<VkExtensionProperties> availableExtensions(count);
        if (enumerate(physical, nullptr, &count, availableExtensions.data()) != VK_SUCCESS)
            throw std::runtime_error("extension enumeration failed");
        availableExtensions.resize(count);
        for (uint32_t i = 0; i < source.enabledExtensionCount; ++i)
            if (std::none_of(extensions.begin(), extensions.end(),
                             [&](auto* e) { return std::strcmp(e, source.ppEnabledExtensionNames[i]) == 0; }))
                extensions.push_back(source.ppEnabledExtensionNames[i]);
        for (auto* required :
             { VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME, VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
               VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, VK_KHR_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME,
               VK_VALVE_SHADER_MIXED_FLOAT_DOT_PRODUCT_EXTENSION_NAME })
        {
            if (std::none_of(availableExtensions.begin(), availableExtensions.end(),
                             [&](const auto& e) { return std::strcmp(e.extensionName, required) == 0; }))
                throw std::runtime_error(std::string("missing extension: ") + required);
            if (std::none_of(extensions.begin(), extensions.end(),
                             [&](auto* e) { return std::strcmp(e, required) == 0; }))
                extensions.push_back(required);
        }
        info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        info.ppEnabledExtensionNames = extensions.data();
        info.pNext = nullptr;
        VkBaseOutStructure* tail = nullptr;
        std::vector<VkStructureType> seen;
        for (auto* n = static_cast<const VkBaseInStructure*>(source.pNext); n; n = n->pNext)
        {
            if (seen.size() >= 64 || std::find(seen.begin(), seen.end(), n->sType) != seen.end())
                throw std::runtime_error("duplicate or cyclic device pNext");
            seen.push_back(n->sType);
            auto* c = clone(n);
            c->pNext = nullptr;
            if (tail)
                tail->pNext = c;
            else
                info.pNext = c;
            tail = c;
        }
        if (auto* f = find<VkPhysicalDeviceFeatures2>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2))
        {
            if (source.pEnabledFeatures)
                throw std::runtime_error("both legacy and features2 enabled");
            f->features.shaderInt16 = VK_TRUE;
            f->features.shaderStorageImageWriteWithoutFormat = VK_TRUE;
            f->features.shaderStorageImageReadWithoutFormat = VK_TRUE;
        }
        else
        {
            if (source.pEnabledFeatures)
                core = *source.pEnabledFeatures;
            core.shaderInt16 = VK_TRUE;
            core.shaderStorageImageWriteWithoutFormat = VK_TRUE;
            core.shaderStorageImageReadWithoutFormat = VK_TRUE;
            info.pEnabledFeatures = &core;
        }
        auto* e12 = find<VkPhysicalDeviceVulkan12Features>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES);
        auto* e13 = find<VkPhysicalDeviceVulkan13Features>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES);
        if (e12)
        {
            e12->shaderFloat16 = e12->shaderInt8 = e12->storageBuffer8BitAccess = e12->runtimeDescriptorArray =
                e12->descriptorBindingVariableDescriptorCount = e12->bufferDeviceAddress = VK_TRUE;
            e12->shaderStorageBufferArrayNonUniformIndexing |= v12.shaderStorageBufferArrayNonUniformIndexing;
            e12->shaderSampledImageArrayNonUniformIndexing |= v12.shaderSampledImageArrayNonUniformIndexing;
            e12->shaderStorageImageArrayNonUniformIndexing |= v12.shaderStorageImageArrayNonUniformIndexing;
        }
        else
        {
            auto* f = ensure<VkPhysicalDeviceShaderFloat16Int8Features>(
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES);
            f->shaderFloat16 = f->shaderInt8 = VK_TRUE;
            ensure<VkPhysicalDevice8BitStorageFeatures>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES)
                ->storageBuffer8BitAccess = VK_TRUE;
            auto* d = ensure<VkPhysicalDeviceDescriptorIndexingFeatures>(
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES);
            d->runtimeDescriptorArray = d->descriptorBindingVariableDescriptorCount = VK_TRUE;
            d->shaderStorageBufferArrayNonUniformIndexing |= v12.shaderStorageBufferArrayNonUniformIndexing;
            d->shaderSampledImageArrayNonUniformIndexing |= v12.shaderSampledImageArrayNonUniformIndexing;
            d->shaderStorageImageArrayNonUniformIndexing |= v12.shaderStorageImageArrayNonUniformIndexing;
            ensure<VkPhysicalDeviceBufferDeviceAddressFeatures>(
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES)
                ->bufferDeviceAddress = VK_TRUE;
        }
        if (e13)
            e13->synchronization2 = e13->shaderIntegerDotProduct = VK_TRUE;
        else
        {
            ensure<VkPhysicalDeviceSynchronization2Features>(
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES)
                ->synchronization2 = VK_TRUE;
            ensure<VkPhysicalDeviceShaderIntegerDotProductFeatures>(
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES)
                ->shaderIntegerDotProduct = VK_TRUE;
        }
        ensure<VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT>(
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_EXT)
            ->mutableDescriptorType = VK_TRUE;
        ensure<VkPhysicalDeviceDescriptorBufferFeaturesEXT>(
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT)
            ->descriptorBuffer = VK_TRUE;
        ensure<VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR>(
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_KHR)
            ->computeDerivativeGroupLinear = VK_TRUE;
        ensure<VkPhysicalDeviceShaderMixedFloatDotProductFeaturesVALVE>(
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MIXED_FLOAT_DOT_PRODUCT_FEATURES_VALVE)
            ->shaderMixedFloatDotProductFloat16AccFloat32 = VK_TRUE;
        if (auto* e14 = find<VkPhysicalDeviceVulkan14Features>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES))
            e14->shaderFloatControls2 = VK_TRUE;
        else
            ensure<VkPhysicalDeviceShaderFloatControls2FeaturesKHR>(
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT_CONTROLS_2_FEATURES_KHR)
                ->shaderFloatControls2 = VK_TRUE;
    }
    DeviceFeatures(const DeviceFeatures&) = delete;
    DeviceFeatures& operator=(const DeviceFeatures&) = delete;
    const VkDeviceCreateInfo* get() const { return &info; }
};
} // namespace fsr4vk
