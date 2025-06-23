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
#include <string.h>
#include <math.h> // coloquei pra usar o abs()

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
    float energia; // energia do jogador, usada para o impulso
    // quando a energia chega a 150, o jogador começa a acumular super impulso
    float superImpulso; // quando o super impulso chega a 150, o jogador pode usar a habilidade especial
    // que aumenta a altura do jogador e o tamanho dele por 10 segundos
    // depois disso, o jogador volta ao tamanho normal
    float tempoSuperImpulso; //autoexplicativo. kkkkkk
    int pontuacao;
    Vector2 posEnergia; //posição do recurso de energia, que é um retângulo que mostra a energia do jogador para o impulso de velocidade
    Vector2 posSuperImpulso; //posição do recurso de super impulso, que é um retângulo que mostra a barra de especial

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
Jogador computador;
Bola bola;
Jogador *vezDe;
int estado;
char *textoJogador1 = "Player 1: W/S para mover, SHIFT para impulso, CTRL para Habilidade (especial cheio)"; //tentei fazer como no template, quando você aperta enter, os texto somem, ele são definidos para ""
char *textoJogador2 = "Player 2: UP/DOWN para mover, SHIFT DIR para impulso, CTRL DIR para Habilidade (especial cheio)";
char *iniciar = "Pressione ENTER para iniciar";
bool modoDeJogo = true; // se o jogo for de 1 ou 2 jogadores
// falso = 1 jogador, true = 2 jogadores

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

void atualizarJogador( Jogador *jogador, float delta, int teclaCima, int teclaBaixo, int teclaImpulso, int teclaSuperImpulso);
void desenharJogador( Jogador *jogador1, Jogador *jogador2);
void desenharQuadra( void );
void desenharPlacar ( Jogador *jogador1, Jogador *jogador2);
void atualizarBola( Bola *bola, float delta, Jogador *jogador1, Jogador *jogador2);
void desenharBola( Bola *bola);
void resolverColisao( Jogador *jogador1, Jogador * jogador2, Bola *bola);
// vou tentar fazer ele se movimentar sozinho de acordo com a posição da bola
void atualizarComputador( Jogador *computador, float delta, Bola *bola);
//tive que fazer uma função separada para o computador, porque ele não usa as mesmas teclas que o jogador 2, e eu não consegui fazer isso de outra forma
// mas o código é praticamente o mesmo, só que com algumas modificações
// talvez eu consiga fazer isso de uma forma mais eficiente no futuro, mas por enquanto tá funcionando bem assim
void usarPoderes( Jogador *jogador, float delta, Bola *bola);



/**
 * @brief Game entry point.
 */
int main( void ) {

    // local variables and initial user input

    // antialiasing
    SetConfigFlags( FLAG_MSAA_4X_HINT );

    // creates a new window 800 pixels wide and 450 pixels high
    InitWindow( 800, 450, "Ping Pong remake em C     :D" ); // fiz o jogo pra rodar somente nessa resolução, a posição dos jogadores, dos recursos de energia e da bola são relativas a essa resolução somente

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
        .cor = BLUE,
        .energia = 0, //energia inicial
        .superImpulso = 0, //especial inicial
        .pontuacao = 0,
        .posEnergia = {
            .x = GetScreenWidth() /2 -80, //posicao dos recursos de energia
            .y = 40
        },
        .posSuperImpulso = {
            .x = GetScreenWidth() /2 -80,
            .y = 40
        },
        .tempoSuperImpulso = 0 //tempo inicial do especial, que é modificado quando o jogador usa a habilidade especial

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
        .cor = RED,
        .energia = 0,
        .superImpulso = 0,
        .pontuacao = 0,
        .posEnergia = {
            .x = GetScreenWidth() /2 +80,
            .y = 40
        },
        .posSuperImpulso = {
            .x = GetScreenWidth() /2 +80,
            .y = 40
        },
        .tempoSuperImpulso = 0
    };
    /* (ideia inutilizada, mas deixei aqui para referência futura,
    estou tentando reutilizar o código do jogador 2 para o computador)
    computador = (Jogador) {
        // computador é o jogador 2 quando o jogo é de 1 jogador, então ele tem as mesmas propriedades do jogador 2
        .pos = {
            .x = GetScreenWidth() - 30,
            .y = GetScreenHeight() / 2 - 75/2
        },
        .dim = {
            .x = 15,
            .y = 75
        },
        .vel = 100,
        .cor = GREEN,
        .energia = 0,
        .superImpulso = 0,
        .pontuacao = 0,
        .posEnergia = {
            .x = GetScreenWidth() /2 +80,
            .y = 40
        },
        .posSuperImpulso = {
            .x = GetScreenWidth() /2 +80,
            .y = 40
        },
        .tempoSuperImpulso = 0
    };
    */
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


        /*if (IsKeyPressed(KEY_ENTER)){ // quando apertar enter, o jogo inicia e os textos somem, não sei se eu podia ter feito de um jeito melhor
            textoJogador1 = "";
            textoJogador2 = "";
            iniciar = "";
        }*/
        if (IsKeyPressed(KEY_ONE)) { // se o jogador apertar 1, o jogo é de 1 jogador
            modoDeJogo = false;
            textoJogador2 = ""; // fiz isso de novo professor, me perdoa D:
        } else if (IsKeyPressed(KEY_TWO)) { // se o jogador apertar 2, o jogo é de 2 jogadores
            modoDeJogo = true;
            textoJogador2 = "Player 2: UP/DOWN para mover, SHIFT DIR para impulso, CTRL DIR para Habilidade (especial cheio)";
            // gosto de ficar brincando com as strings,
            // dá pra ter mais criatividade assim,
            // mas sei que em algum momento isso pode ser um problema
        }
        if (IsKeyPressed(KEY_ENTER)) {
            estado = JOGANDO;
            if (vezDe == &jogador1) {
                bola.vel.x = 200;
                bola.vel.y = GetRandomValue( 0,1 ) == 0 ? 200 : -200;
            }
        }
    
    }else {
        atualizarJogador( &jogador1, delta, KEY_W, KEY_S, KEY_LEFT_SHIFT, KEY_LEFT_CONTROL); //coloquei mais argumentos para as teclas de impulso e especial, pra eu conseguir usar teclas diferentes para cada jogador
        
        if (modoDeJogo) { // se o jogo for de 2 jogadores, o jogador 2 pode usar as teclas de impulso e especial diferentes
            atualizarJogador( &jogador2, delta, KEY_UP, KEY_DOWN, KEY_RIGHT_SHIFT, KEY_RIGHT_CONTROL);
        } else {
            // se for de 1 jogador, o jogador 2 não precisa fazer nada
            atualizarComputador( &jogador2, delta, &bola); // aqui o computador vai tentar se posicionar na altura da bola
            
            
        }
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

    // melhor assim ;)
    if ( estado == PARADO ) {
        DrawText( iniciar, 50, 100, 20, WHITE ); //os textos de instrução do jogo, que aparecem quando o jogo abre
        DrawText( textoJogador1, 50, 150, 15, jogador1.cor );
        DrawText( textoJogador2, 50, 180, 15, jogador2.cor );
        DrawText( "Pressione 1 para jogar sozinho, ou 2 para jogar com outra pessoa", 50, 210, 15, GREEN );
    }
    
    // aprendi a desenhar as bordas sem precisar fazer 4 linhas!!!!! kkkkkkkkk.
    // aqui vai desenhar as bordas dos recursos de energia e especial
    DrawRectangleLines( 
        //esse é o recurso de energia do jogador 1
        GetScreenWidth() /2 -80-150, 
        jogador1.posEnergia.y, 
        150, 
        15,
        jogador1.cor
    );

    DrawRectangleLines( //esse é o recurso de energia do jogador 2
        jogador2.posEnergia.x, 
        jogador2.posEnergia.y, 
        150, 
        15,
        jogador2.cor
    );

    DrawRectangleLines( //esse é o recurso de especial do jogador 1
        GetScreenWidth() /2 -80-150, 
        jogador1.posSuperImpulso.y + 20, 
        150, 
        15,
        YELLOW
    );

    DrawRectangleLines( //esse é o recurso de especial do jogador 2
        jogador2.posSuperImpulso.x, 
        jogador2.posSuperImpulso.y + 20, 
        150, 
        15,
        YELLOW
    );



    //DrawFPS( 20, 20); // Se quiser ver os FPS.

    

    EndDrawing();

}

void atualizarJogador( Jogador *jogador, float delta, int teclaCima, int teclaBaixo, int teclaImpulso, int teclaSuperImpulso) { //aqui que começa a ficar legal

    // se o jogador tiver energia e apertar a tecla de impulso, ele ganha
    // velocidade e perde 8 de energia por tick
    if( jogador->energia > 0 && IsKeyDown( teclaImpulso ) ){ 
        jogador->vel = 500;
        jogador->energia -= 8;
        if (jogador->energia < 0) {
            jogador->energia = 0; // se a energia for menor que 0, ela fica em 0
        }
    } else {
        jogador->vel = 100; 
    }

    if ( jogador->energia < 150 && IsKeyUp( teclaImpulso)){ // se o jogador não estiver apertando a tecla de impulso, ele ganha 1 de energia por tick, até chegar a 150
        
        jogador->energia += 1;
    }
    
    if (jogador->energia > 150) {
        jogador->energia = 150; // se a energia for maior que 150, ela fica em 150
    }

    

    if (jogador->superImpulso < 150){ //aqui é a mecânica de recarregar o super impulso, que é a habilidade especial do jogador

        if(jogador->energia == 150) { //se a energia for 150, o jogador começa a acumular super impulso
            jogador->superImpulso += .1;
        }

    } else if (IsKeyPressed(teclaSuperImpulso)) { //se o jogador apertar a tecla de super impulso, ele usa a habilidade especial, que aumenta o tamanho dele por 7.5 segundos
        jogador->pos.y -= 50/2;
        jogador->dim.y += 50;
        jogador->superImpulso = 0;
        jogador->tempoSuperImpulso = 10*60; // 10 segundos * 60 FPS = 450 ticks, que é o tempo que o jogador fica com a habilidade especial ativa
        
    } else {
        jogador->superImpulso = 150;
    }

    if (jogador->tempoSuperImpulso > 0) { // se o jogador estiver com a habilidade especial ativa, ele perde 1 tick de tempo por tick (faz sentido? kkkkkkkk)
        jogador->tempoSuperImpulso -= 1;
        
    }

    if (jogador->tempoSuperImpulso <= 0 && jogador->dim.y > 75) { // se o tempo do super impulso acabar e o jogador estiver com o tamanho aumentado, ele volta ao tamanho normal
        jogador->pos.y += 50/2;
        jogador->dim.y -= 50;
    }


    if (IsKeyDown( teclaCima) ) {
        jogador->pos.y -= jogador->vel * delta;

    } else if (IsKeyDown(teclaBaixo) ) {
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
        // posição do recurso de energia do jogador 1, consegui fazer ele
        // parecer carregar ao contrário para ficar mais bonito
        GetScreenWidth() /2 -80 -jogador1->energia, 
        jogador1->posEnergia.y,
        jogador1->energia,
        15,
        jogador1->cor
    );

    DrawRectangle( // posição do recurso de energia do jogador 2
        jogador2->posEnergia.x,
        jogador2->posEnergia.y,
        jogador2->energia,
        15,
        jogador2->cor
    );

    DrawRectangle(
        GetScreenWidth() /2 -80 -jogador1->superImpulso, // posição do recurso de super impulso do jogador 1
        jogador1->posSuperImpulso.y + 20,
        jogador1->superImpulso,
        15,
        YELLOW
    );

    DrawRectangle( // posição do recurso de super impulso do jogador 2
        jogador2->posSuperImpulso.x,
        jogador2->posSuperImpulso.y + 20,
        jogador2->superImpulso,
        15,
        YELLOW
    );
}

void desenharPlacar( Jogador *jogador1, Jogador *jogador2){

    int centro = GetScreenWidth() / 2;
    int margem = 30;

    char pj1[3]; // 2 dígitos + 1 para o terminador nulo
    char pj2[3];
    char energia1[5];
    char energia2[5];
    char tempoImpulso1[7];
    char tempoImpulso2[7];
    char superImpulso1[7];
    char superImpulso2[7];

    strcpy(pj1, TextFormat("%d", jogador1->pontuacao));
    strcpy(pj2, TextFormat("%d", jogador2->pontuacao));
    strcpy(energia1, TextFormat("%.0f", jogador1->energia));
    strcpy(energia2, TextFormat("%.0f", jogador2->energia));
    strcpy(tempoImpulso1, TextFormat("%.2f", jogador1->tempoSuperImpulso/60));
    strcpy(tempoImpulso2, TextFormat("%.2f", jogador2->tempoSuperImpulso/60));
    strcpy(superImpulso1, TextFormat("%.0f", jogador1->superImpulso));
    strcpy(superImpulso2, TextFormat("%.0f", jogador2->superImpulso));

    /* "O Raylib usa um buffer interno compartilhado para TextFormat,
    então se você chamar TextFormat várias vezes na mesma função,
    todas as variáveis vão apontar para o mesmo buffer,
    e o conteúdo será sobrescrito a cada chamada."*/

    // mandei a documentação do Raylib para uma IA e ela me respondeu isso,
    // perdão por usar IA professor :´(

    // eu tava tentando colocar mais texto e eles ficavam iguais,
    // esse é o motivo,
    // eu não sabia disso, e achei que era um bug do Raylib,
    // mas é só uma limitação do TextFormat mesmo,
    // aí eu lembrei do que você falou sobre copiar strings e usei o strcpy

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

    DrawText( energia1,
        jogador1->posEnergia.x - 20,
        jogador1->posEnergia.y + 1,
        15, WHITE
    );

    DrawText( energia2,
        jogador2->posEnergia.x + 5,
        jogador2->posEnergia.y + 1,
        15, WHITE
    );

    DrawText( tempoImpulso1,
        jogador1->posSuperImpulso.x - 20,
        jogador1->posSuperImpulso.y + 40,
        15, WHITE
    );

    DrawText( tempoImpulso2,
        jogador2->posSuperImpulso.x + 5,
        jogador2->posSuperImpulso.y + 40,
        15, WHITE
    );

    DrawText( superImpulso1,
        jogador1->posSuperImpulso.x - 20,
        jogador1->posSuperImpulso.y + 21,
        15, PURPLE
    );

    DrawText( superImpulso2,
        jogador2->posSuperImpulso.x + 5,
        jogador2->posSuperImpulso.y + 21,
        15, PURPLE
    );

}
// daqui até o fim do atualizarBola fizemos em sala de aula
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
    if (bola->pos.x <= 1 + bola->raio) {
        
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

    if (bola->pos.x >= GetScreenWidth() - 1 - bola->raio) {
        
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
        bola->vel.x = -bola->vel.x*1.02; // aqui eu aumentei a velocidade da bola quando ela colide com o jogador, pra deixar o jogo mais dinâmico e fazer as habilidades terem mais impacto
        bola->vel.y = bola->vel.y*1.001;
    }

    if (cj2){
        bola->pos.x = jogador2->pos.x - bola->raio;
        bola->vel.x = -bola->vel.x*1.02; // mesma coisa aqui, só que para quando a bola bate no jogador 2
        bola->vel.y = bola->vel.y*1.001;
    }

}

void atualizarComputador( Jogador *jogador2, float delta, Bola *bola) {
    
    jogador2->cor = GREEN; // cor do computador, só pra diferenciar do jogador 2
    bool bolaVindo = bola->vel.x > 0;
    // esse bool vai ser explicado na função "usarPoderes"
    // se a bola estiver vindo, o computador vai tentar se posicionar na altura da bola com margem

    if (bolaVindo && bola->pos.x < (GetScreenWidth() / 2 + 150) - ((GetScreenWidth() / 2 + 150) * bola->vel.x / 200)) {
        // Se a bola está vindo e ainda está no campo do jogador1,
        // o computador tenta se adiantar para onde a bola provavelmente vai.
        if (bola->vel.y < 0) {
            // bola subindo, adianta para cima
            jogador2->pos.y -= jogador2->vel * delta;
        } else if (bola->vel.y > 0) {
            // bola descendo, adianta para baixo
            jogador2->pos.y += jogador2->vel * delta;
        }
    } else if (bolaVindo) {
        if (bola->pos.y < jogador2->pos.y + jogador2->dim.y / 2) {
            jogador2->pos.y -= jogador2->vel * delta;
        } else if (bola->pos.y > jogador2->pos.y - jogador2->dim.y / 2) {
            jogador2->pos.y += jogador2->vel * delta;
        }
    } else {
        // se a bola não estiver vindo, tenta ficar no meio da tela em y
        float centro = GetScreenHeight() / 2 - jogador2->dim.y / 2;
        if (jogador2->pos.y < centro - 2) {
            jogador2->pos.y += jogador2->vel * delta;
        } else if (jogador2->pos.y > centro + 2) {
            jogador2->pos.y -= jogador2->vel * delta;
        }
    }

    // limita o movimento do computador para não sair da tela
    if (jogador2->pos.y < 0) {
        jogador2->pos.y = 0;
    } else if (jogador2->pos.y + jogador2->dim.y > GetScreenHeight()) {
        jogador2->pos.y = GetScreenHeight() - jogador2->dim.y;
    }

    usarPoderes( jogador2, delta , bola); // aqui o computador vai usar os poderes dele
    
}

void usarPoderes( Jogador *jogador2, float delta, Bola *bola) {
    // ativa o impulso se a bola estiver próxima e indo em direção ao jogador2
    // tive que complicar um pouco mais a lógica do impulso pro computador.
    // todas as situações da bola para o computador saber se deve ou não usar o impulso
    // fui ler outras formas de como fazer algo se tornar verdadeiro ou falso,
    // https://www-w3schools-com.translate.goog/c/c_booleans.php?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc
    // não sabia que funcionava exatamente como um if, mas sem precisar da estrutura
    // usando comparações lógicas, o resultado é um booleano.
    // muito louco!
    // agora que eu sei disso, parece óbvio,
    // nas primeiras aulas você já tinha falado disso,
    // if (1) é verdadeiro, e if (0) é falso,


    bool bolaVindo = bola->vel.x > 0;
    // a lógica das situações depende da velocidade da bola,
    // quanto mais rápido a bola estiver, mais o computador vai tentar antecipar a bola,
    // só ativa quando a bola estiver indo em direção ao computador,
    bool bolaProxima = bola->pos.x > jogador2->pos.x - (bola->vel.x/200 * 220) && bola->pos.x < jogador2->pos.x + 40 && bola->pos.x > GetScreenWidth() - 150;
    // 
    bool bolaAbaixo = bola->pos.y < jogador2->pos.y || bola->pos.y > jogador2->pos.y + jogador2->dim.y;

    // impulso se ativa se a bola está próxima, vindo, e o computador não conseguiria alcançar normalmente
    if (jogador2->energia > 0 && bolaVindo && bolaProxima && bolaAbaixo) {
        jogador2->vel = 500;
        jogador2->energia -= 8;
        if (jogador2->energia < 0) jogador2->energia = 0;
    } else {
        jogador2->vel = 100;
    }

    // recupera energia até 150
    if (jogador2->energia < 150 && jogador2->vel == 100) {
        jogador2->energia += 1;
        if (jogador2->energia > 150) jogador2->energia = 150;
    }

    // carrega super impulso se energia cheia
    if (jogador2->superImpulso < 150 && jogador2->energia == 150) {
        jogador2->superImpulso += 0.1;
        if (jogador2->superImpulso > 150) jogador2->superImpulso = 150;
    }

    // ativa super impulso automaticamente ao chegar em 150
    if (jogador2->superImpulso >= 150 && jogador2->tempoSuperImpulso <= 0) {
        jogador2->pos.y -= 25;
        jogador2->dim.y += 50;
        jogador2->superImpulso = 0;
        jogador2->tempoSuperImpulso = 10 * 60;
    }

    // duração da habilidade especial
    if (jogador2->tempoSuperImpulso > 0) {
        jogador2->tempoSuperImpulso -= 1;
        if (jogador2->tempoSuperImpulso <= 0 && jogador2->dim.y > 75) {
            jogador2->pos.y += 25;
            jogador2->dim.y -= 50;
        }
    }

    // tudo o que eu fiz aqui eu poderia ter feito desde o começo para os dois jogadores,
    // mas já que eles estão funcionando é melhor não mexer kkkkkkk
    // regra não escrita: se não está quebrado, não conserte! kkkkkkkk

}



//Muito obrigado pela aula professor! Foi muito divertido fazer esse jogo, e aprendi muito com ele!
// Espero que o senhor goste do resultado, e que tenha sido divertido de jogar! :D