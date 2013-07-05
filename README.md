SlimTFT
=======

More efficient version of Arduino TFT

When I was working on a project using Arduino TFT screen, the stock library had bad performace and potential memory leak for bitmap displaying. So SlimTFT was created to deal with these problems.


Features:
1. It uses Fat16 instead of SD library for accessing SD card, which reduces sketch size, but have less file access features, which are not very neccessary in my opinion.

2. The process of bitmap file display is custom-made. As this library is built within very tight schedule, I didn't have time to check out the differences. Anyways, the process for accessing images is simplier.

3. Pin definition is inside SlimTFT.h instead of the sketches. This may get fixed later

How to use:
First make bitmap images of 128*160, 24-bit color. Save them in the root folder of the SD card. 
Connect the pins as following definition:

sd_cs  10
LCD_CS   3
DC_LCD   9
RST_LCD  8  

For MOSI, MISO, SCK, use the right pins of your Arduino board. In Uno they are 11, 12 and 13 respectively.

The basic sketch is as following:

#include <SPI.h>
#include <SlimTFT.h>

SlimTFT TFTscreen;

void setup(){
  TFTscreen.begin();
  TFTscreen.drawBMP("filename.bmp",0,0);
}
void loop(){

}
