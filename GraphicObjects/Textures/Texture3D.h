#pragma once
#include "BaseTexture.h"

namespace WGF
{

	class Texture3D : public BaseTexture
	{
	public:
		Texture3D() {
			SetDefaultDescriptor<3>();
			SetDefaultViewDescriptor<3>();
		};

		inline void Init(uint32_t width, uint32_t height, uint32_t depth);

		inline void Write(const void* bytes,
			const glm::vec<3, uint32_t>& offset,
			const glm::vec<3, uint32_t>& size,
			size_t pixelSize, uint32_t miplevel = 0);
	};

	inline void Texture3D::Init(uint32_t width, uint32_t height, uint32_t depth)
	{
		i_Init(width, height, depth);
	}

	inline void Texture3D::Write(const void* bytes, const glm::vec<3, uint32_t>& offset, const glm::vec<3, uint32_t>& size, size_t pixelSize, uint32_t miplevel)
	{
		i_Write(bytes, offset, size, pixelSize, miplevel);
	}
}