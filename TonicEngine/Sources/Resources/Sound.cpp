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
	ma_result result;
	ma_data_source* pDataSource = (ma_data_source*)pDevice->pUserData;
	if (pDataSource == NULL) 
	{
		DEBUG_ERROR("DataSource is NULL.");
		return;
	}
	result = ma_data_source_read_pcm_frames(pDataSource, pOutput, frameCount, NULL);
	if (result != MA_SUCCESS) return;
	/*
	ma_spatializer* pDataSpat = (ma_spatializer*)pDevice->pUserData;
	if (pDataSpat == NULL)
	{
		DEBUG_ERROR("DataSpat is NULL.");
		return;
	}

	ma_spatializer_listener* pDataSpatList = (ma_spatializer_listener*)pDevice->pUserData;
	if (pDataSpatList == NULL)
	{
		DEBUG_ERROR("DataSpat is NULL.");
		return;
	}

	bool pDataIsSpat = (bool)pDevice->pUserData;
	if (pDataIsSpat == NULL)
	{
		DEBUG_ERROR("DataSpat is NULL.");
		return;
	}

	if (pDataIsSpat && pDataSpat)
	{
		ma_spatializer_process_pcm_frames(pDataSpat, pDataSpatList, pOutput, pInput,frameCount);
	}

	*/

	(void)pInput;
}

Sound::Sound()
{
	type_ = ResourceType::Sound;
	decoder = new ma_decoder;
	device = new ma_device;
	spatializer = nullptr;
	listener = new ma_spatializer_listener;
}

Sound::~Sound()
{
	Stop();
	if (device)
	{
		ma_device_uninit(device);
		delete device;
	}
	if (decoder)
	{
		ma_decoder_uninit(decoder);
		delete decoder;
	}
	if (spatializer) 
	{
		ma_spatializer_uninit(spatializer, NULL);
		ma_spatializer_listener_uninit(listener, NULL);
		delete spatializer;
	}
}

void Resources::Sound::Destroy() {}

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

void Sound::Restart()
{
	// Cursor to 0
	ma_data_source_seek_to_pcm_frame(decoder, 0);
	Play();
}

void Sound::Play()
{
	if (IsFinished())
	{
		DEBUG_SUCCESS("THE SOUND IS FINISHED");
		Restart();
	}
	if (ma_device_start(device) != MA_SUCCESS)
	{
		ma_device_uninit(device);
		ma_decoder_uninit(decoder);
		DEBUG_ERROR("Failed to start playback device.\n");
	}
	DEBUG_SUCCESS("IT PLAY");

}

void Sound::Pause()
{
	ma_device_stop(device);
}

float Sound::GetOggFileDuration()
{
	return -1;
	/*
	FILE* file = nullptr;
	if (fopen_s(&file, path.string().c_str(), "rb") != 0) {
		std::cerr << "Failed to open file: " << path.string().c_str() << std::endl;
		return -1.0;
	}

	OggVorbis_File vf;
	if (ov_open(file, &vf, NULL, 0) < 0) {
		std::cerr << "Input does not appear to be an Ogg bitstream: " << path.string().c_str() << std::endl;
		fclose(file);
		return -1.0;
	}

	// Obtenez les informations sur le flux Vorbis
	vorbis_info* vi = ov_info(&vf, -1);
	if (!vi) {
		std::cerr << "Failed to get vorbis info: " << path.string().c_str() << std::endl;
		ov_clear(&vf);
		return -1.0;
	}

	// Obtenez la durée en secondes
	double duration = ov_time_total(&vf, -1);
	if (duration == OV_EINVAL) {
		std::cerr << "Unable to get total time: " << path.string().c_str() << std::endl;
		ov_clear(&vf);
		return -1.0;
	}

	// Nettoyez
	ov_clear(&vf);
	std::cout << duration << std::endl;
	return duration;
	*/
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
		return false;
	}

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
	ma_device_stop(device);
	ma_data_source_seek_to_pcm_frame(decoder, 0);
}

void Sound::SetVolume(const float& _volume)
{
	ma_device_set_master_volume(device, _volume);
}

void Sound::SetLoop(const bool& _bLoop)
{
	ma_data_source_set_looping(decoder, _bLoop);
}

void Sound::SetPitch(const float& _pitch)
{
	pitch_ = _pitch;
}

void Sound::InitSpatialization()
{
	spatializer = new ma_spatializer;
	ma_uint32 channelsIn = 2;
	ma_uint32 channelsOut = 2;

	ma_spatializer_config spatializerConfig = ma_spatializer_config_init(channelsIn, channelsOut);
	if (ma_spatializer_init(&spatializerConfig, NULL ,spatializer) != MA_SUCCESS) 
	{
		DEBUG_ERROR("Failed to initialize spatializer");
		return;
	}
	DEBUG_SUCCESS("Spatializer initialized successfully");

	// init listener
	ma_spatializer_listener_config listenerConfig = ma_spatializer_listener_config_init(channelsOut);
	if (ma_spatializer_listener_init(&listenerConfig, NULL, listener) != MA_SUCCESS) 
	{
		DEBUG_ERROR("Failed to initialize spatializer listener");
		return;
	}
	DEBUG_SUCCESS("Spatializer listener initialized successfully");
}

void Sound::SetIsSpatialized(const bool& _enabled)
{
	if (_enabled) 
	{
		if (!isSpatialized) 
		{
			InitSpatialization();
			isSpatialized = true;
			DEBUG_SUCCESS("Spatialization enabled");
		}
	}
	else 
	{
		if (isSpatialized) 
		{
			ma_spatializer_uninit(spatializer, NULL);
			isSpatialized = false;
			DEBUG_SUCCESS("Spatialization disabled");
		}
	}
	listener->isEnabled = _enabled;
}

void Sound::SetPosition(const Maths::Vec3& _position)
{
	if (isSpatialized && spatializer)
	{
		ma_spatializer_set_position(spatializer, _position.x, _position.y, _position.z);
	}
}

void Sound::SetVelocity(const Maths::Vec3& _velocity)
{
	if (isSpatialized) 
	{
		ma_spatializer_set_velocity(spatializer, _velocity.x, _velocity.y, _velocity.z);
	}
}

void Sound::ReadFile(const fs::path _path)
{
	bRead_ = bLoaded_ = true;
}
