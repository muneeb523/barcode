#include <stdint.h>
#include <string.h>
#include <iostream>
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

void drawUI()
{
    setColor(0, 0, 0); // Black background


    // Draw mode image at fixed position
    drawImage(0, 0, T001,140 ,60 );

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