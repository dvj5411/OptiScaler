// c++ -std=c++20 -Iexternal/vulkan/include tests/vulkan-legacy-bda.cpp -o /tmp/bda-test
#include "../OptiScaler/spoofing/Vulkan_Legacy_Bda.h"
#include <cassert>
#include <iostream>

static std::vector<VkExtensionProperties> driver;
static VkResult failure = VK_SUCCESS;
static unsigned incompleteReads = 0;
static const char* seenLayer = nullptr;
static VkResult VKAPI_CALL enumerate(VkPhysicalDevice, const char* layer, uint32_t* count, VkExtensionProperties* out)
{
    seenLayer = layer;
    if (failure != VK_SUCCESS)
        return failure;
    if (!out)
    {
        *count = static_cast<uint32_t>(driver.size());
        return VK_SUCCESS;
    }
    if (incompleteReads)
    {
        --incompleteReads;
        return VK_INCOMPLETE;
    }
    auto n = std::min(*count, static_cast<uint32_t>(driver.size()));
    std::copy_n(driver.begin(), n, out);
    *count = n;
    return n < driver.size() ? VK_INCOMPLETE : VK_SUCCESS;
}

int main()
{
    using namespace VulkanLegacyBda;
    assert(IsRdr2Proton(true, "RDR2.exe"));
    assert(IsRdr2Proton(true, "rDr2.ExE"));
    assert(!IsRdr2Proton(false, "RDR2.exe"));
    assert(!IsRdr2Proton(true, "PlayRDR2.exe"));
    assert(!IsRdr2Proton(true, "NMS.exe"));
    driver = {{"VK_test_first", 3}, {VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, 2},
              {VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, 1}, {VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, 1}};
    const auto original = driver;
    uint32_t count = 0;
    assert(Enumerate(enumerate, {}, nullptr, &count, nullptr) == VK_SUCCESS && count == 3);
    VkExtensionProperties out[5] {};
    std::strcpy(out[3].extensionName, "sentinel");
    count = 5;
    assert(Enumerate(enumerate, {}, nullptr, &count, out) == VK_SUCCESS && count == 3);
    assert(std::strcmp(out[0].extensionName, "VK_test_first") == 0 && out[0].specVersion == 3);
    assert(std::strcmp(out[1].extensionName, VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) == 0);
    assert(std::strcmp(out[3].extensionName, "sentinel") == 0);
    assert(std::memcmp(driver.data(), original.data(), driver.size() * sizeof(driver[0])) == 0);
    count = 1;
    assert(Enumerate(enumerate, {}, nullptr, &count, out) == VK_INCOMPLETE && count == 1);
    count = 0;
    assert(Enumerate(enumerate, {}, nullptr, &count, out) == VK_INCOMPLETE && count == 0);
    count = 5;
    assert(Enumerate(enumerate, {}, "layer", &count, out) == VK_SUCCESS && count == 4);
    assert(std::strcmp(seenLayer, "layer") == 0);
    incompleteReads = 1;
    assert(Enumerate(enumerate, {}, nullptr, &count, nullptr) == VK_SUCCESS && count == 3);
    incompleteReads = 4;
    assert(Enumerate(enumerate, {}, nullptr, &count, nullptr) == VK_ERROR_INITIALIZATION_FAILED);
    failure = VK_ERROR_DEVICE_LOST;
    assert(Enumerate(enumerate, {}, nullptr, &count, nullptr) == VK_ERROR_DEVICE_LOST);
    failure = VK_SUCCESS;
    driver.pop_back(); // No descriptor buffers: preserve the game's EXT option.
    count = 5;
    assert(Enumerate(enumerate, {}, nullptr, &count, out) == VK_SUCCESS && count == 3);
    assert(std::strcmp(out[1].extensionName, VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) == 0);
    driver.pop_back(); // No KHR BDA either.
    assert(Enumerate(enumerate, {}, nullptr, &count, nullptr) == VK_SUCCESS && count == 2);
    driver.clear();
    assert(Enumerate(enumerate, {}, nullptr, &count, out) == VK_SUCCESS && count == 0);
    std::cout << "RDR2 BDA enumeration tests passed\n";
}
