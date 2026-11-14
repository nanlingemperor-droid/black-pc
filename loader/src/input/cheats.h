// Cheats for the single-player game: values frozen once per frame, toggled with F-keys.
#pragma once
namespace cheats {
void poll(); // call once per frame from the game thread (xinput read_pad)
}
