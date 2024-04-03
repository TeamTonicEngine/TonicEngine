#pragma once

#include "DLL_API.hpp"

#include "IResource.hpp"

class Shader;

namespace Resources
{
	class Material : public IResource
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	protected:
		Maths::Vec3 albedo_ = Maths::Vec3(.6f);
		float ao_ = 1.0f;
		float metallic_ = .2f;
		float roughness_ = .1f;
		struct {
			u32 albedo = 0;
			u32 ao = 0;
			u32 normal = 0;
			u32 metallic = 0;
			u32 roughness = 0;
		}map_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	public:
		TONIC_ENGINE_API Material() = default;
		TONIC_ENGINE_API ~Material() = default;
		TONIC_ENGINE_API Material(Maths::Vec3 _albedo, float _metallic, float _roughness, float _ao);;

		void TONIC_ENGINE_API SetUniform(Shader* _shader);

		virtual void TONIC_ENGINE_API ReadFile(const string _name) override;
		virtual void TONIC_ENGINE_API ReadFile(const fs::path _path) override;
	
		virtual void TONIC_ENGINE_API MetaWriteFile(const string _name) override;
		virtual void TONIC_ENGINE_API MetaReadFile(const string _name) override;

		virtual void TONIC_ENGINE_API ResourceUnload() override;
	};
}