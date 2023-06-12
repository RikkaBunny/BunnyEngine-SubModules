#pragma once

#include "BunnyEngine.h"

namespace BE {
	class DockSpace;
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() {};
		//SceneHierarchyPanel(const Ref<Scene>& context);
		
		void SetContext(DockSpace* context);

		void OnImGuiRenderer();

		Entity GetSelectedEntity() { return m_SelectionContext; };
		void SetSelectedEntity(Entity entity) {  m_SelectionContext = entity; };

	private:
		void DrawEntityNode(Entity entity);

		void DrawComponents(Entity entity);



	private:
		

		DockSpace* m_Context = nullptr;
		Entity m_SelectionContext;
		Ref<Texture2D> m_BEIcon = Texture2D::Create("Resources/Icons/Common/BunnyIcon1.png");
		float m_ThumbnailSize = 80.0f;
		friend class Scene;
	};
}
