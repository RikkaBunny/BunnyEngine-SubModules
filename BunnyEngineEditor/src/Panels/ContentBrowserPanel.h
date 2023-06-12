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
		void ContentBrowser();
		void ContentBrowserBreviary();
		void FilesTree(std::filesystem::path filePath);

	private:
		DockSpace* m_Context;

		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DocumentIcon;
		Ref<Texture2D> m_FolderIcon;
	};

}