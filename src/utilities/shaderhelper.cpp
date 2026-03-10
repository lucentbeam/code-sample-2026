// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "shaderhelper.h"

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include <iostream>
#include <unordered_map>

namespace {
    std::unordered_map<std::string, unsigned int> shaders;
}

unsigned int ShaderHelper::createShader(const char * vsrc, const char * fsrc)
{
    unsigned int vertexShader, fragmentShader;
    char failLog[512];
    int success;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vsrc, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, failLog);
        std::cerr << "ERROR: Could not compile vertex shader " << failLog;
        return 0;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &(fsrc), nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, failLog);
        std::cerr << "ERROR: Could not compile fragment shader " << failLog;
        return 0;
    }

    unsigned int shader;
    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, failLog);
        std::cerr << "ERROR: Could not link shader " << failLog;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}

void ShaderHelper::name(std::string id, unsigned int ref)
{
    shaders.emplace(id, ref);
}

void ShaderHelper::createShader(std::string id, const char *vsrc, const char *fsrc)
{
    unsigned int res = createShader(vsrc, fsrc);
    if (res > 0) {
        name(id, res);
    }
}

void ShaderHelper::bind(std::string id)
{
    auto it = shaders.find(id);
    if (it != shaders.end()) {
        glUseProgram(it->second);
    }
}

bool ShaderHelper::exists(std::string id)
{
    return shaders.find(id) != shaders.end();
}

unsigned int ShaderHelper::get(std::string id)
{
    auto it = shaders.find(id);
    if (it != shaders.end()) {
        return it->second;
    }
    return 0;
}
