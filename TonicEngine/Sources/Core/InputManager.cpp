#include "pch.hpp"

#include "Core/InputManager.hpp"

#include <GLFW/glfw3.hpp>

void* Core::Applications::InputManager::s_p_windowIn = nullptr;

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
		return;
	keyboardKeys_.erase(_key);
}

void KeyTracker::Remove(Mouse _button)
{
	if (mouseButtons_.find(_button) == mouseButtons_.end())
		return;
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
		return std::get<0>(keyboardKeys_.at(_key));
	return EventType::Released;
}

EventType KeyTracker::operator[](Mouse _button)
{
	if (mouseButtons_.find(_button) != mouseButtons_.end())
		return std::get<0>(mouseButtons_.at(_button));
	return EventType::Released;
}

// ----------------------------------------
// GLFW CALLBACK FUNCTION
// ----------------------------------------

void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	ENGINE.GUI->RunGuiKeyCallback(_window, _key, _scancode, _action, _mods);
	if (!ENGINE.INP_MNGR)
		return;

	Event event;
	event.key = _key;

	if (_action == GLFW_PRESS)
	{
		event.type = EventType::Pressed;
		ENGINE.INP_MNGR->keys.Add(static_cast<Keyboard>(_key), event.type);
	}
	else if (_action == GLFW_RELEASE)
	{
		event.type = EventType::Released;
		ENGINE.INP_MNGR->keys.Remove(static_cast<Keyboard>(_key));
	}

	ENGINE.INP_MNGR->AddEvent(event);
}

void mouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods)
{
	ENGINE.GUI->RunGuiMouseCallback(_window, _button, _action, _mods);
	if (!ENGINE.INP_MNGR)
		return;

	Event event;
	event.key = _button;

	if (_action == GLFW_PRESS)
	{
		event.type = EventType::Pressed;
		ENGINE.INP_MNGR->keys.Add(static_cast<Mouse>(_button), event.type);
	}
	else if (_action == GLFW_RELEASE)
	{
		event.type = EventType::Released;
		ENGINE.INP_MNGR->keys.Remove(static_cast<Mouse>(_button));
	}
	else return;

	//glfwGetCursorPos(window, &event.mouseX, &event.mouseY);
	ENGINE.INP_MNGR->AddEvent(event);
}

void cursorPositionCallback(GLFWwindow* _window, double _xpos, double _ypos)
{
	if (Core::Applications::InputManager::s_p_windowIn != _window)
		Core::Applications::InputManager::s_p_windowIn = _window;
	if (!ENGINE.INP_MNGR)
		return;

	Event event;
	event.type = EventType::MoveMouse;

	ENGINE.INP_MNGR->mousePosition = { (f32)_xpos,(f32)_ypos };
	ENGINE.INP_MNGR->AddEvent(event);
}

void scrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset)
{
	ENGINE.GUI->RunGuiScrollCallback(_window, _xoffset, _yoffset);
	if (!ENGINE.INP_MNGR)
		return;

	Event eventStart;
	eventStart.type = EventType::ScrollStart;

	Event eventChange;
	eventChange.type = EventType::ScrollChange;

	ENGINE.INP_MNGR->scrollOffset = (f32)_yoffset;
	ENGINE.INP_MNGR->AddEvent(eventStart);
	ENGINE.INP_MNGR->AddEvent(eventChange);
}

// ----------------------------------------
// INPUT MANAGER
// ----------------------------------------

void InputManager::SetCallBacks(void* _window)
{
	glfwSetKeyCallback((GLFWwindow*)_window, keyCallback);
	glfwSetMouseButtonCallback((GLFWwindow*)_window, mouseButtonCallback);
	glfwSetCursorPosCallback((GLFWwindow*)_window, cursorPositionCallback);
	glfwSetScrollCallback((GLFWwindow*)_window, scrollCallback);
}

void InputManager::Init()
{
	s_p_windowIn = ENGINE.WDW->GetWindow();
	SetCallBacks(s_p_windowIn);
}

void InputManager::StartHoldEvents()
{
	std::vector<s32> temp = keys.GetSuitableForHold((std::chrono::milliseconds)50);
	for (s32 i : temp)
		AddEvent({ EventType::Hold,i });
}

void InputManager::resetScrollOffset()
{
	if (scrollOffset)
	{
		scrollOffset = 0.f;

		Event eventEnd;
		eventEnd.type = EventType::ScrollEnd;

		Event eventChange;
		eventChange.type = EventType::ScrollChange;

		AddEvent(eventEnd);
		AddEvent(eventChange);
	}
}

void InputManager::SetCursorVisibility(bool _state)
{
	if (_state)
		glfwSetInputMode((GLFWwindow*)s_p_windowIn, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode((GLFWwindow*)s_p_windowIn, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void InputManager::SetCursorPos(f32_2 _pos)
{
	glfwSetCursorPos((GLFWwindow*)s_p_windowIn, _pos.x, _pos.y);
}

void InputManager::OnEvent(EventType _type, Keyboard _key, std::function<void()> _function)
{
	handlers_.push_back({ _type, static_cast<s32>(_key), _function });
}

void InputManager::OnEvent(EventType _type, Mouse _button, std::function<void()> _function)
{
	handlers_.push_back({ _type, static_cast<s32>(_button), _function });
}

void InputManager::OnEvent(EventType _type, std::function<void()> _function)
{
	handlers_.push_back({ _type, -1, _function });
}

void InputManager::AddEvent(Event _event) { eventQueue_.push(_event); }

void InputManager::ProcessEvent()
{
	while (!eventQueue_.empty())
	{
		Event event = eventQueue_.front();
		for (const auto& handler : handlers_)
			if ((std::get<0>(handler) == event.type) && (std::get<1>(handler) == event.key || std::get<1>(handler) == -1))
				std::get<2>(handler)();
		eventQueue_.pop();
	}
}
// ----------------------------------------