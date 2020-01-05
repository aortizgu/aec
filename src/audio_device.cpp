#if 1

#include "audio_device.h"

#define DR_FLAC_IMPLEMENTATION
#include <extras/dr_flac.h> /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include <extras/dr_mp3.h> /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include <extras/dr_wav.h> /* Enables WAV decoding. */
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <easylogging++.h>

#include <iostream>

static bool initialized = false;
static bool capturing = false;
ma_device captureDevice;
drwav wav;

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    drwav *pWav = (drwav *)pDevice->pUserData;
    if (pWav != NULL)
    {
        drwav_write_pcm_frames(pWav, frameCount, pInput);
    }
    (void)pOutput;
}

bool AudioDevice::StartCapture(const std::string &capture_file_name)
{
    if (capture_file_name.empty())
    {
        LOG(ERROR) << "empty filename";
        return false;
    }
    if (capturing)
    {
        LOG(ERROR) << "already capturing";
        return false;
    }

    ma_result result;
    drwav_data_format wavFormat;
    ma_device_config deviceConfig;

    wavFormat.container = drwav_container_riff;
    wavFormat.format = DR_WAVE_FORMAT_IEEE_FLOAT;
    wavFormat.channels = 2;
    wavFormat.sampleRate = 44100;
    wavFormat.bitsPerSample = 32;

    if (drwav_init_file_write(&wav, capture_file_name.c_str(), &wavFormat, NULL) == DRWAV_FALSE)
    {
        LOG(ERROR) << "Failed to initialize output file.";
        return false;
    }

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format = ma_format_f32;
    deviceConfig.capture.channels = wavFormat.channels;
    deviceConfig.sampleRate = wavFormat.sampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &wav;

    result = ma_device_init(NULL, &deviceConfig, &captureDevice);
    if (result != MA_SUCCESS)
    {
        LOG(ERROR) << "Failed to initialize capture device.";

        return false;
    }

    result = ma_device_start(&captureDevice);
    if (result != MA_SUCCESS)
    {
        ma_device_uninit(&captureDevice);
        LOG(ERROR) << "Failed to start capture device.";
        return false;
    }

    capturing = true;
    return true;
}

bool AudioDevice::StopCapture()
{
    if (!capturing)
    {
        return false;
    }
    ma_device_uninit(&captureDevice);
    drwav_uninit(&wav);
    capturing = false;
    return true;
}

#endif