#pragma once

#include "TonicEngine/DLL_API.hpp"

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
const s32 bufferSize = 1024;\
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
	void TONIC_ENGINE_API FormatString(char* _buffer, u64 _bufferSize, const char* _format, ...);

	class TONIC_ENGINE_API Log
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	private:
		std::ofstream output_;
		HANDLE handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
		// Singleton /!\ BE CAREFUL: IT IS NOT THREAD SAFE
		static Log* s_instance_;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/
	private:
		Log() {};

	public:
		Log(Log& other) = delete;
		~Log();

		void operator=(const Log&) = delete;

		static Log* GetInstance();
		static void DeleteInstance();

		static void OpenFile(std::filesystem::path const& _fileName, bool _erase = false);
		static void Print(const char* _format, ...);

	private:
		void InstanceOpenFile(std::filesystem::path const& _fileName, bool _erase = false);
		void InstancePrint(const char* _format, va_list _args);

	public:
		static void SuccessColor();
		static void WarningColor();
		static void ErrorColor();
		static void ResetColor();

	private:
		enum class Color : const u8
		{
			Default = 15,
			Yellow_Black_BG = 14,
			Green_Black_BG = 10,
			// RED  BG   WHITE OFFSET
			White_Red_BG = (12 * 16 + 15)
		};
		void ChangeColor(u8 _handleWindowsId) const;
		void ChangeColor(Color _handleWindowsId) const;
	};
}
