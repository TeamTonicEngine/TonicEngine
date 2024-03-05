#pragma once

#ifdef TONICENGINE_EXPORTS
#define TONIC_ENGINE_API __declspec(dllexport)
#else
#define TONIC_ENGINE_API __declspec(dllimport)
#endif