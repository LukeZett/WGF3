#pragma once
#include <string.h>
#include <webgpu/webgpu.h>

namespace WGF
{

	class AppWindow;

	class WindowParameters
	{
		friend AppWindow;
	public:
		WindowParameters(int width, int height, const std::string& name)
			: m_width(width), m_height(height), m_name(name)
		{
			config.height = m_height;
			config.width = m_width;
			config.nextInChain = nullptr;
			config.viewFormatCount = 0;
			config.viewFormats = nullptr;
			config.usage = WGPUTextureUsage_RenderAttachment;
			config.presentMode = WGPUPresentMode_Fifo;
			config.alphaMode = WGPUCompositeAlphaMode_Auto;
		}

		WindowParameters& SetPresentMode(WGPUPresentMode mode)
		{
			config.presentMode = mode;
		}

		WGPUSurfaceConfiguration GetSurfaceConfig() const
		{
			return config;
		}

		~WindowParameters() = default;

	private:
		int m_width;
		int m_height;
		WGPUSurfaceConfiguration config = {};
		std::string m_name;

	};
}