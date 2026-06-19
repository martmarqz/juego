#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<allegro5/allegro.h> //allegro
#include<allegro5/allegro_image.h> //imagenes
#include<allegro5/allegro_font.h> //fuentes
#include<allegro5/allegro_ttf.h> //fuentes ttf
#include<allegro5/allegro_primitives.h> //Dibujo 
#include<allegro5/allegro_audio.h> //audio
#include<allegro5/allegro_acodec.h> //audio codecs

struct arma_
{
    int daño;
    int alcance;
    int energia;
    bool en_uso;
}arma_principal, arma_especial;
typedef struct arma_ arma;

struct personaje_
{
    int x;
    int y;
    int vida;
    int energia;
    bool ataque;
    arma armas;
}lester, alien_guardia, alien_jefe, alien_amigo, alien_enemigo;
typedef struct personaje_ personaje;



void limites_pantalla(float *x, float *y);
void gravedad(float *y, float *velocidad_caida);
bool impacto(float x1, float y1, float x2, float y2);

int main()
{
    if(!al_init()) 
    {
        printf("Error al abrir Allegro");
        return -1;
    }
    al_init_primitives_addon();
    al_install_keyboard(); //teclado
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

}

void limites_pantalla(float *x, float *y)
{
    if(*x < 0)
    {
        *x = 0;
    }
    if(*x > 800)
    {
        *x = 800;
    }
    if(*y < 0)
    {
        *y = 0;
    }
    if(*y > 600)
    {
        *y = 600;
    }
}

void gravedad(float *y, float *velocidad_caida)
{
    *y+=*velocidad_caida;
    if(*y>500)
    {
        *y=500;
        *velocidad_caida=0;
    }
}

bool impacto(float x1, float y1, float x2, float y2)
{
    if(x1<x2+50 && x1+50>x2 && y1<y2+50 && y1+50>y2)
    {
        return true;
    }
    return false;
}