#pragma once

#include <string_view>
#include <filesystem>

namespace System
{
	bool isImage(const std::string& ext)
	{
		if (ext == ".bmp" || ".dib" || ".rle") // Bitmap Picture
			return true;

		if (ext == ".png") // Portable Network Graphics
			return true;

		if (ext == ".jpg" || ext == ".jpeg") // Joint Photographic Experts Group
			return true;

		if (ext == ".gif") // Graphics Interchange Format 
			return true;

		if (ext == ".pcx" || ext == ".pcc") // PCExchange
			return true;

		if (ext == ".ppm" || ext == ".pgm" || ext == ".pbm" || ".pnm") // Portable Anymap
			return true;

		if (ext == ".svg" || ext == ".pnm") // Scalable Vector Graphics 
			return true;

		if (ext == ".tga" || ext == ".tpic" || ext == ".vda" || ext == ".vst" || ext == ".icb")
			return true;

		if (ext == ".tif" || ext == ".tiff") // Tagged Image File Format 
			return true;

		if (ext == ".xcf") // eXperimental Computing Facility 
			return true;

		if (ext == ".xpm") // X Pixmap  
			return true;

		return false;
	}
};