#pragma once
#include <webgpu/webgpu.h>
#include "glm/glm.hpp"

namespace WGF
{
	class DeviceLimits
	{
		friend class Framework;
		friend class Device;

		WGPURequiredLimits m_limits = {};
	public:
        DeviceLimits() { DefaultLimits(); }

		inline DeviceLimits& SetVertexRequiredLimits(uint16_t maxVertexAttribs, uint16_t maxVertexBuffers, uint64_t maxBufferSize, uint16_t maxStride);
		inline DeviceLimits& SetInterShaderStageLimits(uint32_t maxInterStageShaderComponents, uint32_t maxInterStageShaderVariables);
		inline DeviceLimits& SetBindGroupsLimits(uint16_t maxBindGroups, uint16_t maxUniformBuffersPerShader, uint64_t maxUniformSize, uint32_t maxGroupBindings);
		inline DeviceLimits& SetStorageTextureLimits(uint16_t maxTextures);
		inline DeviceLimits& SetTextureLimits(uint16_t height, uint16_t width, uint16_t depth, uint16_t maxTextures, uint16_t textureArrayLayers);
		inline DeviceLimits& SetSamplersLimits(uint32_t maxSamplers, uint32_t maxSampledTextures);
        inline DeviceLimits& SetComputeLimits(const glm::uvec3& workGroupSize, uint32_t maxWGStorageSize, uint32_t maxWorkGroupsPerDim, uint32_t maxInvPerWG)
        {
            m_limits.limits.maxComputeWorkgroupSizeX = workGroupSize.x;
            m_limits.limits.maxComputeWorkgroupSizeY = workGroupSize.y;
            m_limits.limits.maxComputeWorkgroupSizeZ = workGroupSize.z;
            m_limits.limits.maxComputeInvocationsPerWorkgroup = maxInvPerWG;
            m_limits.limits.maxComputeWorkgroupStorageSize = maxWGStorageSize;
            m_limits.limits.maxComputeWorkgroupsPerDimension = maxWorkGroupsPerDim;
            return *this;
            
        }

        inline DeviceLimits& SetStorageBufferLimits(uint32_t maxStorageBuffers, uint64_t maxStorageBufferSize, uint32_t maxDynamicStorageBuffers)
        {
            m_limits.limits.maxStorageBuffersPerShaderStage = maxStorageBuffers;
            m_limits.limits.maxStorageBufferBindingSize = maxStorageBufferSize;
            m_limits.limits.maxDynamicStorageBuffersPerPipelineLayout = maxDynamicStorageBuffers;
            return *this;

        }

        inline DeviceLimits& DefaultLimits();

		~DeviceLimits() = default;
	};

    DeviceLimits& DeviceLimits::SetVertexRequiredLimits(uint16_t maxVertexAttribs, uint16_t maxVertexBuffers, uint64_t maxBufferSize, uint16_t maxStride)
    {
        m_limits.limits.maxVertexAttributes = maxVertexAttribs;
        m_limits.limits.maxVertexBuffers = maxVertexBuffers;
        m_limits.limits.maxBufferSize = maxBufferSize;
        m_limits.limits.maxVertexBufferArrayStride = maxStride;
        return *this;
    }

    DeviceLimits& DeviceLimits::SetInterShaderStageLimits(uint32_t maxInterStageShaderComponents, uint32_t maxInterStageShaderVariables)
    {
        m_limits.limits.maxInterStageShaderComponents = maxInterStageShaderComponents;
        m_limits.limits.maxInterStageShaderVariables = maxInterStageShaderVariables;
        return *this;
    }

    DeviceLimits& DeviceLimits::SetBindGroupsLimits(uint16_t maxBindGroups, uint16_t maxUniformBuffersPerShader, uint64_t maxUniformSize, uint32_t maxGroupBindings)
    {
        m_limits.limits.maxBindGroups = maxBindGroups;
        m_limits.limits.maxUniformBufferBindingSize = maxUniformSize; // 16 * float / 4 * vec4f / 1 matrix4f
        m_limits.limits.maxUniformBuffersPerShaderStage = maxUniformBuffersPerShader;
        m_limits.limits.maxBindingsPerBindGroup = maxGroupBindings;
        return *this;
    }

    DeviceLimits& DeviceLimits::SetStorageTextureLimits(uint16_t maxTextures)
    {
        m_limits.limits.maxStorageTexturesPerShaderStage = maxTextures;
        return *this;
    }

    DeviceLimits& DeviceLimits::SetSamplersLimits(uint32_t maxSamplers, uint32_t maxSampledTextures)
    {
        m_limits.limits.maxSamplersPerShaderStage = maxSamplers;
        m_limits.limits.maxSampledTexturesPerShaderStage = maxSampledTextures;
        return *this;
    }

    DeviceLimits& DeviceLimits::SetTextureLimits(uint16_t height, uint16_t width, uint16_t depth, uint16_t maxTextures, uint16_t textureArrayLayers)
    {
        m_limits.limits.maxSampledTexturesPerShaderStage = maxTextures;
        m_limits.limits.maxTextureDimension1D = height;
        m_limits.limits.maxTextureDimension2D = width;
        m_limits.limits.maxTextureDimension3D = depth;
        m_limits.limits.maxTextureArrayLayers = textureArrayLayers;
        return *this;
    }

    DeviceLimits& DeviceLimits::DefaultLimits()
    {
        m_limits.nextInChain = nullptr;
        m_limits.limits.maxTextureDimension1D = 0;
        m_limits.limits.maxTextureDimension2D = 0;
        m_limits.limits.maxTextureDimension3D = 0;
        m_limits.limits.maxBindGroups = 1;
        m_limits.limits.maxUniformBuffersPerShaderStage = 1;
        m_limits.limits.maxUniformBufferBindingSize = 64;
        return *this;
    }


}