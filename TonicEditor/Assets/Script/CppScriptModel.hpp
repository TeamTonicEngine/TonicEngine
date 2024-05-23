#pragma once
#include "ECS/Base/ScriptableEntity.hpp"

//Optionals------------------------------------------//
//---------HERE-FOR-COMPONENTS------------------
#include "ECS/Components.hpp" //All at once
//---------------------------// Or individually
#include "ECS/Components/CppScriptComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/CameraComponent.hpp"
//---------HERE-FOR-INPUTS------------------
#include "Core/InputManager.hpp"
//--------------------------------------------------//

//Not to use directly, only to know waht a script should be like //
struct CppScriptModel : public ECS::ScriptableEntity
{
	//Stock your components first, don't call several times the getters
	ECS::Components::TransformComponent* p_transform = nullptr;
	ECS::Components::CameraComponent* p_cam = nullptr;

	//Members-----------------------------------------------------------------------
	float tmpDeltaTime = 0.f;
	float speed = 5.0f;
	//------------------------------------------------------------------------------

	//Necessary constructor
	CppScriptModel(ECS::EntityID _id) : ScriptableEntity(_id) { scriptType = ECS::ScriptType::Name ; scriptTypeStr = "name"; };

	void OnCreate()
	{
		//Stock your components Here, or at Start don't call several times the getters
		p_transform = &GetComponent<ECS::Components::TransformComponent>();
		p_cam = &GetComponent<ECS::Components::CameraComponent>();

#pragma region Inputs
		//Input Option 1 Register Input Events, need to update the temp deltaTime--------------------------------------------------
		ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyW, [&]()
			{
				p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::FORWARD) * speed * tmpDeltaTime;
			});
		ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyS, [&]()
			{
				p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::BACKWARD) * speed * tmpDeltaTime;
			});
		ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyD, [&]()
			{
				p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::RIGHT) * speed * tmpDeltaTime;
			});
		ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Hold, Core::Applications::Keyboard::KeyA, [&]()
			{
				p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::LEFT) * speed * tmpDeltaTime;
			});
		//-----------------------------------------------------------------------------------------------------------------------
	}
#pragma endregion //Inputs

	void OnDestroy()
	{
	}
	void OnStart()
	{
		//Stock your components Here, or at Creation don't call several times the getters
		p_transform = &GetComponent<ECS::Components::TransformComponent>();
		p_cam = &GetComponent<ECS::Components::CameraComponent>();
	}
	void OnUpdate(float _deltaTime)
	{
#pragma region Inputs

		//Input Option 1 Update the temp deltaTime, need to Register Input Events------------------------------------------------
		tmpDeltaTime = _deltaTime;
		//-----------------------------------------------------------------------------------------------------------------------

		//Input Option 2 Check the Input States----------------------------------------------------------------------------------
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyW] == Core::Applications::EventType::Hold)
			p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::FORWARD) * speed * _deltatime;
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyS] == Core::Applications::EventType::Hold)
			p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::BACKWARD) * speed * _deltatime;
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyD] == Core::Applications::EventType::Hold)
			p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::RIGHT) * speed * _deltatime;
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyA] == Core::Applications::EventType::Hold)
			p_transform->position += p_transform->GetRotation().RotateVector(Maths::Vec3::LEFT) * speed * _deltatime;
		//-----------------------------------------------------------------------------------------------------------------------
#pragma endregion //Inputs
	}
	void OnFixedUpdate(float _fixedDeltaTime)
	{
	}

};