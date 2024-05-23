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
	if (result != MA_SUCCESS)
	{
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
	if (pVorbis == NULL)
	{
		return MA_OUT_OF_MEMORY;
	}

	result = ma_libvorbis_init_file(pFilePath, pConfig, pAllocationCallbacks, pVorbis);
	if (result != MA_SUCCESS)
	{
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
	Sound* pSound = (Sound*)pDevice->pUserData;

	if (pSound == nullptr)
	{
		DEBUG_ERROR("Sound pointer is NULL.");
		return;
	}
	if (pSound->p_decoder == nullptr)
	{
		DEBUG_ERROR("Decoder pointer is NULL.");
		return;
	}
	if (pOutput == nullptr)
	{
		DEBUG_ERROR("Output buffer is NULL.");
		return;
	}

	// Read PCM frames from the decoder into a temporary buffer.
	size_t frameSize = ma_get_bytes_per_frame(pDevice->playback.format, pDevice->playback.channels);
	void* pTempBuffer = malloc(frameCount * frameSize);
	if (pTempBuffer == nullptr)
	{
		DEBUG_ERROR("Failed to allocate memory for temporary buffer.");
		return;
	}

	ma_result result = ma_data_source_read_pcm_frames(pSound->p_decoder, pTempBuffer, frameCount, NULL);
	if (result != MA_SUCCESS)
	{
		DEBUG_ERROR("Failed to read PCM frames.");
		free(pTempBuffer);
		return;
	}

	// If spatialization is enabled, process the output buffer.
	if (pSound->isSpatialized)
	{
		if (pSound->p_spatializer == nullptr)
		{
			DEBUG_ERROR("Spatializer pointer is NULL.");
			free(pTempBuffer);
			return;
		}
		if (pSound->p_listener == nullptr)
		{
			DEBUG_ERROR("Listener pointer is NULL.");
			free(pTempBuffer);
			return;
		}

		// Cast void* pointers to their appropriate types
		//ma_spatializer_listener* pListener = (ma_spatializer_listener*)pSound->p_listener;
		//ma_spatializer* pSpatializer = (ma_spatializer*)pSound->p_spatializer;

		// Process spatialization
		result = ma_spatializer_process_pcm_frames((ma_spatializer*)pSound->p_spatializer, (ma_spatializer_listener*)pSound->p_listener, pOutput, pTempBuffer, frameCount);
		if (result != MA_SUCCESS)
		{
			DEBUG_ERROR("Failed to process spatialization.");
			free(pTempBuffer);
			return;
		}
	}
	else
	{
		// If spatialization is not enabled, copy the temporary buffer to the output buffer.
		memcpy(pOutput, pTempBuffer, frameCount * frameSize);
	}

	free(pTempBuffer);

	(void)pInput; // Silence unused variable warning
}

Sound::Sound()
{
	type_ = ResourceType::Sound;
	p_decoder = new ma_decoder();
	p_device_ = new ma_device();

	p_spatializer = new ma_spatializer;
	p_listener = new ma_spatializer_listener;

	isSpatialized = false;
	DEBUG_SUCCESS("Sound object created.");
}

Sound::~Sound()
{
	Stop();
	if (p_device_)
	{
		ma_device_uninit(p_device_);
		delete p_device_;
		p_device_ = nullptr;
		DEBUG_SUCCESS("Device uninitialized and deleted.");
	}
	if (p_decoder)
	{
		ma_decoder_uninit(p_decoder);
		delete p_decoder;
		p_decoder = nullptr;
		DEBUG_SUCCESS("Decoder uninitialized and deleted.");
	}
	if (p_spatializer != nullptr)
	{
		delete p_spatializer;
		delete p_listener;
		p_spatializer = nullptr;
		p_listener = nullptr;
		DEBUG_SUCCESS("Spatializer and listener uninitialized and deleted.");
	}
	DEBUG_SUCCESS("Sound object destroyed.");
}

bool FileExistsAndAccessible(const std::string& filePath)
{
	std::ifstream file(filePath);
	return file.good();
}

void Sound::LoadSound()
{
	ma_result result;

	ma_format format;
	ma_uint32 channels;
	ma_uint32 sampleRate;

	ma_decoder_config decoderConfig;
	ma_device_config deviceConfig;

	ma_decoding_backend_vtable* pCustomBackendVTables[] =
	{
		&g_ma_decoding_backend_vtable_libvorbis
	};

	// Initialize the decoder.
	decoderConfig = ma_decoder_config_init_default();
	decoderConfig.pCustomBackendUserData = NULL;  // In this example our backend objects are contained within a ma_decoder_ex object to avoid a malloc. Our vtables need to know about this.
	decoderConfig.ppCustomBackendVTables = pCustomBackendVTables;
	decoderConfig.customBackendCount = sizeof(pCustomBackendVTables) / sizeof(pCustomBackendVTables[0]);

	result = ma_decoder_init_file(path.string().c_str(), &decoderConfig, p_decoder);
	if (result != MA_SUCCESS || p_decoder == nullptr)
	{
		DEBUG_ERROR("MINI AUDIO LOAD FAIL");
		return;
	}
	DEBUG_SUCCESS("MINI AUDIO LOAD SUCCESSFULLY");

	// Initialize the device.
	result = ma_data_source_get_data_format(p_decoder, &format, &channels, &sampleRate, NULL, 0);
	if (result != MA_SUCCESS)
	{
		ma_decoder_uninit(p_decoder);
		DEBUG_ERROR("Failed to retrieve decoder data format.");
		return;
	}
	DEBUG_SUCCESS("Succeed to retrieve decoder data format.");

	deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format = format;
	deviceConfig.playback.channels = channels;
	deviceConfig.sampleRate = sampleRate;
	deviceConfig.dataCallback = data_callback;
	deviceConfig.pUserData = this;

	if (ma_device_init(NULL, &deviceConfig, p_device_) != MA_SUCCESS)
	{
		DEBUG_ERROR("Failed to open playback device.\n");
		ma_decoder_uninit(p_decoder);
		return;
	}
	DEBUG_SUCCESS("Succeed to open playback device.\n");
}

void Sound::Restart()
{
	// Cursor to 0
	ma_data_source_seek_to_pcm_frame(p_decoder, 0);
	Play();
}

void Sound::Play()
{
	if (IsFinished())
	{
		DEBUG_SUCCESS("THE SOUND IS FINISHED");
		Restart();
	}
	if (!p_device_) DEBUG_ERROR("DEVICE IS NULL ON PLAY.\n");
	if (ma_device_start(p_device_) != MA_SUCCESS)
	{
		ma_device_uninit(p_device_);
		ma_decoder_uninit(p_decoder);
		DEBUG_ERROR("Failed to start playback device.\n");
		return;
	}
	DEBUG_SUCCESS("Playback started.");
}

void Sound::Pause() { ma_device_stop(p_device_); }

bool Sound::IsFinished() const
{
	ma_uint64 cursor;
	ma_uint64 length;

	if (ma_data_source_get_cursor_in_pcm_frames(p_decoder, &cursor) != MA_SUCCESS)
		return false;

	if (ma_data_source_get_length_in_pcm_frames(p_decoder, &length) != MA_SUCCESS)
		return false;

	if (length == 0.0f)
	{
		DEBUG_ERROR("THE SOUND CAN'T HAVE A LENGTH OF 0 seconde");
		return false;
	}

	std::cout << "cursor : " << cursor << std::endl;
	std::cout << "length : " << length << std::endl;

	return cursor >= length;
}

void Sound::Stop()
{
	if (p_device_)
		ma_device_stop(p_device_);

	if (p_decoder)
		ma_data_source_seek_to_pcm_frame(p_decoder, 0);
}

void Sound::SetVolume(const float& _volume) { ma_device_set_master_volume(p_device_, _volume); }

void Sound::SetLoop(const bool& _bLoop) { ma_data_source_set_looping(p_decoder, _bLoop); }

void Sound::SetPitch(const float& _pitch) { pitch_ = _pitch; }

void Sound::InitSpatialization()
{
	if (p_spatializer == nullptr)
		p_spatializer = new ma_spatializer;

	ma_uint32 channelsIn = 2;
	ma_uint32 channelsOut = 2;

	ma_spatializer_config spatializerConfig = ma_spatializer_config_init(channelsIn, channelsOut);

	if (ma_spatializer_init(&spatializerConfig, NULL, (ma_spatializer*)p_spatializer) != MA_SUCCESS)
	{
		DEBUG_ERROR("Failed to initialize spatializer");
		return;
	}
	DEBUG_SUCCESS("Spatializer initialized successfully");

	if (p_listener == nullptr)
		p_listener = new ma_spatializer_listener();

	ma_spatializer_listener_config listenerConfig = ma_spatializer_listener_config_init(channelsOut);
	if (ma_spatializer_listener_init(&listenerConfig, NULL, (ma_spatializer_listener*)p_listener) != MA_SUCCESS)
	{
		DEBUG_ERROR("Failed to initialize spatializer listener");
		return;
	}
	DEBUG_SUCCESS("Spatializer listener initialized successfully");

	// Set default listener position (usually at the origin)
	ma_spatializer_listener_set_position((ma_spatializer_listener*)p_listener, 0.0f, 0.0f, 0.0f);
}

void Sound::SetIsSpatialized(const bool& _enabled)
{
	if (_enabled)
	{
		InitSpatialization();
		isSpatialized = true;
		DEBUG_SUCCESS("Spatialization enabled");
	}
	else
	{
		ma_spatializer_uninit((ma_spatializer*)p_spatializer, NULL);
		isSpatialized = false;
		DEBUG_SUCCESS("Spatialization disabled");
	}
	((ma_spatializer_listener*)p_listener)->isEnabled = _enabled;
}

void Sound::SetPosition(const Maths::Vec3& _position)
{
	if (isSpatialized && p_spatializer)
		ma_spatializer_set_position((ma_spatializer*)p_spatializer, _position.x, _position.y, _position.z);
}

void Sound::SetVelocity(const Maths::Vec3& _velocity)
{
	if (isSpatialized)
		ma_spatializer_set_velocity((ma_spatializer*)p_spatializer, _velocity.x, _velocity.y, _velocity.z);
}

void Sound::ReadFile(const fs::path _path) { bRead_ = bLoaded_ = true; }