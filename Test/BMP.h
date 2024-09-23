#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#pragma pack(push, 1)

struct BMPFileHeader {
	 uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42
	 uint32_t file_size{ 0 };               // Size of the file (in bytes)
	 uint16_t reserved1{ 0 };               // Reserved, always 0
	 uint16_t reserved2{ 0 };               // Reserved, always 0
	 uint32_t offset_data{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
	
};

struct BMPInfoHeader {
	 uint32_t size{ 0 };                      // Size of this header (in bytes)
	 int32_t width{ 0 };                      // width of bitmap in pixels
	 int32_t height{ 0 };                     // width of bitmap in pixels
	                                              //       (if positive, bottom-up, with origin in lower left corner)
		                                              //       (if negative, top-down, with origin in upper left corner)
	 uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
	 uint16_t bit_count{ 0 };                 // No. of bits per pixel
	 uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	 uint32_t size_image{ 0 };                // 0 - for uncompressed images
	 int32_t x_pixels_per_meter{ 0 };
	 int32_t y_pixels_per_meter{ 0 };
	 uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	 uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
	
};


#pragma pack(pop)

class BMP
{
	public:

		BMP(const char* fileName) {

			openBMP(fileName);
		}

		~BMP() {

			closeBMP();

		}

		void openBMP(const char* fileName) {
			// Open  file
			fileStream.open(fileName, ios::in | ios::binary);
			if (fileStream) {
				fileStream.read((char*)&file_header, sizeof(file_header));
				if (file_header.file_type != 0x4D42) 
					cout << "Error file_type " << endl;

				fileStream.seekg(sizeof(file_header), fileStream.beg);

				fileStream.read((char*)&bmp_info_header, sizeof(bmp_info_header));
				

				if (bmp_info_header.bit_count == 24) {

					// Jump to the pixel data location
					fileStream.seekg(file_header.offset_data, fileStream.beg);

					unsigned int bgr = 0;

					for (int y = bmp_info_header.height; y > 0  ; y--)
					{
						vector<char> width;

						for (int x = 0; x < bmp_info_header.width ; x++)
						{
							fileStream.read((char*)&bgr,3);

							if (bgr == 0xFFFFFF)
								width.push_back((char)254);
							else
								width.push_back(' ');

							bgr = 0;
						}
						map.push_back(width);

					}					

				}
			}
		}
		
		void displayBMP() {

			for (size_t i = map.size() - 1; i > 0; i--) {
				for (size_t j = 0 ; j < map[i].size() - 1; j++) {
					cout << map[i][j];
				}
				cout << endl;
			}
		}

		void closeBMP() {
			fileStream.close();
		}

	private:
		unsigned int bgr;
		vector<vector<char>> map;
		BMPFileHeader file_header;
		BMPInfoHeader bmp_info_header;
		ifstream fileStream;		
};

