#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"

extern Sound pelletSound;
extern Sound collisionSound;
extern Sound gameOverSound;
extern Sound fruitSound;

void initAudio();
void cleanupAudio();

#endif // AUDIO_H