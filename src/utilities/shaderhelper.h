// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef SHADERHELPER_H
#define SHADERHELPER_H

#include <string>

namespace ShaderHelper
{
    unsigned int createShader(const char * vsrc, const char * fsrc);
    void createShader(std::string id, const char * vsrc, const char * fsrc);

    void name(std::string id, unsigned int ref);

    unsigned int get(std::string id);
    bool exists(std::string id);

    void bind(std::string id);

}

#endif // SHADERHELPER_H
