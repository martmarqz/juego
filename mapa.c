#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60.0;
const int TAMAÑO_BLOQUE = 40;

int mapa[15][20];

bool cargar_mapa(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    
    if (archivo == NULL) {
        printf("ERROR: No se pudo encontrar el archivo %s\n", nombre_archivo);
        return false;
    }

    for (int fila = 0; fila < 15; fila++) {
        for (int columna = 0; columna < 20; columna++) {
        
            fscanf(archivo, "%d", &mapa[fila][columna]);
        }
    }

    fclose(archivo); 
    return true;
}

bool hay_colision(float x, float y) {
    int borde_izquierdo = x / TAMAÑO_BLOQUE;
    int borde_derecho = (x + 30) / TAMAÑO_BLOQUE;
    int borde_superior = y / TAMAÑO_BLOQUE;
    int borde_inferior = (y + 30) / TAMAÑO_BLOQUE;

    if (mapa[borde_superior][borde_izquierdo] == 1) return true;
    if (mapa[borde_superior][borde_derecho] == 1) return true;
    if (mapa[borde_inferior][borde_izquierdo] == 1) return true;
    if (mapa[borde_inferior][borde_derecho] == 1) return true;

    return false;
}

int main() 
{
    if (!cargar_mapa("nivel1.txt")) {
        return -1; 
    }

    if (!al_init()) return -1;
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *pantalla = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE *cola_eventos = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));

    bool corriendo = true;
    bool dibujar = true;

    float jugador_x = 80.0;
    float jugador_y = 80.0; 
    float velocidad_y = 0.0; 
    float gravedad = 0.5;      
    float fuerza_salto = -9.0; 
    
    bool tecla_izquierda = false;
    bool tecla_derecha = false;

    al_start_timer(timer);

    while (corriendo) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(cola_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            corriendo = false;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT: tecla_izquierda = true; break;
                case ALLEGRO_KEY_RIGHT: tecla_derecha = true; break;
                case ALLEGRO_KEY_UP: 
                    if (hay_colision(jugador_x, jugador_y + 1)) {
                        velocidad_y = fuerza_salto; 
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE: corriendo = false; break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT: tecla_izquierda = false; break;
                case ALLEGRO_KEY_RIGHT: tecla_derecha = false; break;
            }
        }

        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            if (tecla_izquierda) {
                jugador_x -= 4.0;
                if (hay_colision(jugador_x, jugador_y)) jugador_x += 4.0;
            }
            if (tecla_derecha) {
                jugador_x += 4.0;
                if (hay_colision(jugador_x, jugador_y)) jugador_x -= 4.0;
            }

            velocidad_y += gravedad; 
            jugador_y += velocidad_y;

            if (hay_colision(jugador_x, jugador_y)) {
                if (velocidad_y > 0) {
                    jugador_y = ((int)(jugador_y + 30) / TAMAÑO_BLOQUE) * TAMAÑO_BLOQUE - 30.1;
                }
                else if (velocidad_y < 0) {
                    jugador_y = ((int)jugador_y / TAMAÑO_BLOQUE) * TAMAÑO_BLOQUE + TAMAÑO_BLOQUE + 0.1;
                }
                velocidad_y = 0; 
            }

            dibujar = true;
        }

        if (dibujar && al_is_event_queue_empty(cola_eventos)) {
            dibujar = false;
            al_clear_to_color(al_map_rgb(20, 30, 50)); 

           
            for (int fila = 0; fila < 15; fila++) {
                for (int columna = 0; columna < 20; columna++) {
                    if (mapa[fila][columna] == 1) {
                        float x_bloque = columna * TAMAÑO_BLOQUE;
                        float y_bloque = fila * TAMAÑO_BLOQUE;
                        
                        al_draw_filled_rectangle(x_bloque, y_bloque, x_bloque + TAMAÑO_BLOQUE, y_bloque + TAMAÑO_BLOQUE, al_map_rgb(100, 100, 110));
                        al_draw_rectangle(x_bloque, y_bloque, x_bloque + TAMAÑO_BLOQUE, y_bloque + TAMAÑO_BLOQUE, al_map_rgb(50, 50, 60), 1);
                    }
                }
            }

            // DIBUJAR AL JUGADOR
            al_draw_filled_rectangle(jugador_x, jugador_y, jugador_x + 30, jugador_y + 30, al_map_rgb(255, 60, 60));

            al_flip_display();
        }
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(cola_eventos);
    al_destroy_display(pantalla);


    return 0;
    
}