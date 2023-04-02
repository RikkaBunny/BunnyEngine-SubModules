#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BunnyEngine/Scene/SceneSerializer.h"
#include "BunnyEngine/Utils/PlatformUtils.h"
#include "BunnyEngine/Math/Math.h"

#include "../imgui/imgui.h"

#include "../ImGuizmo/ImGuizmo.h"

namespace BE {
    EditorLayer::EditorLayer()
        :Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {

    }

    void EditorLayer::OnAttcah()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
        m_Framebuffer = Framebuffer::Create(fbSpec);
        m_Texture = Texture2D::Create("assets/textures/UVChecker_21.png");

        m_ActiveScene = std::make_shared<Scene>();
        m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 10000.0f);

#if 0
        m_SquareEntity = m_ActiveScene->CreateEntity("SquareA");
        //BE_CORE_INFO("hastransform {0}", m_SquareEntity.HasComponent<TransformComponent>());
        //square.AddComponent<TransformComponent>();
        //BE_CORE_INFO("hastransform111 {0}", square.HasComponent<TransformComponent>());

        m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f,1.0f,0.0f,1.0f });
        m_ActiveScene->CreateEntity("SquareB").AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f,1.0f,0.0f,1.0f });
        /*      m_ActiveScene->Reg().emplace<TransformComponent>(squareEntity);
        m_ActiveScene->Reg().emplace<SpriteRendererComponent>(squareEntity, glm::vec4{ 0.0f,1.0f,0.0f,1.0f });*/

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();

        class CameraControll : public ScriptableEntity {
        public:

            void OnDestory() {

            }
            void OnUpdate() {
                auto& Translation = GetComponent<TransformComponent>().Translation;
                float speed = 5.0f;

                if (Input::IsKeyPressed(BE_KEY_A))
                    Translation[0] -= speed * Timestep::GetSeconds();
                if (Input::IsKeyPressed(BE_KEY_D))
                    Translation[0] += speed * Timestep::GetSeconds();
                if (Input::IsKeyPressed(BE_KEY_W))
                    Translation[1] += speed * Timestep::GetSeconds();
                if (Input::IsKeyPressed(BE_KEY_S))
                    Translation[1] -= speed * Timestep::GetSeconds();

            }
        };

        m_CameraEntity.AddComponent<ScriptComponent>().Bind<CameraControll>();
#endif
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        
    }

    void EditorLayer::OnDetach()
    {
    }

    float angle = 0;
    void EditorLayer::OnUpdate()
    {
        BE_PROFILE_FUNCTION();

        //Resize
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {

            m_CameraController.OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        {
            BE_PROFILE_SCOPE("CameraController::OnUpdate");
            if (m_ViewportFocused) {
                m_CameraController.OnUpdate();
            }
            m_EditorCamera.OnUpdate();
            m_EditorCamera.SetViewportFocused(m_ViewportFocused);
        }
        {
            m_Framebuffer->Bind();
            
            BE_PROFILE_SCOPE("Renderer::OnUpdate");
            RenderCommand::Clear({ 0.0,0.0,0.0,1 });

            //m_ActiveScene->OnUpdateRuntime();
            m_ActiveScene->OnUpdateEditor(m_EditorCamera);

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
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 300.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
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

            ImGui::EndMenuBar();
        }

        ImGui::End();

        m_SceneHierarchyPanel.OnImGuiRenderer();

  //      ImGui::Begin("Settings");
  //      ImGui::Separator();
  ///*      auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
  //      auto& squareName = m_SquareEntity.GetComponent<NameComponent>().Name;
  //      ImGui::Text("%s", squareName.c_str());
  //      ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));*/

  //      ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Translation));

  //      auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
  //      float orthoSize = camera.GetOrthographicSize();
  //      if (ImGui::DragFloat("Camera Ortho Size", &orthoSize)) {
  //          camera.SetOrthographicSize(orthoSize);
  //      }

  //      /*  uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
  //        ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });*/
  //      ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        //Application::Get().GetImGuiLayer().BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
        Application::Get().GetImGuiLayer().BlockEvents(!m_ViewportFocused );


        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(1);
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        //Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            //Camera
            //auto cameraEntity = m_ActiveScene->GetMainCameraEntity();
            //const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            //const glm::mat4& cameraProjection = camera.GetProjection();
            //glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            //Editor Camera 
            
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
            //Entity Transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                ImGuizmo::OPERATION(m_GizmoType), ImGuizmo::LOCAL, glm::value_ptr(transform));

            if (ImGuizmo::IsUsing()) {
                m_EditorCamera.SetViewportFocused(false);
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::End();
        
        ImGui::PopStyleVar();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
        m_EditorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
        if(event.GetRepeatCount() > 0)
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

                //Gizmos
        case BE_KEY_T: {
            if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE) {
                m_GizmoType = -1;
            }
            else
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        case BE_KEY_R: {
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) {
                m_GizmoType = -1;
            }
            else
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case BE_KEY_E: {
            if (m_GizmoType == ImGuizmo::OPERATION::SCALE) {
                m_GizmoType = -1;
            }
            else
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
            break;
        }
        default:
            break;
        }
    }
    void EditorLayer::NewScene()
    {
        //TODO Save Scene
        m_ActiveScene = std::make_shared<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("BunnyEngine Scene (*.BunnyEngine)\0*.BunnyEngine\0*");
        if (!filepath.empty()) {
            m_ActiveScene = std::make_shared<Scene>();

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);

            m_SceneHierarchyPanel.SetContext(m_ActiveScene);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
    }
    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("BunnyEngine Scene (*.BunnyEngine)\0*.BunnyEngine\0*");
        if (!filepath.empty()) {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }
}