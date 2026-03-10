// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "audio.h"

#include <iostream>
#include <iterator>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "utilities/wavheaderdata.h"

static constexpr int audio_frequency = 44100;
static constexpr int audio_samples = 512;
static constexpr int audio_channels = 2;

struct AudioData {
    int channels = 1;
    const int16_t * data = nullptr;
    size_t sample_count = 0;
};

class AudioController
{
    const int16_t * start;
    const int16_t * end;

    const int16_t * position;

    int channels;

    void stop();
public:
    void play(AudioData data);

    bool playing() const;

    void readToBuffer(int16_t *buffer, int samplecount, int channels, float scaling);
};

void AudioController::stop()
{
    position = end + 1;
}

void AudioController::play(AudioData data)
{
    start = data.data;
    end = start + data.sample_count;
    position = start;
    channels = data.channels;
}

bool AudioController::playing() const
{
    return start != nullptr && position <= end && position >= start;
}

void AudioController::readToBuffer(int16_t *buffer, int samplecount, int channelcount, float scale)
{
    uint8_t scaling_factor = uint8_t(scale * 255.0f);
    if (channelcount == channels) {
        samplecount *= channelcount;
        while (samplecount > 0) {
            if (!playing()) {
                return;
            }
            *buffer++ += int16_t((int(*position) * int(scaling_factor)) >> 8);
            position++;
            samplecount--;
        }
    } else if (channels < channelcount) {
        int mult = channelcount / channels;
        while (samplecount > 0) {
            if (!playing()) {
                return;
            }
            int16_t s = int16_t((int(*position) * int(scaling_factor)) >> 8);
            for(int i = 0; i < mult; ++i) {
                *buffer++ += s;
            }
            position++;
            samplecount--;
        }
    } else {
        std::cerr << "ERROR: Mixing channels from file type not supported." << std::endl;
        std::cerr << "       file channel count: " << channels << std::endl;
        std::cerr << "       buffer channel count: " << channelcount << std::endl;

        stop();
    }
    position += samplecount;
}

unsigned int Audio::audio_device_id = 0;

AudioController Audio::s_clips[];

bool Audio::initialize()
{
    SDL_AudioSpec spec;
    spec.callback = updateSound;
    spec.samples = audio_samples;
    spec.channels = audio_channels;
    spec.freq = int(audio_frequency);
    spec.format = AUDIO_S16;
    SDL_AudioSpec result;
    audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &spec, &result, 0);

    if (audio_device_id == 0) {
        std::cerr << "ERROR: Failed to open audio; " << SDL_GetError() << std::endl;
        return false;
    } else {
        if (result.format != spec.format) {
            std::cout << "WARNING: Couldn't open audio with format int16_t" << std::endl;
        }
        SDL_PauseAudioDevice(audio_device_id, false);
    }

    return true;
}

void Audio::updateSound(void *, uint8_t *b, int length)
{
    int16_t *buffer = reinterpret_cast<int16_t*>(b);
    std::memset(buffer, 0, size_t(length));
    for(int i = 0; i < channel_count; ++i) {
        if (s_clips[i].playing()) {
            s_clips[i].readToBuffer(buffer, audio_samples, audio_channels, 1.0f / channel_count);
        }
    }
}

AudioData Audio::getData(std::string id)
{
    static std::unordered_map<std::string, AudioData> store;

    auto it = store.find(id);
    if (it != store.end()) {
        return it->second;
    }

    std::string path = "resources/audio/" + id + ".wav";
    std::ifstream file(path, std::ios::binary);

    AudioData data;
    if (!file.good()) {
        std::cerr << "ERROR: Couldn't open audio resource " << id.c_str() << std::endl;
        store.emplace(id, data);
        return data;
    }

    WavHeaderData header;
    header.read(file);

    data.channels = header.preamble.NumChannels;
    data.sample_count = size_t(header.sampleCount());
    int16_t * samples = new int16_t[data.sample_count];

    file.read(reinterpret_cast<char*>(samples), header.data_size);
    file.close();

    data.data = samples;

    store.emplace(id, data);

    return data;
}

void Audio::playSFX(std::string name)
{
    for (int i = 0; i < channel_count; i++) {
        if (!s_clips[i].playing()) {
            s_clips[i].play(getData(name));
            return;
        }
    }
}
