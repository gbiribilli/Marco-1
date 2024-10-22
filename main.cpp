#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/keyboard.h>
#include <time.h>

// variaveis globais
const int linhas = 20;
const int colunas = 20;
int board[linhas][colunas];
bool sair = false;
bool sairBomber = true;
int inicialX = 1;
int inicialY = 1;
ALLEGRO_DISPLAY* disp;

#define FRAME_WIDTH 44   
#define FRAME_HEIGHT 48  
#define NUM_FRAMES 4     
#define FRAME_DURATION 0.2 
#define JUMP_HEIGHT 100  
#define GRAVITY 4        
void comecaBoard() {
    srand(time(NULL));

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int n1 = rand() % 10;
            if (n1 % 2 == 0) {
                board[i][j] = 1; // Bloco sólido
            }
            else {
                board[i][j] = 0; // Espaço vazio
            }
        }
    }

    for (int i = 0; i <= linhas; i += 2) {
        for (int j = 0; j <= colunas; j += 2) {
            if (board[i][j] == 0) {
                board[i][j] = 2;
            }
        }
    }

    board[inicialY][inicialX] = 3; //Posição do jogador

    board[linhas - 1][colunas - 1] = 4; //Objetivo


}
void drawBoard(ALLEGRO_DISPLAY* display) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {     
            switch (board[i][j]) {
            case 0: // Espaço em branco
                al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(255, 255, 255));
                break;
            case 1: // Bl solido
                al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(0, 0, 0));
                break;
            case 2: // Bl destruidor
                al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(200, 0, 0));
                break;
            case 3: // Jogador
                al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(0, 0, 255));
                break;
            case 4: // Objetivo
                al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(0, 255, 255));
                break;
            }
        }
    }
}

// Função para mover o jogador
int movJogador(int dx, int dy) {
    int novoX = inicialX + dx;
    int novoY = inicialY + dy;


    if (novoX >= 0 && novoX < colunas && novoY >= 0 && novoY < linhas &&
        (board[novoY][novoX] == 0 || board[novoY][novoX] == 2 || board[novoY][novoX] == 4)) {

        board[inicialY][inicialX] = 0;
        inicialX = novoX;
        inicialY = novoY;
        board[inicialY][inicialX] = 3;
    }

    if (board[novoX][novoY] == board[linhas - 1][colunas - 1]) {
        printf("Voce conseguiu");
        return 2;
    }


}
int Bomberman(int x) {
    srand(time(NULL));
    //ALLEGRO_DISPLAY* disp = al_create_display(1000, 900);
    ALLEGRO_EVENT_QUEUE* fila_de_eventos = al_create_event_queue();
    al_register_event_source(fila_de_eventos, al_get_display_event_source(disp));
    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
    int cont;

    //transformar tudo em Disp (displaY)
    while (!sairBomber) {
        al_clear_to_color(al_map_rgb(0,0,0));
        drawBoard(disp);
        al_flip_display();
        
        ALLEGRO_EVENT event;
        al_wait_for_event(fila_de_eventos, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_RIGHT:
                movJogador(1, 0);
                break;
            case ALLEGRO_KEY_LEFT:
                movJogador(-1, 0);
                break;
            case ALLEGRO_KEY_UP:
                movJogador(0, -1);
                break;
            case ALLEGRO_KEY_DOWN:
                movJogador(0, 1);
                break;
            }

        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_SPACE:
                comecaBoard();
            }

        }

        int numero = movJogador(0, 0);
        if (numero == 2) {
            return 2;
            al_destroy_display(disp);
        }
    }
}
int main() {
    al_init();
    al_install_mouse();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    int dispX = 1000;
    int dispY = 900;
    comecaBoard();
    ALLEGRO_DISPLAY* bomber;
    disp = al_create_display(1000, 900);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

    bool sair = false;
    bool on_homepage = true;
    float x = 30;
    float y = 270;
    float frame = 0;

    bool pulando = false;
    float jump_speed = 0;
    float jump_start_y = y;

    bool andando_esquerda = false;
    bool andando_direita = false;
    bool parado = true;


    ALLEGRO_BITMAP* imagem = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/BG.jpg");
    ALLEGRO_BITMAP* Personagem = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/Walking.png");
    ALLEGRO_BITMAP* Pulando = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/Pulando.png");
    ALLEGRO_BITMAP* Parado = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/PGParado.png");
    ALLEGRO_BITMAP* AndandoESQ = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/WalkingLeft.png");
    ALLEGRO_BITMAP* Homepage = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/bg2.jpg");

    al_register_event_source(fila_eventos, al_get_display_event_source(disp));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_start_timer(timer);
    int cot = 0;
    while (!sair) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            // Atualize a tela e desenhe o personagem aqui

            if (on_homepage) {
                al_draw_scaled_bitmap(Homepage, 0, 0, al_get_bitmap_width(Homepage), al_get_bitmap_height(Homepage), 0, 0, dispX, dispY, 0);
            }
            else {
                // Atualize o movimento do personagem
                if (andando_esquerda) {
                    x -= 3;
                    parado = false;
                }
                if (andando_direita) {
                    x += 3;
                    parado = false;
                }

                if (!andando_esquerda && !andando_direita && !pulando) {
                    parado = true;
                }

                if (pulando) {
                    y -= jump_speed;
                    jump_speed -= GRAVITY * 0.1;

                    if (y >= jump_start_y) {
                        y = jump_start_y;
                        pulando = false;
                        jump_speed = 0;
                    }
                }

                al_draw_scaled_bitmap(imagem, 0, 0, al_get_bitmap_width(imagem), al_get_bitmap_height(imagem), 0, 0, dispX, dispY, 0);

                frame += 0.3;
                if ((int)frame >= NUM_FRAMES) {
                    frame = 0;
                }

                if (pulando) {
                    al_draw_scaled_bitmap(Pulando, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }
                else if (parado) {
                    al_draw_scaled_bitmap(Parado, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }
                else if (andando_esquerda) {
                    al_draw_scaled_bitmap(AndandoESQ, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }
                else if (andando_direita) {
                    al_draw_scaled_bitmap(Personagem, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                }
            }

            al_flip_display();
        }

        // Detecção de teclado e mouse
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andando_esquerda = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                andando_direita = true;
                break;
            case ALLEGRO_KEY_UP:
                if (!pulando) {
                    pulando = true;
                    jump_speed = 10;
                    jump_start_y = y;
                }
                break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andando_esquerda = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                andando_direita = false;
                break;
            }
        }

        // Eventos de mouse
        else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (evento.mouse.x >= dispX / 2 - 100 / 2 &&
                evento.mouse.x <= dispX / 2 + 50 / 2 &&
                evento.mouse.y >= dispY / 1.3 - 105 / 2 &&
                evento.mouse.y <= dispY / 1.3 + 55 / 3) {
                printf("#");
            }
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (on_homepage &&
                evento.mouse.x >= dispX / 2 - 100 / 2 &&
                evento.mouse.x <= dispX / 2 + 50 / 2 &&
                evento.mouse.y >= dispY / 1.3 - 105 / 2 &&
                evento.mouse.y <= dispY / 1.3 + 55 / 2) {
                printf("Botao clicado");
                on_homepage = false;
            }
        }

        

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                cot++;
                printf("%d", cot);
                if (cot >= 5) {
                    sairBomber = false;
                    while (!sairBomber) {
                        Bomberman(1);
                        int volta = Bomberman(1);
                        if (volta == 2) {
                            sairBomber = true;
                            cot = 0;
                        }

                    }
                }
            }
            
        }

        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
        }
    }



    al_destroy_bitmap(Personagem);
    al_destroy_bitmap(imagem);
    al_destroy_bitmap(Homepage);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
