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

#define end(text)   \
    notEnd = false; \
    endText = text;

float abss(float x)
{
    return x < 0 ? -x : x;
}

char map[8][8];

char saveMap(int x, int y)
{
    return (x >= 0 && x <= 7 && y >= 0 && y <= 7) ? map[x][y] : -1;
}

int main(void)
{

    const int screenSize = 1400;
    const int textSize = screenSize / 10;
    const int borderSize = screenSize / 20;
    const int offset = (screenSize - 2 * borderSize) / 8;

    InitWindow(screenSize, screenSize, "Checkers");

    Texture2D blackT = LoadTexture("Black.png");
    Texture2D whiteT = LoadTexture("White.png");

    SetTargetFPS(60);

    const float textureSize = offset / blackT.height / 4 * 3;
    // style
    const Color CheckWhite = {218, 197, 156, 255};
    const Color CheckBlack = {82, 63, 49, 255};

    Vector2 intrp = nullVector2;
    //

    char move = 1;
    int checkoffset[8][8][2];
    bool firstMove = true;
    bool notEnd = true;
    char *endText;


    genOffset();
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
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
    Vector2 last = nullVector2;

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
                        intrp = (Vector2){pos.x*offset+borderSize+offset/6,pos.y*offset+borderSize+offset/6};
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
                                        // move = move == 1 ? 2 : 1;
                                        firstMove = false;
                                        last = pos;
                                    }
                                }
                            }
                        }
                    }
                    sel = false;
                    selected = nullVector2;
                    bool canGo = false; // check
                    char o = move == 1 ? 1 : -1;
                    char oh = move == 1 ? 2 : 1;
                    bool noB = true; // check
                    bool noW = true; // check
                    for (int x = 0; x < 8; x++)
                    {
                        for (int y = 0; y < 8; y++)
                        {
                            if (map[x][y] == 1)
                            {
                                noW = false;
                            }
                            if (map[x][y] == 2)
                            {
                                noB = false;
                            }
                            if (firstMove)
                            {
                                if (map[x][y] == move && !canGo)
                                {
                                    if (saveMap(x + 1, y + o) == 0 || saveMap(x - 1, y + o) == 0)
                                    {
                                        canGo = true;
                                    }
                                    if (saveMap(x + 2, y + 2) == 0 && saveMap(x + 1, y + 1) == oh)
                                    {
                                        canGo = true;
                                    }
                                    if (saveMap(x - 2, y + 2) == 0 && saveMap(x - 1, y + 1) == oh)
                                    {
                                        canGo = true;
                                    }
                                    if (saveMap(x + 2, y - 2) == 0 && saveMap(x + 1, y - 1) == oh)
                                    {
                                        canGo = true;
                                    }
                                    if (saveMap(x - 2, y - 2) == 0 && saveMap(x - 1, y - 1) == oh)
                                    {
                                        canGo = true;
                                    }
                                }
                            }
                        }
                    }
                    if (!firstMove)
                    {
                        if (saveMap(last.x + 2, last.y + 2) == 0 && saveMap(last.x + 1, last.y + 1) == oh)
                        {
                            canGo = true;
                        }
                        else if (saveMap(last.x - 2, last.y + 2) == 0 && saveMap(last.x - 1, last.y + 1) == oh)
                        {
                            canGo = true;
                        }
                        else if (saveMap(last.x + 2, last.y - 2) == 0 && saveMap(last.x + 1, last.y - 1) == oh)
                        {
                            canGo = true;
                        }
                        else if (saveMap(last.x - 2, last.y - 2) == 0 && saveMap(last.x - 1, last.y - 1) == oh)
                        {
                            canGo = true;
                        }
                        else
                        {
                            move = move == 1 ? 2 : 1;
                            firstMove = true;
                            canGo = true;
                        }
                    }
                    if (!canGo)
                    {
                        end("Draw");
                    }
                    if (noB == true)
                    {
                        end("White WIN");
                    }
                    if (noW)
                    {
                        end("Black WIN");
                    }
                }
            }
        }
        BeginDrawing();

        ClearBackground(GRAY);

        DrawRectangleRounded((Rectangle){borderSize / 2, borderSize / 2, screenSize - borderSize, screenSize - borderSize}, 0.05f, false, move == 1 ? WHITE : BLACK);

        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                DrawRectangle(borderSize + x * offset, borderSize + y * offset, offset, offset, (x + y) % 2 == true ? BLACK : WHITE);
                if (map[x][y] != 0)
                {
                    if (selected.x != x || selected.y != y)
                    {
                        DrawTextureEx(map[x][y] == 1 ? whiteT : blackT, (Vector2){borderSize + x * offset + offset / 6 + checkoffset[x][y][1], borderSize + y * offset + offset / 6 + checkoffset[x][y][1]}, 0, textureSize, WHITE);
                    }
                }
            }
        }

        if (sel)
        {
            //DrawTextureEx(move == 1 ? whiteT : blackT, (Vector2){GetMouseX() - offset / 3, GetMouseY() - offset / 3}, 0, textureSize, WHITE);
            DrawTextureEx(move == 1 ? whiteT : blackT, intrp, 0, textureSize*1.2, WHITE);
            intrp.x = intrp.x + (GetMouseX() - offset / 3 - intrp.x)/6;
            intrp.y = intrp.y + (GetMouseY() - offset / 3 - intrp.y)/6;
        }

        if (!notEnd)
        {
            DrawRectangle(0, 0, screenSize, screenSize, (Color){130, 130, 130, 200});
            DrawText(endText, (screenSize - MeasureText(endText, 100)) / 2, (screenSize - 100)/2, 100, (Color){255, 203, 0, 200});
        }

        EndDrawing();
    }
    CloseWindow();

    return false;
}