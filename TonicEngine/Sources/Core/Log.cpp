#include "pch.hpp"
#include "Core/Log.hpp"

#include <iostream>
#include <chrono>

void Core::Debug::FormatString(char* _buffer, u64 _bufferSize, const char* _format, ...)
{
	va_list args;
	va_start(args, _format);
	std::vsnprintf(_buffer, _bufferSize, _format, args);
	va_end(args);
}

Core::Debug::Log* Core::Debug::Log::s_instance_ = nullptr;

Core::Debug::Log::~Log()
{
	s64 now = time(0);
	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &now);
	GetInstance()->Print("End of log entry at %s", buffer);
	if (output_.is_open())
		output_.close();
}

Core::Debug::Log* Core::Debug::Log::GetInstance()
{
	if (s_instance_ == nullptr)
		s_instance_ = new Core::Debug::Log();
	return s_instance_;
}

void Core::Debug::Log::DeleteInstance()
{
	// Useless to check: delete nullptr is safe
	delete s_instance_;
}

void Core::Debug::Log::OpenFile(std::filesystem::path const& _fileName, bool _erase)
{
	GetInstance()->InstanceOpenFile(_fileName, _erase);
}

void Core::Debug::Log::Print(const char* _format, ...)
{
	va_list args;
	va_start(args, _format);
	GetInstance()->InstancePrint(_format, args);
	va_end(args);
}

void Core::Debug::Log::InstanceOpenFile(std::filesystem::path const& _fileName, bool _erase)
{
	if (_erase)
		output_.open(_fileName, std::ios::out | std::ios::trunc);
	else
		output_.open(_fileName, std::ios::out | std::ios::app);
	if (output_.is_open())
	{
		s64 now = time(0);
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &now);
		Print("Core::Debug::Log entry :%s", buffer);
	}
	else
		std::cout << "Failed to open " << _fileName << std::endl;
}

void Core::Debug::Log::InstancePrint(const char* _format, va_list _args)
{
	//vsnprintf instead of manually checking the format[i]
	const s32 bufferSize = 1024;
	char buffer[bufferSize];
	vsnprintf(buffer, bufferSize, _format, _args);
	std::cout << std::string(buffer) << "\n";
	output_ << std::string(buffer) << "\n";
	output_.flush();
}

void Core::Debug::Log::SuccessColor()
{
	GetInstance()->ChangeColor(Color::Green_Black_BG);
}

void Core::Debug::Log::WarningColor()
{
	GetInstance()->ChangeColor(Color::Yellow_Black_BG);
}

void Core::Debug::Log::ErrorColor()
{
	GetInstance()->ChangeColor(Color::White_Red_BG);
}

void Core::Debug::Log::ResetColor()
{
	SetConsoleTextAttribute(GetInstance()->handle_, 15); // Text in white (default)
}

void Core::Debug::Log::ChangeColor(u8 _handleWindowsId) const
{
	SetConsoleTextAttribute(handle_, _handleWindowsId);
}

void Core::Debug::Log::ChangeColor(Color _handleWindowsId) const
{
	SetConsoleTextAttribute(handle_, (WORD)_handleWindowsId);
}
