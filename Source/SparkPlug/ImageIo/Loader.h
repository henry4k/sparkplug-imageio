#ifndef __SPARKPLUG_IMAGEIO_LOADER__
#define __SPARKPLUG_IMAGEIO_LOADER__

#include <SparkPlug/Image.h>

namespace SparkPlug
{
namespace ImageIo
{

enum ImageFormat
{
	ImageFormat_Unknown,
	ImageFormat_Png
};
const char* AsString( ImageFormat format );

/**
 * Tries to load the specified image file.
 * When format is left out or set to unknown,
 * it tries to guess the format from files extension.
 */
bool LoadFromFile( Image* out, const char* file, ImageFormat format = ImageFormat_Unknown );

}
}

#endif
