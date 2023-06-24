#pragma once

namespace BE {
	class IBL
	{
	public:
		virtual void RenderIBL() = 0;
		virtual void RenderIBL(std::string hdrTextureFilePath) = 0;
		virtual void BindEnvIrradianceCubeMap(int index = 0) = 0;
		virtual void BindPrefilteredCubeMap(int index = 0) = 0;
		//virtual void BindSkyboxCubeMap(int index = 0) = 0;

	public:
		static Ref<IBL> Create(const std::string& path);
	};
}


