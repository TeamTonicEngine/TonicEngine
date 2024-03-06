#pragma once

#include "Core/Utils/Type.hpp"

#ifdef TONICENGINE_EXPORTS
#define TONIC_ENGINE_API __declspec(dllexport)
#else
#define TONIC_ENGINE_API __declspec(dllimport)
#endif