#pragma once
#include <glm/glm.hpp>

namespace BE {
	struct LightSpot {
		float cosPhyInner;
		float cosPhyOutter;
		glm::vec3 lightSpotPos;
		glm::vec3 lightSpotColor;
		glm::vec3 lightSpotDir;
	};
	struct LightPoint {
		float constant;
		float linear;
		float quadratic;
		glm::vec3 lightPointPos;
		glm::vec3 lightPointColor;
	};
	struct LightDirection {
		glm::vec4 lightColor;
		glm::vec4 lightDir;
		glm::vec4 lightDirPos;
	};
	struct Light1F{
		glm::vec3 lightColor;
	};
	class Light
	{
		Light();
	};

}
