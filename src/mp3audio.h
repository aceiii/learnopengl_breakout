#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <soloud.h>

#include "minimp3.h"


class MP3Audio;

class MP3AudioInstance : public SoLoud::AudioSourceInstance {
    using size_t = std::size_t;

public:
    MP3AudioInstance(MP3Audio *audio);
    ~MP3AudioInstance() override;

    unsigned int getAudio(float *aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize) override;
    bool hasEnded() override;

private:
    MP3Audio *_audio {nullptr};
    size_t _offset {0};

};

class MP3Audio : public SoLoud::AudioSource {
public:
    MP3Audio();
    virtual ~MP3Audio();

    void load(const std::string &filename);
    virtual SoLoud::AudioSourceInstance *createInstance();

    const float *getLeftSamples() const;
    const float *getRightSamples() const;
    size_t getChannelCount() const;
    size_t getSampleCount() const;

private:
    mp3dec_t _mp3d {};
    std::vector<float> _left_samples {};
    std::vector<float> _right_samples {};
    size_t _num_samples {0};
    size_t _num_channels {0};

};
