#pragma once
#include <vulkan/vulkan_core.h>

// Khronos ABI compatibility for Vulkan headers predating the optional RADV
// mixed-dot backend. Native Windows AMD/NVIDIA use the portable INT8 backend.
#ifndef VK_VALVE_shader_mixed_float_dot_product
#define VK_VALVE_SHADER_MIXED_FLOAT_DOT_PRODUCT_EXTENSION_NAME "VK_VALVE_shader_mixed_float_dot_product"
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MIXED_FLOAT_DOT_PRODUCT_FEATURES_VALVE static_cast<VkStructureType>(1000673000)
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
