
#include "DockSpace.h"
#include "Platform/OpenGL/OpenGLIBL.h"
#include "../EditorLayer.h"

namespace BE {
    //DockSpace::DockSpace( SceneHierarchyPanel* sceneHierarchyPanel,  ContentBrowserPanel* contentBrowserPanel)
    //    :m_SceneHierarchyPanel(sceneHierarchyPanel),m_ContentBrowserPanel(contentBrowserPanel)
    //{
    //}
    //void DockSpace::SetContext( SceneHierarchyPanel* sceneHierarchyPanel,  ContentBrowserPanel* contentBrowserPanel)
    //{
    //    m_SceneHierarchyPanel = sceneHierarchyPanel;
    //    m_ContentBrowserPanel = contentBrowserPanel;
    //}

    //DockSpace::DockSpace()
    //{
    //    FramebufferSpecification fbSpec;
    //    fbSpec.Width = 1280;
    //    fbSpec.Height = 720;
    //    fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

    //    m_SceneHierarchyPanel->SetContext(this);

    //    m_ContentBrowserPanel->SetContext(this);

    //    m_ViewportPanel_0->SetContext(this, fbSpec);
    //}

    void DockSpace::OnAttcah()
    {
        //FramebufferSpecification fbSpec;
        //fbSpec.Width = 1280;
        //fbSpec.Height = 720;
        //fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

        //m_SceneHierarchyPanel->SetContext(this);

        //m_ContentBrowserPanel->SetContext(this);

        //m_ViewportPanel_0->SetContext(this, fbSpec);

        
    }

    void DockSpace::SetContext(SceneHierarchyPanel* sceneHierarchyPanel, PropertiesPanel* propertiesPanel, ContentBrowserPanel* contentBrowserPanel, ViewportPanel* viewportPanel_0)
    {
        m_SceneHierarchyPanel = sceneHierarchyPanel;
        m_PropertiesPanel = propertiesPanel;
        m_ContentBrowserPanel = contentBrowserPanel;
        m_ViewportPanel_0 = viewportPanel_0;
    }

    void DockSpace::OnDetach()
    {
    }

    void DockSpace::OnUpdate()
    {
    }

    void DockSpace::OnImGuiRender()
    {
       

        // 获取当前的内边距
        ImVec2 currentPadding = ImGui::GetStyle().FramePadding;

        //ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Always);
        //ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Always);

        bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            // 设置新的内边距，垂直方向上的值会影响菜单栏的高度
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(currentPadding.x, m_MenuBarHeight));
            //window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(3);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 100.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        m_CurWindowPos = ImGui::GetWindowPos();
        m_CurWindowSize = ImGui::GetWindowSize();
        
        
        // 设置新的内边距，垂直方向上的值会影响菜单栏的高度
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(currentPadding.x, m_MenuBarHeight));

        if (ImGui::BeginMenuBar())
        {
            // Icon
            float frameHeight = ImGui::GetFrameHeight();
            ImGui::Image((ImTextureID)m_BEIcon->GetRendererID(), { frameHeight, frameHeight }, { 0, 1 }, { 1, 0 }, {1,0.5,0.5,1});
            //Menu
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.

                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
                    SaveSceneAs();

                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")){}
                   

                if (ImGui::MenuItem("Redo", "Ctrl+Y")){}


                if (ImGui::MenuItem("Copy", "Ctrl+C")){}


                if (ImGui::MenuItem("Cut", "Ctrl+X")){}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Import"))
            {
                if (ImGui::MenuItem("Import Mesh", "")) {
                    std::string filepath = FileDialogs::OpenFile("Mesh Files (*.obj;*.fbx)\0*.fbx;*.obj\0*");
                    if (!filepath.empty()) {
                        //MeshLoad::SerializeMesh(filepath);
                    }
                }
                if (ImGui::MenuItem("Save Mesh", "")) {
                    std::string filepath = FileDialogs::SaveFile("BunnyMesh Files (*.BunnyMesh)\0*.BunnyMesh\0*");
                    if (!filepath.empty()) {
                        //MeshLoad::DeserializeMesh(filepath);
                    }
                }
                
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Environment"))
            {
                if (ImGui::MenuItem("CubeMapBaker", "")) {
                    std::string filepath = FileDialogs::OpenFile("Texture Files (*.hdr;*.tga)\0*.tga;*.hdr\0*");
                    if (!filepath.empty()) {
                        //MeshLoad::SerializeMesh(filepath);
                        //auto hdrTexture = Texture2D::Create(filepath);
                        //OpenGLIBL::RenderBRDFLUT(filepath);
                        //OpenGLIBL::RenderHdrEnvIrradianceCubemap(filepath);
                        
                        m_ActiveScene->SetCurrentScneneIBL(IBL::Create(filepath));
                    }
                }
                if (ImGui::MenuItem("Save Mesh", "")) {
                    std::string filepath = FileDialogs::SaveFile("BunnyMesh Files (*.BunnyMesh)\0*.BunnyMesh\0*");
                    if (!filepath.empty()) {
                        //MeshLoad::DeserializeMesh(filepath);
                    }
                }

                ImGui::EndMenu();
            }

            // 添加最大化、最小化和关闭按钮
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f, 1.0f, 1.0f, 0.0f });

            float width = ImGui::GetWindowContentRegionMax().x;
            ImGui::SetCursorPosX(width - frameHeight * 3.2f);
            if (ImGui::ImageButton((ImTextureID)m_MinIcon->GetRendererID(), { frameHeight / 2.0f, frameHeight / 2.0f }, { 0, 1 }, { 1, 0 }, -1, { 1.0f, 1.0f, 1.0f, 0.0f }, { 0.5f, 0.5f, 0.5f, 1.0f })) {
                //m_WindowPos = ImGui::GetWindowPos();
                //m_WindowSize = ImGui::GetWindowSize();
                Application::Get().GetWindow().SetWindowRestore();
                //Application::Get().GetWindow().SetWindowSize((int)(m_WindowSize.x), (int)(m_WindowSize.y));
                //Application::Get().GetWindow().SetWindowPos((int)(m_WindowPos.x), (int)(m_WindowPos.y));

                //m_WindowPos = ImGui::GetWindowPos();
                //m_WindowSize = ImGui::GetWindowSize();
                Application::Get().GetWindow().SetWindowIconify();
            }

            if (ImGui::ImageButton((ImTextureID)m_MaxIcon->GetRendererID(), { frameHeight / 2.0f, frameHeight / 2.0f }, { 0, 1 }, { 1, 0 }, -1, { 1.0f, 1.0f, 1.0f, 0.0f }, { 0.5f, 0.5f, 0.5f, 1.0f })) {

                //m_WindowPos = ImGui::GetWindowPos();
                //m_WindowSize = ImGui::GetWindowSize();
                Application::Get().GetWindow().SetWindowRestore();
                //Application::Get().GetWindow().SetWindowSize((int)(m_WindowSize.x), (int)(m_WindowSize.y));
                //Application::Get().GetWindow().SetWindowPos((int)(m_WindowPos.x), (int)(m_WindowPos.y));


                Application::Get().GetWindow().SetWindowMaximize();
                //m_WindowSize = { (float)Application::Get().GetWindow().GetWidth(),(float)Application::Get().GetWindow().GetHeight() };
                //Application::Get().GetWindow().SetWindowSize((int)(m_WindowSize.x), (int)(m_WindowSize.y));
                //Application::Get().GetWindow().SetWindowPos(0, 0);
                //ImGui::SetWindowSize(m_WindowSize);
                //ImGui::SetWindowPos({ 0,0 });
            }
            if (ImGui::ImageButton((ImTextureID)m_CloseIcon->GetRendererID(), { frameHeight / 2.0f, frameHeight/2.0f }, { 0, 1 }, { 1, 0 }, -1, { 1.0f, 1.0f, 1.0f, 0.0f }, { 0.5f, 0.5f, 0.5f, 1.0f })) {
                Application::Get().Close();
                
            }
      
            ImGui::PopStyleColor();

            //在主菜单上进行窗口移动
            if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
                m_WindowPos = ImGui::GetWindowPos();
                ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
                m_WindowPos.x += delta.x;
                m_WindowPos.y += delta.y;
                ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
                
                Application::Get().GetWindow().SetWindowPos((int)(m_WindowPos.x), (int)(m_WindowPos.y));
                
                if (delta.x != 0 || delta.y != 0) {
                    m_Drag = true;
                }
                else {
                    m_Drag = false;
                }

            }
            ImGui::EndMenuBar();
        }
        ImGui::PopStyleVar();

        ImGui::End();
        
        // 根据窗口状态设置窗口大小和位置
        switch (Application::Get().GetWindow().GetWindowState()) {
        case WindowState::NORMAL:
            //窗口拖拽
            if (m_CurWindowSize.x != m_WindowSize.x || m_CurWindowSize.y != m_WindowSize.y) {
                m_WindowPos = m_CurWindowPos;
                m_WindowSize = m_CurWindowSize;
                Application::Get().GetWindow().SetWindowPos((int)(m_WindowPos.x), (int)(m_WindowPos.y));
                Application::Get().GetWindow().SetWindowSize((int)(m_WindowSize.x), (int)(m_WindowSize.y));
            }

            break;
        case WindowState::MINIMIZED:

            break;
        case WindowState::MAXIMIZED:
            if (m_Drag) {
                Application::Get().GetWindow().SetWindowRestore();
                m_Drag = false;
            }
            
            break;
        }
    }

    void DockSpace::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BE_BIND_EVENT_FN(DockSpace::OnKeyPressed));
    }


    bool DockSpace::OnKeyPressed(KeyPressedEvent& event) {
        if (event.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(BE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(BE_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(BE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(BE_KEY_RIGHT_SHIFT);
        switch (event.GetKeyCode())
        {
            //Scene 
        case BE_KEY_S: {
            if (control && shift)
                SaveSceneAs();

            break;
        }
        case BE_KEY_O: {
            if (control)
                OpenScene();

            break;
        }
        case BE_KEY_N: {
            if (control)
                NewScene();

            break;
        }

        }
        return false;
    }
    void DockSpace::NewScene()
    {
        //TODO Save Scene
         m_ActiveScene = std::make_shared<Scene>();

        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportPanel_0->GetViewportSize().x, (uint32_t)m_ViewportPanel_0->GetViewportSize().y);
         //m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_SceneHierarchyPanel->SetSelectedEntity({});
    }
    void DockSpace::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("BunnyEngine Scene (*.BunnyEngine)\0*.BunnyEngine\0*");
        if (!filepath.empty()) {
            OpenScene(filepath);
        }
    }
    void DockSpace::OpenScene(const std::filesystem::path& path)
    {
        m_ActiveScene = std::make_shared<Scene>();

        SceneSerializer serializer(m_ActiveScene);
        serializer.Deserialize(path.string());

        m_SceneHierarchyPanel->SetSelectedEntity({});
        //m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportPanel_0->GetViewportSize().x, (uint32_t)m_ViewportPanel_0->GetViewportSize().y);
    }
    void DockSpace::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("BunnyEngine Scene (*.BunnyEngine)\0*.BunnyEngine\0*");
        if (!filepath.empty()) {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }



}


