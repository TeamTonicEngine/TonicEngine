#include "pch.hpp"

#include "ECS/Systems/MeshRendererSystem.hpp"
#include "ECS/Components/MeshRenderer.hpp"
#include "ECS/Components/Transform.hpp"

#include "Resources/Shader.hpp"
#include "Resources/Material.hpp"


ECS::Systems::MeshRendererSystem::MeshRendererSystem()
{
    AddComponentSignature<Components::Transform>();
    AddComponentSignature<Components::MeshRenderer>();

    //Shortcut to RHI
    Resources::ResourceManager* p_RHI = HEART::GetResourceManager();
    p_shader_ = p_RHI->Get<Resources::Shader>("MESH");
    p_outlineShader_ = p_RHI->Get<Resources::Shader>("OUTLINE");
    p_defaultMaterial = p_RHI->Get<Resources::Material>("DEFAULT");
}

void ECS::Systems::MeshRendererSystem::Render()
{
    p_shader_->Use();
    //Shortcut to RHI
    Core::Renderer::RHI* p_RHI = HEART::GetRenderer();
    //Shortcut to EntityManager
    ECS::EntityManager* p_EM = HEART::GetEntityManager();

    for (auto entity : entities_)
    {
        auto& mesh = p_EM->GetComponent<Components::MeshRenderer>(entity);
        auto& transform = p_EM->GetComponent<Components::Transform>(entity);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //TODO: Uncomment on Material Component creation
        auto material = p_defaultMaterial;

        //auto& material = p_EM->HasComponent<Components::Material>(entity)? p_EM->GetComponent<Components::Material>(entity) : defaultMaterial_;
        //	/!\ PBR Method
        material->SetUniform(p_shader_);
        //{ //SetUniform :
        //p_RHI->SetUniform("material.Ao",material.Ao);
        //p_RHI->SetUniform("material.Albedo",material.Albedo);
        //p_RHI->SetUniform("material.Metallic",material.Metallic);
        //p_RHI->SetUniform("material.Roughness",material.Roughness);
        //p_RHI->SetUniform("Model",transform.Model);
        //}
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //TODO: Uncomment on link Mesh to RHI Component creation
        /*
        if (mesh.name == "CUBE")
            mesh.DrawArrays(p_Shader_); //only Vertex array
        else
            mesh.p_renderer->Draw(p_Shader_);
        */
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}