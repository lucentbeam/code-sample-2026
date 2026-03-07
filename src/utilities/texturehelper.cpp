#include "texturehelper.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include <glad/glad.h>

std::unordered_map<std::string, Texture> TextureHelper::s_textures;

void TextureHelper::loadTexture(Texture &tex, std::string filename)
{

    // get filepath
    std::string path = "resources/textures/" + filename + ".png";

    // load via stb_image
    unsigned char * data = stbi_load(path.c_str(), &tex.w, &tex.h, nullptr, 4);
    if (stbi_failure_reason()) {
        std::cout << "ERROR: Could not load texture " << filename.c_str() << ", reason: " << stbi_failure_reason() << std::endl;
        return;
    }

    // upload texture
    glGenTextures(1, &tex.m_id);
    glBindTexture(GL_TEXTURE_2D, tex.id());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    tex.failed = false;

    stbi_image_free(data);
}

Texture TextureHelper::fetch(std::string texture_name)
{
    auto it = s_textures.find(texture_name);
    if (it != s_textures.end()) {
        return it->second;
    }

    Texture tex;
    loadTexture(tex, texture_name);
    s_textures.emplace(texture_name, tex);

    return tex;
}
