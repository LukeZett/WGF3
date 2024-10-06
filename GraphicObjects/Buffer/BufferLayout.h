#pragma once
#include <vector>
#include "webgpu/webgpu.h"
#include "../Utilities/Logging.h"
#include <memory>

namespace WGF
{
	struct BufferElement
	{
		WGPUVertexFormat format;
		uint16_t elementSize = 0;
		uint16_t elementoffset = 0;
		BufferElement(WGPUVertexFormat format, uint16_t elementSize = 0, uint16_t elementoffset = 0)
		{
			this->format = format;
			this->elementSize = elementSize;
			this->elementoffset = elementoffset;
		}
	};


	class BufferLayout
	{
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
		WGPUVertexStepMode m_mode;
	public:
		inline BufferLayout(bool instanced);

		inline void PrintLayout();

		inline void GenerateVertexBufferLayout(WGPUVertexBufferLayout& vertexBufferLayout, WGPUVertexAttribute*& vertexAttributes, uint16_t shaderLocOffset = 0)
		{
			vertexAttributes = new WGPUVertexAttribute[m_elements.size()];

			for (size_t i = 0; i < m_elements.size(); i++)
			{
				vertexAttributes[i].format = m_elements[i].format;
				vertexAttributes[i].shaderLocation = (uint32_t)i + shaderLocOffset;
				vertexAttributes[i].offset = m_elements[i].elementoffset;
			}

			vertexBufferLayout.attributeCount = m_elements.size();
			vertexBufferLayout.attributes = vertexAttributes;
			vertexBufferLayout.stepMode = m_mode;
			vertexBufferLayout.arrayStride = m_stride;
		}

		template<typename T>
		inline BufferLayout& AddElement(int count) { LOG_ERROR("Unsupported vertex attribute type, skipping"); return *this; }

		template<>
		inline BufferLayout& AddElement<int>(int count)
		{
			i_AddElement<WGPUVertexFormat_Sint32, int>(count);
			return *this;
		}

		template<>
		inline BufferLayout& AddElement<unsigned int>(int count)
		{
			i_AddElement<WGPUVertexFormat_Uint32, unsigned int>(count);
			return *this;
		}

		template<>
		inline BufferLayout& AddElement<float>(int count)
		{
			i_AddElement<WGPUVertexFormat_Float32, float>(count);
			return *this;
		}

		~BufferLayout() = default;

	private:

		template<WGPUVertexFormat Format, typename T>
		inline void i_AddElement(int count) {
			if (count > 4)
			{
				LOG_ERROR("Unsupported vertex attribute count, skipping");
				return;
			}
			// (Format + count - 1) is for choosing the correct vector type (defined by count)
			BufferElement element((WGPUVertexFormat)(Format + count - 1), (uint16_t)sizeof(T) * (uint16_t)count, (uint16_t)m_stride);
			m_elements.push_back(element);
			m_stride += element.elementSize;
		}
	};

	inline BufferLayout::BufferLayout(bool instanced)
	{
		if (!instanced) m_mode = WGPUVertexStepMode_Vertex;
		else m_mode = WGPUVertexStepMode_Instance;
	}
	inline void BufferLayout::PrintLayout()
	{
		if (m_mode & WGPUVertexStepMode_Instance)
			LOG_INFO("Requested buffer layout: Instanced");
		else
			LOG_INFO("Requested buffer layout: Not Instanced");

		for (size_t i = 0; i < m_elements.size(); i++)
		{
			auto& elem = m_elements[i];

			LOG_INFO(std::string("Element: Size ") + std::to_string(elem.elementSize) + "B, offset: " + std::to_string(elem.elementoffset) + "B");
		}
	}
}


