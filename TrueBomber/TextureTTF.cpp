#include "TextureTTF.h"
#include <iostream>
/*This class is the main class to handle TTF textures to be displayed - I think the methods are quite self explanatory. 
 * Most of their understanding relies on how SDL works.
 * */
TextureTTF::TextureTTF()
{
    //Initialize
    g_pTexture = NULL;
    width = 0;
    height = 0;
    g_pFont=NULL;
}

TextureTTF::~TextureTTF()
{
    if( g_pTexture != NULL )
    {
        std::cout<<"Clearing: TTFTexture " + filename<<std::endl;
        SDL_DestroyTexture( g_pTexture );
        g_pTexture = NULL;
        width = 0;
        height = 0;

    }

}

bool TextureTTF::loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* g_pRenderer, TTF_Font *g_pFont )
{
    filename=textureText;
    //free Texture if != null
    if(g_pTexture!=NULL){
        // std::cout<<"Deleting previous allocated Texture"<<std::endl;
        SDL_DestroyTexture( g_pTexture );
        g_pTexture = NULL;
    }
    SDL_Color bg={255,255,0,10};
    //Render text surface
    //SDL_Surface* textSurface = TTF_RenderText_Solid( g_pFont, textureText.c_str(), textColor );
    SDL_Surface* textSurface = TTF_RenderText_Shaded( g_pFont, textureText.c_str(), textColor, bg );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        g_pTexture = SDL_CreateTextureFromSurface( g_pRenderer, textSurface );
        if( g_pTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            width = textSurface->w;
            height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return g_pTexture != NULL;
}

void TextureTTF::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( g_pTexture, red, green, blue );
}

void TextureTTF::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( g_pTexture, blending );
}

void TextureTTF::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( g_pTexture, alpha );
}

void TextureTTF::render(SDL_Renderer* g_pRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    renderQuad.x=x;
    renderQuad.y=y;
    renderQuad.w=width;
    renderQuad.h=height;

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 0, 255);

    SDL_RenderFillRect(g_pRenderer, &renderQuad);

    SDL_RenderCopyEx( g_pRenderer, g_pTexture, clip, &renderQuad, angle, center, flip );
}

int TextureTTF::getWidth()
{
    return width;
}

int TextureTTF::getHeight()
{
    return height;
}



SDL_Texture* TextureTTF::getTexture(){
    return g_pTexture;
}

std::string TextureTTF::getFilename(){
    return filename;
}
//~ TTF_Font * TextureTTF::getFont(){
//~ return g_pFont;
//~ }
//~ void TextureTTF::closeFont(){
//~ TTF_CloseFont(g_pFont);
//~ g_pFont=NULL;
//~ }
SDL_Rect TextureTTF::getTextureSize(){
    SDL_Rect source_rect;
    SDL_QueryTexture(g_pTexture, NULL, NULL,&source_rect.w, &source_rect.h);
    return source_rect;
}
