#include "raylib.h"
#include "game map.h"
#include "AddObject.h"
#include <stdio.h>
#include "leaderboard.h"

#include "logger.h"
char name[50];

extern Score scores[]; // Access global scores array
typedef enum
{
    Play,
    Records,
    Exit
} menu;
#define screenwidth 1200
#define screenheight 900

menu option1 = Play;

Texture logo;
Texture redghost;
Texture yellowghost;
Texture blueghost;
Texture pinkghost;
Texture cherry;
Texture straw;
Texture apple;

void loadTexture()
{
    Image image1 = LoadImage("../image/logo.png");
    logo = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/2.png");
    redghost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/13.png");
    yellowghost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/6.png");
    blueghost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/1.png");
    pinkghost = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/7.png");
    cherry = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/8.png");
    straw = LoadTextureFromImage(image1);
    image1 = LoadImage("../image/apple.jpg");
    apple = LoadTextureFromImage(image1);
}
void Draw(Texture i, int x, int y)
{
    ClearBackground(BLACK);
    DrawTexture(i, x, y, WHITE);
}
void show_menu()
{
    ClearBackground(BLACK);
    loadTexture();
    DrawText("Pac Man", screenwidth / 2 - MeasureText("Pac Man", 100) / 2, screenheight / 2 - 150, 100, YELLOW);
    DrawText("PLAY", screenwidth / 2 - MeasureText("PLAY", 60) / 2, screenheight / 2, 60, option1 == Play ? RED : WHITE);
    DrawText("RECORDS", screenwidth / 2 - MeasureText("RECORDS", 60) / 2, screenheight / 2 + 100, 60,
             option1 == Records ? RED : WHITE);
    DrawText("EXIT", screenwidth / 2 - MeasureText("EXIT", 60) / 2, screenheight / 2 + 200, 60,
             option1 == Exit ? RED : WHITE);
    Draw(redghost, 50, 50);
    Draw(yellowghost, 200, 50);
    Draw(blueghost, 350, 50);
    Draw(pinkghost, 500, 50);
    Draw(cherry, 1050, 700);
    Draw(straw, 900, 680);
    Draw(apple, 750, 700);
}

// Display leaderboard
void displayScoresGraphically(Score scores[])
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Leaderboard:", screenwidth / 2 - MeasureText("Leaderboard:", 50) / 2, 100, 50, YELLOW);

        int hasScores = 0;
        for (int i = 0; i < MAX_SCORES; i++)
        {
            if (scores[i].score > 0)
            {
                hasScores = 1;
                break;
            }
        }

        if (hasScores)
        {
            for (int i = 0; i < MAX_SCORES && scores[i].score > 0; i++)
            {
                char scoreLine[100];
                snprintf(scoreLine, sizeof(scoreLine), "%s: %d (%s)",
                         scores[i].name, scores[i].score, scores[i].timestamp);
                DrawText(scoreLine,
                         screenwidth / 2 - MeasureText(scoreLine, 30) / 2,
                         200 + i * 40,
                         30,
                         WHITE);
            }
        }
        else
        {
            DrawText("No scores available",
                     screenwidth / 2 - MeasureText("No scores available", 30) / 2,
                     screenheight / 2,
                     30,
                     RED);
        }

        DrawText("Press ESC to return",
                 screenwidth / 2 - MeasureText("Press ESC to return", 30) / 2,
                 screenheight - 100,
                 30,
                 RED);

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE))
        {
            break;
        }
    }
}
void getPlayerName()
{
    int textLength = 0;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Display prompt to enter name
        DrawText("Enter your name:", screenwidth / 2 - MeasureText("Enter your name:", 50) / 2, screenheight / 2 - 150, 50, DARKGREEN);
        DrawText(name, screenwidth / 2 - MeasureText(name, 30) / 2, screenheight / 2, 30, RAYWHITE);

        // Input name by capturing keyboard input
        if (textLength < 49)
        {
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                textLength--;
                name[textLength] = '\0'; // Remove last character
            }
            else
            {
                char key = GetCharPressed();
                if (key > 0)
                {
                    name[textLength] = key;
                    textLength++;
                }
            }
        }

        // If Enter is pressed, start the game
        if (IsKeyPressed(KEY_ENTER) && textLength > 0)
        {
            break; // Proceed to game once Enter is pressed
        }

        EndDrawing();
    }
}
void handle_options(menu current_option, Score scores[])
{
    switch (current_option)
    {
    case Play:
        // TODO: pass the player name here
        getPlayerName();
        showmovement(scores, name);
        break;
    case Records:
        displayScoresGraphically(scores);
        break;
    case Exit:
        CloseWindow();
        break;
    default:
        break;
    }
}
void select_option(Score scores[])
{
    if (IsKeyPressed(KEY_DOWN))
    {
        option1 = (option1 + 1) % 3;
    }
    else if (IsKeyPressed(KEY_UP))
    {
        option1 = (option1 + 2) % 3;
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        handle_options(option1, scores);
    }
}
