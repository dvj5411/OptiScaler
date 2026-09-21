#!/usr/bin/env python3
"""Compile the actual Vulkan feature negotiation with a mock driver (no GPU).

Run from any directory. Requires Python 3, a C++20 compiler, and the checked-out
external/vulkan submodule. CXX selects the compiler. SANITIZE=1 enables ASan/UBSan.
Only platform scaffolding is substituted; the helpers, feature-state class and
device extension/feature negotiation are extracted unchanged from production.
"""

import os
from pathlib import Path
import shlex
import subprocess
import tempfile


root = Path(__file__).resolve().parents[1]
source = (root / "OptiScaler/spoofing/Vulkan_Spoofing.cpp").read_text()
header = (root / "OptiScaler/spoofing/Vulkan_Spoofing.h").read_text()


def section(text, start, end):
    return text[text.index(start):text.index(end, text.index(start))]


production = (
    section(header, "class VulkanDeviceFeatureState", "class VulkanSpoofing\n{")
    + "\nclass VulkanSpoofing { public:\n"
    + section(header, "    static VkResult hkvkCreateDevice(", "    static VkResult hkvkCreateInstance(")
    + "};\n"
    + section(source, "static bool HasExtension(", "inline static void hkvkGetPhysicalDeviceMemoryProperties(")
    + section(source, "struct VulkanDeviceFeatureState::Impl\n", "inline static VkResult hkvkEnumerateDeviceExtensionProperties(")
)

with tempfile.TemporaryDirectory(prefix="optiscaler-vulkan-test-") as directory:
    directory = Path(directory)
    (directory / "device-features-under-test.h").write_text(production)
    executable = directory / "device-features-test"
    command = shlex.split(os.environ.get("CXX", "c++")) + [
        "-std=c++20", "-DVK_ENABLE_BETA_EXTENSIONS",
        "-I" + str(root / "external/vulkan/include"), "-I" + str(directory),
        str(root / "tests/vulkan-device-features.cpp"), "-o", str(executable),
    ]
    if os.environ.get("SANITIZE") == "1":
        command += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-g"]
    subprocess.run(command, check=True)
    subprocess.run([str(executable)], check=True)
