#pragma once
#include "ECS/Base/ScriptableEntity.hpp"

#include "ECS/Components/CppScriptComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/CameraComponent.hpp"

#include "Core/InputManager.hpp"

struct PlayerController : public ECS::ScriptableEntity
{
	ECS::Components::TransformComponent* p_transform = nullptr;
	ECS::Components::CameraComponent* p_camera = nullptr;

	float speed = 5.0f;

	float yCam = 0.f;
	f32_2 mouseDelta = { 0.f,0.f };
	f32_2 mousePos;
	float pixel2Deg = 0.02f; //Controls angular Speed of camera rotation

	PlayerController(ECS::EntityID _id) : ScriptableEntity(_id) { scriptTypeStr = "PlayerController";
	};
	ECS::ScriptType GetScriptType() override { return ECS::ScriptType::PlayerController; }

	bool bcaptureMode = true;
	void OnCreate()
	{
		p_transform = &GetComponent<ECS::Components::TransformComponent>();
		p_camera = &GetComponent<ECS::Components::CameraComponent>();
		ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::Pressed, Core::Applications::Keyboard::KeyTab, [&]() {
			bcaptureMode = !bcaptureMode;
			if (bcaptureMode)
			{
				ENGINE.INP_MNGR->SetCursorPos({ ENGINE.WDW->GetWindowPos().x + ENGINE.WDW->GetWindowSize().x / 2.f, ENGINE.WDW->GetWindowPos().y + ENGINE.WDW->GetWindowSize().y / 2.f });
				mousePos = ENGINE.INP_MNGR->mousePosition;
			}
			ENGINE.INP_MNGR->SetCursorVisibility(!bcaptureMode);
			});
		ENGINE.INP_MNGR->OnEvent(Core::Applications::EventType::MoveMouse, [&]() {
			RegisterMouse();
			});

	}

	void OnStart()
	{
		bcaptureMode = !bcaptureMode;
		ENGINE.INP_MNGR->SetCursorPos({ENGINE.WDW->GetWindowPos().x + ENGINE.WDW->GetWindowSize().x / 2.f, ENGINE.WDW->GetWindowPos().y + ENGINE.WDW->GetWindowSize().y / 2.f
	});
		ENGINE.INP_MNGR->SetCursorVisibility(!bcaptureMode);

		mousePos = ENGINE.INP_MNGR->mousePosition;
		//Security check
		if (!p_transform)
			p_transform = &GetComponent<ECS::Components::TransformComponent>();
		if (!p_camera)
			p_camera = &GetComponent<ECS::Components::CameraComponent>();
	}
	void OnUpdate(float _deltatime)
	{
		ManageControls(_deltatime);
	}

private:
	void ManageControls(float _deltatime)
	{
		if (!bcaptureMode)
			return;

		Maths::Vec3 forwardPlayer = p_transform->rotation.RotateVector(Maths::Vec3::FORWARD);
		Maths::Vec3 leftPlayer = Maths::Vectors::CrossProduct(forwardPlayer, Maths::Vec3::UP);

		Maths::Vec3 forwardPlayerXZ = Maths::Vec3(forwardPlayer.x, 0.f, forwardPlayer.z).GetNormalized();
		Maths::Vec3 leftPlayerXZ = Maths::Vectors::CrossProduct(forwardPlayerXZ, Maths::Vec3::UP);

		p_transform->rotation = Maths::Quaternions::FromAngleAxis(mouseDelta.x * pixel2Deg, Maths::Vec3::UP) * Maths::Quaternions::FromAngleAxis(mouseDelta.y * pixel2Deg, -leftPlayerXZ) * p_transform->rotation;

		Maths::Vec3 direction;

		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyW] == Core::Applications::EventType::Hold)
			direction += Maths::Vec3::FORWARD;
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyS] == Core::Applications::EventType::Hold)
			direction += Maths::Vec3::BACKWARD;
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyD] == Core::Applications::EventType::Hold)
			direction += Maths::Vec3::RIGHT;
		if (ENGINE.INP_MNGR->keys[Core::Applications::Keyboard::KeyA] == Core::Applications::EventType::Hold)
			direction += Maths::Vec3::LEFT;

		Maths::Vec3 normalDir = direction.GetNormalized();

		Maths::Vec3 xMove = -leftPlayerXZ * normalDir.x;
		Maths::Vec3 zMove = forwardPlayerXZ * normalDir.z;


		p_transform->position += (xMove + zMove) * speed * _deltatime;


		mousePos = ENGINE.INP_MNGR->mousePosition;
		mouseDelta = { 0.f,0.f };
	}
	void RegisterMouse()
	{
		f32_2 newPos = ENGINE.INP_MNGR->mousePosition;

		mouseDelta.x = -(newPos.x - mousePos.x);
		mouseDelta.y = (newPos.y - mousePos.y);

		//clamp position Y
		if (yCam + mouseDelta.y * pixel2Deg > 75.f)
		{
			mouseDelta.y = 0.f; //(75.f - yCam) / pixel2Deg;
			yCam = 74.99f;
		}
		if (yCam + mouseDelta.y * pixel2Deg < -75.f)
		{
			mouseDelta.y = 0.f;//(-75.f - yCam) / pixel2Deg;
			yCam = -74.99f;
		}
		yCam += mouseDelta.y;
	}
};