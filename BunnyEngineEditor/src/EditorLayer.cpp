#include "EditorLayer.h"

#include "../imgui/imgui.h"



namespace BE {
    
    extern const std::filesystem::path g_AssetsPath;

    EditorLayer::EditorLayer()
        :Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {

    }

    void EditorLayer::OnAttcah() 
    {
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F,FramebufferTextureFormat::RGBA16F,FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

        //m_DockSpace = new DockSpace(){};
        m_DockSpace.SetContext(&m_SceneHierarchyPanel, &m_ContentBrowserPanel, &m_ViewportPanel);

        m_SceneHierarchyPanel.SetContext(&m_DockSpace);

        m_ContentBrowserPanel.SetContext(&m_DockSpace);

        m_ViewportPanel.SetContext(&m_DockSpace, fbSpec);


        
    }

    void EditorLayer::OnDetach()
    {
        m_DockSpace.OnDetach();
    }

    void EditorLayer::OnUpdate()
    {
        BE_PROFILE_FUNCTION();

        m_DockSpace.OnUpdate();


        
        m_DockSpace.GetViewportPanel()->GetFramebuffer()->Bind();
            
        BE_PROFILE_SCOPE("Renderer::OnUpdate");
        RenderCommand::Clear({ 0.0,0.0,0.0,1 });

        m_DockSpace.GetViewportPanel()->GetFramebuffer()->ClearAttachment(1, -1);
        //m_ActiveScene->OnUpdateRuntime();
        m_DockSpace.GetActiveScene()->OnUpdateEditor(m_DockSpace.GetViewportPanel()->GetEditorCamera());

        m_DockSpace.GetViewportPanel()->OnUpdate();
            
        m_DockSpace.GetViewportPanel()->GetFramebuffer()->UnBind();

        

    }

    void EditorLayer::OnImGuiRender()
    {
        //Panel Render
        m_DockSpace.OnImGuiRender();

        m_DockSpace.GetSceneHierarchyPanel()->OnImGuiRenderer();
        m_DockSpace.GetContentBrowserPanel()->OnImGuiRenderer();
        m_DockSpace.GetViewportPanel()->OnImGuiRenderer();

        ImGui::Begin("Stats");
        ImGui::Separator();
        std::string name = "None";
        if(m_DockSpace.GetViewportPanel()->GetHoveredEntity())
            //name = m_DockSpace.GetViewportPanel()->GetHoveredEntity().GetComponent<NameComponent>().Name;
        ImGui::Text("Hovered Entity : %s", name.c_str());
        ImGui::End();

    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);

        m_DockSpace.OnEvent(event);

        m_DockSpace.GetViewportPanel()->OnEvent(event);

    }

    

}