#include "../include/text.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/******************************************************************************
 * Public methods Text
******************************************************************************/

Text::Text() {
    m_text = "";
    m_position_x = 0;
    m_position_y = 0;
    m_height = 0;
    m_width = 0;
}

Text::Text(std::string text, int position_x, int position_y, int height, int width)
    : m_text(text), m_position_x(position_x), m_position_y(position_y),
        m_height(height), m_width(width)
{}

std::string Text::getText() {
    return m_text;
}

int Text::getX() {
    return m_position_x;
}

int Text::getY() {
    return m_position_y;
}

int Text::getHeight() {
    return m_height;
}

int Text::getWidth() {
    return m_width;
}

/******************************************************************************
 * Public constants Font
******************************************************************************/

const std::string Font::defaultFont = "Fonts/Roboto-Bold.ttf";
const SDL_Color Font::defaultFontColor = SDL_Color{255, 33, 52, 255};

/******************************************************************************
 * Public methods Font
******************************************************************************/
Font::Font() {
    m_font = TTF_OpenFont(defaultFont.c_str(), defaultFontSize);
    m_fontColor = defaultFontColor;
}

Font::Font(std::string const& path, int fontSize, SDL_Color color)
    : m_fontColor(color)
{
     m_font = TTF_OpenFont(path.c_str(), fontSize);
}

void Font::RenderText(SDL_Renderer* renderer, Text text) {
     SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, text.getText().c_str(), m_fontColor);
     SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
     SDL_FreeSurface(textSurface);

    SDL_Rect Message_rect;
        Message_rect.x = text.getX();
        Message_rect.y = text.getY();
        Message_rect.w = text.getWidth();
        Message_rect.h = text.getHeight();

    SDL_RenderCopy(renderer, textTexture, NULL, &Message_rect);
    SDL_DestroyTexture(textTexture);
 }