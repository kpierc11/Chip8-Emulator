#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <fstream>
import std;

using namespace std;

// The chip 8 has 16 8 bit registers Vx where x is a hexadecmimal digit through F. There is also a 16-bit register called I. This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
struct Chip8Registers
{
    // General purpose 8 bit registers
    uint8_t V[16];

    // Used to store memory addresses only the lowest rightmost 12 bits are used.
    uint8_t I;

    // When these registers are non-zero they are automatically decremented at a rate of 60Hz
    uint8_t delayTimer;
    uint8_t soundTimer;

    // Stores the current executing address
    uint16_t programCounter;

    // Points to top most level of stack
    uint8_t *stackPointer;

    // Used to store addresses for subroutines
    uint16_t stack[16];
};

struct Person
{
    std::string height;
    std::string weight;
};

void readROM()
{
}

int main(int argc, char *argv[])
{
    // The chip 8 is capable fo up  to 4KB (4096 bytes) of Ram from location 0x0000
    unsigned char memoryBuffer[4096] =
        {
            // Font
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F

        };

    std::println("Memory Block Size: {}", sizeof(memoryBuffer));

    Chip8Registers cpuRegisters;

    // registers.stackPointer = memoryBuffer;

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

    // Read Rom file
    ifstream romFile("roms/ibm-logo.ch8", ios::binary);

    char ch;

    if (!romFile)
    {
        println("Couldn't read file");
        return 1;
    }

    romFile.seekg(0, romFile.end);
    int length = romFile.tellg();
    romFile.seekg(0, romFile.beg);

    println("Length of file: {}", length);

    int programEntryPoint = 0x200;

    while (romFile.get(ch))
    {
        std::println("{:02x}", ch);
        //  load program data into memory
        memoryBuffer[programEntryPoint++] = static_cast<int>(ch);
    }

    romFile.close();

    // Set the program counter initial address
    cpuRegisters.programCounter = 0x200;

    // uint16_t opcode = static_cast<uint16_t>((memoryBuffer[cpuRegisters.programCounter]) << 8) | memoryBuffer[cpuRegisters.programCounter + 1];
    // uint8_t firstOpcodeByte = opcode & 0x0F;
    // uint16_t lastChunkOpcode = opcode & 0x0FFF;
    // uint16_t registerNum = opcode & 0x0F00;

    // println("Register Number: {:X}", registerNum);

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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // // Fetch instructions
        uint16_t opcode = static_cast<uint16_t>((memoryBuffer[cpuRegisters.programCounter]) << 8) | memoryBuffer[cpuRegisters.programCounter + 1];

        println("Current opcode: {:X}",opcode);

        uint8_t firstOpcodeNibble = opcode >> 0x0F;
        uint16_t address = opcode & 0x0FFF;
        uint16_t vxRegister = opcode >> 8 & 0x0F;
        uint16_t values = opcode & 0xFF;
        uint16_t spriteBytes = opcode & 0x0F;


        if(memoryBuffer[cpuRegisters.programCounter] < 4096) {
             cpuRegisters.programCounter += 2;
        }

        switch (firstOpcodeNibble)
        {

        case 0x0:

            if (opcode == 0x00E0)
            {
                println("cmd: display clear ");
            }

            if (opcode == 0x00EE)
            {
                println("cmd: display clear ");
            }
            break;

        case 0x1:
            cpuRegisters.programCounter = address;
            break;

        case 0x2:

            break;

        case 0x3:

            break;

        case 0x4:

            break;

        case 0x5:

            break;

        case 0x6:
            cpuRegisters.V[vxRegister] = values;
            break;

        case 0x7:
            cpuRegisters.V[vxRegister] = values;
            break;

        case 0x8:

            break;

        case 0x9:

            break;

        case 0xA:
            cpuRegisters.I = values;
            break;

        case 0xD:
            SDL_FRect rect;
            uint8_t xCor = (opcode >> 8) & 0x0F;
            uint8_t yCor = (opcode >> 8) & 0x0F;

            rect.x = xCor;
            rect.y = yCor;
            rect.h = 5;
            rect.w = 8;
            println("cmd: display Draw ");
            SDL_RenderRect(renderer, &rect);
            break;
        }
       // Decode

        //  Execute
        SDL_RenderPresent(renderer);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}