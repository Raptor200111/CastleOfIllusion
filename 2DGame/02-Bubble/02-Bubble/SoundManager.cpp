#include "SoundManager.h"
#include <iostream>


// Initialize SDL2_mixer
bool SoundManager::init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    SoundManager::instance().loadMusic("menu", "sound/01-Title.mp3");
    SoundManager::instance().loadMusic("level", "sound/03-Stage1-1.mp3");
    SoundManager::instance().loadMusic("gameOver", "sound/26-Game Over.mp3");
    SoundManager::instance().loadMusic("gameCompleted", "sound/10 - Stage Clear.mp3");
    SoundManager::instance().loadMusic("levelCompleted", "sound/07_Level_Complete.mp3");
    SoundManager::instance().loadMusic("finalBoss", "sound/08 - Boss.mp3");
    SoundManager::instance().loadMusic("instructions", "sound/02 - Castle.mp3");
    SoundManager::instance().loadMusic("credits", "sound/04 - Substage.mp3");

    SoundManager::instance().loadSoundEffect("gameLose", "sound/lose.wav");
    SoundManager::instance().loadSoundEffect("gameWin", "sound/gameWin0.wav");
    SoundManager::instance().loadSoundEffect("playRestart", "sound/levelRestart.wav");
    SoundManager::instance().loadSoundEffect("playerJump", "sound/playerJump0.wav");
    SoundManager::instance().loadSoundEffect("practiceWon", "sound/practiceWon0.wav");
    SoundManager::instance().loadSoundEffect("playerCoin", "sound/playerCoin.wav");
    SoundManager::instance().loadSoundEffect("playerCake", "sound/playerCake.wav");
    SoundManager::instance().loadSoundEffect("playerDamage", "sound/playerDamage.wav");
    SoundManager::instance().loadSoundEffect("break", "sound/break.wav");
    SoundManager::instance().loadSoundEffect("shoot", "sound/shoot.wav");


    return true;
}

// Load a sound effect (e.g., WAV)
bool SoundManager::loadSoundEffect(const std::string& id, const std::string& filename) {
    Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
    if (chunk == nullptr) {
        std::cerr << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    soundEffects[id] = chunk;
    return true;
}

// Load music (e.g., MP3, OGG)
bool SoundManager::loadMusic(const std::string& id, const std::string& filename) {
    Mix_Music* music = Mix_LoadMUS(filename.c_str());
    if (music == nullptr) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    musics[id] = music;
    return true;
}

// Play sound effect
void SoundManager::playSoundEffect(const std::string& id, int loops) {
    Mix_PlayChannel(-1, soundEffects[id], loops);
}

// Play music
void SoundManager::playMusic(const std::string& id, int loops) {
    Mix_PlayMusic(musics[id], loops);
}

// Stop music
void SoundManager::stopMusic() {
    Mix_HaltMusic();
}

// Clean up and free resources
void SoundManager::cleanUp() {
    for (auto& pair : soundEffects) {
        Mix_FreeChunk(pair.second);
    }
    soundEffects.clear();

    for (auto& pair : musics) {
        Mix_FreeMusic(pair.second);
    }
    musics.clear();

    Mix_CloseAudio();
}

void SoundManager::setSoundEffectVolume(const std::string& id, int volume) {
    if (soundEffects.find(id) != soundEffects.end()) {
        Mix_VolumeChunk(soundEffects[id], volume);
    }
    else {
        std::cerr << "Sound effect " << id << " not found!" << std::endl;
    }
}

// Destructor
SoundManager::~SoundManager() {
    cleanUp();
}


/*
    if (!initAudio()) {
        std::cout << "Failed to initialize audio." << std::endl;
        return -1;
    }

    if (!loadMusic("path_to_your_music_file.mp3")) {
        std::cout << "Failed to load music." << std::endl;
    } else {
        playMusic();
    }

    // Simulate game loop or logic
    SDL_Delay(5000); // Play for 5 seconds

    // Clean up
    closeAudio();

*/