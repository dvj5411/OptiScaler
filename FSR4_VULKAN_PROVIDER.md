# Native FSR 4 Vulkan provider contract

OptiScaler can use an FFX API upscaler provider directly from a Vulkan game.
It searches the configured OptiScaler library directory in this order:

1. `amd_fidelityfx_loader_vk.dll`
2. `amd_fidelityfx_upscaler_vk.dll`
3. `amd_fidelityfx_vk.dll` (the legacy monolithic runtime)

`FfxVkPath` in the `[Libraries]` section can instead name either a directory
or a provider file. A file path may use any file name.

## Required ABI

The module must export all five FFX API entry points:

- `ffxConfigure`
- `ffxCreateContext`
- `ffxDestroyContext`
- `ffxDispatch`
- `ffxQuery`

The provider must support the stable upscaler descriptor IDs rooted at
`FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE` (`0x00010000`) and version
enumeration through `FFX_API_QUERY_DESC_TYPE_GET_VERSIONS`.

For native Vulkan context creation, OptiScaler appends an
`ffxCreateBackendVKDesc` with type
`FFX_API_CREATE_CONTEXT_DESC_TYPE_BACKEND_VK` (`3`) to the upscaler creation
chain. It supplies the game's `VkDevice`, `VkPhysicalDevice`, and
`PFN_vkGetDeviceProcAddr`. A requested provider version is appended to that
chain with `ffxOverrideVersion`.

Dispatches pass the game's `VkCommandBuffer` and `VkImage` handles through
`ffxDispatchDescUpscale` and `FfxApiResource`. The provider records work into
that command buffer; it must not submit it or take ownership of game resources.

## FSR 4 implementation requirements

The provider is responsible for checking that the existing game-created
Vulkan device has every feature needed by its shader set before creating a
context. In particular, the INT8 implementation is expected to need integer
dot-product support and may need 8-bit/16-bit shader or storage features. A
Vulkan device's enabled features cannot be expanded after device creation, so
unsupported devices must fail context creation cleanly rather than dispatching
an invalid pipeline.

The provider must enumerate its FSR 4 implementation from `ffxQuery` (including
a stable version ID and name) so OptiScaler can select it through the FFX
upscaler menu.
