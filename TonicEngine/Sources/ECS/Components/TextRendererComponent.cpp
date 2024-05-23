#include "pch.hpp"

#include "ECS/Components/MeshRendererComponent.hpp"
#include "Resources\Mesh.hpp"
#include "..\..\..\Headers\ECS\Components\TextRendererComponent.hpp"

using namespace ECS::Components;

TextRendererComponent::~TextRendererComponent() {}

TextRendererComponent::TextRendererComponent()
{
	p_font = Resources::Font::s_p_defaultFont;
}

TextRendererComponent::TextRendererComponent(std::string _text)
{
	text = _text;
}

TextRendererComponent::TextRendererComponent(Resources::FontPtr _p_font)
{
	p_font = _p_font;
}