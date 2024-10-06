#pragma once
#include <filesystem>

namespace WGF
{
	class Image
	{
		int width, height, channels;
		char* data;
	public:

		Image(const std::filesystem::path& path);

		inline int Width() const { return width; };

		inline int Height() const { return height; };
		
		inline int Channels() const { return channels; };

		inline char* Data() const { return data; };

		~Image();
	};
}