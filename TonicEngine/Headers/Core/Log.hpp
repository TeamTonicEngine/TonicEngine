#pragma once

#include "DLL_API.hpp"

#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

#define NOMINMAX
#include <Windows.h>

// Windows only /!\

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define DEBUG_LOG(format, ...)\
{\
std::ostringstream debugLogStream;\
debugLogStream << __FILENAME__ << "(" << __LINE__ << "): ";\
const int bufferSize = 1024;\
char buffer[bufferSize];\
Core::Debug::FormatString(buffer, bufferSize, format, ##__VA_ARGS__);\
debugLogStream << buffer << std::endl;\
Core::Debug::Log::Print(format, ##__VA_ARGS__);\
OutputDebugStringA(debugLogStream.str().c_str());\
}\

#define DEBUG_SUCCESS(format, ...)\
{\
Core::Debug::Log::SuccessColor();\
std::ostringstream WarningLogStream;\
WarningLogStream <<"Success: " << format;\
DEBUG_LOG(WarningLogStream.str().c_str(), ##__VA_ARGS__)\
Core::Debug::Log::ResetColor();\
}\

#define DEBUG_WARNING(format, ...)\
{\
Core::Debug::Log::WarningColor();\
std::ostringstream WarningLogStream;\
WarningLogStream <<"Warning: " << format;\
DEBUG_LOG(WarningLogStream.str().c_str(), ##__VA_ARGS__)\
Core::Debug::Log::ResetColor();\
}\

#define DEBUG_ERROR(format, ...)\
{\
Core::Debug::Log::ErrorColor();\
std::ostringstream ErrorLogStream;\
ErrorLogStream <<"ERROR: " << format;\
DEBUG_LOG(ErrorLogStream.str().c_str(), ##__VA_ARGS__)\
Core::Debug::Log::ResetColor();\
}\

namespace Core::Debug
{
	void TONIC_ENGINE_API FormatString(char* _buffer, size_t _bufferSize, const char* _format, ...);

	class Log
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		std::ofstream output_;
		HANDLE handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
		// Singleton /!\ BE CAREFUL: IT IS NOT THREAD SAFE
		static Log* p_s_instance_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	private:
		TONIC_ENGINE_API Log() {};

	public:
		TONIC_ENGINE_API Log(Log& other) = delete;
		TONIC_ENGINE_API ~Log();

		void TONIC_ENGINE_API operator=(const Log&) = delete;

		static TONIC_ENGINE_API Log* GetInstance();
		static void TONIC_ENGINE_API DeleteInstance();

		static void TONIC_ENGINE_API OpenFile(std::filesystem::path const& _fileName, bool _erase = false);
		static void TONIC_ENGINE_API Print(const char* _format, ...);

	private:
		void TONIC_ENGINE_API InstanceOpenFile(std::filesystem::path const& _fileName, bool _erase = false);
		void TONIC_ENGINE_API InstancePrint(const char* _format, va_list _args);

	public:
		static void TONIC_ENGINE_API SuccessColor();
		static void TONIC_ENGINE_API WarningColor();
		static void TONIC_ENGINE_API ErrorColor();
		static void TONIC_ENGINE_API ResetColor();

	private:
		enum class Color : const unsigned char
		{
			Default = 15,
			Yellow_Black_BG = 14,
			Green_Black_BG = 10,
			// RED  BG   WHITE OFFSET
			White_Red_BG = (12 * 16 + 15)
		};
		void ChangeColor(unsigned char _handleWindowsId) const;
		void ChangeColor(Color _handleWindowsId) const;
	};
}