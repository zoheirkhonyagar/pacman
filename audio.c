#include "raylib.h"
#include "audio.h"
// Declare global sounds
// Define global sound variables
Sound pelletSound;
Sound collisionSound;
Sound gameOverSound;
Sound fruitSound;

void initAudio()
{
  InitAudioDevice(); // Initialize audio device
  if (!IsAudioDeviceReady())
  {
    // printf("Error: Audio device initialization failed.\n");
  }
  else
  {
    // printf("Audio device initialized successfully.\n");
  }
  pelletSound = LoadSound("../assets/audio/pellet.wav");
  collisionSound = LoadSound("../assets/audio/collision.wav");
  gameOverSound = LoadSound("../assets/audio/gameover.wav");
  fruitSound = LoadSound("../assets/audio/fruit.wav");

  SetSoundVolume(pelletSound, 1.0f);   // Full volume
  SetSoundVolume(gameOverSound, 1.0f); // Full volume
  SetSoundVolume(fruitSound, 1.0f);    // Full volume
}

void cleanupAudio()
{
  UnloadSound(pelletSound);
  UnloadSound(collisionSound);
  UnloadSound(gameOverSound);
  UnloadSound(fruitSound);
  CloseAudioDevice();
}