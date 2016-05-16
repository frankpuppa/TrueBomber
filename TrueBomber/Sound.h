#ifndef _SOUND_H
#define _SOUND_H
#include <SDL2/SDL_mixer.h>

class Sound
{
    public:
        static void init();
        static void playMusic();
        static void stopMusic();
        static void playExplosion();
        static void playDropBomb();
        static void playShoot();
        static void playCocaine();
        static void playJoint();
        static void playYuppi();
        static void playTrap();
        static void setMusicVolume(int);
        static void freeSounds();
	static void freeMusic();

        static void loadSounds();
        static void loadMusic(const char*);
	static void soundHandler(int );
	static void setAudioOn(bool);
	static bool getAudioStatus();

    private:
        static Mix_Music *gMusic;
        static Mix_Chunk *gSoundShoot;
        static Mix_Chunk *gSoundExplosion;
        static Mix_Chunk *gSoundDropBomb;
        static Mix_Chunk *gSoundCocaine;
        static Mix_Chunk *gSoundYuppy;
        static Mix_Chunk *gSoundTrap;
        static Mix_Chunk *gSoundJoint;
	static bool audioOn;
};
#endif
