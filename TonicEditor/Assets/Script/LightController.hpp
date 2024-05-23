#pragma once
#include "ECS/Base/ScriptableEntity.hpp"

#include <algorithm>

#include "ECS/Components/CppScriptComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/SpotLightComponent.hpp"
#include "ECS/Components/AudioSourceComponent.hpp"

#include "Core/InputManager.hpp"

struct LightController : public ECS::ScriptableEntity
{
	ECS::Components::TransformComponent* p_transform = nullptr;
	ECS::Components::SpotLightComponent* p_spotComp = nullptr;
	ECS::Components::AudioSourceComponent* p_clickSound = nullptr;

	LowRenderer::Lights::SpotLight* p_spot = nullptr;


	float defaultStrength = 1.f;
	float battery = 1.f;

	float batteryRechargeTime = 10.f;
	float batteryDechargeTime = 20.f;
	float batteryCriticalState = 0.1f;
	bool bOn = true;
	bool bFlickering = false;

	LightController(ECS::EntityID _id) : ScriptableEntity(_id) {  scriptTypeStr = "LightController"; };
	ECS::ScriptType GetScriptType() override {return ECS::ScriptType::LightController;}

	void OnCreate()
	{
		if (HasComponent<ECS::Components::TransformComponent>())
			p_transform = &GetComponent<ECS::Components::TransformComponent>();
		if (HasComponent<ECS::Components::SpotLightComponent>())
			p_spotComp = &GetComponent<ECS::Components::SpotLightComponent>();
		if (HasComponent<ECS::Components::AudioSourceComponent>())
		{
			p_clickSound = &GetComponent<ECS::Components::AudioSourceComponent>();
			p_spot = &p_spotComp->light;
		}
	}

	void OnStart()
	{
		//Security check
		if (HasComponent<ECS::Components::TransformComponent>())

			if (!p_transform)
				if (HasComponent<ECS::Components::TransformComponent>())
					p_transform = &GetComponent<ECS::Components::TransformComponent>();
		p_clickSound = &GetComponent<ECS::Components::AudioSourceComponent>();

		if (!p_clickSound)
			p_clickSound = &GetComponent<ECS::Components::AudioSourceComponent>();

		if (HasComponent<ECS::Components::SpotLightComponent>())
			if (!p_spotComp)
			{
				p_spotComp = &GetComponent<ECS::Components::SpotLightComponent>();
				p_spot = &p_spotComp->light;
			}
		defaultStrength = p_spotComp->light.strength;
	}
	void OnUpdate(float _deltatime)
	{
		ManageControls(_deltatime);
	}

private:
	void ManageControls(float _deltatime)
	{
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyF] == Core::Applications::EventType::Pressed)
		{
			SwitchFlashLight();
		}
	}

	void ManageBattery(float _deltatime)
	{
		if (bOn)
			battery -= _deltatime * battery / batteryDechargeTime;
		else
			battery -= _deltatime * battery / batteryRechargeTime;
		battery = std::clamp(battery, 0.f, 1.f);
		if (bFlickering)
		{
			if (!bOn)
				bFlickering = false;
			Flicker(_deltatime);
			return;
		}
		if (battery < batteryCriticalState)
		{
			float CriticalStateTolerance = 0.01f;
			if (battery > batteryCriticalState - CriticalStateTolerance)
				bFlickering = true;
			p_spot->strength = defaultStrength * battery / batteryCriticalState;
		}
	}

	void SwitchFlashLight()
	{
		bFlickering = false;
		p_clickSound->Play();
		//Can't Activate FlashLight if battery isn't high enough
		if (!bOn && battery < batteryCriticalState)
			return;
		p_spotComp->bEnabled = !p_spotComp->bEnabled;
		bOn = !bOn;
	}

	void Flicker(float _deltatime)
	{
		p_spot->strength -= _deltatime * (0.8f - (float)rand() / RAND_MAX);
		if (p_spot->strength < 0.01f)
		{
			p_spot->strength = defaultStrength;
			bFlickering = false;
		}
	}

};