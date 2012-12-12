#include <cstring>
#include <png.h>

#include <SparkPlug/Common.h>
#include <SparkPlug/Pixel.h>
#include <SparkPlug/ImageIo/Png.h>
#include <SparkPlug/Image.h>

namespace SparkPlug
{
namespace ImageIo
{

bool LoadPngImage( Image* out, FILE* fp )
{
	assert(out);
	assert(fp);
	
	vec3i imageSize(0, 0, 0);
	png_byte* imageData = NULL;
	PixelSemantic  pixelSemantic;
	PixelComponent pixelComponent = PixelComponent_UInt8;
	
	
	//header for testing if it is a png
	png_byte header[8];

	//read the header
	size_t read = fread(header, 1, 8, fp);
	if(read != 8 && ferror(fp))
		return false;
	
	//test if png
	if(png_sig_cmp(header, 0, 8) != 0)
	{
		fclose(fp);
		return false;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr)
	{
		fclose(fp);
		return false;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return false;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if(!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return false;
	}

	//png error stuff, not sure libpng man suggests this.
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// Only 8-bit!
	png_set_strip_16(png_ptr);

	// OGL doesn't likes indexed data. (Extensions?)
	png_set_expand(png_ptr);

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

	//update width and height based on png info
	imageSize.x = int(twidth);
	imageSize.y = int(theight);

	switch(color_type)
	{
		case PNG_COLOR_TYPE_GRAY:       pixelSemantic = PixelSemantic_Luminance; break;
		case PNG_COLOR_TYPE_GRAY_ALPHA: pixelSemantic = PixelSemantic_LuminanceAlpha; break;
		case PNG_COLOR_TYPE_RGB:        pixelSemantic = PixelSemantic_RGB; break;
		case PNG_COLOR_TYPE_RGB_ALPHA:  pixelSemantic = PixelSemantic_RGBA; break;
		case PNG_COLOR_TYPE_PALETTE:
		{
			int trans = 0;
			png_get_tRNS(png_ptr, info_ptr, NULL, &trans, NULL);
			if(trans > 0)
				pixelSemantic = PixelSemantic_RGBA;
			else
				pixelSemantic = PixelSemantic_RGB;
		} break;
		
		default:
			LogError("Something went wrong; Can't detect bpp");
	}

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the imageData as a big block, to be given to opengl
	imageData = new png_byte[rowbytes*theight];
	if(!imageData)
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	//row_pointers is for pointing to imageData for reading the png with libpng
	png_bytep* row_pointers = new png_bytep[theight];
	if(!row_pointers)
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] imageData;
		fclose(fp);
		return false;
	}

	// set the individual row_pointers to point at the correct offsets of imageData
	for(png_uint_32 i = 0; i < theight; ++i)
		row_pointers[theight - 1 - i] = imageData + i * rowbytes;

	//read the png into imageData through row_pointers
	png_read_image(png_ptr, row_pointers);

	out->initFromRaw(PixelFormat(pixelSemantic, pixelComponent), imageSize, reinterpret_cast<char*>(imageData));
	
	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;
	fclose(fp);

	return true;
}
}

}
