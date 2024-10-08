#pragma once


#include "GLFW/glfw3.h"
#include <iostream>
#include "webgpu/webgpu.h"
#include "glfw3webgpu.h"
#include "Events.h"
#include <functional>
#include "WindowParameters.h"
#include "Textures/DepthTexture.h"

#define RESIZE_CALLBACK(method, object) (std::function<void(WGF::ResizeEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define KEYBOARD_CALLBACK(method, object) (std::function<void(WGF::KeyboardEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define FILEDROP_CALLBACK(method, object) (std::function<void(WGF::FileDropEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define MOUSEMOVE_CALLBACK(method, object) (std::function<void(WGF::MouseMoveEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define MOUSEBUTTON_CALLBACK(method, object) (std::function<void(WGF::MouseButtonEvent&)>)std::bind(&method, &object, std::placeholders::_1)

namespace WGF
{
	enum WindowMode
	{
		Fullscreen,
		WindowedFullScreen,
		Windowed
	};

	enum DepthTexFormat {
		Depth24Plus = WGPUTextureFormat_Depth24Plus,
		Depth24Stencil8 = WGPUTextureFormat_Depth24PlusStencil8,
		Depth32 = WGPUTextureFormat_Depth32Float,
		Depth16UNorm = WGPUTextureFormat_Depth16Unorm,
		Depth32Stencil8 = WGPUTextureFormat_Depth32FloatStencil8
	};

	enum CursorMode
	{
		Visible,
		Hidden,
		HiddenRaw
	};

	class AppWindow
	{
		WindowMode m_windowMode = Windowed;
		WGPUSurface m_surface = nullptr;
		WGPUSurfaceConfiguration m_surfaceConfig = {};
		DepthTexture m_depthTexture;
		GLFWwindow* m_window = nullptr;
		
		std::function<void(FileDropEvent&)> m_fileDropHandler = nullptr;
		std::function<void(ResizeEvent&)> m_resizeHandler = nullptr;
		std::function<void(KeyboardEvent&)> m_keyboardHandler = nullptr;
		std::function<void(MouseButtonEvent&)> m_mouseClickHandler = nullptr;
		std::function<void(MouseMoveEvent&)> m_mouseMoveHandler = nullptr;

		bool m_useDepth = false;
	public:
		/**
		* @brief Create unintialized window instance
		*/
		AppWindow();

		/**
		* @brief Initialize window with glfw
		*/
		bool Init(const WindowParameters& options);
	
		/**
		* @brief returns window's close flag
		*/
		inline bool ShouldClose() { return glfwWindowShouldClose(m_window); }

		/**
		* @brief Process events such as user input
		*/
		inline void PollEvents() { return glfwPollEvents(); }

		/**
		* @brief initialize surface for drawing
		* @param instance initialized webgpu instance
		*/
		inline void InitSurface(WGPUInstance instance) { m_surface = glfwGetWGPUSurface(instance, m_window); }

		void ConfigureSurface(WGPUAdapter adapter);

		WGPUTextureView GetNextSurfaceTextureView();

		inline GLFWwindow* GetGLFWwin() { return m_window; }

		inline WGPUSurface GetSurface() const { return m_surface; }

		inline const WGPUTextureFormat& GetTextureFormat() const { return m_surfaceConfig.format; }
		
		inline int Width() const { return m_surfaceConfig.width; }

		inline int Height() const { return m_surfaceConfig.height; }

		inline bool Minimized() const { return Width() == 0 || Height() == 0; }

		inline void PresentSurface() const { wgpuSurfacePresent(m_surface); }

		inline std::tuple<int, int> GetWindowSize() const { return { Width(), Height()}; }

		inline float GetAspectRatio() const { return Width() / (float)Height(); }

		void ToggleFullScreen();

		void SetCursorMode(CursorMode mode);

		~AppWindow();

		inline void SetResizeCallback(std::function<void(ResizeEvent&)> callback) { m_resizeHandler = callback; }

		inline void SetKeyCallback(std::function<void(KeyboardEvent&)> callback) { m_keyboardHandler = callback; }

		inline void SetFileDropCallback(std::function<void(FileDropEvent&)> callback) { m_fileDropHandler = callback; }

		inline void SetMouseMoveCallback(std::function<void(MouseMoveEvent&)> callback) { m_mouseMoveHandler = callback; }

		inline void SetMouseClickCallback(std::function<void(MouseButtonEvent&)> callback) { m_mouseClickHandler = callback; }

#pragma region eventHandlers

		void OnResize(int width, int height);

		void OnKey(int key, int scancode, int action, int mods);

		void OnFileDrop(int count, const char** paths);

		void OnMouseMove(double xpos, double ypos);

		void OnMouseClick(int key, int action, int mods);

#pragma endregion

		void UseDepth(DepthTexFormat format = Depth24Plus);

		WGPUTextureFormat GetDepthFormat() { return m_depthTexture.GetDescriptor().format; }

		WGPUTextureView GetDepthTexView() { return m_depthTexture.GetView(); }
	};

	//helper callbacks for GLFW
	inline void onWindowResize(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		app->OnResize(width, height);
	}
	

	inline void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		app->OnKey(key, scancode, action, mods);
	}

	inline void onFileDrop(GLFWwindow* window, int count, const char** paths)
	{
		auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		app->OnFileDrop(count, paths);
	}

	inline void onMouseMove(GLFWwindow* window, double xpos, double ypos)
	{
		auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		app->OnMouseMove(xpos, ypos);
	}

	inline void onMouseClick(GLFWwindow* window, int button, int action, int mods)
	{
		auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		app->OnMouseClick(button, action, mods);
	}
}


