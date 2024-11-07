#pragma once
#include <webgpu/webgpu.h>
#include "../Utilities/Logging.h"
#include "DeviceLimits.h"
#include "Adapter.h"
#include "Device.h"
#include "AppWindow.h"
#include "memory"
#include "RenderPassFactory.h"

namespace WGF
{
	class Framework
	{
	public:

		static bool Init(const DeviceLimits& deviceLimits, [[maybe_unused]] const WindowParameters& windowParameters)
		{
			if (!s_instance.CreateInstance()) return false;
			s_instance.m_appWindow = std::make_unique<AppWindow>();
			s_instance.m_appWindow->Init(windowParameters);
			s_instance.m_appWindow->InitSurface(s_instance.m_wgpuInstance);
			if (Adapter::Init(s_instance.m_wgpuInstance, s_instance.m_appWindow->GetSurface())
				&& Device::Init(Adapter::Get(), deviceLimits))
			{
				s_instance.m_appWindow->ConfigureSurface(Adapter::Get());
				return true;
			}
			return false;
		}

		static bool InitHeadless(const DeviceLimits& deviceLimits)
		{
			return	s_instance.CreateInstance()
					&& Adapter::Init(s_instance.m_wgpuInstance, NULL)
					&& Device::Init(Adapter::Get(), deviceLimits);
		}

		static AppWindow* Window()
		{
			return s_instance.m_appWindow.get();
		}

		static void Finish()
		{
			Device::Release();
			Adapter::Release();
			wgpuInstanceRelease(s_instance.m_wgpuInstance);
			LOG_INFO("WGPU uninitialized");
		}

		static bool ShouldClose()
		{
			return s_instance.m_appWindow->ShouldClose();
		}

	private:
		static Framework s_instance;

	private:
		WGPUInstance m_wgpuInstance = nullptr;
		std::unique_ptr<AppWindow> m_appWindow = nullptr;

	private:
		bool CreateInstance();

		Framework() {}
		~Framework() {}
	};
}