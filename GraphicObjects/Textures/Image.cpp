#include "Image.h"
#pragma warning(push)
#pragma warning(disable : 4244)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

WGF::Image::Image(const std::filesystem::path& path)
{
    stbi_set_flip_vertically_on_load(1);
    data = (char*)stbi_load(((path).generic_string()).data(), &width, &height, &channels, 4);
    channels = 4;
}

WGF::Image::~Image()
{
    stbi_image_free(data);
}
