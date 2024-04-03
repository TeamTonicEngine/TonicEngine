#include "pch.hpp"
#include "Resources/Sound.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "Audio/miniaudio.h"
#include "Core/Log.hpp"

using namespace Resources;
ma_engine engine;
ma_sound m_Sound;
Sound::Sound() { type_ = ResourceType::Sound; }

Sound::~Sound()
{
	ma_engine_uninit(&engine);
	ma_sound_uninit(&m_Sound);
}

void Sound::LoadSound()
{
	ma_result result;

	result = ma_sound_init_from_file(&engine, path.string().c_str(), 0, NULL, NULL, &m_Sound);
	if (result != MA_SUCCESS)
	{
		DEBUG_ERROR("MINI AUDIO LOAD FAIL");
	}
	DEBUG_SUCCESS("MINI AUDIO LOAD SUCCESSFULLY");

	// TODO : .OGG SOUND FILE
}

void Sound::Play(const float& _millisecond)
{
	ma_sound_set_start_time_in_milliseconds(&m_Sound, ma_engine_get_time_in_milliseconds(&engine) + _millisecond);
	ma_sound_start(&m_Sound);
}

void Sound::Pause()
{
	ma_sound_stop(&m_Sound);
}

void Sound::Stop()
{
	ma_sound_seek_to_pcm_frame(&m_Sound, 0);
	ma_sound_stop(&m_Sound);
}

void Sound::SetLoop(const bool& _bLoop)
{
	ma_sound_set_looping(&m_Sound, _bLoop);
}

void Sound::SetVolume(const float& volume)
{
	ma_sound_set_volume(&m_Sound, volume);
}

void Sound::ReadFile(const string _name)
{
	if (!path.empty())
		ReadFile(path);
	else
	{
		auto path = FindFile(_name, type_);
		ReadFile(path);
	}
}

void Sound::ReadFile(const fs::path _path)
{
	path = _path;
	ma_result result;

	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS)
	{
		DEBUG_ERROR("MINI AUDIO INIT FAIL");
	}
	DEBUG_SUCCESS("MINI AUDIO INIT SUCCESSFULLY");
}