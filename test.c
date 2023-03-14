#include "include/raylib.h"
#include <stdio.h>

#define genOffset()                                                               \
    for (int x = 0; x < 8; x++)                                                   \
    {                                                                             \
        for (int y = 0; y < 8; y++)                                               \
        {                                                                         \
            for (int z = 0; z < 2; z++)                                           \
            {                                                                     \
                checkoffset[x][y][z] = GetRandomValue(-offset / 20, offset / 20); \
            }                                                                     \
        }                                                                         \
    }
#define nullVector2 \
    (Vector2)       \
    {               \
        -1, -1      \
    }
#define saveMap(x, y) (x >= 0 && x <= 7 && y >= 0 && y <= 7) ? map[x][y] : -1;

#define end(text)   \
    notEnd = false; \
    endText = text;

float abss(float x)
{
    return x < 0 ? -x : x;
}

int main(void)
{
    const int screenSize = 1400;
    const int textSize = screenSize / 10;
    const int borderSize = screenSize / 20;
    const int offset = (screenSize - 2 * borderSize) / 8;

    InitWindow(screenSize, screenSize, "Сheckers");

    SetTargetFPS(60);

    const Color CheckWhite = {218, 197, 156, 255};
    const Color CheckBlack = {82, 63, 49, 255};

    char move = 1;
    char map[8][8];
    int checkoffset[8][8][2];
    bool firstMove = true;
    bool notEnd = true;
    char *endText;

    genOffset();
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (map[x][y] != 0)
                map[x][y] = 0;
        }
    }
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if ((x + y) % 2 == 1)
            {
                map[x][y] = 1;
            }
        }
    }
    for (int x = 0; x < 8; x++)
    {
        for (int y = 5; y < 8; y++)
        {
            if ((x + y) % 2 == 1)
            {
                map[x][y] = 2;
            }
        }
    }

    Vector2 selected = nullVector2;
    bool sel = false;

    while (!WindowShouldClose())
    {
        if (notEnd)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 pos = GetMousePosition();
                if (pos.x > borderSize && pos.y > borderSize && pos.x < screenSize - borderSize && pos.y < screenSize - borderSize)
                {
                    pos = (Vector2){(int)(pos.x - borderSize) / offset, (int)(pos.y - borderSize) / offset};
                    if (move == map[(int)pos.x][(int)pos.y])
                    {
                        selected = pos;
                        sel = true;
                    }
                }
            }
            else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (sel)
                {
                    Vector2 pos = GetMousePosition();
                    if (pos.x > borderSize && pos.y > borderSize && pos.x < screenSize - borderSize && pos.y < screenSize - borderSize)
                    {
                        pos = (Vector2){(int)(pos.x - borderSize) / offset, (int)(pos.y - borderSize) / offset};
                        if (map[(int)pos.x][(int)pos.y] == 0)
                        {
                            if (abss(pos.x - selected.x) == abss(pos.y - selected.y))
                            {
                                if (firstMove && abss(pos.x - selected.x) == 1)
                                {
                                    if ((pos.y - selected.y) * (move == 1 ? 1 : -1) > 0)
                                    {
                                        map[(int)selected.x][(int)selected.y] = 0;
                                        map[(int)pos.x][(int)pos.y] = move;
                                        move = move == 1 ? 2 : 1;
                                        genOffset();
                                    }
                                }
                                else if (abss(pos.y - selected.y) == 2)
                                {
                                    if (map[(int)(pos.x - (pos.x - selected.x) / 2)][(int)(pos.y - (pos.y - selected.y) / 2)] == (move == 1 ? 2 : 1))
                                    {
                                        map[(int)selected.x][(int)selected.y] = 0;
                                        map[(int)pos.x][(int)pos.y] = move;
                                        map[(int)(pos.x - (pos.x - selected.x) / 2)][(int)(pos.y - (pos.y - selected.y) / 2)] = 0;
                                        genOffset();
                                        move = move == 1 ? 2 : 1;
                                        firstMove = true;
                                    }
                                }
                            }
                        }
                    }
                    sel = false;
                    selected = nullVector2;
                    bool canGo = false;
                    for (int x = 0; x < 8; x++)
                    {
                        for (int y = 0; y < 8; y++)
                        {
                            
                        }
                    }
                }
            }
        }
        BeginDrawing();

        ClearBackground(GRAY);

        if (move == 1)
        {
            DrawRectangle(0, 0, screenSize, screenSize / 2, WHITE);
            DrawRectangleRounded((Rectangle){borderSize / 2, borderSize / 2, screenSize - borderSize, screenSize - borderSize}, 0.1f, false, WHITE);
        }
        else
        {
            DrawRectangle(0, screenSize / 2, screenSize, screenSize, BLACK);
            DrawRectangleRounded((Rectangle){borderSize / 2, borderSize / 2, screenSize - borderSize, screenSize - borderSize}, 0.1f, false, BLACK);
        }

        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                DrawRectangle(borderSize + x * offset, borderSize + y * offset, offset, offset, (x + y) % 2 == true ? DARKGRAY : LIGHTGRAY);
                if (map[x][y] != 0)
                {
                    if (selected.x != x || selected.y != y)
                    {
                        DrawCircle(borderSize + x * offset + offset / 2 + checkoffset[x][y][0], borderSize + y * offset + offset / 2 + checkoffset[x][y][1], offset / 3, map[x][y] == 1 ? CheckWhite : CheckBlack);
                    }
                }
            }
        }

        if (sel)
        {
            DrawCircle(GetMouseX(), GetMouseY(), offset / 3, move == 1 ? CheckWhite : CheckBlack);
        }

        if (!notEnd)
        {
            DrawRectangle(0, 0, screenSize, screenSize, (Color){130, 130, 130, 200});
            DrawText(endText, (screenSize - MeasureText(endText, 40)) / 2, screenSize * 0.48, 40, (Color){255, 203, 0, 200});
        }

        EndDrawing();
    }

    CloseWindow();

    return false;
}