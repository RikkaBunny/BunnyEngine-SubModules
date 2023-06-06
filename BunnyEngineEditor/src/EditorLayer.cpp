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

        m_NodeEditorPanel.OnAttcah();

        
    }

    void EditorLayer::OnDetach()
    {
        m_DockSpace.OnDetach();
        //m_NodeEditorPanel.OnDetach();
    }

    void EditorLayer::OnUpdate()
    {
        BE_PROFILE_FUNCTION();

        m_DockSpace.OnUpdate();
        m_NodeEditorPanel.OnUpdate();

        
        m_DockSpace.GetViewportPanel()->GetFramebuffer()->Bind();
            
        BE_PROFILE_SCOPE("Renderer::OnUpdate");
        RenderCommand::Clear({ 0.0,0.0,0.0,1 });

        m_DockSpace.GetViewportPanel()->GetFramebuffer()->ClearAttachment(1, 0);
        m_DockSpace.GetViewportPanel()->GetFramebuffer()->ClearAttachment(2, 0);
        m_DockSpace.GetViewportPanel()->GetFramebuffer()->ClearAttachment(3, 0);
        //m_ActiveScene->OnUpdateRuntime();
        m_DockSpace.GetActiveScene()->OnUpdateEditor(m_DockSpace.GetViewportPanel()->GetEditorCamera());
        
        m_DockSpace.GetViewportPanel()->OnUpdate();

        m_DockSpace.GetViewportPanel()->GetFramebuffer()->UnBind();

        RenderCommand::Postprocess();
        m_DockSpace.GetViewportPanel()->GetFramebuffer(1)->Bind();
        RenderCommand::Clear({ 0.0,0.0,0.0,0.0 });
        Renderer2D::DrawScreenVisibleBuffer(m_DockSpace.GetViewportPanel()->GetFramebuffer().get(), (int)OutBuffer::GetOutBuffer());
        m_DockSpace.GetViewportPanel()->GetFramebuffer(1)->UnBind();
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
        if (m_DockSpace.GetViewportPanel()->GetHoveredEntity())
            //name = m_DockSpace.GetViewportPanel()->GetHoveredEntity().GetComponent<NameComponent>().Name;
        ImGui::Text("Hovered Entity : %s", name.c_str());
        float pixelData = m_DockSpace.GetViewportPanel()->GetPixelData();
        //BE_CORE_INFO("Current Pixel Data : {0}", pixelData);
        ImGui::Text("Current Pixel Data : %d", pixelData);
        Ref<Texture2D> bule = Texture2D::Create("Resources/Icons/ContentBrowser/Blue.png");
        ImGui::End();
        m_NodeEditorPanel.OnImGuiRender();

    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);

        m_DockSpace.OnEvent(event);

        m_DockSpace.GetViewportPanel()->OnEvent(event);

    }

    

}