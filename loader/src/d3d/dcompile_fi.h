// Force-included (/FI) into the files that call D3DCompile: the call goes to the run-time loaded compiler
// (dcompile.cpp) instead of d3dcompiler_47.dll, which Windows 7 does not have.
#pragma once
#include "d3d/dcompile.h"
#define D3DCompile hlsl_compile
