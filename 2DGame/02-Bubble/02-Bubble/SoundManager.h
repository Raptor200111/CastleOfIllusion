#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_mixer.h>
#include <map>
#include <string>
class SoundManager
{

public:
    static SoundManager& instance()
    {
        static SoundManager soundManager;

        return soundManager;
    }
    ~SoundManager();

    bool init();

    // Loads a sound effect (wav) or music file (mp3, ogg)
    bool loadSoundEffect(const std::string& id, const std::string& filename);
    bool loadMusic(const std::string& id, const std::string& filename);

    // Plays sound effect or music
    void playSoundEffect(const std::string& id, int loops = 0);
    void playMusic(const std::string& id, int loops = -1); // -1 for infinite loops

    // Stops playing music
    void stopMusic();

    // Cleans up all loaded sounds
    void cleanUp();

    //volume out of 128)
    void setMusicVolume(int volume) { Mix_VolumeMusic(volume); }
    void setSoundEffectVolume(const std::string& id, int volume);

private:
    SoundManager() {}
    std::map<std::string, Mix_Chunk*> soundEffects;
    std::map<std::string, Mix_Music*> musics;

};

