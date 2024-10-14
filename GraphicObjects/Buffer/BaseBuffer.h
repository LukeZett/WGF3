#pragma once
#include <webgpu/webgpu.h>
#include "Device.h"
#include "../Utilities/Logging.h"

namespace WGF
{
	/**
	* @brief Simple GPU buffer with minimal functionality
	* and simple non-templated interface
	*/
	class BaseBuffer
	{
#ifdef WEBGPU_BACKEND_DAWN
		using WGPUBufferUsageFlags = WGPUBufferUsage;
#endif // WEBGPU_BACKEND_DAWN

		WGPUBuffer m_buffer = nullptr;
		WGPUBufferDescriptor m_bufferDesc = {};
	public:
		BaseBuffer() = default;

		BaseBuffer(WGPUBufferUsageFlags usage, uint64_t size)
		{
			m_bufferDesc.nextInChain = nullptr;
			m_bufferDesc.label = "GPU buffer";
			m_bufferDesc.usage = usage;
			m_bufferDesc.mappedAtCreation = false;
			if ((usage | size) == 0) return;
			Allocate(size);
		}

		BaseBuffer(WGPUBufferUsageFlags usage, uint64_t size, void* data) : BaseBuffer(usage, size)
		{
			Write(data, 0, size);
		}

		~BaseBuffer()
		{
			if (m_buffer == nullptr) return;
			wgpuBufferDestroy(m_buffer);
			wgpuBufferRelease(m_buffer);
		}

		BaseBuffer(const BaseBuffer& other)
		{
			LOG_DEBUG(std::string("GPU Buffer created through copy."));
			this->m_bufferDesc = other.m_bufferDesc;
			Allocate(other.Size());
			if (Device::GetEncoder() == nullptr)
			{
				Device::CreateEncoder();
				wgpuCommandEncoderCopyBufferToBuffer(Device::GetEncoder(), other.GetBufferHandle(), 0, this->m_buffer, 0, m_bufferDesc.size);
				Device::ReleaseEncoder();
			}
			else
			{
				wgpuCommandEncoderCopyBufferToBuffer(Device::GetEncoder(), other.GetBufferHandle(), 0, this->m_buffer, 0, m_bufferDesc.size);
			}
		}

		BaseBuffer(BaseBuffer&& other) noexcept
		{
			this->m_buffer = other.m_buffer;
			this->m_bufferDesc = other.m_bufferDesc;
			other.m_buffer = nullptr;
		}

		BaseBuffer& operator=(const BaseBuffer& other)
		{
			if (this == &other)
			{
				return *this;
			}
			this->m_bufferDesc = other.m_bufferDesc;
			BaseBuffer temp(other);
			std::swap(m_buffer, temp.m_buffer);
			return *this;

		}

		BaseBuffer& operator=(BaseBuffer&& other) noexcept
		{
			this->m_bufferDesc = other.m_bufferDesc;
			this->m_buffer = other.m_buffer;
			other.m_buffer = nullptr;
			return *this;
		}

		WGPUBuffer GetBufferHandle() const { return m_buffer; }

		const WGPUBufferDescriptor& GetDesc() const { return m_bufferDesc; }

		void Write(const void* data, uint64_t offset, uint64_t size)
		{
			wgpuQueueWriteBuffer(Device::GetQueue(), m_buffer, offset, data, size);
		}

		uint64_t Size() const { return m_bufferDesc.size; };

		void CopyTo(BaseBuffer& dst) const
		{
			if (Device::GetEncoder() == nullptr)
			{
				Device::CreateEncoder();
				wgpuCommandEncoderCopyBufferToBuffer(Device::GetEncoder(), GetBufferHandle(), 0, dst.m_buffer, 0, m_bufferDesc.size);
				Device::ReleaseEncoder();
			}
			else
			{
				wgpuCommandEncoderCopyBufferToBuffer(Device::GetEncoder(), GetBufferHandle(), 0, dst.m_buffer, 0, m_bufferDesc.size);
			}
		}


	private:
		void Allocate(uint64_t size)
		{
			m_bufferDesc.size = size;
			m_buffer = wgpuDeviceCreateBuffer(Device::Get(), &m_bufferDesc);
		}
	};
}