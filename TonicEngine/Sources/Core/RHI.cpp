#include <pch.hpp>

#include "Core/RHI.hpp"

const bool Core::Renderer::RHI::Init(Core::Applications::Window* _p_window)
{
	p_window_ = _p_window;
	// Here in child add any additional function to link the window to specific RHI
	return true;
}

const unsigned Core::Renderer::RHI::GetFrameBufferID() const { return frameBufferID; }

void Core::Renderer::RHI::SetCamera(LowRenderer::Cameras::FreeCamera* _p_newCamera) { p_currentCamera_ = _p_newCamera; }

void Core::Renderer::RHI::LoadResource(Resources::IResource* _p_resource)
{
	if (!_p_resource)
		throw ("Nullptr sent to Core::Renderer::RHI::LoadResource.");
	//TODO: Uncomment as soon as ResourceManager is created and Type enum is set
	/*
	switch (_p_resource.Type()) // returns enum type, if none -> throw, else call right class function
	{
	case(ResourceType::Material):
		LoadMaterial(dynamic_cast<Material*>(_p_resource));
		break;
	case(ResourceType::Mesh):
		LoadMesh(dynamic_cast<Mesh*>(_p_resource));
		break;
	case(ResourceType::Texture):
		LoadTexture(dynamic_cast<Texture*>(_p_resource));
		break;
	case(ResourceType::Shader):
		LoadShader(dynamic_cast<Shader*>(_p_resource));
		break;
	default:
		throw ("Unknown Resource Type sent to Core::Renderer::RHI::LoadResource.");
	}
	*/
}
void Core::Renderer::RHI::UseResource(const Resources::IResource* _p_resource)
{
	if (!_p_resource)
		throw ("Nullptr sent to Core::Renderer::RHI::UseResource.");
	//TODO: Uncomment as soon as ResourceManager is created and Type enum is set
/*
	switch (_p_resource.Type()) // returns enum type, if none -> throw, else call right class function
	{
	case(ResourceType::Material):
		UseMaterial(dynamic_cast<Material*>(_p_resource));
		break;
	case(ResourceType::Mesh):
		UseMesh(dynamic_cast<Mesh*>(_p_resource));
		break;
	case(ResourceType::Texture):
		UseTexture(dynamic_cast<Texture*>(_p_resource));
		break;
	case(ResourceType::Shader):
		UseShader(dynamic_cast<Shader*>(_p_resource));
		break;
	default:
		throw ("Unknown Resource Type sent to Core::Renderer::RHI::UseResource.");
	}
	*/
}
void Core::Renderer::RHI::UnloadResource(const Resources::IResource* _p_resource)
{
	if (!_p_resource)
		throw ("Nullptr sent to Core::Renderer::RHI::UnloadResource.");
	//TODO: Uncomment as soon as ResourceManager is created and Type enum is set
/*
	switch (_p_resource.Type()) // returns enum type, if none -> throw, else call right class function
	{
	case(ResourceType::Material):
		UnloadMaterial(dynamic_cast<Material*>(_p_resource));
		break;
	case(ResourceType::Mesh):
		UnloadMesh(dynamic_cast<Mesh*>(_p_resource));
		break;
	case(ResourceType::Texture):
		UnloadTexture(dynamic_cast<Texture*>(_p_resource));
		break;
	case(ResourceType::Shader):
		UnloadShader(dynamic_cast<Shader*>(_p_resource));
		break;
	default:
		throw ("Unknown Resource Type sent to Core::Renderer::RHI::UnloadResource.");
	}
	*/
}
