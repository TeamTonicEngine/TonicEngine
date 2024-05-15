#include "pch.hpp"

#include "ECS/Components/MeshRendererComponent.hpp"
#include "Resources\Mesh.hpp"

using namespace ECS::Components;

MeshRendererComponent::~MeshRendererComponent() { renderer.p_materials.clear(); }

MeshRendererComponent::MeshRendererComponent() : name("sphere")
{
	renderer.p_mesh = ENGINE.RES_MNGR->Get<Resources::Mesh>("StaticAssets\\Meshes\\sphere.obj");
}

MeshRendererComponent::MeshRendererComponent(std::string _path)
{
	renderer.p_mesh = ENGINE.RES_MNGR->Get<Resources::Mesh>(_path);
	name = renderer.p_mesh->name;
}
MeshRendererComponent::MeshRendererComponent(Resources::MeshPtr _p_mesh)
{
	renderer.p_mesh = _p_mesh;
	name = renderer.p_mesh->name;
}
