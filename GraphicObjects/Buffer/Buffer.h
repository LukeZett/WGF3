#pragma once
#include "BaseBuffer.h"
#include <vector>

namespace WGF
{
	template<typename T>
	class Buffer
	{
#ifdef WEBGPU_BACKEND_DAWN
		using WGPUBufferUsageFlags = WGPUBufferUsage;
#endif // WEBGPU_BACKEND_DAWN

	protected:
		BaseBuffer m_buffer;
		uint64_t m_size = 0;
	public:
		Buffer() = default;
		
		Buffer(WGPUBufferUsageFlags usage, uint64_t capacity) : m_buffer(usage, capacity * sizeof(T)) {}

		uint64_t Capacity() const { return m_buffer.Size() / sizeof(T); }

		uint64_t Size() const { return m_size; }

		WGPUBuffer GetBufferHandle() { return m_buffer.GetBufferHandle(); }

#pragma region BufferOperations

		void Write(const std::vector<T>& data)
		{
			Write(&data[0], data.size());
		}

		void Write(const std::vector<T>& data, uint64_t offset)
		{
			Write(&data[0], data.size(), offset);
		}

		void Append(const std::vector<T>& data)
		{
			Append(&data[0], data.size());
		}

		void Write(const T* data, uint64_t size)
		{
			CheckCapacity(size);
			m_size = size;
			m_buffer.Write(data, 0, size * sizeof(T));
		}

		void Write(const T* data, uint64_t size, uint64_t offset)
		{
			CheckCapacity(size + offset);
			m_size = std::max(size + offset, m_size);
			m_buffer.Write(data, offset * sizeof(T), size * sizeof(T));
		}

		void Append(const T* data, uint64_t size)
		{
			CheckCapacity(size + m_size);
			m_size += size;
			m_buffer.Write(data, Size() * sizeof(T), size * sizeof(T));
		}

		void CheckCapacity(uint64_t newSize)
		{
			if (newSize <= Capacity())
			{
				return;
			}
			uint64_t newCapacity = Capacity() + 1;
			while(newSize > newCapacity)
			{
				newCapacity *= 2;
			}
			Realloc(newCapacity);
		}

		void Realloc(uint64_t newCapacity)
		{
			BaseBuffer newBuffer = BaseBuffer(this->m_buffer.GetDesc().usage, newCapacity);
			m_buffer.CopyTo(newBuffer);
			m_buffer = newBuffer;
		}

#pragma endregion



	private:

	};

}