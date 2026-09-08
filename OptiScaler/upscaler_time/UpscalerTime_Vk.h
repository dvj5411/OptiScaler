#pragma once

#include "SysUtils.h"

#include <vulkan/vulkan.hpp>
#include <deque>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

class UpscalerTimeVk
{
  public:
    static void Init(VkDevice device, VkPhysicalDevice pd);
    static void UpscaleStart(VkCommandBuffer cmdBuffer);
    static void UpscaleEnd(VkCommandBuffer cmdBuffer);
    static void ReadUpscalingTime(VkDevice device);

  private:
    struct QueryPool
    {
        VkQueryPool handle = VK_NULL_HANDLE;
        uint32_t usedPairs = 0;
        uint32_t pending = 0;
    };

    struct Measurement
    {
        std::shared_ptr<QueryPool> pool;
        uint32_t firstQuery = 0;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    };

    static std::shared_ptr<QueryPool> CreatePool();

    static inline constexpr uint32_t PairsPerPool = 256;
    static inline constexpr size_t MaxPools = 4;
    static inline VkDevice _device = VK_NULL_HANDLE;
    static inline double _timeStampPeriod = 1.0;
    static inline std::shared_ptr<QueryPool> _currentPool;
    static inline std::vector<std::shared_ptr<QueryPool>> _pools;
    static inline std::deque<Measurement> _pending;
    static inline std::optional<Measurement> _active;
    static inline std::mutex _mutex;
};
