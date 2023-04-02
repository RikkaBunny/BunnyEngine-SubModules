#pragma once

#include "BunnyEngine.h"


namespace BE {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() {};
		SceneHierarchyPanel(const Ref<Scene>& context);
		
		void SetContext(const Ref<Scene>& context);

		void OnImGuiRenderer();

		Entity GetSelectedEntity() { return m_SelectionContext; };

	private:
		void DrawEntityNode(Entity entity);

		void DrawComponents(Entity entity);



	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		friend class Scene;
	};
}
