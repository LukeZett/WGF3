#pragma once
#include <webgpu/webgpu.h>

#ifdef WEBGPU_BACKEND_DAWN
using WGPUShaderStageFlags = WGPUShaderStage;
using WGPUTextureUsageFlags = WGPUTextureUsage;
using WGPUBufferUsageFlags = WGPUBufferUsage;
#endif // WEBGPU_BACKEND_DAWN

#ifdef WEBGPU_BACKEND_WGPU
using WGPUOptionalBool = bool;
#endif