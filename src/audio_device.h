#if 1
#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

#include <string>
#include <thread>

class AudioDevice
{
public:
    static bool StartCapture(const std::string &capture_file_name);
    static bool StopCapture();
};
#endif
#endif