#pragma once

#include <string_view>

namespace StartupProbeDiagnostic
{
// Fork-only diagnostic, not a renderer selection or a global DX12 disable.
constexpr bool SkipBackgroundD3D12Probe(bool runningOnWine, std::string_view executable)
{
    constexpr std::string_view target = "rdr2.exe";
    if (!runningOnWine || executable.size() != target.size())
        return false;

    for (size_t i = 0; i < target.size(); ++i)
    {
        const char c = executable[i];
        const char lower = c >= 'A' && c <= 'Z' ? static_cast<char>(c + ('a' - 'A')) : c;
        if (lower != target[i])
            return false;
    }
    return true;
}
} // namespace StartupProbeDiagnostic
