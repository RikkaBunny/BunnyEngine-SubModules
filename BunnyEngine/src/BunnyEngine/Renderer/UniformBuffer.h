#pragma once
#include "BEpch.h"
#include <map>
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace BE {

	class UniformBuffer {
	public:

		//static Ref<OpenGLUniformBuffer> CreateUniformBuffer(std::string name, int uniformPoint);
		//static Ref<UniformBuffer> CreateUniformBuffer(std::string name, int uniformPoint);
		template<typename T>
		static void CreateUniformBuffer(std::string name, const T& buffer) {
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return;
			case RendererAPI::API::OpenGL:
				unsigned int uniformPoint = UniformBufferPoint(name);
				s_OpenGLUniformBuffer->SetUniformBuffer<T>(buffer, uniformPoint);
				return;
			}

			BE_CORE_ASSERT(false, "Unknow RendererAPI!");
		}


		static unsigned int UniformBufferPoint(std::string name) {
			for (unsigned int i = 0; i < s_UniformBufferList.size(); i++) {
				if (name == s_UniformBufferList[i]) {
					return i;
				}
			}
			s_UniformBufferList.push_back(name);
			return (s_UniformBufferList.size()-1);
		}

	private:

		//static std::map<std::string, unsigned int> s_UniformBufferPoint;
		static std::vector<std::string> s_UniformBufferList;
		static OpenGLUniformBuffer* s_OpenGLUniformBuffer;

	};

}