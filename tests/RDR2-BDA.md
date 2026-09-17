# RDR2 Proton / fsr4vk device negotiation experiment

Base: published OptiScaler PR #1161 head `b063b22f667b83aff24e02d60b233670add185dc`.
Includes the existing fsr4vk feature-contract completion patch (original commit
`9c1dbe91`, cherry-picked here). The PR author's newer distributed binary has
unpublished differences; this is not a reconstruction of that binary.

RDR2 advertised/requested `VK_EXT_buffer_device_address`, which cannot coexist
with `VK_KHR_buffer_device_address`. Descriptor buffers need the modern address
path. Adding KHR at device creation while keeping EXT is not a valid fix.

For `RDR2.exe` on Wine/Proton only, filter EXT from the game's extension
enumeration when the physical device also advertises KHR BDA and descriptor
buffers. This happens before the game chooses its device features. Leave other
extensions, layer-specific queries, and DXVK/VKD3D device creation unchanged.
Do not re-add EXT through OptiScaler's NVIDIA extension list for this game.
If the game still explicitly requests EXT, leave its device request unchanged
and log the conflict instead of attempting an unsafe address-API translation.
The narrowly scoped enumeration path takes precedence over extension spoofing
in this game; it does not fabricate NVIDIA extensions.

This is an experiment: RDR2 may insist on its legacy path. Successful build,
enumeration tests, or isolated provider context creation do not prove the game
can render using this alternative. An in-game FSR4 test is required.

CPU regression test (no Vulkan loader or GPU required):

```sh
c++ -std=c++20 -Wall -Wextra -Werror -Iexternal/vulkan/include tests/vulkan-legacy-bda.cpp -o /tmp/bda-test
/tmp/bda-test
```

Covers scope, count/data queries, truncated buffers, zero capacity, output
sentinels, driver data preservation, layer passthrough, transient/continuous
`VK_INCOMPLETE`, driver errors, missing modern capabilities, and empty lists.

For deployment, back up the existing `OptiScaler.asi` and copy the built
`OptiScaler.dll` as `OptiScaler.asi`. Keep `dinput8.dll`, `OptiPatcher.asi`, INI,
fsr4vk DLL, and launch options unchanged. Expected log markers:

- `RDR2 Proton: enumerating extensions with modern BDA preference`
- `RDR2 Proton: no legacy EXT address request; enabling supported FFX KHR/core capabilities`
- Final device extensions include KHR BDA and descriptor buffers, not EXT BDA.
- Provider context creation and first dispatch succeed (requires live test).

Rollback: stop the game and restore the backed-up `OptiScaler.asi`.
