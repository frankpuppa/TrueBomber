#ifndef _TEXTURETTF_H
#define _TEXTURETTF_H

#include <string>
#include <SDL2/SDL_ttf.h>



class TextureTTF
{
    public:
        //Initializes variables
        TextureTTF();
        ~TextureTTF();
        //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor,SDL_Renderer* , TTF_Font*);
        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );
        //Set blending
        void setBlendMode( SDL_BlendMode blending );
        //Set alpha modulation
        void setAlpha( Uint8 alpha );
        //Renders texture at given point
        void render(SDL_Renderer* g_pRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        SDL_Rect getTextureSize();
        //Gets image dimensions
        int getWidth();
        int getHeight();


        SDL_Texture* getTexture();
        std::string getFilename();
        //~ TTF_Font * getFont();
        //~ void closeFont();

    private:
        //The actual hardware texture
        SDL_Texture* g_pTexture;
        TTF_Font *g_pFont;
        SDL_Rect renderQuad;
        //Image dimensions
        int width;
        int height;
        std::string filename;
};
#endif
