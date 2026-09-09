#pragma once

#include "ffx_api.h"
#include <cstdint>
#include <vulkan/vulkan.h>

// Optional, provider-agnostic Vulkan device negotiation through ffxQuery.
// Providers that do not recognize these descriptors are left untouched.
constexpr ffxStructType_t FFX_API_QUERY_DESC_TYPE_VK_PREPARE_DEVICE = 0x4656584450524550ull;
constexpr ffxStructType_t FFX_API_QUERY_DESC_TYPE_VK_RELEASE_DEVICE = 0x4656584452454c53ull;

struct ffxQueryDescVkPrepareDevice
{
    ffxQueryDescHeader header;
    VkPhysicalDevice physicalDevice;
    const VkDeviceCreateInfo* sourceCreateInfo;
    std::uint32_t apiVersion;
    PFN_vkGetPhysicalDeviceFeatures2 getPhysicalDeviceFeatures2;
    PFN_vkEnumerateDeviceExtensionProperties enumerateDeviceExtensionProperties;
    const VkDeviceCreateInfo* outputCreateInfo;
    void* token;
    const char* errorMessage;
};

struct ffxQueryDescVkReleaseDevice
{
    ffxQueryDescHeader header;
    void* token;
};
