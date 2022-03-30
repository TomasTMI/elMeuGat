// C:\Users\USER\Documents\Arduino\libraries\TFT_eSPI\User_Setup.h

#define ILI9341_DRIVER       // Generic driver for common displays

#define TFT_PARALLEL_8_BIT

#define TFT_CS   32		// 33  // Chip select control pin (library pulls permanently low
#define TFT_DC   5		// 15  // Data Command control pin - must use a pin in the range 0-31
#define TFT_RST  26		// 32  // Reset pin, toggles on startup

#define TFT_WR   27   // 4  // Write strobe control pin - must use a pin in the range 0-31
#define TFT_RD   25   // 2  // Read strobe control pin

#define TFT_D0   16		// 12  // Must use pins in the range 0-31 for the data bus
#define TFT_D1   4		// 13  // so a single register write sets/clears all bits.
#define TFT_D2   23		// 26  // Pins can be randomly assigned, this does not affect
#define TFT_D3   22		// 25  // TFT screen update performance.
#define TFT_D4   21		// 17
#define TFT_D5   19		// 16
#define TFT_D6   18		// 27
#define TFT_D7   17		// 14

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
#define SMOOTH_FONT
