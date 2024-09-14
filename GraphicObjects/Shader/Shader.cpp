#include "Shader.h"

using namespace WGF;

Shader::Shader()
{
	SetDesc();
	m_codeDesc.code =
		"@vertex																			  "
		"fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {"
		"	var p = vec2f(0.0, 0.0);														  "
		"	if (in_vertex_index == 0u) {													  "
		"		p = vec2f(-0.5, -0.5);														  "
		"	}																				  "
		"	else if (in_vertex_index == 1u) {												  "
		"		p = vec2f(0.5, -0.5);														  "
		"	}																				  "
		"	else {																			  "
		"		p = vec2f(0.0, 0.5);														  "
		"	}																				  "
		"	return vec4f(p, 0.0, 1.0);														  "
		"}																					  "
		"																					  "
		"@fragment																			  "
		"fn fs_main() -> @location(0) vec4f {												  "
		"	return vec4f(0.8, 0.2, 1.0, 1.0);												  "
		"}																					  ";
	// Connect the chain
	m_desc.nextInChain = &m_codeDesc.chain;
	m_shaderModule = wgpuDeviceCreateShaderModule(Device::Get(), &m_desc);
}


void WGF::Shader::SetDesc()
{
#ifdef WEBGPU_BACKEND_WGPU
	m_desc.hintCount = 0;
	m_desc.hints = nullptr;
#endif
	m_desc.nextInChain = &m_codeDesc.chain;
	m_codeDesc.chain.next = nullptr;
	m_codeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
}

Shader::Shader(const std::filesystem::path& source)
{
	SetDesc();
	m_source = source;
	ReloadModule();
}


Shader::~Shader()
{
	if (m_shaderModule == nullptr) return;
	wgpuShaderModuleRelease(m_shaderModule);
}

void WGF::Shader::ReloadModule()
{
	if (m_shaderModule != nullptr) wgpuShaderModuleRelease(m_shaderModule);

	std::ifstream ifs(m_source);
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	m_codeDesc.code = content.c_str();

	// Connect the chain
	m_desc.nextInChain = &m_codeDesc.chain;
	m_shaderModule = wgpuDeviceCreateShaderModule(Device::Get(), &m_desc);

}


inline std::string Shader::LoadFromPath(const std::filesystem::path& path)
{
	std::ifstream ifs(path);
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return content;
}