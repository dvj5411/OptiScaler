#pragma once

#include <algorithm>
#include <cstring>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace VulkanLegacyBda
{
inline bool IsRdr2Proton(bool linuxHost, std::string_view executable)
{
    constexpr std::string_view target = "rdr2.exe";
    if (!linuxHost || executable.size() != target.size())
        return false;
    for (size_t i = 0; i < target.size(); ++i)
    {
        auto c = executable[i];
        if (c >= 'A' && c <= 'Z')
            c += 'a' - 'A';
        if (c != target[i])
            return false;
    }
    return true;
}

// Filter before the game selects its capabilities, never after it requests EXT.
// EXT and KHR buffer device address cannot coexist on a VkDevice. Only hide EXT
// when the driver advertises the modern path needed by descriptor buffers.
inline VkResult Enumerate(PFN_vkEnumerateDeviceExtensionProperties original, VkPhysicalDevice physicalDevice,
                          const char* layer, uint32_t* count, VkExtensionProperties* output)
{
    if (layer != nullptr || count == nullptr)
        return original(physicalDevice, layer, count, output);

    for (unsigned attempt = 0; attempt < 4; ++attempt)
    {
        uint32_t available = 0;
        auto result = original(physicalDevice, nullptr, &available, nullptr);
        if (result != VK_SUCCESS)
            return result;
        std::vector<VkExtensionProperties> extensions(available);
        if (available != 0)
        {
            result = original(physicalDevice, nullptr, &available, extensions.data());
            if (result == VK_INCOMPLETE)
                continue;
            if (result != VK_SUCCESS)
                return result;
            extensions.resize(available);
        }
        const auto has = [&](const char* name)
        {
            return std::any_of(extensions.begin(), extensions.end(), [&](const auto& extension)
                               { return std::strcmp(extension.extensionName, name) == 0; });
        };
        if (has(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) && has(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME))
            std::erase_if(extensions, [](const auto& extension)
                          { return std::strcmp(extension.extensionName, VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) == 0; });

        if (output == nullptr)
        {
            *count = static_cast<uint32_t>(extensions.size());
            return VK_SUCCESS;
        }
        const auto written = std::min(*count, static_cast<uint32_t>(extensions.size()));
        std::copy_n(extensions.begin(), written, output);
        *count = written;
        return written < extensions.size() ? VK_INCOMPLETE : VK_SUCCESS;
    }
    return VK_ERROR_INITIALIZATION_FAILED;
}
} // namespace VulkanLegacyBda
