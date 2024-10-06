#pragma once
#include <filesystem>
#include "BaseTexture.h"

namespace WGF
{

	class Texture2D : public BaseTexture
	{
	public:
		Texture2D() {
			SetDefaultDescriptor<2>();
			SetDefaultViewDescriptor<2>();
		};

		void Init(uint32_t width, uint32_t height);

		void Init(const std::filesystem::path& path);

		inline void Write(void* bytes,
			const glm::vec<2, uint32_t>& offset,
			const glm::vec<2, uint32_t>& size,
			size_t pixelSize, uint32_t miplevel = 0);
	};

	inline void Texture2D::Write(void* bytes, const glm::vec<2, uint32_t>& offset, const glm::vec<2, uint32_t>& size, size_t pixelSize, uint32_t miplevel)
	{
		i_Write(bytes, { offset, 1 }, { size, 1 }, pixelSize, miplevel);
	}
}