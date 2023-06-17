#include "BEpch.h"
#include "UniformBuffer.h"

namespace BE {

	OpenGLUniformBuffer* UniformBuffer::s_OpenGLUniformBuffer;
	std::vector<std::string> UniformBuffer::s_UniformBufferList;
	//std::map<std::string, unsigned int> UniformBuffer::s_UniformBufferPoint;

}