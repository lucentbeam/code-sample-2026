#include <window.h>

#include <iostream>

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "utilities/shaderhelper.h"
#include "utilities/texturehelper.h"
#include "utilities/font.h"

namespace {
    SDL_Window * m_window;
    int window_width = 240;
    int window_height = 180;

    int screen_width = 240;
    int screen_height = 180;

    unsigned int m_texture_vao;
    unsigned int shader;

    DrawSettings active_settings;

    int resizingEventWatcher(void* data, SDL_Event* event) {
        if (event->type == SDL_WINDOWEVENT &&
            event->window.event == SDL_WINDOWEVENT_RESIZED) {
          SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
          if (win == (SDL_Window*)data) {
              SDL_GetWindowSize(win, &window_width, &window_height);
              glViewport(0, 0, window_width, window_height);
          }
        }
        return 0;
    }
}

bool Window::initialize(std::string window_title, int resolution_x, int resolution_y, double starting_scale)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    screen_width = resolution_x;
    screen_height = resolution_y;

    window_width = int(resolution_x * starting_scale);
    window_height = int(resolution_y * starting_scale);

    m_window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);

    if (m_window == nullptr) {
        std::cerr << "ERROR: Failed to create SDL_GL window; " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_AddEventWatch(resizingEventWatcher, m_window);

    SDL_GL_CreateContext(m_window);

    if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress))
    {
        std::cerr << "ERROR: Failed to initialize GLAD" << std::endl;
        return false;
    }

    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    std::cout << std::flush;

    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader = ShaderHelper::createShader(
            #include "shaders/texture.vert"
                ,
            #include "shaders/texture.frag"
                );
    glUseProgram(shader);
    setDrawSettings();

    // upload the vertex array to use for drawing textures
    {
        float verts[] = {
             -0.5f, -0.5f, 0, 0, // x y u v
              0.5f, -0.5f, 1, 0,
             -0.5f,  0.5f, 0, 1,
              0.5f,  0.5f, 1, 1,
        };

        unsigned int indices[] = {
            0, 1, 2,
            1, 3, 2,
        };

        unsigned int vbo, ebo;

        glGenVertexArrays(1, &m_texture_vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(m_texture_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), verts, GL_STATIC_DRAW);

        // vertex attributes
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // uv mapping attributes
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        glBindVertexArray(0);
    }

    SDL_GetWindowSize(m_window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);

    return true;
}

void Window::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update()
{
    SDL_GL_SwapWindow(m_window);
}

void Window::setDrawSettings(DrawSettings settings)
{
    active_settings = settings;
    glUniform1f(glGetUniformLocation(shader, "scale"), float(settings.scale));
    glUniform1f(glGetUniformLocation(shader, "rotation"), settings.rotation);
    glUniform1f(glGetUniformLocation(shader, "y_sort"), settings.y_sorted ? 1.0f : 0.0f);
    glUniform1i(glGetUniformLocation(shader, "tileID"), settings.tile_id);
    glUniform2f(glGetUniformLocation(shader, "tileSheetSize"), float(settings.tiles_wide), float(settings.tiles_tall));
    glUniform2f(glGetUniformLocation(shader, "anchor"), float(settings.anchor.x), float(settings.anchor.y));
    glUniform4f(glGetUniformLocation(shader, "color"), float(settings.color >> 16)/255.0f, float((settings.color >> 8) & 0xff)/255.0f, float((settings.color) & 0xff)/255.0f, settings.alpha);

    if (settings.y_sorted) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Window::setClearColor(int value)
{
    glClearColor(float(value >> 16)/255.0f, float(0xff & (value >> 8))/255.0f, float(0xff & value)/255.0f, 1.0f);
}

void Window::bind(std::string sprite)
{
    Texture tex = TextureHelper::fetch(sprite);
    if (!tex.available()) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.id());

    glUniform2f(glGetUniformLocation(shader, "screenSize"), float(screen_width), float(screen_height));
    glUniform2f(glGetUniformLocation(shader, "size"), float(tex.width() / active_settings.tiles_wide), float(tex.height() / active_settings.tiles_tall));

}

void Window::drawBound(double x, double y, double)
{
    glUniform2f(glGetUniformLocation(shader, "center"), float(x), float(y));

    glBindVertexArray(m_texture_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Window::draw(std::string sprite, double x, double y, double z)
{
    bind(sprite);
    drawBound(x, y, z);
}

void Window::print(std::string line, double x, double y, double scale)
{
    Font::bind();

    glBindVertexArray(m_texture_vao);

    glUniform2f(glGetUniformLocation(shader, "screenSize"), float(screen_width), float(screen_height));
    glUniform1f(glGetUniformLocation(shader, "scale"), float(scale));
    glUniform2f(glGetUniformLocation(shader, "tileSheetSize"), float(16), float(16));

    // TODO: handle print anchoring & coloring?
    glUniform2f(glGetUniformLocation(shader, "anchor"), 0.0f, 0.0f);
    glUniform4f(glGetUniformLocation(shader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);

    for (char c : line) {
        glUniform2f(glGetUniformLocation(shader, "center"), float(x), float(y));
        glUniform2f(glGetUniformLocation(shader, "size"), float(Font::getGlyphHBound()), float(Font::getGlyphVBound()));
        glUniform1i(glGetUniformLocation(shader, "tileID"), int(c));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        x += scale * double(Font::getGlyphWidth(c));
    }
}
