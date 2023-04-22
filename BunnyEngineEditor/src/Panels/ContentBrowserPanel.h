#pragma once
#include<filesystem>
#include "BunnyEngine.h"

namespace BE {
	class DockSpace;
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		
		void SetContext(DockSpace* context);

		void OnImGuiRenderer();

	private:
		DockSpace* m_Context;

		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DocumentIcon;
		Ref<Texture2D> m_FolderIcon;
	};

}