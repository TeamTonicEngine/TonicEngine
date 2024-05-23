#pragma once

#include "ECS/Base/BaseComponent.hpp"
#include "LowRenderer/Meshes/MeshRenderer.hpp"

namespace ECS::Systems
{
	class TextRendererSystem;
}

namespace ECS::Components
{
	struct TextRendererComponent : public ECS::BaseComponent
	{

		std::string text;
		Resources::FontPtr p_font;
		TNCColor color = TNCColors::WHITE;
		u32 priority = 0;
		bool b3D = false;

		TONIC_ENGINE_API TextRendererComponent();
		TONIC_ENGINE_API TextRendererComponent(std::string _text);
		TONIC_ENGINE_API TextRendererComponent(Resources::FontPtr _p_font);
		TONIC_ENGINE_API ~TextRendererComponent();

	protected:
		Maths::Mat4 model_ = Maths::Mat4(1.f);

		friend class ECS::Systems::TextRendererSystem;
	};
}