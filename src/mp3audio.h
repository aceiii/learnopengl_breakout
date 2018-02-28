#ifndef __MP3_AUDIO_HEADER_GUARD__
#define __MP3_AUDIO_HEADER_GUARD__

#include <soloud/soloud.h>
#include <soloud/soloud_thread.h>
#include <soloud/soloud_audiosource.h>

#include "minimp3.h"


class MP3Audio;
class MP3AudioInstance : public SoLoud::AudioSourceInstance {
public:
    MP3AudioInstance(MP3Audio *audio);
    virtual ~MP3AudioInstance();

    virtual void getAudio(float *aBuffer, unsigned int aSamples);
    virtual bool hasEnded();

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



#endif//__MP3_AUDIO_HEADER_GUARD__
