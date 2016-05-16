#include "Texture.h"
#include <SDL2/SDL_image.h>
#include <iostream>

/*This class is the main class to handle images to be displayed - I think the methods are quite self explanatory. Most of their understanding relies on how SDL works.
 * */
Texture::Texture(){
    std::cout<<"Texture from Texture Class created--Texture.cpp Constructor\n";
    g_pTexture=NULL;
}

Texture::~Texture(){
    std::cout << "Texture "<<filename<<" deleted\n";
    if( g_pTexture != NULL )
    {
        SDL_DestroyTexture( g_pTexture );
        g_pTexture = NULL;
        width = 0;
        height = 0;
    }
}

void Texture::render(SDL_Renderer* g_pRenderer,SDL_Rect*g_sourceR, SDL_Rect* g_destR, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    // std::cout<<"width "<<g_destR.w<<" height "<<g_destR.h<<std::endl;

    SDL_RenderCopyEx (g_pRenderer, g_pTexture, g_sourceR, g_destR, angle, center, flip);
}
int Texture::intrender(SDL_Renderer* g_pRenderer,SDL_Rect*g_sourceR, SDL_Rect* g_destR)
{
    // std::cout<<"width "<<g_destR.w<<" height "<<g_destR.h<<std::endl;

    return SDL_RenderCopy (g_pRenderer, g_pTexture, g_sourceR, g_destR);
}

bool Texture::loadTexture(std::string filename, SDL_Renderer * g_pRenderer){

    this->filename.assign(filename);

    if(g_pTexture!=NULL){
        SDL_DestroyTexture( g_pTexture );
        g_pTexture = NULL;
    }

    SDL_Surface* pTempSurface = IMG_Load(filename.c_str());

    if( pTempSurface == NULL ){
        std::cout<<"Unable to load image "<<filename<<" SDL_image Error\n";
        return false;
    }else{
        g_pTexture = SDL_CreateTextureFromSurface(g_pRenderer,pTempSurface);
        if(g_pTexture==NULL){
            std::cout<<"Error creating texture\n";
            return false;
        }else{
            height=pTempSurface->h;
            width=pTempSurface->w;
            //std::cout<<"width "<<mwidth<<" height "<<mheight<<std::endl;
            SDL_FreeSurface(pTempSurface);
        }
        return true;
    }
}
bool Texture::loadTextureMapColor(std::string filename, SDL_Renderer* g_pRenderer, Uint8 red,Uint8 green,Uint8 blue){
    filename=filename;
    SDL_Surface* pTempSurface = IMG_Load(filename.c_str());

    if( pTempSurface == NULL ){
        std::cout<<"Unable to load image "<<filename<<" SDL_image Error\n";
        return false;
    }else{
        SDL_SetColorKey( pTempSurface, SDL_TRUE, SDL_MapRGB( pTempSurface->format, red, green, blue ) );
        g_pTexture = SDL_CreateTextureFromSurface(g_pRenderer,pTempSurface);
        if(g_pTexture==NULL){
            std::cout<<"Error creating texture\n";
            return false;
        }else{
            height=pTempSurface->h;
            width=pTempSurface->w;
            //std::cout<<"width "<<mwidth<<" height "<<mheight<<std::endl;
            SDL_FreeSurface(pTempSurface);
        }
        return true;
    }
}

SDL_Rect Texture::getTextureSize(){
    SDL_Rect source_rect;
    SDL_QueryTexture(g_pTexture, NULL, NULL,&source_rect.w, &source_rect.h);
    return source_rect;
}
void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( g_pTexture, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( g_pTexture, blending );
}

void Texture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( g_pTexture, alpha );
}

