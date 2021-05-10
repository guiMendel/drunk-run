#ifndef DRUNKRUN_TEXT_H
#define DRUNKRUN_TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>

class Text {
public:

  /* Default Constructor */
  Text() = default;

  /* Constructor
  * text is the message to display
  * position_x, position_y : the positions
  * height, width: the size of the text
  */
  Text(std::string text, int position_x, int position_y, int width, int height) :
    textMessage(text), positionX(position_x), positionY(position_y), width(width), height(height) {
  }

private:
  // Font has access to this class
  friend class Font;

  // Message this object holds
  std::string textMessage;

  // X position of message on screen
  int positionX;

  // Y position of message on screen
  int positionY;

  // Width of message
  int width;

  // Height of message
  int height;
};

#define DEFAULT_FONT_PATH "Fonts/Roboto-Bold.ttf"
#define DEFAULT_FONT_SIZE 128
#define DEFAULT_FONT_COLOR SDL_Color{ 0x00, 0x00, 0x00, 0xFF }

class Font {
public:
  /* Constructor
  * path is the path to the font
  * fontSize, the size of the font
  * color, the color of the font (r, g, b, alpha)
  */
  Font(std::string const& path, int fontSize, SDL_Color color, SDL_Renderer* renderer) :
    font(TTF_OpenFont(path.c_str(), fontSize), TTF_CloseFont),
    fontColor(color),
    renderer(renderer) {
  }

  Font(SDL_Renderer* renderer) : Font(DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR, renderer) {}
    
  /* Display some text */
  void render(Text text);

private:
  // Font
  std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> font;

  // Font's color
  SDL_Color fontColor{ DEFAULT_FONT_COLOR };

  // Pointer to renderer for this font
  SDL_Renderer* renderer;
};

#endif // DRUNKRUN_TEXT_H