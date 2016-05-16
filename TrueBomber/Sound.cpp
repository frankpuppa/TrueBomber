#include "Sound.h"
#include <iostream>
/*This is a static class which is available everywhere in the game, as the audio effects are available everywhere*/
Mix_Music* Sound::gMusic=NULL;
Mix_Chunk* Sound::gSoundShoot=NULL;
Mix_Chunk* Sound::gSoundExplosion=NULL;
Mix_Chunk* Sound::gSoundDropBomb=NULL;
Mix_Chunk* Sound::gSoundCocaine=NULL;
Mix_Chunk* Sound::gSoundYuppy=NULL;
Mix_Chunk* Sound::gSoundTrap=NULL;
Mix_Chunk* Sound::gSoundJoint=NULL;

bool Sound::audioOn=true;

void Sound::init(){
    //if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 8192 ) == -1 )
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
}
void Sound::loadSounds(){

    if(gSoundShoot==NULL){
        gSoundShoot = Mix_LoadWAV( "audio/shot.wav" );
        if( gSoundShoot == NULL )
        {
            printf( "Failed to load gSoundShoot.wav sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }
    if(gSoundExplosion==NULL){
        gSoundExplosion=Mix_LoadWAV("audio/explosion.wav");
        if(gSoundExplosion==NULL)
        {
            printf( "Failed to load gSoundExposion.wav sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }
    if(gSoundDropBomb==NULL){
        gSoundDropBomb=Mix_LoadWAV("audio/dropbomb.wav");
        if(gSoundDropBomb==NULL)
        {
            printf( "Failed to load gSoundDropbomb.wav sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }
    if(gSoundCocaine==NULL){
        gSoundCocaine=Mix_LoadWAV("audio/cocaine.wav");
        if(gSoundCocaine==NULL)
        {
            printf( "Failed to load gSoundCocaine.wav sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }
    if(gSoundYuppy==NULL){
        gSoundYuppy=Mix_LoadWAV("audio/yuppi.wav");
        if(gSoundYuppy==NULL)
        {
            printf( "Failed to load gSoundYuppy.wav sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }
    if(gSoundJoint==NULL){
        gSoundJoint=Mix_LoadWAV("audio/joint.wav");
        if(gSoundJoint==NULL)
        {
            printf( "Failed to load gSoundJoint.wav sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }
    if(gSoundTrap==NULL){
        gSoundTrap=Mix_LoadWAV("audio/trap.wav");
        if(gSoundTrap==NULL)
        {
            printf( "Failed to load gSoundTrap.wav sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }
}
void Sound::loadMusic(const char* filename){
    gMusic = Mix_LoadMUS( filename );
    if( gMusic == NULL )
    {
        printf( "Failed to load Musicfor the  music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
}
void Sound::freeSounds(){
    //Free Sounds
    std::cout<<"Freeing Audio"<<std::endl;
    Mix_FreeChunk(gSoundShoot);
    Mix_FreeChunk(gSoundDropBomb);
    Mix_FreeChunk(gSoundExplosion);
    Mix_FreeChunk(gSoundJoint);
    Mix_FreeChunk(gSoundYuppy);
    Mix_FreeChunk(gSoundTrap);
    Mix_FreeChunk(gSoundCocaine);


    stopMusic();

    gSoundCocaine=NULL;
    gSoundTrap=NULL;
    gSoundJoint=NULL;
    gSoundYuppy=NULL;
    gSoundShoot=NULL;
    gSoundDropBomb=NULL;
    gSoundExplosion=NULL;
}
void Sound::freeMusic(){
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
}
void Sound::playMusic(){
  
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( gMusic, -1 );
    }
}
void Sound::stopMusic(){
    Mix_HaltMusic();
}
void Sound:: playShoot(){
    Mix_PlayChannel( -1, gSoundShoot, 0 );
}
void Sound:: playExplosion(){
    Mix_PlayChannel( -1, gSoundExplosion, 0 );
}
void Sound:: playDropBomb(){
    Mix_PlayChannel( -1, gSoundDropBomb, 0 );
}
void Sound:: playJoint(){
    Mix_PlayChannel( -1, gSoundJoint, 0 );
}
void Sound:: playCocaine(){
    Mix_PlayChannel( -1, gSoundCocaine, 0 );
}
void Sound:: playYuppi(){
    Mix_PlayChannel( -1, gSoundYuppy, 0 );
}
void Sound:: playTrap(){
    Mix_PlayChannel( -1, gSoundTrap, 0 );
}
void Sound::setMusicVolume(int volume){
    std::cout<<"Music Volume Was: "<<Mix_VolumeMusic(volume)<<std::endl;
    std::cout<<"Music Volume Is now: "<<Mix_VolumeMusic(-1)<<std::endl;
}
void Sound::setAudioOn(bool value){
  audioOn=value;
}
void Sound::soundHandler(int value){
  if(!audioOn)
      return;
      
     switch(value){
      case 1:
	  playShoot();
	  break;
      case 2:
	  playYuppi();
	  break;
      case 3:
	  playExplosion();
	  break;
      case 4:
	  playDropBomb();
	  break;
      case 5:
	  playTrap();
	  break;
      case 6:
	  playCocaine();
	  break;
      case 7:
	  playJoint();
	  break;
      case 8:
	  playMusic();
	  break;
      }
}
bool Sound::getAudioStatus(){
  return audioOn;
}
