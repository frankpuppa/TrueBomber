#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include <SDL2/SDL.h>




class Texture
{
    public:
        Texture();
        ~Texture();
        // simply set the running variable to true

        void render(SDL_Renderer*,SDL_Rect*,SDL_Rect*, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        int intrender(SDL_Renderer*,SDL_Rect*,SDL_Rect*);
        bool loadTexture(std::string, SDL_Renderer* );
        void setColor( Uint8 red, Uint8 green, Uint8 blue );
        void setBlendMode( SDL_BlendMode blending );
        void setAlpha( Uint8 alpha );
        bool loadTextureMapColor(std::string, SDL_Renderer*,Uint8,Uint8,Uint8);
        SDL_Rect getTextureSize();

        // a function to access the private running variable
    private:
        SDL_Texture* g_pTexture;
        int height;
        int width;
        std::string filename;
};
#endif // _TEXTURE_H
