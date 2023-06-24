#pragma once
#include "BunnyEngine/Renderer/IBL.h"
#include "BunnyEngine/Renderer/Shader.h"


namespace BE {

	class OpenGLIBL : public IBL
	{
	public:
		OpenGLIBL() {};
		OpenGLIBL(std::string hdrTextureFilePath);
		virtual void RenderIBL() override;
		virtual void RenderIBL(std::string hdrTextureFilePath) override;
		virtual void BindEnvIrradianceCubeMap(int index = 0) override;
		virtual void BindPrefilteredCubeMap(int index = 0) override;
		//virtual void BindSkyboxCubeMap(int index = 0) override;

	public:
		static void RenderBRDFLUT(std::string hdrTextureFilePath);

	private:
		void RenderHdrEnvCubemap();
		void RenderHdrEnvIrradianceCubemap();
		void RenderHdrPrefilterCubemap();

		
	private:
		unsigned int m_SkyboxCubeMap;
		unsigned int m_EnvIrradianceCubeMap;
		unsigned int m_PrefilteredCubeMap;
		std::string m_HdrTextureFilePath;
	};

}

