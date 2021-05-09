#include "../include/text.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/******************************************************************************
 * Public methods Font
******************************************************************************/

void Font::RenderText(SDL_Renderer* renderer, Text text) {
  // Create text
  SDL_Surface* textSurface = TTF_RenderText_Solid(font.get(), text.getText().c_str(), fontColor);
  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  
  // Free surface
  SDL_FreeSurface(textSurface);

  // Create text rectangle
  SDL_Rect Message_rect{
    text.getX(), text.getY(), text.getWidth(), text.getHeight()
  };

  // Render text
  SDL_RenderCopy(renderer, textTexture, NULL, &Message_rect);
  
  // Free texture
  SDL_DestroyTexture(textTexture);
}