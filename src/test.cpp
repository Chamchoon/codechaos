#include "raylib.h"
#include <iostream>

int main() {
    // 1. Initialize window (optional if only playing sound)
    InitWindow(800, 600, "Sound Test");
    
    // 2. Initialize audio system (REQUIRED)
    InitAudioDevice();

    // 3. Load sound (check if successful)
    Sound doorSound = LoadSound("../sounds/openDoor.wav");
    
    if (doorSound.frameCount > 0) {
        std::cout << "Sound loaded successfully!\n";
        PlaySound(doorSound);
    } else {
        std::cerr << "ERROR: Failed to load sound!\n";
    }

    // 4. Keep the program running long enough to hear the sound
    // (Alternatively, use a while loop for a real game)
    WaitTime(2.0);  // Waits 2 seconds before closing

    // 5. Cleanup
    UnloadSound(doorSound);
    CloseAudioDevice();
    CloseWindow();  // Close if window was opened

    return 0;
}