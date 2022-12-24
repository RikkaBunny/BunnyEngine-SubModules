#pragma once

#include "BunnyEngine/Layer.h"
#include "BunnyEngine/Events/ApplicationEvent.h"
#include "BunnyEngine/Events/KeyEvent.h"
#include "BunnyEngine/Events/MouseEvent.h"

namespace BE {

	class BE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttcah() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begine();
		void End();

	private:
		float m_Time = 0.0f;
	};

}


