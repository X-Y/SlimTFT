#include "SlimTFT.h"

#define BUFFPIXEL 20

uint16_t read16(Fat16& f);
uint32_t read32(Fat16& f);

SlimTFT::SlimTFT():Arduino_LCD(LCD_CS,DC_LCD,RST_LCD){

}

void SlimTFT::begin(){
	card.init();
	file.init(&card);
  
	Arduino_LCD::initB();
}

void SlimTFT::drawBMP(char* filename, uint8_t posX, uint8_t posY){
	uint8_t  bmpWidth, bmpHeight;   // W+H in pixels
	uint8_t  bmpDepth;              // Bit depth (currently must be 24)
	uint32_t bmpImageoffset;        // Start of image data in file
	uint32_t rowSize;               // Not always = bmpWidth; may have padding
	uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
	uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
	bool  goodBmp = false;       // Set to true on valid header parse
	bool  flip    = true;        // BMP is stored bottom-to-top
	uint8_t  w, h, row, col;
	uint8_t  r, g, b;
	uint32_t pos = 0;
	
	//bool success=false;

	// Open requested file on SD card
	if ((file.open(filename,O_READ)) == NULL) {
		//return false;
		return;
	}

	// Parse BMP header
	if(read16(file) == 0x4D42) { // BMP signature
		read32(file);//uint32_t aux = read32(file);
		(void)read32(file); // Read & ignore creator bytes
		bmpImageoffset = read32(file); // Start of image data
		
		// Read DIB header
		(void)read32(file);//aux = read32(file);
		bmpWidth  = read32(file);
		bmpHeight = read32(file);
		
		if(read16(file) == 1) { // # planes -- must be '1'
			bmpDepth = read16(file); // bits per pixel
			if((bmpDepth == 24) && (read32(file) == 0)) { // 0 = uncompressed
				goodBmp = true; // Supported BMP format -- proceed!

				// BMP rows are padded (if needed) to 4-byte boundary
				rowSize = (bmpWidth * 3 + 3) & ~3;

				// If bmpHeight is negative, image is in top-down order.
				// This is not canon but has been observed in the wild.
				if(bmpHeight < 0) {
					bmpHeight = -bmpHeight;
					flip      = false;
				}

				// Crop area to be loaded
				w = bmpWidth;
				h = bmpHeight;

				//  Start drawing
				//_enableLCD();
				Arduino_LCD::setAddrWindow(posX, posY, posX+bmpWidth-1, posY+bmpHeight-1);  

				for (row=0; row<h; row++) { // For each scanline...
					if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
						pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
					else     // Bitmap is stored top-to-bottom
						pos = bmpImageoffset + row * rowSize;
						
					if(file.curPosition() != pos) { // Need seek?
						//_enableSD();
						file.seekSet(pos);
						buffidx = sizeof(sdbuffer); // Force buffer reload
						//_enableLCD();
					}
					for (col=0; col<w; col++) { // For each pixel...
						// Time to read more pixel data?
						if (buffidx >= sizeof(sdbuffer)) { // Indeed
							//_enableSD();
							file.read(sdbuffer, sizeof(sdbuffer));
							buffidx = 0; // Set index to beginning
							//_enableLCD();
						}
						// Convert pixel from BMP to TFT format, push to display
						b = sdbuffer[buffidx++];
						g = sdbuffer[buffidx++];
						r = sdbuffer[buffidx++];

						int  color = Arduino_LCD::Color565(r,g,b);
						
						Arduino_LCD::pushColor(color);
					} // end pixel
				} // end scanline
				//success=true;
				//_enableSD();
			} // end goodBmp*/
		}
	}
	file.close();
	//return success;
	//_enableLCD();
}
uint16_t read16(Fat16& f) {
  uint16_t result;
  f.read(&result,sizeof(result));
  return result;
}
uint32_t read32(Fat16& f) {
  uint32_t result;
  f.read(&result,sizeof(result));
  return result;
}
