#include "Texture2D.h"
#include <vector>
#include "Device.h"

#pragma warning(push)
#pragma warning(disable : 4244)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

using namespace WGF;

void Texture2D::Init(uint32_t width, uint32_t height)
{
    i_Init(width, height, 1);
}

void Texture2D::Init(const std::filesystem::path& path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    char* data = (char*)stbi_load(((path).generic_string()).data(), &width, &height, &channels, 4);

    channels = 4;
    SetDefaultDescriptor<2>();
    SetDefaultViewDescriptor<2>();

    i_Init(width, height, 1);
    i_Write(data, { 0,0,0 }, { width, height, 1 }, channels);

    stbi_image_free(data);
}