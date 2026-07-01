#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<allegro5/allegro.h> 
#include<allegro5/allegro_image.h> 
#include<allegro5/allegro_font.h> //fuentes
#include<allegro5/allegro_ttf.h> //fuentes ttf
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h> 
#include<allegro5/allegro_acodec.h> //audio codecs
#define TAM_TILE 40
#define fila 15
#define columna 20
#define MAX_ENEMIGOS 4
#define MAX_LASERS 10

struct arma_
{
    int prejuicio;
    int alcance;
    int energia;
    bool en_uso;
};
typedef struct arma_ arma;

//INCLUIR A CADA STRUCT QUE LO NECEISITE UN ALTO Y ANCHO
struct personaje_
{
    float x;
    float y;
    float velocidad;
    int vida;
    int energia;
    bool ataque;
    arma armas;
};
typedef struct personaje_ personaje;

struct enemigo_
{
    float x;
    float y;
    int tipo;
    int vida;
    int energia;
    int herida;
    int alcance;
    float velocidad;
    bool ataque;
};
typedef struct enemigo_ enemigo;

struct cofre_
{
    float x;
    float y;
    bool abierto;
    int contenido;
};
typedef struct cofre_ cofre;

struct pocion_
{
    float x;
    float y;
    bool usada;
    int tipo; 
};
typedef struct pocion_ pocion;

struct lasers_
{
    float x;
    float y;
    float velocidad;
    bool activo;
};
typedef struct lasers_ lasers;

char mapa[fila][columna];
int total_enemigos=0;
personaje heroe;
personaje amigo;
enemigo enemigos[MAX_ENEMIGOS];
lasers laser_protagonista[MAX_LASERS];
lasers laser_enemigos[MAX_LASERS];
cofre cofrecito;

//AGREGAR MÁS FUNCIONES PARA ORDENAR EL JUEGO(SEPARAR DIBUJO DE LOGICA)
void limites_pantalla(float *x, float *y);
void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial);
void mover_personaje(personaje *p,bool izq, bool der,float *vel_caida);
void mover_enemigos(enemigo ejercito[],int total);
void mover_lasers(lasers mis_lasers[],int maximo);
void mover_lasers_enemigos(lasers lasers_malos[],int maximo);
bool colision(float x,float y);
bool cargar_mapa(const char *nombre_archivo);

int main()
{
    //BORRAR BOOLEANOS Y USAR ALLEGRO_KEYBOARD_STATE PARA DETECTAR TECLAS
    bool corriendo=true;
    bool dibujar=true;
    bool izquierda=false;
    bool derecha=false;
    int i,j;
    int fila_animacion=0;
    float x_bloque,y_bloque;
    float velocidad_caida=0.0;
    float ancho_casilla=922.0/10.0;
    float alto_casilla=505.0/5.0;
    float r_x=1*ancho_casilla; 
    float r_y=fila_animacion*alto_casilla;

    if(!cargar_mapa("nivel1.txt"))
    {
        return -1;
    }

    if(!al_init()) 
    {
        printf("Error al abrir Allegro");
        return -1;
    }

    al_init_primitives_addon();
    al_install_keyboard(); 
    al_init_image_addon();

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_BITMAP *fondo_nivel1=al_load_bitmap("Fondo1.jpg");
    if(!fondo_nivel1)
    {
        printf("Error fatal: No se encontro 'Fondo1.jpg'\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_jugador=al_load_bitmap("prota_caminar.png");
    if(!img_jugador)
    {
        printf("Error fatal: No se encontro 'prota_caminar.png'\n");
        return -1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // 
    al_convert_mask_to_alpha(img_jugador,al_map_rgb(0, 255, 44));
   
    al_start_timer(timer);

    while (corriendo)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue, &evento);

        if(evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            corriendo=false;
        }
        else if(evento.type==ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_LEFT:
                izquierda=true;
                break;
            
            case ALLEGRO_KEY_RIGHT:
                derecha=true;
                break;

            case ALLEGRO_KEY_UP:
                if(colision(heroe.x,heroe.y+1.0))
                {
                    velocidad_caida=-9.0;
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                corriendo=false;
                break;
            }
        }
        else if(evento.type==ALLEGRO_EVENT_KEY_UP)
        {
            switch (evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_LEFT:
                izquierda=false;
                break;
            
            case ALLEGRO_KEY_RIGHT:
                derecha=false;
                break;
            }
        }
        else if(evento.type==ALLEGRO_EVENT_TIMER)
        {
            if(izquierda)
            {
                heroe.x-=4.0;
                if(colision(heroe.x,heroe.y))
                {
                    heroe.x+=4.0;
                }
            }
            if(derecha)
            {
                heroe.x+=4.0;
                if(colision(heroe.x,heroe.y))
                {
                    heroe.x-=4.0;
                }
            }
            velocidad_caida+=0.5;
            heroe.y+=velocidad_caida;
            if(colision(heroe.x,heroe.y))
            {
                if(velocidad_caida>0)
                {
                    heroe.y = ((int)(heroe.y + 30) / TAM_TILE) * TAM_TILE - 30.1;
                }
                else if (velocidad_caida < 0) 
                {
                    heroe.y = ((int)heroe.y / TAM_TILE) * TAM_TILE + TAM_TILE + 0.1;
                }
                velocidad_caida = 0;
            }
            for(i=0;i<total_enemigos;i++)
            {
                if(enemigos[i].velocidad!=0)
                enemigos[i].x+=enemigos[i].velocidad;
                if(colision(enemigos[i].x,enemigos[i].y))
                {
                    enemigos[i].velocidad=-enemigos[i].velocidad;
                    enemigos[i].x+=enemigos[i].velocidad;
                }
            }
            dibujar=true;
        }

        if(dibujar&&al_is_event_queue_empty(queue))
        {
            dibujar=false;
            al_clear_to_color(al_map_rgb(20,30,50));

            al_draw_scaled_bitmap(
                fondo_nivel1,
                0,0,al_get_bitmap_width(fondo_nivel1),al_get_bitmap_height(fondo_nivel1),
                0,0,columna*TAM_TILE,fila*TAM_TILE,
                0
            );

            for(i=0;i<fila;i++)
            {
                for(j=0;j<columna;j++)
                {
                    if(mapa[i][j]=='1')
                    {
                        x_bloque=j*TAM_TILE;
                        y_bloque=i*TAM_TILE; 
                        al_draw_filled_rectangle(x_bloque,y_bloque,x_bloque+TAM_TILE,y_bloque+TAM_TILE,al_map_rgb(100,100,100));
                        al_draw_rectangle(x_bloque,y_bloque,x_bloque+TAM_TILE,y_bloque+TAM_TILE,al_map_rgb(50,50,60),1);
                    }
                }
            }
            if(heroe.vida>0)
            {
                if(izquierda||derecha)
                {
                    fila_animacion=1; 
                }
                al_draw_scaled_bitmap(
                        img_jugador,
                        r_x,r_y,
                        ancho_casilla,alto_casilla,
                        heroe.x,heroe.y,
                        30,30,
                        0
                    );
            }
            for(i=0;i<total_enemigos;i++)
            {
                if(enemigos[i].tipo=='2')
                {
                    al_draw_filled_rectangle(enemigos[i].x,enemigos[i].y,enemigos[i].x+25,enemigos[i].y+25,al_map_rgb(50,200,50));
                }
                if(enemigos[i].tipo=='5')
                {
                    al_draw_filled_rectangle(enemigos[i].x,enemigos[i].y,enemigos[i].x+35,enemigos[i].y+35,al_map_rgb(200,50,200));
                }
                if(enemigos[i].tipo=='9')
                {
                    al_draw_filled_rectangle(enemigos[i].x,enemigos[i].y,enemigos[i].x+50,enemigos[i].y+50,al_map_rgb(50,50,200));
                }
            }

            al_flip_display();
        }
    }
    
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_bitmap(fondo_nivel1);
    al_destroy_bitmap(img_jugador);
    
    return 0;
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

bool colision(float x,float y)
{
    int izq=x/TAM_TILE;
    int der=(x+30)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+30)/TAM_TILE;
    if(izq<0||der>=20||arr<0||aba>=15)
    {
        return true;
    }
    if (mapa[arr][izq]=='1') 
    {
        return true;
    }
    if (mapa[arr][der]=='1') 
    {
        return true;
    }
    if (mapa[aba][izq]=='1') 
    {
        return true;
    }
    if (mapa[aba][der]=='1') 
    {
        return true;
    }
    return false;
}

bool cargar_mapa(const char *nombre_archivo)
{
    int i,j;
    FILE *archivo=fopen(nombre_archivo, "r");
    if(archivo==NULL)
    {
        printf("Error al abrir el archivo");
        return false;
    }
    for (i=0;i<fila;i++)
    {
        for(j=0;j<columna;j++)
        {
            fscanf(archivo," %c",&mapa[i][j]);
            if(mapa[i][j]=='*')
            {
                heroe.x=TAM_TILE*j;
                heroe.y=TAM_TILE*i;  
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='2'||mapa[i][j]=='5'||mapa[i][j]=='9')
            {
                if(total_enemigos<MAX_ENEMIGOS)
                {
                    enemigos[total_enemigos].x=TAM_TILE*j;
                    enemigos[total_enemigos].y=TAM_TILE*i;
                    enemigos[total_enemigos].tipo=mapa[i][j];
                    if(mapa[i][j]=='2')
                    {
                        enemigos[total_enemigos].velocidad=0.5;
                    }
                    if(mapa[i][j]=='5')
                    {
                        enemigos[total_enemigos].velocidad=1.5;
                    }
                    if(mapa[i][j]=='9')
                    {
                        enemigos[total_enemigos].velocidad=0.0;
                    }
                    inicializar_enemigo(&enemigos[total_enemigos],mapa[i][j],TAM_TILE*j,TAM_TILE*i);
                    total_enemigos++;
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='#')
            {
                amigo.x=TAM_TILE*j;
                amigo.y=TAM_TILE*i;
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='c')
            {
                cofrecito.x=TAM_TILE*j;
                cofrecito.y=TAM_TILE*i;
                mapa[i][j]=0;
            }
            
        }
    }
    fclose(archivo);
    return true;
}

void inicializar_personaje(personaje *entidad,int vida_base,float velocidad_base)
{
    (*entidad).velocidad=velocidad_base;
    (*entidad).vida=vida_base;
    (*entidad).energia=100;
    (*entidad).ataque=false;
}

void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial)
{
    (*villano).tipo=tipo_char;
    (*villano).x=x_inicial;
    (*villano).y=y_inicial;
    (*villano).ataque=false;
    if(tipo_char=='2')
    {
        (*villano).vida=50;
        (*villano).velocidad=0.5;
        (*villano).herida=5;
    }
    if(tipo_char=='5')
    {
        (*villano).vida=100;
        (*villano).velocidad=1.5;
        (*villano).herida=15;
    }
    if(tipo_char=='9')
    {
        (*villano).vida=250;
        (*villano).velocidad=3.0;
        (*villano).herida=35;
    }
}

void mover_personaje(personaje *p,bool izq, bool der,float *vel_caida)
{
    if(izq)
    {
        (*p).x-=4.0;
        if(colision((*p).x,(*p).y))
        {
            (*p).x+=4.0;
        }
    }
    if(der)
    {
        (*p).x+=4.0;
        if(colision((*p).x,(*p).y))
        {
            (*p).x-=4.0;
        }
    }
    *vel_caida+=0.5;
    (*p).y=*vel_caida;
    if(colision((*p).x,(*p).y))
    {
        if(*vel_caida>0)
        {
            (*p).y=((int)((*p).y+30)/TAM_TILE)*TAM_TILE-30.1;
        }
        else if (*vel_caida<0) 
        {
            (*p).y=((int)(*p).y/TAM_TILE)*TAM_TILE+TAM_TILE+0.1;
        }
        *vel_caida=0;
    }
}

void mover_enemigos(enemigo ejercito[],int total)
{
    for(int i=0;i<total;i++)
    {
        if(ejercito[i].velocidad!=0)
        {
            ejercito[i].x+=ejercito[i].velocidad;
            if(colision(ejercito[i].x, ejercito[i].y))
            {
                ejercito[i].velocidad=-ejercito[i].velocidad;
                ejercito[i].x+=ejercito[i].velocidad;
            }
        }
    }
}

void mover_lasers(lasers mis_lasers[],int maximo)
{
    int i;
    for(i=0;i<maximo;i++)
    {
        if(mis_lasers[i].activo) 
        {
            mis_lasers[i].x+=mis_lasers[i].velocidad;
            if(colision(mis_lasers[i].x, mis_lasers[i].y)||mis_lasers[i].x>800||mis_lasers[i].x<0)
            {
                mis_lasers[i].activo=false;
            }
        }
    }
}

void mover_lasers_enemigos(lasers lasers_malos[],int maximo)
{
    int i;
    for(i=0;i<maximo;i++)
    {
        if(lasers_malos[i].activo)
        {
            lasers_malos[i].x+=lasers_malos[i].velocidad;
            
            if(colision(lasers_malos[i].x,lasers_malos[i].y)||lasers_malos[i].x>800||lasers_malos[i].x<0)
            {
                lasers_malos[i].activo=false;
            }
        }
    }
}