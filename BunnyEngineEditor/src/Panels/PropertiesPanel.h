#pragma once

#include "BunnyEngine.h"

namespace BE {
	class DockSpace;
	class PropertiesPanel
	{
	public:
		PropertiesPanel() {};
		//SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(DockSpace* context);

		void OnImGuiRenderer();


	private:

		void DrawComponents(Entity entity);



	private:


		DockSpace* m_Context = nullptr;
		Entity m_SelectionContext;
		Ref<Texture2D> m_BEIcon = Texture2D::Create("Resources/Icons/Common/BunnyIcon1.png");
		float m_ThumbnailSize = 80.0f;
		friend class Scene;
	};
}


