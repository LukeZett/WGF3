#pragma once
#include "BaseTexture.h"


namespace WGF
{
	class Texture1D : public BaseTexture
	{
	public:
		Texture1D() {
			SetDefaultDescriptor<1>();
			SetDefaultViewDescriptor<1>();
		};

		inline void Init(uint32_t size);

		inline void Write(void* bytes,
			uint32_t offset,
			uint32_t size,
			size_t pixelSize, uint32_t miplevel = 0);
	};

	inline void Texture1D::Init(uint32_t size)
	{
		i_Init(size, 1, 1);
	}

	inline void Texture1D::Write(void* bytes, uint32_t offset, uint32_t size, size_t pixelSize, uint32_t miplevel)
	{
		i_Write(bytes, glm::uvec3(offset, 0, 0), glm::uvec3(size, 1, 1), pixelSize, miplevel);
	}
};
