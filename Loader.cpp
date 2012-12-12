#include <cstdio>
#include <SparkPlug/Common.h>
#include <SparkPlug/ImageIo/Loader.h>
#include <SparkPlug/ImageIo/Png.h>

namespace SparkPlug
{
namespace ImageIo
{

const char* AsString( ImageFormat format )
{
	switch(format)
	{
		case ImageFormat_Png:
			return "PNG";
		
		case ImageFormat_Unknown:
		default:
			return "Unknown";
	}
}

bool LoadFromFile( Image* out, const char* file, ImageFormat format )
{
	assert(out);
	
	FILE* fp = fopen(file, "rb");
	if(!fp)
	{
		LogError("Can't read file %s", file);
		return false;
	}
	
	switch(format)
	{
		case ImageFormat_Png:
			return LoadPngImage(out, fp);
		
		default:
			LogError("Image format %s is unsupported. :(", AsString(format));
			return false;
	}
}

}
}
