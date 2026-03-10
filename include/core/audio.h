// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef AUDIO_H
#define AUDIO_H

#include <string>

struct AudioData;
class AudioController;
class Audio {
    static unsigned int audio_device_id;

    static constexpr int channel_count = 8;
    static AudioController s_clips[channel_count];

    static void updateSound(void *userdata, uint8_t *buffer, int length);

    static AudioData getData(std::string id);
public:
    static bool initialize();

    static void playSFX(std::string name);
};

#endif // AUDIO_H
