#include "pch.hpp"

#include "Audio/miniaudio.h"
#include "Audio/miniaudio_libvorbis.h"

#include "Resources/Sound.hpp"

#include "Core/Log.hpp"

using namespace Resources;

static ma_result ma_decoding_backend_init__libvorbis(void* pUserData, ma_read_proc onRead, ma_seek_proc onSeek, ma_tell_proc onTell, void* pReadSeekTellUserData, const ma_decoding_backend_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_data_source** ppBackend)
{
	ma_result result;
	ma_libvorbis* pVorbis;

	(void)pUserData;

	pVorbis = (ma_libvorbis*)ma_malloc(sizeof(*pVorbis), pAllocationCallbacks);
	if (pVorbis == NULL) {
		return MA_OUT_OF_MEMORY;
	}

	result = ma_libvorbis_init(onRead, onSeek, onTell, pReadSeekTellUserData, pConfig, pAllocationCallbacks, pVorbis);
	if (result != MA_SUCCESS) {
		ma_free(pVorbis, pAllocationCallbacks);
		return result;
	}

	*ppBackend = pVorbis;

	return MA_SUCCESS;
}

static ma_result ma_decoding_backend_init_file__libvorbis(void* pUserData, const char* pFilePath, const ma_decoding_backend_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_data_source** ppBackend)
{
	ma_result result;
	ma_libvorbis* pVorbis;

	(void)pUserData;

	pVorbis = (ma_libvorbis*)ma_malloc(sizeof(*pVorbis), pAllocationCallbacks);
	if (pVorbis == NULL) {
		return MA_OUT_OF_MEMORY;
	}

	result = ma_libvorbis_init_file(pFilePath, pConfig, pAllocationCallbacks, pVorbis);
	if (result != MA_SUCCESS) {
		ma_free(pVorbis, pAllocationCallbacks);
		return result;
	}

	*ppBackend = pVorbis;

	return MA_SUCCESS;
}

static void ma_decoding_backend_uninit__libvorbis(void* pUserData, ma_data_source* pBackend, const ma_allocation_callbacks* pAllocationCallbacks)
{
	ma_libvorbis* pVorbis = (ma_libvorbis*)pBackend;

	(void)pUserData;

	ma_libvorbis_uninit(pVorbis, pAllocationCallbacks);
	ma_free(pVorbis, pAllocationCallbacks);
}

static ma_result ma_decoding_backend_get_channel_map__libvorbis(void* pUserData, ma_data_source* pBackend, ma_channel* pChannelMap, size_t channelMapCap)
{
	ma_libvorbis* pVorbis = (ma_libvorbis*)pBackend;

	(void)pUserData;

	return ma_libvorbis_get_data_format(pVorbis, NULL, NULL, NULL, pChannelMap, channelMapCap);
}

static ma_decoding_backend_vtable g_ma_decoding_backend_vtable_libvorbis =
{
	ma_decoding_backend_init__libvorbis,
	ma_decoding_backend_init_file__libvorbis,
	NULL, /* onInitFileW() */
	NULL, /* onInitMemory() */
	ma_decoding_backend_uninit__libvorbis
};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	ma_data_source* pDataSource = (ma_data_source*)pDevice->pUserData;
	if (pDataSource == NULL) {
		return;
	}

	ma_data_source_read_pcm_frames(pDataSource, pOutput, frameCount, NULL);

	(void)pInput;
}

Sound::Sound()
{
	type_ = ResourceType::Sound;
	//p_sound_ = new ma_sound;
	decoder = new ma_decoder;
	device = new ma_device;
}

Sound::~Sound()
{
	Stop();
	ma_device_uninit(device);
	ma_decoder_uninit(decoder);
	//ma_sound_uninit(p_sound_);
	delete device;
	delete decoder;
}

void Resources::Sound::Destroy() {}

void Sound::LoadSound()
{
	ma_result result;

	ma_decoder_config decoderConfig;
	ma_device_config deviceConfig;

	ma_format format;
	ma_uint32 channels;
	ma_uint32 sampleRate;

	ma_decoding_backend_vtable* pCustomBackendVTables[] =
	{
		&g_ma_decoding_backend_vtable_libvorbis
	};

	/* Initialize the decoder. */
	decoderConfig = ma_decoder_config_init_default();
	decoderConfig.pCustomBackendUserData = NULL;  /* In this example our backend objects are contained within a ma_decoder_ex object to avoid a malloc. Our vtables need to know about this. */
	decoderConfig.ppCustomBackendVTables = pCustomBackendVTables;
	decoderConfig.customBackendCount = sizeof(pCustomBackendVTables) / sizeof(pCustomBackendVTables[0]);

	result = ma_decoder_init_file(path.string().c_str(), &decoderConfig, decoder);
	if (result != MA_SUCCESS || decoder == nullptr)
	{
		DEBUG_ERROR("MINI AUDIO LOAD FAIL");
		return;
	}
	DEBUG_SUCCESS("MINI AUDIO LOAD SUCCESSFULLY");

	/* Initialize the device. */
	result = ma_data_source_get_data_format(decoder, &format, &channels, &sampleRate, NULL, 0);
	if (result != MA_SUCCESS)
	{
		ma_decoder_uninit(decoder);
		DEBUG_ERROR("Failed to retrieve decoder data format.");
		return;
	}
	DEBUG_SUCCESS("Succeed to retrieve decoder data format.");

	deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format = format;
	deviceConfig.playback.channels = channels;
	deviceConfig.sampleRate = sampleRate;
	deviceConfig.dataCallback = data_callback;
	deviceConfig.pUserData = decoder;

	if (ma_device_init(NULL, &deviceConfig, device) != MA_SUCCESS)
	{
		DEBUG_ERROR("Failed to open playback device.\n");
		ma_decoder_uninit(decoder);
		return;
	}
	DEBUG_SUCCESS("Succeed to open playback device.\n");
}

void Sound::Play()
{
	if (ma_device_start(device) != MA_SUCCESS)
	{
		ma_device_uninit(device);
		ma_decoder_uninit(decoder);
		DEBUG_ERROR("Failed to start playback device.\n");
	}
}

void Sound::Pause()
{
	ma_device_stop(device);
}

bool Sound::IsFinished() const
{
	ma_uint64 cursor;
	ma_uint64 length;

	if (ma_data_source_get_cursor_in_pcm_frames(decoder, &cursor) != MA_SUCCESS)
	{
		return false;
	}

	if (ma_data_source_get_length_in_pcm_frames(decoder, &length) != MA_SUCCESS)
	{
		//std::cout << "length : " << length << std::endl;
		return false;
	}

	return cursor >= length;
}

void Sound::Stop()
{
	ma_device_stop(device);
}

void Sound::SetLoop(const bool& _bLoop)
{
	ma_data_source_set_looping(&decoder, _bLoop);
}

void Sound::SetVolume(const float& _volume)
{
	//ma_sound_set_volume(decoder, _volume);
}

void Sound::SetEngineVolume(const float& _volume)
{
	ma_device_set_master_volume(device, _volume);
}

void Sound::SetPitch(const float& _pitch)
{
	//ma_sound_set_pitch(p_sound_, _pitch);
}

void Sound::SetIsSpatialized(const bool& _enabled)
{
	//ma_sound_set_spatialization_enabled(p_sound_, _enabled);
}

void Sound::SetPosition(const Maths::Vec3& _position)
{
	//ma_sound_set_position(p_sound_, _position.x, _position.y, _position.z);
}

void Sound::SetVelocity(const Maths::Vec3& _velocity)
{
	//ma_sound_set_velocity(p_sound_, _velocity.x, _velocity.y, _velocity.z);
}

void Sound::ReadFile(const fs::path _path)
{
	bRead_ = bLoaded_ = true;
}