// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef TEXTUREHELPER_H
#define TEXTUREHELPER_H

#include <unordered_map>

class TextureHelper;

class Texture {
    unsigned int m_id = 0;
    int w = 0, h = 0;

    bool failed = true;

    friend TextureHelper;
public:
    unsigned int id() const { return m_id; }
    bool available() const { return !failed; }

    int width() const { return w; }
    int height() const { return h; }
};

class TextureHelper
{
    static std::unordered_map<std::string, Texture> s_textures;

    static void loadTexture(Texture &tex, std::string filename);
public:
    static Texture fetch(std::string texture_name);
};

#endif // TEXTUREHELPER_H
