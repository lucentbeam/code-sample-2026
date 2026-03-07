#ifndef FONT_H
#define FONT_H

#include <cstdint>
#include <string>

extern int magofont3_width;
extern int magofont3_height;
extern uint8_t magofont3_data[];
extern uint8_t magofont3_glyph_widths[];

class Font
{
    static void load();

    static const int letter_spacing = 1;
public:
    static int getGlyphWidth(int idx);

    static int getGlyphHBound();
    static int getGlyphVBound();

    static double getLineWidth(std::string line, double scale = 1);
    static double getLineHeight(double scale = 1);

    static void bind();
};

#endif // FONT_H
