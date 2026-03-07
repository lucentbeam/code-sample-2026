#ifndef WAVHEADERDATA_H
#define WAVHEADERDATA_H

#include <cstdint>
#include <fstream>
#include <vector>

struct WavHeaderData {
#pragma pack(push,4)
    struct WavHeaderPreamble {
        char ChunkId[4];
        int32_t ChunkSize;
        char Format[4];
        char Subchunk1ID[4];
        int32_t Subchunk1Size;
        int16_t AudioFormat;
        int16_t NumChannels;
        int32_t SampleRate;
        int32_t ByteRate;
        int16_t BlockAlign;
        int16_t BitsPerSample;
    } preamble;
#pragma pack(pop)

    struct SubChunk {
        char id[4];
        int32_t size;
        bool isData() const { return tolower(id[0]) == 'd' && tolower(id[1]) == 'a' && tolower(id[2]) == 't' && tolower(id[3]) == 'a'; }
    };

    int32_t data_size;
    int size;

    std::vector<SubChunk> subchunks;

    bool isUncompressed() const { return preamble.AudioFormat == 1; }
    int sampleCount() const { return data_size / (preamble.NumChannels * preamble.BitsPerSample / 8); }

    void read(std::ifstream &file);
};

#endif // WAVHEADERDATA_H
