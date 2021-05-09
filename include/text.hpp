#ifndef DRUNKRUN_TEXT_H
#define DRUNKRUN_TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text {
public:

    /* Default Constructor */
    Text();

    /* Constructor
    * text is the message to display
    * position_x, position_y : the positions
    * height, width: the size of the text
    */
    Text(std::string text, int position_x, int position_y, int height, int width);

    /* Getters */

    std::string getText();
    int getX();
    int getY();
    int getHeight();
    int getWidth();

private:

    std::string m_text;
    int m_position_x;
    int m_position_y;
    int m_height;
    int m_width;
};

class Font {
public:

    static const std::string defaultFont;
    static const int defaultFontSize = 128;
    static const SDL_Color defaultFontColor;

    /* Default Constructor */
    Font();

    /* Constructor
    * path is the path to the font
    * fontSize, the size of the font
    * color, the color of the font (r, g, b, alpha)
    */
    Font(std::string const& path, int fontSize, SDL_Color color);

    /* Destructor */
    ~Font() {
        TTF_CloseFont(m_font);
    }

    /* Display the text */
    void RenderText(SDL_Renderer* renderer, Text text);

private:

    TTF_Font* m_font;
    SDL_Color m_fontColor;
};

#endif // DRUNKRUN_TEXT_H