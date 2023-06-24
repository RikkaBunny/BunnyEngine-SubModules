#include "BEpch.h"
#include "OpenGLIBL.h"

#include <glad/glad.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "stb_image_write.h"
#include "BunnyEngine/Renderer/Texture.h"
#include "BunnyEngine/Renderer/Buffer.h"
#include "BunnyEngine/Renderer/VertexArray.h"
#include "BunnyEngine/Renderer/RenderCommand.h"

namespace BE {

	float skyboxVertices[] = {
	    -1.0, -1.0, -1.0,  // 0
		 1.0, -1.0, -1.0,  // 1
		 1.0,  1.0, -1.0,  // 2
		-1.0,  1.0, -1.0,  // 3
		-1.0, -1.0,  1.0,  // 4
		 1.0, -1.0,  1.0,  // 5
		 1.0,  1.0,  1.0,  // 6
		-1.0,  1.0,  1.0   // 7
	};
	uint32_t skyboxIndices[] = {
		0, 1, 2, 2, 3, 0,   // front
		1, 5, 6, 6, 2, 1,   // right
		5, 4, 7, 7, 6, 5,   // back
		4, 0, 3, 3, 7, 4,   // left
		3, 2, 6, 6, 7, 3,   // top
		4, 5, 1, 1, 0, 4    // bottom
	};
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};


	OpenGLIBL::OpenGLIBL(std::string hdrTextureFilePath)
		:m_HdrTextureFilePath(hdrTextureFilePath)
	{
		RenderIBL();
	}

	void OpenGLIBL::RenderIBL()
	{
		//RenderHdrEnvCubemap();
		RenderHdrEnvIrradianceCubemap();
		RenderHdrPrefilterCubemap();
	}

	void OpenGLIBL::RenderIBL(std::string hdrTextureFilePath)
	{
		m_HdrTextureFilePath = hdrTextureFilePath;
		RenderIBL();
	}

	//渲染HDR环境天空盒
	void OpenGLIBL::RenderHdrEnvCubemap() {
		auto hdrSkyBoxShader = Shader::Create("Assets/shaders/IBL_CubeMapBaker.glsl");
		auto hdrTexture = Texture2D::Create(m_HdrTextureFilePath);
		auto m_VertexArray = VertexArray::Create();

		auto m_VertexBuffer = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));

		BufferLayout layout = { { ShaderDataType::Float3, "a_Position" } };

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		auto m_IndexBuffer = IndexBuffer::Create(skyboxIndices, sizeof(skyboxIndices)/sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		int width = 1024;
		int height = 1024;
		
		unsigned int captureFBO, envCubemap;
		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glGenTextures(1, &envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		
		hdrSkyBoxShader->Bind();
		hdrSkyBoxShader->SetInt("u_EquirectangularMap", 0);
		hdrSkyBoxShader->SetMat4("u_ProjMat", captureProjection);
		
		for (unsigned int i = 0; i < 6; ++i)
		{
			hdrTexture->Bind();
			glViewport(0, 0, width, height);
			glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
			hdrSkyBoxShader->SetMat4("u_ViewMat", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RenderCommand::DrawIndexed(m_VertexArray);

			//float* pixels = new float[width * height * 3];
			//glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, pixels);
			//char filename[256];
			//sprintf(filename, "Assets/IBL/Skybox%d.hdr", i);
			////stbi_flip_vertically_on_write(true);
			//int result = stbi_write_hdr(filename, width, height, 3, pixels);
			//free(pixels);
		}
		m_SkyboxCubeMap = envCubemap;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	//渲染HDR环境漫反射天空盒
	void OpenGLIBL::RenderHdrEnvIrradianceCubemap() {
		auto hdrSkyBoxShader = Shader::Create("Assets/shaders/IBL_CubeMapIrradianceBaker.glsl");
		auto hdrTexture = Texture2D::Create(m_HdrTextureFilePath);
		auto m_VertexArray = VertexArray::Create();

		auto m_VertexBuffer = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));

		BufferLayout layout = { { ShaderDataType::Float3, "a_Position" } };

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		auto m_IndexBuffer = IndexBuffer::Create(skyboxIndices, sizeof(skyboxIndices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		int width = 32;
		int height = 32;

		unsigned int captureFBO, envCubemap;
		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO); 
		glGenTextures(1, &envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		
		hdrSkyBoxShader->Bind();
		hdrSkyBoxShader->SetInt("u_Skybox", 0);
		//BindSkyboxCubeMap();
		
		hdrSkyBoxShader->SetMat4("u_ProjMat", captureProjection);

		for (unsigned int i = 0; i < 6; ++i)
		{
			hdrTexture->Bind();
			glViewport(0, 0, width, height);
			glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
			hdrSkyBoxShader->SetMat4("u_ViewMat", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RenderCommand::DrawIndexed(m_VertexArray);
			//输出为HDR贴图
			//float* pixels = new float[width * height * 3];
			//glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, pixels);
			//char filename[256];
			//sprintf(filename, "Assets/IBL/SkyboxIrradiance%d.hdr", i);
			//stbi_flip_vertically_on_write(true);
			//int result = stbi_write_hdr(filename, width, height, 3, pixels);
			//free(pixels);
		}
		m_EnvIrradianceCubeMap = envCubemap;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	//预渲染BRDF LUT（IBL BRDF部分）
	void OpenGLIBL::RenderBRDFLUT(std::string hdrTextureFilePath) {

		auto BRDFLUTShader = Shader::Create("Assets/shaders/IBL_BRDFLUTBaker.glsl");
		float vertices[4 * 5] = {
			-1.0f, -1.0f, 0.0f, 0.0f,0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f,0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f,1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f,1.0f
		};
		auto m_VertexArray = VertexArray::Create();
		Ref<VertexBuffer> m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		int width = 512;
		int height = 512;

		unsigned int captureFBO;
		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		unsigned int brdfLUTTexture;
		glGenTextures(1, &brdfLUTTexture);

		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

		glViewport(0, 0, width, height);
		//brdfLUTShader->use();
		BRDFLUTShader->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderCommand::DrawIndexed(m_VertexArray);

		//输出HDR贴图
		stbi_flip_vertically_on_write(1);
		float* pixels = new float[width * height * 3];
		glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, pixels);
		int result = stbi_write_hdr("Assets/IBL/BRDFLUT.hdr", width, height, 3, pixels);
		free(pixels);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//预滤波环境贴图（IBL镜面入射部分）
	void OpenGLIBL::RenderHdrPrefilterCubemap() {
		auto hdrSkyBoxShader = Shader::Create("Assets/shaders/IBL_CubeMapPrefilterBaker.glsl");
		auto hdrTexture = Texture2D::Create(m_HdrTextureFilePath);
		auto m_VertexArray = VertexArray::Create();

		auto m_VertexBuffer = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));

		BufferLayout layout = { { ShaderDataType::Float3, "a_Position" } };

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		auto m_IndexBuffer = IndexBuffer::Create(skyboxIndices, sizeof(skyboxIndices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		int width = 128;
		int height = 128;

		unsigned int captureFBO, envCubemap;
		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glGenTextures(1, &envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		hdrSkyBoxShader->Bind();
		hdrSkyBoxShader->SetInt("u_Skybox", 0);
		//BindSkyboxCubeMap();
		
		hdrSkyBoxShader->SetMat4("u_ProjMat", captureProjection);
		
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; mip++) {

			unsigned int mipWidth = width * std::pow(0.5, mip);
			unsigned int mipHeight = height * std::pow(0.5, mip);
			float roughness = (float)mip / (float)(maxMipLevels - 0.9999);
			hdrSkyBoxShader->SetFloat("u_Roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				hdrTexture->Bind();
				glViewport(0, 0, mipWidth, mipHeight);
				glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
				hdrSkyBoxShader->SetMat4("u_ViewMat", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, mip);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				RenderCommand::DrawIndexed(m_VertexArray);

				//输出HDR贴图
				//float* pixels = new float[mipWidth * mipHeight * 3];
				//glReadPixels(0, 0, mipWidth, mipHeight, GL_RGB, GL_FLOAT, pixels);
				//char filename[256];
				//sprintf(filename, "Assets/IBL/SkyboxPrefilter%d.hdr", i);
				//stbi_flip_vertically_on_write(true);
				//int result = stbi_write_hdr(filename, mipWidth, mipHeight, 3, pixels);
				//free(pixels);
			}
		}
		m_PrefilteredCubeMap = envCubemap;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//void OpenGLIBL::BindSkyboxCubeMap(int index)
	//{
	//	glBindTextureUnit(index, m_SkyboxCubeMap);
	//}

	void OpenGLIBL::BindEnvIrradianceCubeMap(int index)
	{
		glBindTextureUnit(index, m_EnvIrradianceCubeMap);
	}

	void OpenGLIBL::BindPrefilteredCubeMap(int index)
	{
		glBindTextureUnit(index, m_PrefilteredCubeMap);
	}


}

