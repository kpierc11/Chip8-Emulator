#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
import std;

// The chip 8 has 16 8 bit registers Vx where x is a hexadecmimal digit through F. There is also a 16-bit register called I. This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
struct Chip8Registers
{
    // General purpose 8 bit registers
    uint8_t V0;
    uint8_t V1;
    uint8_t V2;
    uint8_t V3;
    uint8_t V4;
    uint8_t V5;
    uint8_t V6;
    uint8_t V7;
    uint8_t V8;
    uint8_t V9;
    uint8_t VA;
    uint8_t VB;
    uint8_t VC;
    uint8_t VD;
    uint8_t VE;
    uint8_t VF; // should not be used by any program used as a flag by some instructions;

    // Used to store memory addresses only the lowest rightmost 12 bits are used.
    uint8_t I;

    // When these registers are non-zero they are automatically decremented at a rate of 60Hz
    uint8_t delayTimer;
    uint8_t soundTimer;

    // Stores the current executing address
    uint16_t programCounter;

    // Points to top most level of stack
    uint8_t *stackPointer;
};

void readROM()
{
}

int main(int argc, char *argv[])
{
    // The chip 8 is capable fo up  to 4KB (4096 bytes) of Ram from location 0x0000
    uint8_t memoryBuffer[4096] = {0x00E0};

    std::println("Memory Block Size: {}", sizeof(memoryBuffer));

    Chip8Registers registers;

    registers.stackPointer = memoryBuffer;

    SDL_Window *window;
    SDL_Renderer *renderer;
    bool done = false;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "An SDL3 window",
        640,
        480,
        SDL_WINDOW_OPENGL);

    if (!window)
    {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    while (!done)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                done = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // // Fetch instructions
        for (auto &instruction : memoryBuffer)
        {
            std::println("This is a hex instruction {:#X}", instruction);
            switch (instruction)
            {
            case 0x0E0:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderPoint(renderer, 40, 40);
                break;
            }
        }

        // Decode

        // Execute
        SDL_RenderPresent(renderer);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}