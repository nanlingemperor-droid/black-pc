#include "d3d/dcompile.h"

#include "common.h"

#include <mutex>

namespace {

typedef HRESULT(WINAPI *CompileFn)(LPCVOID, SIZE_T, LPCSTR, const D3D_SHADER_MACRO *, ID3DInclude *, LPCSTR, LPCSTR, UINT, UINT,
                                   ID3DBlob **, ID3DBlob **);

std::once_flag g_once;
CompileFn g_compile;

void load()
{
    for (int v = 47; v >= 33; v--) {
        wchar_t name[32];
        swprintf_s(name, L"d3dcompiler_%d.dll", v);
        HMODULE m = LoadLibraryW(name);
        if (!m) continue;
        g_compile = (CompileFn)GetProcAddress(m, "D3DCompile");
        if (g_compile) {
            logf("[d3d] shader compiler: %ls", name);
            return;
        }
    }
    logf("[d3d] no d3dcompiler_NN.dll found: install the DirectX End-User Runtime (June 2010) or copy a 32-bit "
         "d3dcompiler_47.dll next to black.exe");
}

} // namespace

bool hlsl_available()
{
    std::call_once(g_once, load);
    return g_compile != nullptr;
}

HRESULT hlsl_compile(LPCVOID src, SIZE_T size, LPCSTR name, const D3D_SHADER_MACRO *defines, ID3DInclude *include, LPCSTR entry,
                     LPCSTR target, UINT flags1, UINT flags2, ID3DBlob **code, ID3DBlob **errors)
{
    if (!hlsl_available()) return E_FAIL;
    return g_compile(src, size, name, defines, include, entry, target, flags1, flags2, code, errors);
}
