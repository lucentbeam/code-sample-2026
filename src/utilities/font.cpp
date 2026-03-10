// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "font.h"

#include <string>
#include <fstream>

#include "glad/glad.h"

namespace {

    int font_width = magofont3_width;
    int font_height = magofont3_height;
    uint8_t * glyph_widths = magofont3_glyph_widths;
    uint8_t * texture_data = magofont3_data;

    unsigned int texture_id = 0;
}

void Font::load()
{
    if (texture_id > 0) return;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_width, font_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
}

void Font::bind()
{
    load();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

int Font::getGlyphWidth(int idx)
{
    return glyph_widths[idx] + letter_spacing;
}

int Font::getGlyphHBound()
{
    return font_width / 16;
}

int Font::getGlyphVBound()
{
    return font_height / 16;
}

double Font::getLineWidth(std::string line, double scale)
{
    double len = 0;
    for(const char &ch : line) {
        len += double(getGlyphWidth(ch) + letter_spacing) * scale;
    }
    return len;
}

double Font::getLineHeight(double scale)
{
    return scale * double(getGlyphVBound());
}
