#ifndef __SPARKPLUG_IMAGEIO_PNG__
#define __SPARKPLUG_IMAGEIO_PNG__

#include <cstdio>
#include <SparkPlug/Image.h>

namespace SparkPlug
{
namespace ImageIo
{

bool LoadPngImage( Image* out, FILE* fp );

}
}

#endif
