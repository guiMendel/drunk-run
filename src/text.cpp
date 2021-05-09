#include "../include/text.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void Font::render(Text text) {
  // Create text
  SDL_Surface* textSurface = TTF_RenderText_Solid(font.get(), text.textMessage.c_str(), fontColor);
  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  // Free surface
  SDL_FreeSurface(textSurface);

  // Create text rectangle
  SDL_Rect Message_rect{ text.positionX, text.positionY, text.width, text.height };

  // Render text
  SDL_RenderCopy(renderer, textTexture, NULL, &Message_rect);

  // Free texture
  SDL_DestroyTexture(textTexture);
}