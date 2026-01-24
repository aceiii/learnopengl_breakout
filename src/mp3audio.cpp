#include "mp3audio.h"

#include <array>
#include <vector>
#include <fstream>
#include <iostream>


typedef unsigned char byte;
typedef std::vector<byte> byte_buffer_t;

byte_buffer_t loadFile(const std::string &filename) {
    byte_buffer_t buffer;

    std::ifstream file {filename};
    if (!file) {
        std::cerr << "Failed to load file: " << filename << '\n';
        return buffer;
    }

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(size);
    auto *buf = reinterpret_cast<char*>(buffer.data());
    file.read(buf, size);

    return buffer;
}


MP3AudioInstance::MP3AudioInstance(MP3Audio *audio)
    : SoLoud::AudioSourceInstance {},
      _audio {audio}
{
}

MP3AudioInstance::~MP3AudioInstance() {
}

unsigned int MP3AudioInstance::getAudio(float *aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize) {
    if (!_audio) {
        return 0;
    }

    size_t written = 0;
    size_t num_samples = _audio->getSampleCount();
    const float *left_data = _audio->getLeftSamples();
    const float *right_data = _audio->getRightSamples();
    const float *data[] = {left_data, right_data};

    auto bytes_to_write = std::min(aSamplesToRead, aBufferSize);

    while (written < bytes_to_write) {
        size_t to_write = std::min<size_t>(bytes_to_write - written, num_samples - _offset);
        for (size_t i = 0; i < mChannels; i += 1) {
            memcpy(aBuffer + i * bytes_to_write, data[i] + _offset, sizeof(float) * to_write);
        }

        written += to_write;
        _offset += to_write;

        size_t left = bytes_to_write - written;
        if (left && !(mFlags & AudioSourceInstance::LOOPING)) {
            for (size_t i = 0; i < mChannels; i += 1) {
                memset(aBuffer + i * bytes_to_write + written, 0, sizeof(float) * (bytes_to_write - to_write));
            }

            if (_offset >= num_samples) {
                _offset = 0;
            }
        }

        if (_offset >= _audio->getSampleCount() && (mFlags & AudioSourceInstance::LOOPING)) {
            _offset = 0;
        }
    }

    return written;
}

bool MP3AudioInstance::hasEnded() {
    return !(mFlags & AudioSourceInstance::LOOPING) && _offset >= _audio->getSampleCount();
}


MP3Audio::MP3Audio() {
    mp3dec_init(&_mp3d);
}

MP3Audio::~MP3Audio() {
    stop();
}

void MP3Audio::load(const std::string &filename) {
    byte_buffer_t mp3_buffer = loadFile(filename);
    byte *buf = mp3_buffer.data();
    size_t buf_size = mp3_buffer.size();

    mp3dec_frame_info_t info;

    _left_samples.reserve(buf_size);
    _right_samples.reserve(buf_size);

    do {
        std::array<short, MINIMP3_MAX_SAMPLES_PER_FRAME> pcm;
        int samples = mp3dec_decode_frame(&_mp3d, buf, buf_size, pcm.data(), &info);

        for (auto i = 0; i < (samples * info.channels); i += 1) {
            int j = i % 2;

            float s = float(pcm[i]) / float(32768);

            if (j == 0) {
                _left_samples.push_back(s);
            } else {
                _right_samples.push_back(s);
            }
        }


        buf += info.frame_bytes;
        buf_size -= info.frame_bytes;
    } while (info.frame_bytes > 0);

    _num_channels = info.channels;
    _num_samples = _left_samples.size();
}

SoLoud::AudioSourceInstance *MP3Audio::createInstance() {
    return new MP3AudioInstance {this};
}

const float *MP3Audio::getLeftSamples() const {
    return _left_samples.data();
}

const float *MP3Audio::getRightSamples() const {
    return _right_samples.data();
}

size_t MP3Audio::getChannelCount() const {
    return _num_channels;
}

size_t MP3Audio::getSampleCount() const {
    return _num_samples;
}
