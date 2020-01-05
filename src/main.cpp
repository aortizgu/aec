
#if 1
#include <string>
#include <iostream>
#include <thread>

#include <easylogging++.h>
// #include <belle.hh>
// namespace Belle = OB::Belle;

#include "audio_device.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
    el::Loggers::configureFromGlobal("log.conf");
    LOG(INFO) << "AEC Starts";

    std::string cmd;
    do
    {
        std::cin >> cmd;
        if (cmd == "exit")
        {
            break;
        }
        else if (cmd == "capture_start")
        {
            std::string file_name;
            LOG(INFO) << "instert file name";
            std::cin >> file_name;
            if (!cmd.empty())
            {
                LOG(INFO) << "result: " << AudioDevice::StartCapture(file_name);
            }
            else
            {
                LOG(ERROR) << "instert file name fail";
            }
        }
        else if (cmd == "capture_stop")
        {
            LOG(ERROR) << "result: " << AudioDevice::StopCapture();
        }
        else
        {
            LOG(ERROR) << "unknow command[exit|capture_start|capture_stop]";
        }
    } while (true);
    return 0;
}
#else

/* This example simply captures data from your default microphone until you press Enter. The output is saved to the file specified on the command line. */

#define DR_FLAC_IMPLEMENTATION
#include <miniaudio/extras/dr_flac.h> /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include <miniaudio/extras/dr_mp3.h> /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include <miniaudio/extras/dr_wav.h> /* Enables WAV decoding. */
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

#include <stdlib.h>
#include <stdio.h>

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    drwav *pWav = (drwav *)pDevice->pUserData;
    ma_assert(pWav != NULL);

    drwav_write_pcm_frames(pWav, frameCount, pInput);

    (void)pOutput;
}

int main(int argc, char **argv)
{
    ma_result result;
    drwav_data_format wavFormat;
    drwav wav;
    ma_device_config deviceConfig;
    ma_device device;

    if (argc < 2)
    {
        printf("No input file.\n");
        return -1;
    }

    wavFormat.container = drwav_container_riff;
    wavFormat.format = DR_WAVE_FORMAT_IEEE_FLOAT;
    wavFormat.channels = 2;
    wavFormat.sampleRate = 44100;
    wavFormat.bitsPerSample = 32;

    if (drwav_init_file_write(&wav, argv[1], &wavFormat, NULL) == DRWAV_FALSE)
    {
        printf("Failed to initialize output file.\n");
        return -1;
    }

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format = ma_format_f32;
    deviceConfig.capture.channels = wavFormat.channels;
    deviceConfig.sampleRate = wavFormat.sampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &wav;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize capture device.\n");
        return -2;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS)
    {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        return -3;
    }

    printf("Press Enter to stop recording...\n");
    getchar();

    ma_device_uninit(&device);
    drwav_uninit(&wav);

    return 0;
}
#endif