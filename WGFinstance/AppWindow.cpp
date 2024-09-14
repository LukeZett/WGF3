#include "AppWindow.h"
#include "../Utilities/Logging.h"
#include "Adapter.h"
#include "Device.h"

using namespace WGF;

AppWindow::AppWindow()
{
}

bool AppWindow::Init(const WindowParameters& options)
{
	if (!glfwInit()) {
		LOG_ERROR("Could not initialize GLFW!");
		return false;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(options.m_width, options.m_height, options.m_name.c_str(), NULL, NULL);
	if (!m_window) {
		LOG_ERROR("Window creation failed! ");
		return false;
	}
	else LOG_INFO("GLFW window initialized");
	
	m_surfaceConfig = options.GetSurfaceConfig();
	

	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, onWindowResize);
	
	glfwSetKeyCallback(m_window, onKey);
	glfwSetDropCallback(m_window, onFileDrop);
	glfwSetMouseButtonCallback(m_window, onMouseClick);
	glfwSetCursorPosCallback(m_window, onMouseMove);
	return true;
}

void WGF::AppWindow::ConfigureSurface(WGPUAdapter adapter)
{
	m_surfaceConfig.format = wgpuSurfaceGetPreferredFormat(m_surface, adapter);
	m_surfaceConfig.viewFormatCount = 0;
	m_surfaceConfig.viewFormats = nullptr;
	m_surfaceConfig.device = Device::Get();
	wgpuSurfaceConfigure(m_surface, &m_surfaceConfig);
}

WGPUTextureView WGF::AppWindow::GetNextSurfaceTextureView()
{
	WGPUSurfaceTexture surfaceTexture;
	wgpuSurfaceGetCurrentTexture(m_surface, &surfaceTexture);
	if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success) {
		return nullptr;
	}
	WGPUTextureViewDescriptor viewDescriptor;
	viewDescriptor.nextInChain = nullptr;
	viewDescriptor.label = "Surface texture view";
	viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
	viewDescriptor.dimension = WGPUTextureViewDimension_2D;
	viewDescriptor.baseMipLevel = 0;
	viewDescriptor.mipLevelCount = 1;
	viewDescriptor.baseArrayLayer = 0;
	viewDescriptor.arrayLayerCount = 1;
	viewDescriptor.aspect = WGPUTextureAspect_All;
	WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);
	return targetView;

}



void AppWindow::ToggleFullScreen()
{
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videomode = glfwGetVideoMode(monitor);
	if (m_windowMode == Fullscreen) {
		m_windowMode = Windowed;
		m_surfaceConfig.width = videomode->width / 2;
		m_surfaceConfig.height = videomode->height / 2;
		glfwSetWindowMonitor(m_window, NULL, Width() / 2, Height() / 2, Width(), Height(), videomode->refreshRate);
	}
	else if (m_windowMode == Windowed)
	{
		m_surfaceConfig.width = videomode->width;
		m_surfaceConfig.height = videomode->height;
		m_windowMode = Fullscreen;
		glfwSetWindowMonitor(m_window, monitor, 0, 0, Width(), Height(), videomode->refreshRate);
	}
}

void AppWindow::SetCursorMode(CursorMode mode)
{
	switch (mode)
	{
	case Visible:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case Hidden:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	case HiddenRaw:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported()) glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		break;
	default:
		break;
	}
}

AppWindow::~AppWindow()
{
	wgpuSurfaceUnconfigure(m_surface);
	if (m_surface) wgpuSurfaceRelease(m_surface);
	if (m_window) glfwDestroyWindow(m_window);
	glfwTerminate();
}

void WGF::AppWindow::OnResize(int width, int height)
{
	m_surfaceConfig.height = height;
	m_surfaceConfig.width = width;
	wgpuSurfaceUnconfigure(m_surface);
	wgpuSurfaceConfigure(m_surface, &m_surfaceConfig);

	if (m_useDepth)
	{
		m_depthTexture.Init(Width(), Height());
		m_depthTexture.CreateView();
	}

	if (m_resizeHandler != nullptr)
	{
		ResizeEvent e(width, height);
		m_resizeHandler(e);
	}
}

void AppWindow::OnKey(int key, int scancode, int action, int mods)
{
	KeyboardEvent e(key, scancode, action, mods);
	if (key == GLFW_KEY_F11) { // F11 is used as fullscreen toggle and is not propagated further
		if (action == GLFW_PRESS) ToggleFullScreen();
	}
	else if (m_keyboardHandler != nullptr)
	{
		m_keyboardHandler(e);
	}
}

void AppWindow::OnFileDrop(int count, const char** paths)
{
	FileDropEvent e(count, paths);
	if (m_fileDropHandler != nullptr)
	{
		m_fileDropHandler(e);
	}
}

void AppWindow::OnMouseMove(double xpos, double ypos)
{
	if (m_mouseMoveHandler)
	{
		auto e = MouseMoveFactory::CreateMouseMove({ xpos, ypos });
		m_mouseMoveHandler(e);
	}
}

void AppWindow::OnMouseClick(int key, int action, int mods)
{
	if (m_mouseClickHandler) {
		MouseButtonEvent e;
		e.action = action;
		e.button = key;
		e.mods = mods;
		glfwGetCursorPos(m_window, &e.mousePos.x, &e.mousePos.y);
		m_mouseClickHandler(e);
	}
}

void WGF::AppWindow::UseDepth(DepthTexFormat format)
{
	m_useDepth = true;
	WGPUTextureFormat f = static_cast<WGPUTextureFormat>(format);
	WGPUTextureDescriptor& desc = m_depthTexture.GetDescriptor();
	desc.format = f;
	desc.usage = WGPUTextureUsage_RenderAttachment;
	desc.viewFormatCount = 1;
	desc.viewFormats = &desc.format;


	WGPUTextureViewDescriptor& viewDesc = m_depthTexture.GetViewDesc();

	viewDesc.aspect = WGPUTextureAspect_DepthOnly;
	viewDesc.format = f;

	m_depthTexture.Init(Width(), Height());
	m_depthTexture.CreateView();
}
