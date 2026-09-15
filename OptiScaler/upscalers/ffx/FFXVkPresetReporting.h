#pragma once

#include "ffx_vk_preset_query.h"
#include <optional>

namespace FFXVkPresetReporting
{
// Wire return code from newer FFX SDKs; older SDK headers omit its enum name.
constexpr ffxReturnCode_t ProviderNoSupportNewDescriptor = 7;
inline std::optional<uint32_t> SupportedPresets(PfnFfxQuery query, ffxContext* context)
{
    if (query == nullptr || context == nullptr || *context == nullptr)
        return std::nullopt;
    Fsr4VkQueryPresetCapabilities desc {};
    desc.header.type = FSR4VK_QUERY_DESC_TYPE_PRESET_CAPABILITIES;
    if (query(context, &desc.header) != FFX_API_RETURN_OK || desc.forcedPresetMask == 0 ||
        (desc.forcedPresetMask & ~0x3fu) != 0)
        return std::nullopt;
    return desc.forcedPresetMask;
}

inline bool CanForce(uint32_t mask, uint32_t preset)
{
    return preset == FSR4VK_PRESET_AUTO || (preset < 32 && (mask & (1u << preset)) != 0);
}

inline ffxReturnCode_t Apply(PfnFfxConfigure configure, ffxContext* context,
                            uint32_t mask, uint32_t preset)
{
    if (configure == nullptr || context == nullptr || *context == nullptr || !CanForce(mask, preset))
        return FFX_API_RETURN_ERROR_PARAMETER;
    Fsr4VkConfigurePreset desc {};
    desc.header.type = FSR4VK_CONFIGURE_DESC_TYPE_PRESET;
    desc.preset = preset;
    return configure(context, &desc.header);
}

// Publish once when EvaluateInternal returns. Early exits clear the old value
// without making the label flicker to "Failed to hook" during every dispatch.
struct DispatchReport
{
    std::optional<uint32_t>& destination;
    std::optional<uint32_t> activePreset {};
    ~DispatchReport() { destination = activePreset; }
};

inline std::optional<uint32_t> Read(PfnFfxQuery query, ffxContext* context, bool& supported)
{
    if (!supported || query == nullptr || context == nullptr || *context == nullptr)
        return std::nullopt;

    Fsr4VkQueryActivePreset desc {};
    desc.header.type = FSR4VK_QUERY_DESC_TYPE_ACTIVE_PRESET;
    desc.activePreset = FSR4VK_PRESET_UNKNOWN;
    const auto result = query(context, &desc.header);
    if (result == FFX_API_RETURN_ERROR_UNKNOWN_DESCTYPE ||
        result == ProviderNoSupportNewDescriptor)
        supported = false; // Older/other providers: probe only once per context.

    if (result != FFX_API_RETURN_OK)
        return std::nullopt;

    switch (desc.activePreset)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return desc.activePreset;
    default:
        return std::nullopt;
    }
}
} // namespace FFXVkPresetReporting
