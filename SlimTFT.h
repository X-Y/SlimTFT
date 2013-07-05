#ifndef MYTFT_H
#define MYTFT_H

#include "Arduino.h"
#include "Arduino_LCD.h"
#include "Fat16.h"

#define sd_cs  10
#define LCD_CS   3
#define DC_LCD   9
#define RST_LCD  8  

class SlimTFT:public Arduino_LCD{
	public:
		SlimTFT();
		void begin();
		void drawBMP(char* filename, uint8_t x, uint8_t y);//detect if draw with EEPROM or SD, and draw it
	private:
		SdCard card;
		Fat16 file;


};


#endif