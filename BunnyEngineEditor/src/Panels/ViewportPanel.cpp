#include "ViewportPanel.h"
#include "../imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../ImGuizmo/ImGuizmo.h"

#include "BunnyEngine/Math/Math.h"

#include "DockSpace.h"


namespace BE {

    extern const std::filesystem::path g_AssetsPath;

    ViewportPanel::ViewportPanel()
    {
        //fbSpec = FramebufferSpecification();
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 10000.0f);

    }

    void ViewportPanel::SetContext(DockSpace* context, const FramebufferSpecification& fbSpec, EditorCamera editorCamera)
    {
        m_Context = context;

        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 10000.0f);
        switch (Renderer::GetRenderPipeline())
        {
            case Renderer::RenderPipeline::DeferredRendering :
                FramebufferSpecification fbSpec1 = fbSpec;
                fbSpec1.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
                m_Framebuffer1 = Framebuffer::Create(fbSpec1);
                m_Framebuffer1->UnBind();
                break;
 
        }
        m_Framebuffer->Bind();
    }
    //ViewportPanel::ViewportPanel(const FramebufferSpecification& framebufferSpecification, Ref<Scene>* activeScene, SceneHierarchyPanel* sceneHierarchyPanel, EditorCamera editorCamera)
    //    :fbSpec(framebufferSpecification), m_ActiveScene(activeScene), m_EditorCamera(editorCamera), m_SceneHierarchyPanel(sceneHierarchyPanel)
    //{
    //    m_Framebuffer = Framebuffer::Create(fbSpec);

    //}
    //void ViewportPanel::SetContext(const FramebufferSpecification& fbSpec, Ref<Scene>* activeScene, SceneHierarchyPanel* sceneHierarchyPanel, EditorCamera editorCamera)
    //{
    //    m_Framebuffer = Framebuffer::Create(fbSpec);
    //    m_EditorCamera = editorCamera;
    //    m_ActiveScene = activeScene;
    //    m_SceneHierarchyPanel = sceneHierarchyPanel;
    //}

    void ViewportPanel::OnUpdate()
    {

        m_EditorCamera.OnUpdate();
        m_EditorCamera.SetViewportFocused(IsViewportFocused());

        // Editor Mode Scele Entity
        auto [mx, my] = ImGui::GetMousePos();
        mx -= GetViewportBoundsMin().x;
        my = GetViewportBoundsMax().y - my;
        glm::vec2 viewportSize = GetViewportBoundsMax() - GetViewportBoundsMin();

        int mouseX = (int)mx;
        int mouseY = (int)my;
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
            m_PixelData = GetFramebuffer()->ReadPixel(1, mouseX, mouseY);
            //m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_Context->GetActiveScene().get());
        }

    }

    void ViewportPanel::UI_Toolbar()
    {
        if (ImGui::BeginTabBar("sdfsdf")) {
            float size = ImGui::GetFrameHeight() - 2.0f;
            ImGui::SameLine();
            const char* items[] = { "FinalBuffer","ColorBuffer", "DepthBuffer", "LightBuffer","WorldNormalBuffer","RoughnessBuffer","MetallicBuffer","AOBuffer"};
            int item_current = (int)OutBuffer::GetOutBuffer();
            if (ImGui::BeginCombo("Visible Buffer", items[item_current], ImGuiComboFlags_NoPreview))
            {
                for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
                {
                    bool is_selected = (item_current == i);
                    if (ImGui::Selectable(items[i], is_selected)) {
                        OutBuffer::SetOutBuffer((OutBuffer::VisibleBuffer)i);
                        item_current = i;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }


            ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f, 1.0f, 1.0f, 0.0f });
            if (ImGui::ImageButton((ImTextureID)m_ScneneStatesIcon->GetRendererID(), { size, size }, { 0, 1 }, { 1, 0 }))
            {
                auto currentScneneStates = m_Context->GetActiveScene()->GetCurrentScneneStates();
                switch (currentScneneStates)
                {
                case BE::ScneneStates::Ediotr:
                    m_ScneneStatesIcon = m_PauseIcon;
                    m_Context->GetActiveScene()->SetCurrentScneneStates(ScneneStates::Runtime);
                    break;
                case BE::ScneneStates::Runtime:
                    m_ScneneStatesIcon = m_PlayIcon;
                    m_Context->GetActiveScene()->SetCurrentScneneStates(ScneneStates::Ediotr);
                    break;
                }
            }
            ImGui::PopStyleColor();
            ImGui::EndTabBar();
        }
    }




    void ViewportPanel::OnImGuiRenderer()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

        ImGui::Begin("Viewport",nullptr,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        UI_Toolbar();

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        //Application::Get().GetImGuiLayer().BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
        Application::Get().GetImGuiLayer().BlockEvents(!m_ViewportFocused);


        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        viewportPanelSize = ImGui::GetContentRegionAvail();
        //µ÷ÕûÆÁÄ»±ÈÀý
        if (viewportPanelSize.x != m_ViewportSize.x || viewportPanelSize.y != m_ViewportSize.y) {
            m_Context->GetActiveScene()->OnViewportResize(viewportPanelSize.x, viewportPanelSize.y);
            m_EditorCamera.SetViewportSize(viewportPanelSize.x, viewportPanelSize.y);
        }
        m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
        switch (Renderer::GetRenderPipeline())
        {
        case Renderer::RenderPipeline::ForwardRendering:
            textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
            break;
        case Renderer::RenderPipeline::DeferredRendering:
            textureID = m_Framebuffer1->GetColorAttachmentRendererID(0);
            break;

        }
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ///////////////////////////////////////////////////////////////////////////////
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const wchar_t* path = (const wchar_t*)payload->Data;
                m_Context->OpenScene(std::filesystem::path(g_AssetsPath) / path);
            }
            ImGui::EndDragDropTarget();
        }
        ///////////////////////////////////////////////////////////////////////////////
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();

        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


        /*   BE_CORE_WARN(" Min Bounds = {0} , {1}", m_ViewportBounds[0].x, m_ViewportBounds[0].y);
           BE_CORE_WARN(" Max Bounds = {0} , {1}", m_ViewportBounds[1].x, m_ViewportBounds[1].y);*/

           //Gizmos
        Entity selectedEntity = m_Context->GetSceneHierarchyPanel()->GetSelectedEntity();
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



    void ViewportPanel::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BE_BIND_EVENT_FN(ViewportPanel::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BE_BIND_EVENT_FN(ViewportPanel::OnMouseButtonPressed));
    }

    bool ViewportPanel::OnKeyPressed(KeyPressedEvent& event) {
        if(event.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(BE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(BE_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(BE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(BE_KEY_RIGHT_SHIFT);
        switch (event.GetKeyCode())
        {

                //Gizmos
        case BE_KEY_W: {
            if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE) {
                m_GizmoType = -1;
            }
            else
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        case BE_KEY_E: {
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) {
                m_GizmoType = -1;
            }
            else
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case BE_KEY_R: {
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
        return false;
    }
    bool ViewportPanel::OnMouseButtonPressed(MouseButtonPressedEvent& event)
    {
        bool leftMouse = Input::IsMouseButtonPressed(BE_MOUSE_BUTTON_LEFT);
        if (leftMouse && m_ViewportHovered && !ImGuizmo::IsOver()) {
            m_Context->GetSceneHierarchyPanel()->SetSelectedEntity(m_HoveredEntity);
        }
        return false;
    }

}