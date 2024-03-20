#include "pch.hpp"
#include "TonicEngine/Resources/Sound.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "../../Externals/include/Audio/miniaudio.h"

#include "TonicEngine/Core/Log.hpp"

using namespace Resources;
ma_engine engine;
ma_sound m_Sound;
Sound::Sound(std::filesystem::path const _soundPath)
{
	p_soundPath_ = _soundPath;
    Init();
}

Sound::~Sound()
{
    ma_engine_uninit(&engine);
    ma_sound_uninit(&m_Sound);
}

void Sound::Init()
{
	ma_result result;
	
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) 
    {
        DEBUG_ERROR("MINI AUDIO INIT FAIL");
    }
    DEBUG_SUCCESS("MINI AUDIO INIT SUCCESSFULLY");

}

void Sound::LoadSound()
{
    ma_result result;

    result = ma_sound_init_from_file(&engine, p_soundPath_.string().c_str(), 0, NULL, NULL, &m_Sound);
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




std::string Resources::Sound::ReadFile(std::filesystem::path const _path)
{
	return "";
}

void Resources::Sound::MetaWriteFile(const string _name)
{

}

void Resources::Sound::MetaReadFile(const string _name)
{

}

void Resources::Sound::ResourceUnload()
{

}