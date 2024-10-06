#pragma once
#include "BaseTexture.h"
#include "Image.h"

namespace WGF
{
	class Texture2DArray : public BaseTexture
	{
	public:
		inline Texture2DArray();
		
		inline void Init(uint32_t width, uint32_t height, uint32_t depth);

		inline void Init(const std::filesystem::path& path, uint32_t depth);

		inline void Write(void* bytes,
			const glm::vec<2, uint32_t>& offset,
			const glm::vec<2, uint32_t>& size,
			uint32_t index,
			size_t pixelSize, uint32_t miplevel = 0);

		inline void LoadFromFile(const std::filesystem::path& path, uint32_t index);
	};

	Texture2DArray::Texture2DArray()
	{
		SetDefaultDescriptor<2>();
		SetDefaultViewDescriptor<20>();
	}
	inline void Texture2DArray::Init(uint32_t width, uint32_t height, uint32_t depth)
	{
		m_viewDesc.arrayLayerCount = depth;
		i_Init(width, height, depth);
	}
	inline void Texture2DArray::Init(const std::filesystem::path& path, uint32_t depth)
	{
		m_viewDesc.arrayLayerCount = depth;
		Image image(path);
		i_Init(image.Width(), image.Height(), depth);
		i_Write(image.Data(), { 0, 0, 0 }, { image.Width(), image.Height(), 1 }, image.Channels());
	}
	inline void Texture2DArray::Write(void* bytes, const glm::vec<2, uint32_t>& offset, const glm::vec<2, uint32_t>& size, uint32_t index, size_t pixelSize, uint32_t miplevel)
	{
		i_Write(bytes, { offset.x, offset.y, index }, { size.x, size.y, 1 }, pixelSize, miplevel);
	}
	inline void Texture2DArray::LoadFromFile(const std::filesystem::path& path, uint32_t index)
	{
		Image image(path);
		i_Write(image.Data(), { 0, 0, index }, { image.Width(), image.Height(), 1 }, image.Channels());
	}
}