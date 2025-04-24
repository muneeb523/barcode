#include <stdint.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#define IMAGE_WIDTH 140
#define IMAGE_HEIGHT 60
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT)
extern "C"
{
#include "../appgpio.h"
#include "../st7735s.h"
#include "../fonts.h"
#include "../gfx.h"
#include "../image.h"
#include "../st7735s_compat.h"
#include "../screen.h"
}

bool loadBarcodeImage(const char *path, uint16_t *buffer, size_t size)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open barcode image file: " << path << std::endl;
        return false;
    }

    file.read(reinterpret_cast<char *>(buffer), size * sizeof(uint16_t));
    if (file.gcount() != static_cast<std::streamsize>(size * sizeof(uint16_t)))
    {
        std::cerr << "Error: Failed to read full barcode image." << std::endl;
        return false;
    }

    return true;
}


void drawUI()
{
    uint16_t barcode[IMAGE_SIZE];
    setColor(255,255,255); // Black background

    if (loadBarcodeImage("/home/barcode/image.raw", barcode, IMAGE_SIZE))
    {
        std::cout << "Drawing barcode image...\n";
        drawImage(10, 10, barcode, IMAGE_WIDTH, IMAGE_HEIGHT);

        for (int i = 0; i < IMAGE_SIZE; ++i) {
            if (barcode[i] != 0xFFFF) {
                std::cout << "Non-white pixel found at " << i << ": 0x" << std::hex << barcode[i] << std::endl;
                break;
            }
        }
        
    }
    else
    {
        std::cerr << "Failed to load barcode image.\n";
    }

    flushBuffer();
}

int main()
{
    /* Example: Render barcode for "CODE39" */
    ST7735S_Init();
    setOrientation(R180);
    fillScreen();
    flushBuffer();
    drawUI();
    return 0;


}