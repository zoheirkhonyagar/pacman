#include <raylib.h>
#include <stdlib.h>
#include "game map.h"
#include <time.h>
#include <stdbool.h>
#include "leaderboard.h"

#define screenwidth 1200
#define screenheight 900
#define starNum 10

typedef struct Ghost
{
    Vector2 position;
    int direction; // Direction vector to move the ghost
} Ghost;
typedef struct
{
    Vector2 starpos;
    bool eaten;
} Star;
typedef struct
{
    Vector2 fruitpos;
    bool eat;
} Fruit;
Ghost ghosts[5] = {
    {{30 * 18, 30 * 11}, -1},
    {{30 * 19, 30 * 11}, -1},
    {{30 * 20, 30 * 11}, -1},
    {{30 * 21, 30 * 11}, -1},
    {{30 * 22, 30 * 11}, -1}};
Fruit fruit[4];
Star star[10];

Vector2 pacmanpos = {30 * 19, 30 * 20};
Vector2 pacmandirection = {0, 0};
float ghostspeed = 8.0f;
float pacmanspeed = 7.0f;
float speedboost = 0.0;
float speedBoostDuration = 12.0;
int score = 0;
int lives = 3;
Texture pacman1;
Texture redghoost;
Texture yellowghoost;
Texture blueghoost;
Texture greenghoost;
Texture purpleghost;
Texture fapple;
Texture mushroom;
Texture pepper;

void loadtexture()
{
    Image image1 = LoadImage("../image/pac.jpg");
    pacman1 = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/red.jpg");
    redghoost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/yellow.jpg");
    yellowghoost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/blue.jpg");
    blueghoost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/green.jpg");
    greenghoost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/photo_2025-01-14_00-15-05.jpg");
    purpleghost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/9.png");
    fapple = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/mushroom (1).jpg");
    mushroom = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/pepper.jpg");
    pepper = LoadTextureFromImage(image1);
}

void draw(Texture j, int x, int y)
{
    ClearBackground(BLACK);
    DrawTexture(j, x, y, WHITE);
}
int checkghostwallcollision(Vector2 newpos, Texture i)
{
    int x1 = (int)(newpos.x / 30); // Top-left corner
    int y1 = (int)(newpos.y / 30);

    int x2 = (int)((newpos.x + i.width - 1) / 30); // Top-right corner
    int y2 = (int)(newpos.y / 30);

    int x3 = (int)(newpos.x / 30); // Bottom-left corner
    int y3 = (int)((newpos.y + i.height - 1) / 30);

    int x4 = (int)((newpos.x + i.width - 1) / 30); // Bottom-right corner
    int y4 = (int)((newpos.y + i.height - 1) / 30);

    // Check if any of the corners are inside a wall (map[tileY][tileX] == 1 means a wall)
    if (map[y1][x1] == 1 || map[y2][x2] == 1 || map[y3][x3] == 1 || map[y4][x4] == 1)
    {
        return 1; // There is a wall
    }
    return 0;
}
int checkstarWallCollision(Vector2 pos)
{
    int x1 = (int)((pos.x + 8) / 30); // Get the grid coordinates of the position
    int y1 = (int)(pos.y / 30);
    int x2 = (int)(pos.x / 30); // Get the grid coordinates of the position
    int y2 = (int)((pos.y + 8) / 30);
    int x3 = (int)(pos.x / 30); // Get the grid coordinates of the position
    int y3 = (int)(pos.y / 30);
    int x4 = (int)((pos.x - 8) / 30); // Get the grid coordinates of the position
    int y4 = (int)(pos.y / 30);
    int x5 = (int)(pos.x / 30); // Get the grid coordinates of the position
    int y5 = (int)((pos.y - 8) / 30);

    // Check if the position is on a wall (map[y][x] == 1 indicates a wall)
    if (map[y1][x1] == 1 || map[y2][x2] == 1 || map[y3][x3] == 1 || map[y4][x4] == 1 || map[y5][x5] == 1)
    {
        return 1; // Collision with wall
    }
    return 0; // No collision
}
Vector2 generateRandomPosition(int i)
{
    Vector2 position;
    srand(time(NULL) * i);
    do
    {
        // Generate random coordinates
        position.x = (rand() % 40) * 30; // Align with grid
        position.y = (rand() % 23) * 30;
    } while (checkstarWallCollision(position)); // Keep trying until no collision with wall
    return position;
}
void initializeStars()
{
    for (int i = 0; i < 10; i++)
    {
        star[i].starpos = generateRandomPosition((i + 1) * 5); // Generate a valid position for each star
        star[i].eaten = false;
    }
}
void checkStarCollision()
{
    Rectangle pacmanRect = {pacmanpos.x, pacmanpos.y, pacman1.width, pacman1.height};
    for (int i = 0; i < starNum; i++)
    {
        if (!star[i].eaten)
        {
            Rectangle starRect = {star[i].starpos.x, star[i].starpos.y, 16, 16};
            if (CheckCollisionRecs(pacmanRect, starRect))
            {
                star[i].eaten = true; // Mark the star as eaten
                score += 10;          // Increase score
            }
        }
    }
}
void initializeFruit()
{
    for (int i = 0; i < 4; i++)
    {
        fruit[i].fruitpos = generateRandomPosition((i + 1) * 33);
        fruit[i].eat = false;
    }
    /*apple 0  mushroom 1 pepper 2 cherry 3*/
}
int checkfruitcollision(Texture t, int i)
{
    Rectangle pacmanRect = {pacmanpos.x, pacmanpos.y, pacman1.width, pacman1.height};
    if (!fruit[i].eat)
    {
        Rectangle textureRect = {fruit[i].fruitpos.x, fruit[i].fruitpos.y, t.width, t.height};
        if (CheckCollisionRecs(pacmanRect, textureRect))
        {
            fruit[i].eat = true;
            return 1;
        }
    }
    return 0;
}
void fruitlogic()
{
    if (checkfruitcollision(fapple, 0))
    {
        if (lives < 3)
        {
            lives++;
        }
        fruit[0].fruitpos = generateRandomPosition(29);
        fruit[0].eat = false;
    }
    if (checkfruitcollision(mushroom, 1))
    {
        lives--;
        fruit[1].fruitpos = generateRandomPosition(47);
        fruit[1].eat = false;
    }
    if (checkfruitcollision(pepper, 2))
    {
        pacmanspeed = 10.0;
        fruit[2].fruitpos = generateRandomPosition(59);
        fruit[2].eat = false;
        speedboost = GetTime();
    }
}
void replaceStars()
{
    bool allStarsEaten = true;
    for (int i = 0; i < starNum; i++)
    {
        if (!star[i].eaten)
        {
            allStarsEaten = false;
            break;
        }
    }
    if (allStarsEaten)
    {
        // Reset all stars
        for (int i = 0; i < starNum; i++)
        {
            star[i].eaten = false;
            star[i].starpos = generateRandomPosition(i * 5);
        }
    }
}

void MoveGhost1(Ghost *ghost)
{
    srand(time(NULL) * 17);
    if (ghost->direction == -1)
    {
        // Assign a random direction (0=UP, 1=DOWN, 2=LEFT, 3=RIGHT)
        ghost->direction = rand() % 4;
    }

    // Try moving the ghost based on the current direction
    Vector2 newghostpos = {ghost->position.x, ghost->position.y};

    switch (ghost->direction)
    {
    case 0:
        newghostpos.y -= ghostspeed;
        break;
    case 1:
        newghostpos.y += ghostspeed;
        break;
    case 2:
        newghostpos.x -= ghostspeed;
        break;
    case 3:
        newghostpos.x += ghostspeed;
        break;
    }

    // Check for collision before moving
    if (!checkghostwallcollision(newghostpos, yellowghoost))
    {
        // No collision, update ghost position
        ghost->position.x = newghostpos.x;
        ghost->position.y = newghostpos.y;
    }
    else
    {
        // Collision detected, pick a new random direction
        ghost->direction = -1;
    }
}

void MoveGhost2(Ghost *ghost)
{
    srand(time(NULL) * 13);
    if (ghost->direction == -1)
    {
        // Assign a random direction (0=UP, 1=DOWN, 2=LEFT, 3=RIGHT)
        ghost->direction = rand() % 4;
    }

    // Try moving the ghost based on the current direction
    Vector2 newghostpos = {ghost->position.x, ghost->position.y};

    switch (ghost->direction)
    {
    case 0:
        newghostpos.y -= ghostspeed;
        break;
    case 1:
        newghostpos.y += ghostspeed;
        break;
    case 2:
        newghostpos.x -= ghostspeed;
        break;
    case 3:
        newghostpos.x += ghostspeed;
        break;
    }

    // Check for collision before moving
    if (!checkghostwallcollision(newghostpos, redghoost))
    {
        // No collision, update ghost position
        ghost->position.x = newghostpos.x;
        ghost->position.y = newghostpos.y;
    }
    else
    {
        // Collision detected, pick a new random direction
        ghost->direction = -1;
    }
}
void MoveGhost3(Ghost *ghost)
{
    srand(time(NULL) * 19);
    if (ghost->direction == -1)
    {
        // Assign a random direction (0=UP, 1=DOWN, 2=LEFT, 3=RIGHT)
        ghost->direction = rand() % 4;
    }
    // Try moving the ghost based on the current direction
    Vector2 newghostpos = {ghost->position.x, ghost->position.y};

    switch (ghost->direction)
    {
    case 0:
        newghostpos.y -= ghostspeed;
        break;
    case 1:
        newghostpos.y += ghostspeed;
        break;
    case 2:
        newghostpos.x -= ghostspeed;
        break;
    case 3:
        newghostpos.x += ghostspeed;
        break;
    }

    // Check for collision before moving
    if (!checkghostwallcollision(newghostpos, greenghoost))
    {
        // No collision, update ghost position
        ghost->position.x = newghostpos.x;
        ghost->position.y = newghostpos.y;
    }
    else
    {
        // Collision detected, pick a new random direction
        ghost->direction = -1;
    }
}
void MoveGhost4(Ghost *ghost)
{
    srand(time(NULL));
    if (ghost->direction == -1)
    {
        // Assign a random direction (0=UP, 1=DOWN, 2=LEFT, 3=RIGHT)
        ghost->direction = rand() % 4;
    }

    // Try moving the ghost based on the current direction
    Vector2 newghostpos = {ghost->position.x, ghost->position.y};

    switch (ghost->direction)
    {
    case 0:
        newghostpos.y -= ghostspeed;
        break;
    case 1:
        newghostpos.y += ghostspeed;
        break;
    case 2:
        newghostpos.x -= ghostspeed;
        break;
    case 3:
        newghostpos.x += ghostspeed;
        break;
    }

    // Check for collision before moving
    if (!checkghostwallcollision(newghostpos, blueghoost))
    {
        // No collision, update ghost position
        ghost->position.x = newghostpos.x;
        ghost->position.y = newghostpos.y;
    }
    else
    {
        // Collision detected, pick a new random direction
        ghost->direction = -1;
    }
}
void MoveGhost5(Ghost *ghost)
{
    srand(time(NULL) * 47);
    if (ghost->direction == -1)
    {
        // Assign a random direction (0=UP, 1=DOWN, 2=LEFT, 3=RIGHT)
        ghost->direction = rand() % 4;
    }

    // Try moving the ghost based on the current direction
    Vector2 newghostpos = {ghost->position.x, ghost->position.y};

    switch (ghost->direction)
    {
    case 0:
        newghostpos.y -= ghostspeed;
        break;
    case 1:
        newghostpos.y += ghostspeed;
        break;
    case 2:
        newghostpos.x -= ghostspeed;
        break;
    case 3:
        newghostpos.x += ghostspeed;
        break;
    }

    // Check for collision before moving
    if (!checkghostwallcollision(newghostpos, purpleghost))
    {
        // No collision, update ghost position
        ghost->position.x = newghostpos.x;
        ghost->position.y = newghostpos.y;
    }
    else
    {
        // Collision detected, pick a new random direction
        ghost->direction = -1;
    }
}

void movepac()
{

    pacmandirection = (Vector2){0, 0};
    if (IsKeyDown(KEY_UP))
    {
        pacmandirection = (Vector2){0, -pacmanspeed};
    }
    if (IsKeyDown(KEY_DOWN))
    {
        pacmandirection = (Vector2){0, pacmanspeed};
    }
    if (IsKeyDown(KEY_LEFT))
    {
        pacmandirection = (Vector2){-pacmanspeed, 0};
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        pacmandirection = (Vector2){pacmanspeed, 0};
    }
    // update position
    Vector2 newpos = {pacmanpos.x + pacmandirection.x, pacmanpos.y + pacmandirection.y};

    if (checkghostwallcollision(newpos, pacman1) == 0)
    {
        pacmanpos = newpos;
    }

    if (pacmanpos.x < 0)
    {
        pacmanpos.x = 0;
    }
    if (pacmanpos.y < 0)
    {
        pacmanpos.y = 0;
    }
    if (pacmanpos.x + pacman1.width > screenwidth)
    {
        pacmanpos.x = screenwidth - pacman1.width;
    }
    if (pacmanpos.y + pacman1.height > screenheight)
    {
        pacmanpos.y = screenheight - pacman1.height;
    }
}

void showmovement(Score scores[], const char *playerName)
{
    initializeStars();
    initializeFruit();
    while (!WindowShouldClose() && lives > 0)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawMap();
        loadtexture();
        if (speedboost > 0 && GetTime() - speedboost >= speedBoostDuration)
        {
            pacmanspeed = 5.0f; // Reset speed back to normal after 10 seconds
            speedboost = 0.0f;  // Stop the timer
        }
        // Call each ghost's movement function
        movepac();
        MoveGhost1(&ghosts[0]);
        MoveGhost2(&ghosts[1]);
        MoveGhost3(&ghosts[2]);
        MoveGhost4(&ghosts[3]);
        MoveGhost5(&ghosts[4]);

        Rectangle pacmanRect = {pacmanpos.x, pacmanpos.y, pacman1.width, pacman1.height};
        Rectangle ghostRect[5] = {
            {ghosts[0].position.x, ghosts[0].position.y, redghoost.width, redghoost.height},
            {ghosts[1].position.x, ghosts[1].position.y, yellowghoost.width, yellowghoost.height},
            {ghosts[2].position.x, ghosts[2].position.y, greenghoost.width, greenghoost.height},
            {ghosts[3].position.x, ghosts[3].position.y, blueghoost.width, blueghoost.height},
            {ghosts[4].position.x, ghosts[4].position.y, purpleghost.width, purpleghost.height}};
        bool collisiondetected = false;
        for (int i = 0; i < 5; i++)
        {
            if (CheckCollisionRecs(pacmanRect, ghostRect[i]))
            {
                collisiondetected = true;
                break;
            }
        }
        if (collisiondetected)
        {
            lives--;                                 // Decrease lives if there is a collision
            pacmanpos = (Vector2){30 * 19, 30 * 20}; // Reset Pacman's position to the start
        }
        checkStarCollision();
        replaceStars();
        fruitlogic();
        draw(pacman1, pacmanpos.x, pacmanpos.y);
        draw(redghoost, ghosts[0].position.x, ghosts[0].position.y);
        draw(blueghoost, ghosts[1].position.x, ghosts[1].position.y);
        draw(greenghoost, ghosts[2].position.x, ghosts[2].position.y);
        draw(yellowghoost, ghosts[3].position.x, ghosts[3].position.y);
        draw(purpleghost, ghosts[4].position.x, ghosts[4].position.y);
        draw(fapple, fruit[0].fruitpos.x, fruit[0].fruitpos.y);
        draw(mushroom, fruit[1].fruitpos.x, fruit[1].fruitpos.y);
        draw(pepper, fruit[2].fruitpos.x, fruit[2].fruitpos.y);
        for (int i = 0; i < starNum; i++)
        {
            if (!star[i].eaten && checkstarWallCollision(star[i].starpos) == 0)
            {
                DrawCircle(star[i].starpos.x, star[i].starpos.y, 8, ORANGE);
            }
        }
        DrawText(TextFormat("SCORE : %d", score), 0, 24 * 30, 30, RED);
        DrawText(TextFormat("LIVES : %d", lives), 0, 26 * 30, 30, RED);
        EndDrawing();
    }

    if (lives == 0)
    {

        addScore(scores, playerName, score); // Add the final score to the leaderboard
        saveScores(scores);

        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("!!GAME OVER!!", screenwidth / 2 - MeasureText("GAME OVER", 70) / 2, screenheight / 2 - 100, 70, RED);
            DrawText(TextFormat("FINAL SCORE : %d", score), screenwidth / 2 - MeasureText("FINAL SCORE  ", 50) / 2, screenheight / 2, 50, WHITE);
            EndDrawing();
        }
    }
}
