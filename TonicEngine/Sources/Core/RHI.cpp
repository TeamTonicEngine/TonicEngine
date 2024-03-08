#include "pch.hpp"
#include "TonicEngine/Core/RHI.hpp"
#include "TonicEngine\Resources\IResource.hpp"

void Core::RHI::Init(u32 width, u32 height)
{

}

void Core::RHI::StartFrame()
{

}

void Core::RHI::EndFrame()
{

}

void Core::RHI::InitShader()
{

}

void Core::RHI::CheckCompileErrors(unsigned int shader, std::string type)
{

}

void Core::RHI::InitShaderData()
{

}

void Core::RHI::DrawTriangle()
{

}

void Core::RHI::LoadResource(const Resources::IResource* _p_resource)
{
	if (!_p_resource)
	{
		DEBUG_WARNING("RESOURCE NULL");
		return;
	}
}

void Core::RHI::UnloadResource(const Resources::IResource* _p_resource)
{
	if (!_p_resource)
		DEBUG_WARNING("RESOURCE NULL");
}

void Core::RHI::InitFrameBuffer()
{

}

void Core::RHI::BindFrameBuffer()
{

}

void Core::RHI::UnbindFrameBuffer()
{

}

void Core::RHI::RescaleFrameBuffer(s32 width, s32 height)
{

}

void Core::RHI::ResetViewPort(f32 width, f32 height)
{

}

u32 Core::RHI::GetTextureID()
{
	return 0;
}
void Core::RHI::CleanUp()
{

}