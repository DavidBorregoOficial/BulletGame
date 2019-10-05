/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#define MAX_ENEMIGOS 100 //cantidad de bolas
#define MAX_BULLET 100 //cantidad de bolas

typedef struct Enemies{ //estructura de los enemigos
    int speed;
    Rectangle body;
} Enemies;
typedef struct Bullet{ //estructura de los balas
    int speed;
    Rectangle body;
    bool active;
} Bullet;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1280;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");


    Enemies bola [MAX_ENEMIGOS];
    for(int i = 0; i < MAX_ENEMIGOS; i++) //array con velocidad, tamaño, posicion de los enemigos
    {
        bola[i].body.width = 40;
        bola[i].body.height = 40;
        bola[i].body.x = GetRandomValue(screenWidth, screenWidth *2);
        bola[i].body.y = GetRandomValue(0, screenHeight-bola[i].body.height);
        bola[i].speed = -3;
    }
    
    Rectangle rec = { 0, 300, 20, 20 }; //jugador    
    Bullet bala [MAX_BULLET];
    for(int i = 0; i < MAX_BULLET; i++) //array con velocidad, tamaño, posicion de los enemigos
    {
        bala[i].body.width = 10;
        bala[i].body.height = 10;
        bala[i].body.x = -100;
        bala[i].body.y = -100;
        bala[i].speed = 5;
        bala[i].active = false;
    }    
    Rectangle fondo = { 0, 0, screenWidth, screenHeight }; //fondo gameover
    
    int puntoplayer = 0; //puntuacion
    int hightscore = 0; //record de puntuacion
    int cadencia = 0; //cadencia de disparo
    
    bool GameOver = true;
    bool pause = false;
    int framesCounter = 30;
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        if(GameOver) 
            {
                if (!pause)
                    {
                        if(IsKeyDown (KEY_W)) rec.y -=10; // Controles
                        if(IsKeyDown (KEY_S)) rec.y +=10;
                        if(IsKeyDown (KEY_A)) rec.x -=10;
                        if(IsKeyDown (KEY_D)) rec.x +=10;
                     
                        if(rec.y <= 0) rec.y = 0; //limites de pantalla para el jugador
                        if(rec.x <= 0) rec.x = 0;
                        if(rec.y + rec.height >= screenHeight) rec.y = screenHeight - rec.height;
                        if(rec.x + rec.width >= screenWidth) rec.x = screenWidth - rec.width;
                     
                        for(int i = 0; i < MAX_ENEMIGOS; i++)
                            {        
                                if(bola[i].body.x <= -40) //reset de enemigos
                                {
                                    bola[i].body.x = GetRandomValue(screenWidth, screenWidth *2);
                                    bola[i].body.y = GetRandomValue(0, screenHeight-bola[i].body.height);
                                    if(puntoplayer >= hightscore) //si el puntuaje supera el highscore
                                    {
                                        hightscore = puntoplayer;
                                    }
                                }
                                bola[i].body.x += bola[i].speed; //direccion de los enemigos
                                if(puntoplayer >= 500) //level de juego
                                    {
                                        bola[i].speed = -8;
                                    }
                                if(puntoplayer >= 1000) //level de juego
                                    {
                                       bola[i].speed = -12;
                                    }
                                if(puntoplayer >= 1500) //level de juego
                                    {
                                        bola[i].speed = -15;
                                    }
                                if(puntoplayer >= 2000) //level de juego
                                    {
                                        bola[i].speed = -20;
                                    }                                    
                                if(CheckCollisionRecs(bola[i].body,rec)) //colision con enemigos = derrotado
                                    {
                                        if(bola[i].body.x >= rec.x || bola[i].body.y <= rec.y + rec.width)
                                            {
                                                pause = !pause;                                        
                                                GameOver = !GameOver;
                                            }            
                                    }                                    
                            }
                    if(IsKeyDown(KEY_SPACE))//disparo player
                        {
                            cadencia++;
                            for(int i = 0; i < MAX_BULLET; i++)
                                {
                                    if(!bala[i].active && cadencia == 5)
                                    {
                                        bala[i].body.x = rec.x + 10;
                                        bala[i].body.y = rec.y + 5;
                                        bala[i].active = true;
                                        cadencia = 0;
                                        break;
                                    }                              
                                }
                        }
                    for(int i = 0; i < MAX_BULLET; i++) //Activacion de bala
                            {
                                if(bala[i].active)
                                {
                                    bala[i].body.x += bala[i].speed;
                                    for (int j = 0; j < MAX_ENEMIGOS; j++)
                                    {
                                        if (CheckCollisionRecs(bala[i].body,bola[j].body))
                                        {
                                            bala[i].active = false;
                                            bala[i].body.x = -100;
                                            bala[i].body.y = -100;
                                            bola[j].body.x = GetRandomValue(screenWidth, screenWidth *2);
                                            bola[j].body.y = GetRandomValue(0, screenHeight-bola[i].body.height);                          
                                            puntoplayer += 10; //suma de puntuacion            
                                        }
                                    }
                                }
                                if(bala[i].body.x >= screenWidth+40)
                                {
                                    bala[i].body.x = -100;
                                    bala[i].body.y = -100;
                                    bala[i].active = false;
                                }
                            }                                
                    }
            }
        if (IsKeyPressed(KEY_P)) //pausa
            {
                pause = !pause;
                framesCounter = 30;
            }
        if(!GameOver)
            {
                if(IsKeyPressed (KEY_ENTER)) //tecla de reset
                    {
                        for(int i = 0; i < MAX_ENEMIGOS; i++) 
                        {
                            bola[i].speed = -3;
                            rec.x = 0;
                            rec.y = 300;
                            bola[i].body.x = GetRandomValue(screenWidth, screenWidth *2);
                            bola[i].body.y = GetRandomValue(0, screenHeight-bola[i].body.height);
                            puntoplayer = 0;                        
                            GameOver = true;  
                            pause = false;                            
                        } 
                        for(int j = 0; j < MAX_BULLET; j++)
                            {
                                bala[j].body.x = -100;
                                bala[j].body.y = -100;
                            }                        
                    }           
            }            
        else
        {
            framesCounter++;
        }
             
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText(FormatText("%i", puntoplayer), 500, 10, 80, RED); //puntuacion
            DrawText(FormatText("%i", hightscore), 700, 10, 80, GREEN); //alta puntuacion
            DrawRectangleRec(rec, BLUE);
            for(int i = 0; i < MAX_ENEMIGOS; i++)
            {
                DrawRectangle(bola[i].body.x,bola[i].body.y,bola[i].body.width,bola[i].body.height,RED); //enemigos
            }
            for(int i = 0; i < MAX_BULLET; i++)
            {
                DrawRectangle(bala[i].body.x,bala[i].body.y,bala[i].body.width,bala[i].body.height,BLUE); //balas
            }                            
                
                if(pause)
                {
                    if((framesCounter/50)%2) DrawText("PAUSE", (screenWidth - MeasureText("PAUSE", 85))/2, screenHeight/2.25 -5, 75, RED); //pausa
                }            
            if(!GameOver) 
            {
                DrawRectangleRec(fondo,BLACK); //fondo game over
                DrawText("GAMEOVER", 450, 300, 60, LIGHTGRAY);
                DrawText("Press ENTER to restart", 350, 430, 60, LIGHTGRAY); // boton de reset
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}