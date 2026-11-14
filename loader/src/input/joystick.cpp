#include "joystick.h"
#include <windows.h>
#include <mmsystem.h>
#include <algorithm>
#include <cstdlib>

namespace joystick {
namespace {
// Typical PS2->USB adapter in digital+analog mode (DirectInput layout). Button index -> Xbox control.
// 0 Triangle 1 Circle 2 Cross 3 Square 4 L2 5 R2 6 L1 7 R1 8 Select 9 Start 10 L3 11 R3
enum { TRI, CIR, CRO, SQU, L2, R2, L1, R1, SEL, STA, L3, R3 };

int16_t axis(DWORD v, bool invert)
{
    int n = (int)v - 32767; // winmm range 0..65535
    if (std::abs(n) < 6000) return 0; // dead zone
    n = std::clamp(n, -32767, 32767);
    return (int16_t)(invert ? -n : n);
}
void merge_stick(int16_t &x, int16_t &y, int16_t px, int16_t py)
{
    if ((int64_t)px * px + (int64_t)py * py > (int64_t)x * x + (int64_t)y * y) {
        x = px;
        y = py;
    }
}
} // namespace

void merge(uint16_t &wButtons, uint8_t analog[8], int16_t &lx, int16_t &ly, int16_t &rx, int16_t &ry)
{
    static UINT count = joyGetNumDevs();
    static DWORD absent[16];
    static DWORD polls;
    polls++;
    for (UINT id = 0; id < std::min<UINT>(count, 16); id++) {
        if (absent[id] && polls - absent[id] < 120) continue; // re-probe an empty slot every ~2 s
        JOYINFOEX j = {};
        j.dwSize = sizeof j;
        j.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(id, &j) != JOYERR_NOERROR) {
            absent[id] = polls;
            continue;
        }
        absent[id] = 0;
        auto b = [&](int n) { return (int)((j.dwButtons >> n) & 1); };
        if (b(CRO)) analog[0] = 255; // A
        if (b(CIR)) analog[1] = 255; // B
        if (b(SQU)) analog[2] = 255; // X
        if (b(TRI)) analog[3] = 255; // Y
        if (b(R1)) analog[4] = 255;  // BLACK
        if (b(L1)) analog[5] = 255;  // WHITE
        if (b(L2)) analog[6] = 255;  // left trigger
        if (b(R2)) analog[7] = 255;  // right trigger
        if (b(STA)) wButtons |= 0x10; // START
        if (b(SEL)) wButtons |= 0x20; // BACK
        if (b(L3)) wButtons |= 0x40;
        if (b(R3)) wButtons |= 0x80;
        if ((j.dwPOV & 0xFFFF) != 0xFFFF && j.dwPOV < 36000) { // hat: hundredths of a degree, 0 = up; 0xFFFF = centred
            int d = (int)(j.dwPOV / 100);
            if (d >= 315 || d <= 45) wButtons |= 0x01;
            if (d >= 45 && d <= 135) wButtons |= 0x08;
            if (d >= 135 && d <= 225) wButtons |= 0x02;
            if (d >= 225 && d <= 315) wButtons |= 0x04;
        }
        merge_stick(lx, ly, axis(j.dwXpos, false), axis(j.dwYpos, true));
        merge_stick(rx, ry, axis(j.dwZpos, false), axis(j.dwRpos, true)); // right stick = Z/R on most adapters
    }
}
} // namespace joystick
