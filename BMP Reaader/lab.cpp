// takes in bitmap file outputs inverted/flipped bitmap file
#include <cstdio>
#include <cstdint>

// pragma pack disallows the additional padding
// 3 structs
#pragma pack(push, 2)
struct BitmapFileHeader {
	uint16_t type; // 2 bytes
	uint32_t size; //4 bytes
	uint16_t reservedOne; //2 bytes
	uint16_t reservedTwo; //2 bytes
	uint32_t offset; //4 bytes
 // total 14 bytes
};
struct BitmapInfoHeader {
	uint32_t size; //4 bytes
	uint32_t width; //4 bytes
	uint32_t height; //4 bytes
	uint16_t planes; //2 bytes
	uint16_t bitcount; //2 bytes
	uint32_t compression; //4 bytes
	uint32_t imagesize; //4 bytes
	uint32_t x_pixels_per_meter; //4 bytes
	uint32_t  y_pixels_per_meter; //4 bytes
	uint32_t color_used; //4 bytes
	uint32_t color_important; //4 bytes
 // total 40 bytes
};
struct Pixel {
	uint8_t blue; // 1 byte
	uint8_t green; // 1 byte
	uint8_t red; // 1 byte
 // total 3 bytes
};
#pragma pack(pop)



int main(int argc, char* argv[]) {
	// argc and argv to use user input

	if (3 != argc)
	{
		// checks if the user input were valid
		printf("Usage: %s <source bitmap file>  <output bitmap file>\n", argv[0]);
		return 1;
	}


	FILE* fl;
	fl = fopen(argv[1], "rb");
	//if the file does not open the user is notified the bmp file did not open(fail)
	if (nullptr == fl) {
		printf("Unable to open file %s \n", argv[1]);
		return -1;
	}

	// struct BitmapFileHeader initials = BFH
	BitmapFileHeader BFH;
	// struct BitmapInfoHeader initials = BIF
	BitmapInfoHeader BIF;

	// since the file starts at 0, the program reads right away
	// read sizeof(BitmapFileHeader) bytes at buffer BFH from file opened
	fread(&BFH, 1, sizeof(BitmapFileHeader), fl);

	// after reading the FileHeader, the file should be at the beginning of the InfoHeader
	// read sizeof(BitmapInfoHeader) bytes at buffer BIF from file opened
	fread(&BIF, 1, sizeof(BitmapInfoHeader), fl);


	// at this point the program can calculate the padding
	// padding = 4 - (width*3 % 4)
	int px_pad = 4 - (BIF.width * sizeof(Pixel) % 4);

	// data allocation to store pixels [width*height] = [# of pixels]
	Pixel* Pin = new Pixel[(BIF.width * BIF.height)];

	// to prevent the case of the offset not being after the 54th bit
	// I used the offset data to make sure I start reading at the right place
	fseek(fl, BFH.offset, SEEK_SET);


	// number of iterations = height, read row by row
	for (unsigned int i = 0; i < BIF.height; i++)
	{
		// start reading (3*width) bytes at memory Pin[width*current height]
		// fseek skips the padding, so the program is at the pixel information again
		fread(&Pin[BIF.width * i], 1, sizeof(Pixel) * BIF.width, fl);
		fseek(fl, px_pad, SEEK_CUR);
	}

	// the file opened is not needed anymore => close
	fclose(fl);



	// inverter
 // goes pixel by pixel
	for (unsigned int i = 0; i < BIF.height; i++) {
		//  0 -> height
		for (unsigned int j = 0; j < BIF.width; j++) {
			// 0 -> width
			// Ex. Start at Pixel height 0, width 0
			// inverts the Pixel
			// goes to the next width (repeat to the # of width)
			// the inner for loop finishes and height 1 now
			// inverts the Pixel
			// goes to the next width (repeat to the # of width)
			// repeat
			Pin[i * BIF.width + j].blue = 255 - Pin[i * BIF.width + j].blue;
			Pin[i * BIF.width + j].green = 255 - Pin[i * BIF.width + j].green;
			Pin[i * BIF.width + j].red = 255 - Pin[i * BIF.width + j].red;
		}
	}


	//flipper
 // 3 byte structure temporary | used for data swopping
	Pixel temporary;
	for (unsigned int i = 0; i < BIF.height; i++) {
		for (unsigned int j = 0; j < BIF.width / 2; j++) {

			// ex.
			// j is the current width position f(x,y) = (j , height)
			// I figured the location of the opposite Pixel is at g(x,y) = ( width - 1 - j , height)

			// to swop the information 
			// the Pixel information at (j , height) is stored at temporary 
			// Pixel address of (j , height) stores the pixel information at ( width - 1 - j , height)
			// the Pixel information temporarily saved is stored at the pixel address of ( width - 1 - j , height)
      // looped till halfway 
      // repeats for each height

			temporary = Pin[i * BIF.width + j];
			Pin[i * BIF.width + j] = Pin[BIF.width - 1 - j + i * BIF.width];
			Pin[BIF.width - 1 - j + i * BIF.width] = temporary;

		}
	}



	// attempts to open output file
	// if it doesn't open the program creates a file
	FILE* floutput;
	floutput = fopen(argv[2], "wb");
	if (nullptr == floutput) {
		printf("Unable to open file %s \n", argv[2]);
		return 1;
	}



	// since the file starts at 0, the program writes right away
	// write sizeof(BitmapFileHeader) bytes from buffer BFH to file opened
	fwrite(&BFH, 1, sizeof(BitmapFileHeader), floutput);

	// after writing the FileHeader, the file should be at the beginning of the InfoHeader
	// write sizeof(BitmapInfoHeader) bytes from buffer BIF to file opened
	fwrite(&BIF, 1, sizeof(BitmapInfoHeader), floutput);



	// to prevent the case of the offset not being after the 54th bit
	// I used the offset data to make sure
	fseek(floutput, BFH.offset, SEEK_SET);


	// I needed a buffer that had 0's to write on the padding
  // the size of char array will be the number of bytes of padding needed 
	char paddingInformation[px_pad];
	for (int i = 0; i < px_pad; i++)
	{
		paddingInformation[i] = 0;
	}

	for (unsigned int i = 0; i < BIF.height; i++)
	{
		// start writing (3*width) bytes at file opened from Pin[width*current height]
	   // next line writes the padding
	   // repeat
		fwrite(&Pin[BIF.width * i], 1, sizeof(Pixel) * BIF.width, floutput);
		fwrite(paddingInformation, 1, px_pad, floutput);
	}

	// file opened is not neeeded => close
	// Pointer is not needed => delete[]
	fclose(floutput);
	delete[] Pin;

	return 0;
}


