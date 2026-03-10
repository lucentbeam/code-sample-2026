// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "wavheaderdata.h"

void WavHeaderData::read(std::ifstream &file)
{
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&preamble), sizeof(WavHeaderPreamble));

    int sz = sizeof(WavHeaderPreamble);
    WavHeaderData::SubChunk chunk;
    file.read(reinterpret_cast<char*>(&chunk), sizeof(WavHeaderData::SubChunk));
    subchunks.clear();
    subchunks.push_back(chunk);
    sz += sizeof(WavHeaderData::SubChunk);
    while (!chunk.isData()) {
        sz += chunk.size;
        file.ignore(chunk.size);
        file.read(reinterpret_cast<char*>(&chunk), sizeof(WavHeaderData::SubChunk));
        subchunks.push_back(chunk);
        sz += sizeof(WavHeaderData::SubChunk);
    }
    data_size = chunk.size;
    size = sz;
}
