#include "pch.hpp"

#include "ECS/Systems/MeshRendererSystem.hpp"
#include "ECS/Components/MeshRendererComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"

#include "Resources/Shader.hpp"
#include "Resources/Material.hpp"
#include "LowRenderer/Meshes/MeshRenderer.hpp"

ECS::Systems::MeshRendererSystem::MeshRendererSystem()
{
	AddComponentSignature<Components::TransformComponent>();
	AddComponentSignature<Components::MeshRendererComponent>();
}

ECS::Systems::MeshRendererSystem::~MeshRendererSystem()
{
}

const bool ECS::Systems::MeshRendererSystem::Init()
{
	bool success = true;
	//Shortcut to RHI
	Resources::ResourceManager* p_RHI = ENGINE.RES_MNGR;

	//p_outlineShader_ = p_RHI->Get<Resources::Shader>("OUTLINE");
	//////////////////////////////////////////////////////////////////
	//TODO: Uncomment on Material Default Creation
	//p_defaultMaterial_ = p_RHI->Get<Resources::Material>("DEFAULT");
	// success = p_shader_ && p_outlineShader_ && p_defaultMaterial_;
	//////////////////////////////////////////////////////////////////
	auto magentaD = p_RHI->Create<Resources::Texture>("StaticAssets\\Textures\\magenta.png");
	magentaD->textureType = Resources::TextureType::Diffuse;
	auto magentaS = p_RHI->Create<Resources::Texture>("StaticAssets\\Textures\\magenta.png");
	magentaS->textureType = Resources::TextureType::Specular;

	p_defaultMaterial_ = std::make_shared<Resources::Material>(magentaD);

	if (success)
		DEBUG_SUCCESS("Initialized MeshRenderer Renderer System")
	else
		DEBUG_ERROR("Failed to initialize MeshRenderer Renderer System");
	return success;
}

void ECS::Systems::MeshRendererSystem::Update()
{
	//Shortcut to EntityManager
	ECS::EntityManager* p_EM = ENGINE.ENT_MNGR;
	for (auto entity : entities_)
	{
		auto* transformComp = &p_EM->GetComponent<Components::TransformComponent>(entity);

		if (transformComp->HasChanged())
		{
			//Update stored model matrix
			auto* meshComp = &p_EM->GetComponent<Components::MeshRendererComponent>(entity);
			meshComp->model_ = transformComp->GetModel();
			//TODO : Think about storing entity ids + matrices in a temp map/vector of tuples the browse the Meshrenderer in another loop
		}
	}
}
void ECS::Systems::MeshRendererSystem::Render()
{
	//Shortcut to Renderer
	auto p_RDR = ENGINE.RDR;
	//Shortcut to EntityManager
	ECS::EntityManager* p_EM = ENGINE.ENT_MNGR;

	for (auto entity : entities_)
	{
		auto* meshComp = &p_EM->GetComponent<Components::MeshRendererComponent>(entity);
		if (meshComp->renderer.p_materials.size())
		{
			bool transparent = false;
			for (auto material : meshComp->renderer.p_materials)
			{
				if (material->IsTransparent())
				{
					Maths::Vec3 pos = meshComp->model_.data4V[3];
					//Check the distance from camera here
					Maths::Vec3 camPos = ENGINE.RDR->GetCamera()->eye;
					float sqDist = (camPos - pos).GetMagnitudeSquared();
					transparentEntities_.emplace(sqDist, entity);
					transparent = true;
				}
				p_RDR->UseResource(material->GetShader());
				p_RDR->SetModel(meshComp->model_);
			}
			if (!transparent)
				meshComp->renderer.p_mesh->Use(meshComp->renderer.p_materials);
		}
		else
		{
			p_defaultMaterial_->Use();
			p_RDR->SetModel(meshComp->model_);
			meshComp->renderer.p_mesh->Use();
		}
	}
	//Alpha Rendering
	for (auto it = transparentEntities_.rbegin(); it != transparentEntities_.rend(); ++it)
	{
		auto* meshComp = &p_EM->GetComponent<Components::MeshRendererComponent>(it->second);
		p_RDR->SetModel(meshComp->model_);
		meshComp->renderer.p_mesh->Use(meshComp->renderer.p_materials);
	}
	transparentEntities_.clear();
}

void ECS::Systems::MeshRendererSystem::RenderEditorScene()
{
	Render();
	//Could put any Editor linked
}