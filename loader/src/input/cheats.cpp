// Memory cheats. The game's memory sits at its original Xbox addresses, so a cheat is "write VALUE at ADDRESS".
// The addresses are NOT known yet: find them in Cheat Engine (attach to black.exe, scan health/ammo) and put them
// in black.ini under [cheats] (e.g. health_addr=0x00123456, health_value=100). A cheat with address 0 does nothing.
// Hotkeys (game window focused): F1 God Mode, F2 Health, F3 Unlimited Ammo, F4 Infinite Grenades,
// F5 Unlock All Weapons (written once).
#include "cheats.h"
#include <windows.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <string>

namespace cheats {
namespace {
struct Cheat {
    const wchar_t *key;
    int vk;
    bool on;
    uint32_t addr;
    uint32_t value;
    bool oneshot;
};
Cheat g_c[] = {
    {L"god", VK_F1, false, 0, 1, false},       // flag/health that blocks damage
    {L"health", VK_F2, false, 0, 100, false},  // player health (store as the game does: int or float bits)
    {L"ammo", VK_F3, false, 0, 999, false},    // current weapon magazine / reserve
    {L"grenades", VK_F4, false, 0, 9, false},
    {L"weapons", VK_F5, false, 0, 0xFF, true}, // weapon-unlocked bitmask
};
constexpr int N = sizeof g_c / sizeof g_c[0];
bool g_loaded;
bool g_down[N];

void load()
{
    g_loaded = true;
    wchar_t path[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    wchar_t *s = wcsrchr(path, L'\\');
    if (!s || (size_t)(s - path) + 10 >= MAX_PATH) return;
    wcscpy(s + 1, L"black.ini");
    for (Cheat &c : g_c) {
        wchar_t buf[32];
        std::wstring k = std::wstring(c.key) + L"_addr";
        GetPrivateProfileStringW(L"cheats", k.c_str(), L"0", buf, 32, path);
        c.addr = (uint32_t)wcstoul(buf, nullptr, 0);
        k = std::wstring(c.key) + L"_value";
        GetPrivateProfileStringW(L"cheats", k.c_str(), L"", buf, 32, path);
        if (buf[0]) c.value = (uint32_t)wcstoul(buf, nullptr, 0);
    }
}
} // namespace

void poll()
{
    if (!g_loaded) load();
    for (int i = 0; i < N; i++) {
        Cheat &c = g_c[i];
        bool down = (GetAsyncKeyState(c.vk) & 0x8000) != 0;
        if (down && !g_down[i]) c.on = !c.on || c.oneshot;
        g_down[i] = down;
        if (c.on && c.addr) {
            *(volatile uint32_t *)(uintptr_t)c.addr = c.value;
            if (c.oneshot) c.on = false;
        }
    }
}
} // namespace cheats
