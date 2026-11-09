// HLSL compiler loaded at run time. d3dcompiler_47.dll ships with Windows 8 and later; Windows 7 has no such file
// unless the DirectX End-User Runtime (d3dcompiler_43.dll) or update KB4019990 is installed. Linking d3dcompiler.lib
// would stop black.exe from starting there at all, so the compiler is looked up when first needed
// (a d3dcompiler_47.dll placed next to black.exe is found first).
#pragma once

#include <windows.h>
#include <d3dcompiler.h>

// Same arguments as D3DCompile.
HRESULT hlsl_compile(LPCVOID src, SIZE_T size, LPCSTR name, const D3D_SHADER_MACRO *defines, ID3DInclude *include, LPCSTR entry,
                     LPCSTR target, UINT flags1, UINT flags2, ID3DBlob **code, ID3DBlob **errors);

// True when a d3dcompiler DLL could be loaded (logs which one).
bool hlsl_available();
