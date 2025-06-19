/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Simplified template for game
 * development in C using Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*---------------------------------------------
 * Library headers.
 *-------------------------------------------*/
#include "raylib/raylib.h"
//#include "raylib/raymath.h"

/*---------------------------------------------
 * Project headers.
 *-------------------------------------------*/


/*---------------------------------------------
 * Macros. 
 *-------------------------------------------*/


/*--------------------------------------------
 * Constants. 
 *------------------------------------------*/
const int PARADO = 0;
const int JOGANDO = 1;
 
/*---------------------------------------------
 * Custom types (enums, structs, unions, etc.)
 *-------------------------------------------*/
typedef struct Jogador {

    Vector2 pos;
    Vector2 dim;
    float vel;
    Color cor;
    float energia;
    int pontuacao;
    Vector2 posEnergia;

}  Jogador;

typedef struct Bola {
    Vector2 pos;
    Vector2 vel;
    float raio;
    Color cor;
} Bola;

/*---------------------------------------------
 * Global variables.
 *-------------------------------------------*/
Jogador jogador1;
Jogador jogador2;
Bola bola;
Jogador *vezDe;
int estado;
char *textoJogador1 = "Player 1: W/S para mover, SHIFT para impulso";
char *textoJogador2 = "Player 2: UP/DOWN para mover, SHIFT DIR para impiulso";
char *iniciar = "Pressione ENTER para iniciar";

/*---------------------------------------------
 * Function prototypes. 
 *-------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 */
void update( float delta );

/**
 * @brief Draws the state of the game.
 */
void draw( void );

void atualizarJogador( Jogador *jogador, float delta, int teclaCima, int teclaBaixo, int teclaImpulso);
void desenharJogador( Jogador *jogador1, Jogador *jogador2);
void desenharQuadra( void );
void desenharPlacar ( Jogador *jogador1, Jogador *jogador2);
void atualizarBola( Bola *bola, float delta, Jogador *jogador1, Jogador *jogador2);
void desenharBola( Bola *bola);
void resolverColisao( Jogador *jogador1, Jogador * jogador2, Bola *bola);


/**
 * @brief Game entry point.
 */
int main( void ) {

    // local variables and initial user input

    // antialiasing
    SetConfigFlags( FLAG_MSAA_4X_HINT );

    // creates a new window 800 pixels wide and 450 pixels high
    InitWindow( 800, 450, "Window Title" );

    // init audio device only if your game uses sounds
    //InitAudioDevice();

    // FPS: frames per second
    SetTargetFPS( 60 );    

    // you must load game resources here
    jogador1 = (Jogador) {
        .pos = {
            .x = 20,
            .y = GetScreenHeight() / 2 - 75/2
        },
        .dim = {
            .x = 15,
            .y = 75
        },
        .vel = 100,
        .cor = GREEN,
        .energia = 50,
        .pontuacao = 0,
        .posEnergia = {
            .x = GetScreenWidth() /2 -80,
            .y = 40
        }
    };

    jogador2 = (Jogador) {
        .pos = {
            .x = GetScreenWidth() - 30,
            .y = GetScreenHeight() / 2 - 75/2
        },
        .dim = {
            .x = 15,
            .y = 75
        },
        .vel = 100,
        .cor = BLUE,
        .energia = 50,
        .pontuacao = 0,
        .posEnergia = {
            .x = GetScreenWidth() /2 +80,
            .y = 40
        }
    };

    bola = (Bola) {
        .pos = {
            .x = GetScreenWidth() / 2,
            .y = GetScreenHeight() /2
        },
        .vel = {
            .x = 200,
            .y = 200
        },
        .raio = 5,
        .cor = WHITE
    };

    vezDe = &jogador1;
    estado = PARADO;

    // game loop
    while ( !WindowShouldClose() ) {
        update( GetFrameTime() );
        draw();
    }

    // you should unload game resources here

    
    // close audio device only if your game uses sounds
    //CloseAudioDevice();
    CloseWindow();

    return 0;

}

void update( float delta ) {

    if (estado == PARADO) {

        if  (IsKeyPressed(KEY_ENTER)){
        textoJogador1 = "";
        textoJogador2 = "";
        iniciar = "";
    }
        if (IsKeyPressed(KEY_ENTER)) {
            estado = JOGANDO;
            if (vezDe == &jogador1) {
                bola.vel.x = 200;
                bola.vel.y = GetRandomValue( 0,1 ) == 0 ? 200 : -200;
            }
        }
    
    }else {
        atualizarJogador( &jogador1, delta, KEY_W, KEY_S, KEY_LEFT_SHIFT);
        atualizarJogador( &jogador2, delta, KEY_UP, KEY_DOWN, KEY_RIGHT_SHIFT);
        atualizarBola( &bola, delta, &jogador1, &jogador2);
        resolverColisao( &jogador1, &jogador2, &bola);
    }

    
}

void draw( void ) {

    BeginDrawing();
    ClearBackground( BLACK );
    
    desenharJogador( &jogador1, &jogador2);
    
    desenharQuadra();
    desenharPlacar ( &jogador1, &jogador2);
    desenharBola( &bola);

    
    
    DrawText( iniciar, 50, 100, 20, WHITE );

    DrawText( textoJogador1, 50, 150, 20, jogador1.cor );
    DrawText( textoJogador2, 50, 180, 20, jogador2.cor );

    DrawFPS( 20, 20);

    

    EndDrawing();

}

void atualizarJogador( Jogador *jogador, float delta, int teclaCima, int teclaBaixo, int teclaImpulso){
    if( jogador->energia >= 0 && IsKeyDown( teclaImpulso)){
            jogador->vel = 500;
            jogador->energia -= 8;
    }else
            jogador->vel = 100; 

    if (jogador->energia < 150 && IsKeyUp( teclaImpulso)){
    jogador->energia += 1;
    }
    
    if (IsKeyDown( teclaCima) ) {
        jogador->pos.y -= jogador->vel * delta;

    }else if (IsKeyDown(teclaBaixo) ) {
        jogador->pos.y += jogador->vel * delta;

    }

    if ( jogador->pos.y < 0) {
        jogador->pos.y = 0;
    } else if ( jogador->pos.y + jogador->dim.y > GetScreenHeight()) {
        jogador->pos.y = GetScreenHeight() - jogador->dim.y;
    }

    

}

void desenharJogador( Jogador *jogador1, Jogador *jogador2){
    DrawRectangle( 
    jogador1->pos.x, 
    jogador1->pos.y, 
    jogador1->dim.x, 
    jogador1->dim.y, 
    jogador1->cor
    );

    DrawRectangle( 
        jogador2->pos.x, 
        jogador2->pos.y, 
        jogador2->dim.x, 
        jogador2->dim.y, 
        jogador2->cor
    );

    DrawRectangle(
        GetScreenWidth() /2 -80 -jogador1->energia,
        jogador1->posEnergia.y,
        jogador1->energia,
        15,
        jogador1->cor
    );

    DrawRectangle(
        jogador2->posEnergia.x,
        jogador2->posEnergia.y,
        jogador2->energia,
        15,
        jogador2->cor
    );
}

void desenharPlacar ( Jogador *jogador1, Jogador *jogador2){

    int centro = GetScreenWidth() / 2;
    int margem = 30;

    const char *pj1 = TextFormat( "%d", jogador1->pontuacao);
    const char *pj2 = TextFormat( "%d", jogador2->pontuacao);

    DrawText(
        pj1, 
        centro + margem - TextLength(pj1),
        margem,
        40,jogador2->cor
    );

    DrawText(
        pj2, 
        centro - margem - 20,
        margem,
        40,jogador1->cor
    );

}

void desenharQuadra(void){

    int centro = GetScreenWidth() / 2;
    int mLarg = 5;
    int altura = 30;

    for (int i = 0; i < 20; i += 2) {
        
        DrawRectangle( centro - mLarg, altura * i, mLarg *2, altura, WHITE );
    }

    
}

void atualizarBola( Bola *bola, float delta, Jogador *jogador1, Jogador *jogador2){

    bola->pos.x += bola->vel.x * delta;
    bola->pos.y += bola->vel.y * delta;

    if (bola->pos.x + bola->raio > GetScreenWidth()) {
        bola->vel.x = -bola->vel.x;
    } else if ( bola->pos.x - bola->raio < 0) {
        bola->vel.x = -bola->vel.x;
    }

    if (bola->pos.y + bola->raio > GetScreenHeight()) {
        bola->vel.y = -bola->vel.y;
    } else if ( bola->pos.y - bola->raio < 0) {
        bola->vel.y = -bola->vel.y;
    }
    if (bola->pos.x <= 10 + bola->raio) {
        
        jogador1->pontuacao++;
        
        bola->pos = (Vector2) {
            .x = GetScreenWidth() / 2,
            .y = GetScreenHeight() / 2
        };
        bola->vel = (Vector2) {
            .x = 200,
            .y = GetRandomValue( 0,1 ) == 0 ? 200 : -200
        };
    }

    if (bola->pos.x >= GetScreenWidth() - 10 - bola->raio) {
        
        jogador2->pontuacao++;
        
        bola->pos = (Vector2) {
            .x = GetScreenWidth() / 2,
            .y = GetScreenHeight() / 2
        };
        bola->vel = (Vector2) {
            .x = -200,
            .y = GetRandomValue( 0,1 ) == 0 ? 200 : -200
        };
    }
}

void desenharBola( Bola *bola){

    DrawCircleV( bola->pos, bola->raio, bola->cor);


}

void resolverColisao( Jogador *jogador1, Jogador * jogador2, Bola *bola){

    bool cj1 = CheckCollisionCircleRec(
        bola->pos,
        bola->raio,
        (Rectangle) {
            .x = jogador1->pos.x,
            .y = jogador1->pos.y,
            .width = jogador1->dim.x,
            .height = jogador1->dim.y

        }
    );

    bool cj2 = CheckCollisionCircleRec(
        bola->pos,
        bola->raio,
        (Rectangle) {
            .x = jogador2->pos.x,
            .y = jogador2->pos.y,
            .width = jogador2->dim.x,
            .height = jogador2->dim.y

        }
    );

    if (cj1){
        bola->pos.x = jogador1->pos.x + jogador1->dim.x + bola->raio;
        bola->vel.x = -bola->vel.x;
    }

    if (cj2){
        bola->pos.x = jogador2->pos.x - bola->raio;
        bola->vel.x = -bola->vel.x;
    }

}