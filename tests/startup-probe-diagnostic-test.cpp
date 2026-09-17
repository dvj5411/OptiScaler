#include "../OptiScaler/misc/StartupProbeDiagnostic.h"

using StartupProbeDiagnostic::SkipBackgroundD3D12Probe;
static_assert(SkipBackgroundD3D12Probe(true, "RDR2.exe"));
static_assert(SkipBackgroundD3D12Probe(true, "rdr2.exe"));
static_assert(SkipBackgroundD3D12Probe(true, "RdR2.ExE"));
static_assert(!SkipBackgroundD3D12Probe(false, "RDR2.exe"));
static_assert(!SkipBackgroundD3D12Probe(true, "PlayRDR2.exe"));
static_assert(!SkipBackgroundD3D12Probe(true, "NMS.exe"));
static_assert(!SkipBackgroundD3D12Probe(true, "bg3.exe"));
static_assert(!SkipBackgroundD3D12Probe(true, ""));
static_assert(!SkipBackgroundD3D12Probe(true, "rdr2.exe.other"));
int main() { return 0; }
