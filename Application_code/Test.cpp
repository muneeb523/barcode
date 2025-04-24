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
        std::cerr << "Error: Failed to read the full barcode image data." << std::endl;
        return false;
    }

    return true;
}

void drawUI()
{

    uint16_t barcode[IMAGE_SIZE];
    setColor(0, 0, 0); // Black background

    if (loadBarcodeImage("/root/image.raw", barcode, IMAGE_SIZE))
    {
        // Draw mode image at fixed position
        drawImage(0, 0, barcode, 140, 60);
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