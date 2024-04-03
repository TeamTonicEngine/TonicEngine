#include "pch.hpp"

#include "ECS/Components/MeshRenderer.hpp"
#include "Resources\Mesh.hpp"

using namespace Components;

MeshRenderer::~MeshRenderer() = default;

MeshRenderer::MeshRenderer() : name("Cube")
{
	p_renderer = GetRM()->Get<Mesh>("Cube");
};

MeshRenderer::MeshRenderer(std::string _name) : name(_name)
{
	p_renderer = GetRM()->Get<Mesh>(_name);
};
