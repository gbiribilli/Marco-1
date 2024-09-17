#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

int main()
{
    al_init();
    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_DISPLAY* disp = al_create_display(600, 400);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

    // Variáveis para controle
    bool sair = false;
    int tecla = 0;
    int x = 30;
    int y = 270;

    // Carregar bitmaps
    ALLEGRO_BITMAP* imagem = al_load_bitmap("F:/vscode/repos/Marco 1/BG.jpg");
    ALLEGRO_BITMAP* Personagem = al_load_bitmap("F:/vscode/repos/Marco 1/PG.png");

    // Desenhar a tela inicial
    al_draw_scaled_bitmap(imagem, 0, 0, 600, 400, 0, 0, 600, 400, 0);
    al_draw_scaled_bitmap(Personagem, 0, 0, 400, 400, x, y, 100, 100, 0);
    al_flip_display();
    // Registrar fontes de eventos
        al_register_event_source(fila_eventos, al_get_display_event_source(disp));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    al_start_timer(timer); // Começar o temporizador

    // Loop principal
    while (!sair) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        // Tratamento de eventos
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            // Atualizar a posição do personagem
            al_draw_scaled_bitmap(imagem, 0, 0, 600, 400, 0, 0, 600, 400, 0); // Redesenhar fundo
            al_draw_scaled_bitmap(Personagem, 0, 0, 400, 400, x, y, 100, 100, 0); // Desenhar personagem
            al_flip_display();
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Movimentar o personagem com as setas
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                x -= 3;
                break;

            case ALLEGRO_KEY_RIGHT:
                x += 3;
                break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true; // Fechar janela
        }
    }

    // Limpar a memória
    al_destroy_bitmap(Personagem);
    al_destroy_bitmap(imagem);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
