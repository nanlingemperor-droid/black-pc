// Generic USB joystick / PS2-style pad (via a USB adapter) through winmm. Merged into Xbox port 0 beside XInput.
#pragma once
#include <cstdint>
namespace joystick {
// Adds every connected winmm joystick to the Xbox pad state (same fields as XGamepad in xinput.cpp).
void merge(uint16_t &wButtons, uint8_t analog[8], int16_t &lx, int16_t &ly, int16_t &rx, int16_t &ry);
}
