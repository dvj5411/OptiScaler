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
            // Generated from pinned Vulkan registry: only sType, pNext, and VkBool32 members.
            // Preserve caller feature values; reject other unknown structures below.
            FSR4_COPY(VkPhysicalDevice4444FormatsFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceASTCDecodeFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceAccelerationStructureFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceAddressBindingReportFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ADDRESS_BINDING_REPORT_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceAmigoProfilingFeaturesSEC,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_AMIGO_PROFILING_FEATURES_SEC);
            FSR4_COPY(VkPhysicalDeviceAntiLagFeaturesAMD, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ANTI_LAG_FEATURES_AMD);
            FSR4_COPY(VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceBorderColorSwizzleFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BORDER_COLOR_SWIZZLE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceClusterAccelerationStructureFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_ACCELERATION_STRUCTURE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_CULLING_SHADER_FEATURES_HUAWEI);
            FSR4_COPY(VkPhysicalDeviceCoherentMemoryFeaturesAMD,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD);
            FSR4_COPY(VkPhysicalDeviceColorWriteEnableFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceCommandBufferInheritanceFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMMAND_BUFFER_INHERITANCE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceConditionalRenderingFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceCooperativeMatrix2FeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_2_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceCooperativeMatrixFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceCooperativeMatrixFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceCooperativeVectorFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_VECTOR_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceCopyMemoryIndirectFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COPY_MEMORY_INDIRECT_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceCornerSampledImageFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CORNER_SAMPLED_IMAGE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceCoverageReductionModeFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COVERAGE_REDUCTION_MODE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceCubicClampFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUBIC_CLAMP_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceCubicWeightsFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUBIC_WEIGHTS_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceCustomBorderColorFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceDepthBiasControlFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_BIAS_CONTROL_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDepthClampControlFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLAMP_CONTROL_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDepthClampZeroOneFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLAMP_ZERO_ONE_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceDepthClipControlFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_CONTROL_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDepthClipEnableFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_POOL_OVERALLOCATION_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_SET_HOST_MAPPING_FEATURES_VALVE);
            FSR4_COPY(VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_COMPUTE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceDeviceMemoryReportFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_MEMORY_REPORT_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceDiagnosticsConfigFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceDynamicRenderingFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES);
            FSR4_COPY(VkPhysicalDeviceDynamicRenderingLocalReadFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES);
            FSR4_COPY(VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceExclusiveScissorFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXCLUSIVE_SCISSOR_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceExtendedDynamicState2FeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceExtendedDynamicState3FeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceExtendedDynamicStateFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_SPARSE_ADDRESS_SPACE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceExternalMemoryRDMAFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_RDMA_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceFaultFeaturesEXT, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FAULT_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceFragmentDensityMap2FeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceFragmentDensityMapFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_OFFSET_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceFragmentShadingRateFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceFrameBoundaryFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAME_BOUNDARY_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceGlobalPriorityQueryFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GLOBAL_PRIORITY_QUERY_FEATURES);
            FSR4_COPY(VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceHdrVividFeaturesHUAWEI,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HDR_VIVID_FEATURES_HUAWEI);
            FSR4_COPY(VkPhysicalDeviceHostImageCopyFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES);
            FSR4_COPY(VkPhysicalDeviceHostQueryResetFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES);
            FSR4_COPY(VkPhysicalDeviceImage2DViewOf3DFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_2D_VIEW_OF_3D_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceImageAlignmentControlFeaturesMESA,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ALIGNMENT_CONTROL_FEATURES_MESA);
            FSR4_COPY(VkPhysicalDeviceImageCompressionControlFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_COMPRESSION_CONTROL_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_COMPRESSION_CONTROL_SWAPCHAIN_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceImageProcessing2FeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_2_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceImageProcessingFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceImageRobustnessFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES);
            FSR4_COPY(VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_SLICED_VIEW_OF_3D_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceImageViewMinLodFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_MIN_LOD_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceImagelessFramebufferFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES);
            FSR4_COPY(VkPhysicalDeviceIndexTypeUint8Features,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES);
            FSR4_COPY(VkPhysicalDeviceInheritedViewportScissorFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INHERITED_VIEWPORT_SCISSOR_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceInlineUniformBlockFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES);
            FSR4_COPY(VkPhysicalDeviceInvocationMaskFeaturesHUAWEI,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INVOCATION_MASK_FEATURES_HUAWEI);
            FSR4_COPY(VkPhysicalDeviceLegacyDitheringFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LEGACY_DITHERING_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LEGACY_VERTEX_ATTRIBUTES_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceLineRasterizationFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES);
            FSR4_COPY(VkPhysicalDeviceLinearColorAttachmentFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINEAR_COLOR_ATTACHMENT_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceMaintenance4Features, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES);
            FSR4_COPY(VkPhysicalDeviceMaintenance5Features, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_FEATURES);
            FSR4_COPY(VkPhysicalDeviceMaintenance6Features, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_6_FEATURES);
            FSR4_COPY(VkPhysicalDeviceMaintenance7FeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_7_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceMaintenance8FeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_8_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceMapMemoryPlacedFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceMemoryDecompressionFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_DECOMPRESSION_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceMeshShaderFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceMeshShaderFeaturesNV, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceMultiDrawFeaturesEXT, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceMultiviewFeatures, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES);
            FSR4_COPY(VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_RENDER_AREAS_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_VIEWPORTS_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceNestedCommandBufferFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_NESTED_COMMAND_BUFFER_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_NON_SEAMLESS_CUBE_MAP_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceOpacityMicromapFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPACITY_MICROMAP_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceOpticalFlowFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPTICAL_FLOW_FEATURES_NV);
            FSR4_COPY(VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PAGEABLE_DEVICE_LOCAL_MEMORY_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDevicePartitionedAccelerationStructureFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PARTITIONED_ACCELERATION_STRUCTURE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDevicePerStageDescriptorSetFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PER_STAGE_DESCRIPTOR_SET_FEATURES_NV);
            FSR4_COPY(VkPhysicalDevicePerformanceQueryFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDevicePipelineBinaryFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_BINARY_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDevicePipelineCreationCacheControlFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES);
            FSR4_COPY(VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_LIBRARY_GROUP_HANDLES_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDevicePipelineOpacityMicromapFeaturesARM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_OPACITY_MICROMAP_FEATURES_ARM);
            FSR4_COPY(VkPhysicalDevicePipelinePropertiesFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_PROPERTIES_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDevicePipelineProtectedAccessFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_PROTECTED_ACCESS_FEATURES);
            FSR4_COPY(VkPhysicalDevicePipelineRobustnessFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_ROBUSTNESS_FEATURES);
            FSR4_COPY(VkPhysicalDevicePresentBarrierFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_BARRIER_FEATURES_NV);
            FSR4_COPY(VkPhysicalDevicePresentIdFeaturesKHR, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_MODE_FIFO_LATEST_READY_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDevicePresentWaitFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVE_TOPOLOGY_LIST_RESTART_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVES_GENERATED_QUERY_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDevicePrivateDataFeatures, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES);
            FSR4_COPY(VkPhysicalDeviceProtectedMemoryFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES);
            FSR4_COPY(VkPhysicalDeviceProvokingVertexFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA10X6_FORMATS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceRawAccessChainsFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAW_ACCESS_CHAINS_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceRayQueryFeaturesKHR, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_INVOCATION_REORDER_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_LINEAR_SWEPT_SPHERES_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MAINTENANCE_1_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceRayTracingMotionBlurFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MOTION_BLUR_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceRayTracingPipelineFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_POSITION_FETCH_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceRayTracingValidationFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_VALIDATION_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RELAXED_LINE_RASTERIZATION_FEATURES_IMG);
            FSR4_COPY(VkPhysicalDeviceRenderPassStripedFeaturesARM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RENDER_PASS_STRIPED_FEATURES_ARM);
            FSR4_COPY(VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceRobustness2FeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceSamplerYcbcrConversionFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES);
            FSR4_COPY(VkPhysicalDeviceScalarBlockLayoutFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES);
            FSR4_COPY(VkPhysicalDeviceSchedulingControlsFeaturesARM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCHEDULING_CONTROLS_FEATURES_ARM);
            FSR4_COPY(VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT16_VECTOR_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_2_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceShaderAtomicFloatFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceShaderAtomicInt64Features,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderClockFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_BUILTINS_FEATURES_ARM);
            FSR4_COPY(VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderDrawParametersFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_FEATURES_AMD);
            FSR4_COPY(VkPhysicalDeviceShaderExpectAssumeFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_EXPECT_ASSUME_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceShaderImageFootprintFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_FUNCTIONS_2_FEATURES_INTEL);
            FSR4_COPY(VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MAXIMAL_RECONVERGENCE_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MODULE_IDENTIFIER_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceShaderObjectFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceShaderQuadControlFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_QUAD_CONTROL_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_RELAXED_EXTENDED_INSTRUCTION_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_REPLICATED_COMPOSITES_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceShaderSMBuiltinsFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderSubgroupRotateFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_ROTATE_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceShaderTerminateInvocationFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES);
            FSR4_COPY(VkPhysicalDeviceShaderTileImageFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TILE_IMAGE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceShadingRateImageFeaturesNV,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV);
            FSR4_COPY(VkPhysicalDeviceSubgroupSizeControlFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES);
            FSR4_COPY(VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_MERGE_FEEDBACK_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceSubpassShadingFeaturesHUAWEI,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_FEATURES_HUAWEI);
            FSR4_COPY(VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceTextureCompressionASTCHDRFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES);
            FSR4_COPY(VkPhysicalDeviceTilePropertiesFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TILE_PROPERTIES_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceTransformFeedbackFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceUniformBufferStandardLayoutFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES);
            FSR4_COPY(VkPhysicalDeviceVariablePointersFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES);
            FSR4_COPY(VkPhysicalDeviceVertexAttributeDivisorFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES);
            FSR4_COPY(VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_ROBUSTNESS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceVideoEncodeAV1FeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_ENCODE_AV1_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_ENCODE_QUANTIZATION_MAP_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceVideoMaintenance1FeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_MAINTENANCE_1_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceVideoMaintenance2FeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_MAINTENANCE_2_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceVulkanMemoryModelFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES);
            FSR4_COPY(VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR);
            FSR4_COPY(VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_2_PLANE_444_FORMATS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceYcbcrDegammaFeaturesQCOM,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_DEGAMMA_FEATURES_QCOM);
            FSR4_COPY(VkPhysicalDeviceYcbcrImageArraysFeaturesEXT,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT);
            FSR4_COPY(VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures,
                      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES);
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
