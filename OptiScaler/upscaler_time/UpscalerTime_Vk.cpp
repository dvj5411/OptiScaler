#include "pch.h"
#include "UpscalerTime_Vk.h"

#include <State.h>

void UpscalerTimeVk::Init(VkDevice device, VkPhysicalDevice pd)
{
    std::scoped_lock lock(_mutex);
    if (_device == device)
        return;
    // OptiScaler's Vulkan input currently owns one device. Do not destroy pools
    // from an older device here: its lifetime may already have ended.
    if (_device != VK_NULL_HANDLE)
        return;

    _device = device;
    VkPhysicalDeviceProperties deviceProperties {};
    vkGetPhysicalDeviceProperties(pd, &deviceProperties);
    _timeStampPeriod = deviceProperties.limits.timestampPeriod;
}

std::shared_ptr<UpscalerTimeVk::QueryPool> UpscalerTimeVk::CreatePool()
{
    VkQueryPoolCreateInfo queryPoolInfo = {};
    queryPoolInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    queryPoolInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
    queryPoolInfo.queryCount = PairsPerPool * 2;
    auto pool = std::make_shared<QueryPool>();
    if (vkCreateQueryPool(_device, &queryPoolInfo, nullptr, &pool->handle) != VK_SUCCESS)
        return {};
    return pool;
}

void UpscalerTimeVk::UpscaleStart(VkCommandBuffer cmdBuffer)
{
    std::scoped_lock lock(_mutex);
    if (_device == VK_NULL_HANDLE || _active.has_value())
        return;

    if (!_currentPool || _currentPool->usedPairs == PairsPerPool)
    {
        if (_pools.size() >= MaxPools)
            return;
        _currentPool = CreatePool();
        if (!_currentPool)
            return;
        _pools.push_back(_currentPool);
    }

    const uint32_t firstQuery = _currentPool->usedPairs++ * 2;
    ++_currentPool->pending;
    vkCmdResetQueryPool(cmdBuffer, _currentPool->handle, firstQuery, 2);
    vkCmdWriteTimestamp(cmdBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, _currentPool->handle, firstQuery);
    _active = Measurement { _currentPool, firstQuery, cmdBuffer };
}

void UpscalerTimeVk::UpscaleEnd(VkCommandBuffer cmdBuffer)
{
    std::scoped_lock lock(_mutex);
    if (!_active || _active->commandBuffer != cmdBuffer)
        return;

    vkCmdWriteTimestamp(cmdBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, _active->pool->handle,
                        _active->firstQuery + 1);
    _pending.push_back(*_active);
    _active.reset();
}

void UpscalerTimeVk::ReadUpscalingTime(VkDevice device)
{
    std::vector<double> completedTimes;
    {
        std::scoped_lock lock(_mutex);
        if (device != _device)
            return;

        for (auto measurement = _pending.begin(); measurement != _pending.end();)
        {
            // Each query returns its timestamp followed by an availability word.
            // Pools are append-only: a query index is never reset and reused, so
            // availability cannot be stale from an earlier frame.
            uint64_t results[4] = {};
            const VkResult result = vkGetQueryPoolResults(
                device, measurement->pool->handle, measurement->firstQuery, 2, sizeof(results), results,
                sizeof(uint64_t) * 2, VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
            if (result != VK_SUCCESS || results[1] == 0 || results[3] == 0)
            {
                ++measurement;
                continue;
            }

            const uint64_t elapsedTicks = results[2] - results[0];
            const double elapsedTimeMs = elapsedTicks * _timeStampPeriod / 1e6;
            if (elapsedTimeMs > 0.0 && elapsedTimeMs < 5000.0)
                completedTimes.push_back(elapsedTimeMs);
            --measurement->pool->pending;
            measurement = _pending.erase(measurement);
        }

        for (auto pool = _pools.begin(); pool != _pools.end();)
        {
            if (*pool != _currentPool && (*pool)->pending == 0)
            {
                vkDestroyQueryPool(device, (*pool)->handle, nullptr);
                pool = _pools.erase(pool);
            }
            else
                ++pool;
        }
    }

    if (!completedTimes.empty())
    {
        std::scoped_lock lock(State::Instance().frameTimeMutex);
        for (const double elapsedTimeMs : completedTimes)
        {
            State::Instance().upscaleTimes.push_back(elapsedTimeMs);
            State::Instance().upscaleTimes.pop_front();
        }
    }
}
