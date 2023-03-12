#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../imgui/imgui.h"

#include <chrono>

namespace BE {
    EditorLayer::EditorLayer()
        :Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {

    }

    void EditorLayer::OnAttcah()
    {
        BE::FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = BE::Framebuffer::Create(fbSpec);
        m_Texture = BE::Texture2D::Create("assets/textures/UVChecker_21.png");

        m_ActiveScene = std::make_shared<Scene>();

        auto square = m_ActiveScene->CreateEntity("Square");
        BE_CORE_INFO("hastransform {0}", square.HasComponent<TransformComponent>());
        //square.AddComponent<TransformComponent>();
        //BE_CORE_INFO("hastransform111 {0}", square.HasComponent<TransformComponent>());

        square.AddComponent<SpriteRenderComponent>(glm::vec4{ 0.0f,1.0f,0.0f,1.0f });
        /*      m_ActiveScene->Reg().emplace<TransformComponent>(squareEntity);
        m_ActiveScene->Reg().emplace<SpriteRenderComponent>(squareEntity, glm::vec4{ 0.0f,1.0f,0.0f,1.0f });*/
        m_SquareEntity = square;

    }

    void EditorLayer::OnDetach()
    {
    }

    float angle = 0;
    void EditorLayer::OnUpdate()
    {
        BE_PROFILE_FUNCTION();

        {
            BE_PROFILE_SCOPE("CameraController::OnUpdate");
            if(m_ViewportFocused)
                m_CameraController.OnUpdate();
        }
        {
            m_Framebuffer->Bind();

            BE_PROFILE_SCOPE("Renderer::OnUpdate");
            BE::RenderCommand::Clear({ 0.8,0.2,0.3,1 });

            BE::Renderer2D::BeginScene(m_CameraController.GetCamera());

            //BE::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.2f, 0.8f }, m_SquareColor);
            //angle += 1.0f;
            //BE::Renderer2D::DrawQuad({ 1.0f, 0.0f, }, { 0.5f, 0.5f }, glm::radians(angle), m_SquareColor);

            //BE::Renderer2D::DrawQuad({ 1.0f, 0.5f, }, { 10.0f, 10.0f }, m_Texture, { 0.5f,0.8f,0.3f,1.0f }, { 10.0f, 10.0f });
            m_ActiveScene->OnUpdate();

            BE::Renderer2D::EndScene();

            m_Framebuffer->UnBind();

        }

    }

    void EditorLayer::OnImGuiRender()
    {


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
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
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
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }


        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                if (ImGui::MenuItem("Exit")) { BE::Application::Get().Close(); }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::Separator();
        auto& squareColor = m_SquareEntity.GetComponent<SpriteRenderComponent>().Color;
        auto& squareName = m_SquareEntity.GetComponent<NameComponent>().Name;
        ImGui::Text("%s", squareName.c_str());
        ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

        /*  uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
          ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });*/
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer().BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (m_ViewportSize != glm::vec2(viewportPanelSize.x, viewportPanelSize.y) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
            m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        }
        ImGui::End();
        
        ImGui::PopStyleVar();
    }

    void EditorLayer::OnEvent(BE::Event& event)
    {
        m_CameraController.OnEvent(event);
    }
}