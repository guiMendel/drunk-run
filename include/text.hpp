#ifndef DRUNKRUN_TEXT_H
#define DRUNKRUN_TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text {
public:

  /* Default Constructor */
  Text() = default;

  /* Constructor
  * text is the message to display
  * position_x, position_y : the positions
  * height, width: the size of the text
  */
  Text(std::string text, int position_x, int position_y, int height, int width) :
    m_text(text), m_position_x(position_x), m_position_y(position_y), m_height(height), m_width(width) {
  }

  /* Getters */

  std::string getText() { return m_text; }
  int getX() { return m_position_x; }
  int getY() { return m_position_y; }
  int getHeight() { return m_height; }
  int getWidth() { return m_width; }

private:

  std::string m_text;
  int m_position_x;
  int m_position_y;
  int m_height;
  int m_width;
};

#define DEFAULT_FONT_PATH "Fonts/Roboto-Bold.ttf"
#define DEFAULT_FONT_SIZE 128
#define DEFAULT_FONT_COLOR SDL_Color{ 255, 33, 52, 255 }

class Font {
public:
  /* Constructor
  * path is the path to the font
  * fontSize, the size of the font
  * color, the color of the font (r, g, b, alpha)
  */
  Font(std::string const& path, int fontSize, SDL_Color color) : fontColor(color) {
    font = TTF_OpenFont(path.c_str(), fontSize);
  }

  /* Default Constructor */
  Font() : Font(DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR) {}

  /* Destructor */
  ~Font() {
    TTF_CloseFont(font);
  }

  /* Display the text */
  void RenderText(SDL_Renderer* renderer, Text text);

private:

  // Font
  TTF_Font* font;
  // Font's color
  SDL_Color fontColor{ DEFAULT_FONT_COLOR };
};

#endif // DRUNKRUN_TEXT_H