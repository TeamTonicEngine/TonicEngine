#include "pch.hpp"
#include "Core/InputManager.hpp"

#include <GLFW/glfw3.hpp>

using namespace Core::Applications;

// ----------------------------------------
// KEY TRACKER
// ----------------------------------------

void KeyTracker::Add(Keyboard _key, EventType _type)
{
	if (keyboardKeys_.find(_key) != keyboardKeys_.end())
	{
		keyboardKeys_.at(_key) = { _type, std::chrono::steady_clock::now() };
		return;
	}
	
	keyboardKeys_.insert({ _key, {_type, std::chrono::steady_clock::now()} });
}
void KeyTracker::Add(Mouse _button, EventType _type)
{
	if (mouseButtons_.find(_button) != mouseButtons_.end())
	{
		mouseButtons_.at(_button) = { _type, std::chrono::steady_clock::now() };
		return;
	}

	mouseButtons_.insert({ _button, {_type, std::chrono::steady_clock::now()} });
}

void KeyTracker::Remove(Keyboard _key)
{
	if (keyboardKeys_.find(_key) == keyboardKeys_.end())
	{
		return;
	}

	keyboardKeys_.erase(_key);
}
void KeyTracker::Remove(Mouse _button)
{
	if (mouseButtons_.find(_button) == mouseButtons_.end())
	{
		return;
	}

	mouseButtons_.erase(_button);
}

std::vector<s32> KeyTracker::GetSuitableForHold(std::chrono::milliseconds _duration)
{
	std::vector<s32> result;
	auto currentTime = std::chrono::steady_clock::now();

	for (auto& key : keyboardKeys_)
	{
		if (std::get<0>(key.second) == EventType::Hold)
		{
			result.push_back((s32)key.first);
			continue;
		}
		if (currentTime - std::get<1>(key.second) >= _duration)
		{
			std::get<0>(key.second) = EventType::Hold;
			result.push_back((s32)key.first);
		}
	}

	for (auto& button : mouseButtons_)
	{
		if (std::get<0>(button.second) == EventType::Hold)
		{
			result.push_back((s32)button.first);
			continue;
		}
		if (currentTime - std::get<1>(button.second) >= _duration)
		{
			std::get<0>(button.second) = EventType::Hold;
			result.push_back((s32)button.first);
		}
	}

	return result;
}

EventType KeyTracker::operator[](Keyboard _key)
{
	if (keyboardKeys_.find(_key) != keyboardKeys_.end())
	{
		return std::get<0>(keyboardKeys_.at(_key));
	}

	return EventType::Released;
}
EventType KeyTracker::operator[](Mouse _button)
{
	if (mouseButtons_.find(_button) != mouseButtons_.end())
	{
		return std::get<0>(mouseButtons_.at(_button));
	}

	return EventType::Released;
}

// ----------------------------------------
// GLFW CALLBACK FUNCTION
// ----------------------------------------

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!ENGINE.INP_MNGR)
		return;

	Event event;
	event.key = key;

	if (action == GLFW_PRESS) {
		event.type = EventType::Pressed;
		ENGINE.INP_MNGR->keys.Add(static_cast<Keyboard>(key),event.type);
	}
	else if (action == GLFW_RELEASE) {
		event.type = EventType::Released;
		ENGINE.INP_MNGR->keys.Remove(static_cast<Keyboard>(key));
	}

	ENGINE.INP_MNGR->AddEvent(event);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (!ENGINE.INP_MNGR)
		return;

	Event event;
	event.key = button;

	if (action == GLFW_PRESS) {
		event.type = EventType::Pressed;
		ENGINE.INP_MNGR->keys.Add(static_cast<Mouse>(button),event.type);
	}
	else if (action == GLFW_RELEASE) {
		event.type = EventType::Released;
		ENGINE.INP_MNGR->keys.Remove(static_cast<Mouse>(button));
	}
	else {
		return;
	}

	//glfwGetCursorPos(window, &event.mouseX, &event.mouseY);
	ENGINE.INP_MNGR->AddEvent(event);
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if(!ENGINE.INP_MNGR)
		return;

	Event event;
	event.type = EventType::MoveMouse;
	event.key = -1;

	ENGINE.INP_MNGR->mousePosition = { (f32)xpos,(f32)ypos };
	ENGINE.INP_MNGR->AddEvent(event);
}

// ----------------------------------------
// INPUT MANAGER
// ----------------------------------------

InputManager::InputManager()
{

}

void InputManager::Init()
{
	glfwSetKeyCallback((GLFWwindow*)ENGINE.WDW->GetWindow(), keyCallback);
	glfwSetMouseButtonCallback((GLFWwindow*)ENGINE.WDW->GetWindow(), mouseButtonCallback);
	glfwSetCursorPosCallback((GLFWwindow*)ENGINE.WDW->GetWindow(), cursorPositionCallback);
}

void InputManager::StartHoldEvents()
{
	std::vector<s32> temp = keys.GetSuitableForHold((std::chrono::milliseconds)100);
	for (s32 i : temp)
		AddEvent({ EventType::Hold,i });
}

void InputManager::OnEvent(EventType _type, Keyboard _key, std::function<void()> _function)
{
	handlers_.push_back({ _type, static_cast<s32>(_key), _function });
}
void InputManager::OnEvent(EventType _type, Mouse _button, std::function<void()>_function)
{
	handlers_.push_back({ _type, static_cast<s32>(_button), _function });
}
void InputManager::OnEvent(EventType _type, std::function<void()>_function)
{
	handlers_.push_back({ _type, -1, _function });
}


void InputManager::AddEvent(Event _event)
{
	eventQueue_.push(_event);
}

void InputManager::ProcessEvent()
{
	while (!eventQueue_.empty())
	{
		Event event = eventQueue_.front();
		for (const auto& handler : handlers_)
		{
			if ((std::get<0>(handler) == event.type || (std::get<0>(handler)==EventType::Hold && event.type == EventType::Pressed) ) && (std::get<1>(handler) == event.key || std::get<1>(handler) == -1))
			{
				std::get<2>(handler)();
			}
		}
		eventQueue_.pop();
	}
}

// ----------------------------------------