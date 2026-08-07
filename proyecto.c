#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<allegro5/allegro.h> 
#include<allegro5/allegro_image.h> 
#include<allegro5/allegro_font.h> 
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h> 
#include<allegro5/allegro_acodec.h> 
#define TAM_TILE 40
#define fila 27
#define columna 48
#define alto_pantalla 1080
#define ancho_pantalla 1920
#define MAX_ENEMIGOS 10
#define MAX_LASERS 5
#define ANCHO_SPRITE 128.0
#define ALTO_SPRITE 128.0
#define TAM_DIBUJO 80.0
#define AJUSTE_X ((TAM_DIBUJO-40.0)/2.0)
#define AJUSTE_Y (TAM_DIBUJO-40.0)
#define MAX_ITEMS 150
#define MAX_COSAS 10
#define MAX_TRAMPAS 100
#define MAX_BOMBARDEO 10
#define MUNICION_LASER 1
#define MUNICION_VENENO 2
#define MUNICION_BOMBA 3

enum Estado_Personaje
{
    ESTADO_IDLE=0, 
    ESTADO_CAMINAR,
    ESTADO_SALTO,
    ESTADO_DISPARO,
    ESTADO_ATAQUE,
    ESTADO_DANO,
    ESTADO_CORRER,
    ESTADO_RECARGA,
    ESTADO_MUERTE
};

enum Tipo_Item
{
    ITEM_VIDA=1,
    ITEM_MUNICION,
    ITEM_ENERGIA,
    ITEM_LLAVE_FINAL,
    ITEM_LLAVE_ROJA,
    ITEM_LLAVE_AZUL,
    ITEM_LLAVE_AMARILLA,
    ITEM_LLAVE_VERDE,
    ITEM_VENENO,
    ITEM_BOMBA,
    ITEM_PUNTOS,
    ITEM_ESCUDO,
    ITEM_LINTERNA,
    ITEM_MINIMAPA
};

struct municion_
{
    int tipo;
    float x;
    float y;
    float velocidad_x;
    float velocidad_y;
    float distancia_recorrida;
    bool activo;
};
typedef struct municion_ municion;

struct personaje_
{
    float x;
    float y;
    float velocidad;
    float spawn_x;
    float spawn_y;
    float y_azote;
    int vida;
    int escudo;
    int energia;
    int estado;
    int tiempo_dano;
    int cuadro_actual;
    int contador_animacion;
    int municion;
    int inventario[MAX_COSAS];
    int puntos;
    int cantidad_balas;
    int duracion_linterna;
    bool tiene_mapita;
    bool ataque;
    municion proyectiles[MAX_ITEMS];
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
    int estado;
    int cuadro_actual;
    int contador_animacion; 
    int municion;
    float velocidad;
    float direccion_mirada;
    float velocidad_y;
    bool ataque;
    municion laser_enemigos[MAX_LASERS];
};
typedef struct enemigo_ enemigo;

struct trampas_
{
    int tipo;
    int tiempo;
    float x;
    float y;
    bool activado;
};
typedef struct trampas_ trampas;

struct item_
{
    float x;
    float y;
    float velocidad_y;
    enum Tipo_Item tipo;
    int valor; 
    bool activo;
    
};
typedef struct item_ item;

struct recursos_
{
    ALLEGRO_BITMAP *fondo_menu;
    ALLEGRO_BITMAP *fondo_nivel1;
    ALLEGRO_BITMAP *img_ataque;
    ALLEGRO_BITMAP *img_dano;
    ALLEGRO_BITMAP *img_caminar;
    ALLEGRO_BITMAP *img_correr;
    ALLEGRO_BITMAP *img_disparo;
    ALLEGRO_BITMAP *img_muerte;
    ALLEGRO_BITMAP *img_recarga;
    ALLEGRO_BITMAP *img_salto;
    ALLEGRO_BITMAP *img_idle;
    ALLEGRO_BITMAP *img_basico;
    ALLEGRO_BITMAP *textura_luna;
    ALLEGRO_BITMAP *pocion_vida;
    ALLEGRO_BITMAP *alien5;
    ALLEGRO_BITMAP *veneno;
    ALLEGRO_BITMAP *llave_final;
    ALLEGRO_BITMAP *llave_roja;
    ALLEGRO_BITMAP *portal;
    ALLEGRO_BITMAP *punto;
    ALLEGRO_BITMAP *puas;
    ALLEGRO_BITMAP *municion;
    ALLEGRO_BITMAP *palanca;
    ALLEGRO_BITMAP *llave_verde;
    ALLEGRO_BITMAP *llave_azul;
    ALLEGRO_BITMAP *llave_amarilla;
    ALLEGRO_BITMAP *door;
    ALLEGRO_BITMAP *intenemy;
    ALLEGRO_BITMAP *boss;
    ALLEGRO_BITMAP *checkpoint;
    ALLEGRO_BITMAP *bombita;
    ALLEGRO_BITMAP *img_quema;
    ALLEGRO_BITMAP *linternita;
    ALLEGRO_BITMAP *soy_mapa;
    ALLEGRO_BITMAP *proteccion;
    ALLEGRO_BITMAP *bomba_enemiga;
    ALLEGRO_BITMAP *big_map;
    ALLEGRO_BITMAP *img_final;
    ALLEGRO_SAMPLE *disparo_prota;
    ALLEGRO_SAMPLE *disparo_alien5;
    ALLEGRO_SAMPLE *disparo_alien6;
    ALLEGRO_SAMPLE *sonido_portal;
    ALLEGRO_SAMPLE *sonido_municion;
    ALLEGRO_SAMPLE *sonido_moneda;
    ALLEGRO_SAMPLE *sonido_llaves;
    ALLEGRO_SAMPLE *sonido_proyectil;
    ALLEGRO_SAMPLE *sonido_equipar;
    ALLEGRO_SAMPLE *sonido_saltar;
    ALLEGRO_SAMPLE *sonido_vida;
    ALLEGRO_SAMPLE *sonido_puerta;
    ALLEGRO_SAMPLE *sonido_pasos;
    ALLEGRO_SAMPLE *sonido_palanca;
    ALLEGRO_SAMPLE *pantalla_carga;
};
typedef struct recursos_ recursos;

struct estado_juego_
{
    trampas mis_trampas[MAX_TRAMPAS];
    item mis_items[MAX_ITEMS];
    enemigo enemigos[MAX_ENEMIGOS];
    municion bombas_entorno[MAX_BOMBARDEO];
    int total_enemigos;
    int luz_apagada;
    int secuencia_llaves[10];
    int total_llaves_secuencia;
    int indice_llave_actual;
    int cuadro_portal;
    int contador_portal;
    int total_venenos_bombas;
    int recolectados_venenos_bombas;
    int temporizador_suelo;
    float x_portal;
    float y_portal;
    float x_amigo;
    float y_amigo;
    bool amigo_activo;
    bool juego_terminado;
    bool portal_activo;
    bool suelo_destruido;
};
typedef struct estado_juego_ estado_juego;

struct contexto_dibujo_
{
    int espejo;
    int cuadro_moneda;
    int cuadro_bomba;
    int cuadro_portal;
    int nivel_actual;
    float r_x;
    float movimiento_items;
    bool portal_abierto;
    bool mostrar_mapa;
    recursos *catalogo;
    estado_juego *nivel;
    ALLEGRO_BITMAP *img_heroe; 
    ALLEGRO_FONT *fuente;
};
typedef struct contexto_dibujo_ contexto_dibujo;

struct datos_ranking_
{
    char nombre[20];
    int puntaje;
    int nivel;
};
typedef struct datos_ranking_ datos_ranking;

char mapa[fila][columna];
personaje heroe;

int mostrar_menu(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_titulo,ALLEGRO_FONT *f_opciones,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs);
void inicializar_personaje(personaje *entidad,int vida_base,float velocidad_base);
void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial);
void mover_personaje(personaje *p,bool izq, bool der,float *vel_caida,estado_juego *nivel);
void mover_enemigos(enemigo ejercito[],int total,recursos *sonidos,estado_juego *nivel);
void mover_jefe(enemigo *boss,recursos *sonidos,estado_juego *nivel);
void mover_municion_enemigos(municion balas[], int maximo);
void dibujar_juego(contexto_dibujo *graficos);
void recolectar_items(item lista_items[],int maximo,recursos *sonidos, estado_juego *nivel);
void limpieza(recursos *imgs,ALLEGRO_FONT *fuente,ALLEGRO_TIMER *timer,ALLEGRO_EVENT_QUEUE *queue,ALLEGRO_DISPLAY *display);
void animacion_entorno(int *cont_moneda,int *cuad_moneda,float *mov_items,float *vel_flote,int *cont_bomba,int *cuadro_bombita);
void actualizar_estado_heroe(personaje *h,bool izq,bool der,float *vel_caida,int max_cuadros,recursos *imgs);
void mover_municion_heroe(municion balas[],int maximo,estado_juego *nivel);
void trampitas(personaje *h,float *vel_caida,estado_juego *nivel);
void portalito(personaje *h,bool *portal_abierto,bool *cambiar_nivel,recursos *imgs,estado_juego *nivel);
void cambios_de_niveles(int *nivel_actual,bool *corriendo,bool *portal_abierto,float *vel_caida,estado_juego *nivel,ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_titulo,ALLEGRO_FONT *f_opciones,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs);
void disparar_proyectil(personaje *h,int tipo_municion,float direccion_mirada,recursos *imgs);
void pedir_nombre(char *nombre,ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_nombre,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs);
void mostrar_pantalla_carga(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_titulo,ALLEGRO_FONT *f_opciones,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs);
void ranking(char *nombre,int puntaje,int nivel,ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_ranking,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs,bool modo_guardar);
void mostrar_informacion(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_texto,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs);
void the_final(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *fuente_titulo,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs);
bool cargar_mapa(const char *nombre_archivo,estado_juego *nivel);
bool colision(float x,float y);
bool colision_lasers(float x,float y);
bool colision_jefe(float x,float y);
bool cargar_recursos(recursos *imgs);
bool colision_bombas_enemigas(float x,float y);

int main()
{
    bool programa_completo=true;
    bool corriendo,menu,tiene_mapa,tecla_izq,tecla_der;
    bool dibujar=true;
    bool portal_abierto=false;
    bool cambiar_nivel=false;
    bool llamar_mapa=false;
    int i,j,k,casilla_x,casilla_y,fil,col,seleccion;
    int voltear=0;
    int max_cuadros_heroe=5;
    int cuadro_animacion_moneda=0;
    int contador_animacion_moneda=0;
    int nivel_actual=1;
    int cuadro_animacion_bombita=0;  
    int contador_animacion_bomba=0;
    float x_bloque,y_bloque;
    float velocidad_caida=0.0;
    float r_x=0.0;
    float direccion_mirada=1.0;
    float movimiento_y_items=0.0;
    float velocidad_flotacion=0.25;
    char nombre_jugador[20];
    estado_juego mi_nivel;
    recursos mis_imagenes;
    mi_nivel.total_enemigos=0;
    mi_nivel.luz_apagada=0;
    mi_nivel.portal_activo=false;
    mi_nivel.suelo_destruido=false;
    mi_nivel.temporizador_suelo=0;
    mi_nivel.amigo_activo=false;
    mi_nivel.juego_terminado=false;
    
    srand(time(NULL));

    for(i=0;i<MAX_TRAMPAS;i++) 
    {
        mi_nivel.mis_trampas[i].activado=false; 
    }
    for(i=0;i<MAX_ITEMS;i++) 
    {
        mi_nivel.mis_items[i].activo=false;
    }
    for(i=0;i<MAX_BOMBARDEO;i++) 
    {
        mi_nivel.bombas_entorno[i].activo=false;
    }

    inicializar_personaje(&heroe,100,4.0);

    if(!cargar_mapa("nivel1.txt",&mi_nivel))
    {
        return -1;
    }

    al_init();
    al_init_primitives_addon();
    al_install_keyboard(); 
    al_install_mouse();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);
    al_init_font_addon();
    al_init_ttf_addon();
    
    if(!cargar_recursos(&mis_imagenes)) 
    {
        return -1; 
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_MAXIMIZED | ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *display = al_create_display(ancho_pantalla,alto_pantalla);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_FONT *fuente_texto=al_load_ttf_font("mifuente.ttf",24,0); 
    if(!fuente_texto) 
    {
        printf("No se encontro mifuente.ttf\n");
        fuente_texto = al_create_builtin_font();
    }
    ALLEGRO_FONT *fuente_titulo=al_load_ttf_font("airstrikechrome.ttf",60,0);
    ALLEGRO_FONT *fuente_opciones=al_load_ttf_font("Conthrax-SemiBold.otf",30,0);
    ALLEGRO_FONT *fuente_ranking=al_load_ttf_font("Ethnocentric-Regular.otf",24,0);
    ALLEGRO_FONT *fuente_nombre=al_load_ttf_font("Cynatar.otf",35,0);

    al_register_event_source(queue,al_get_keyboard_event_source());
    al_register_event_source(queue,al_get_mouse_event_source());
    al_register_event_source(queue,al_get_display_event_source(display));
    al_register_event_source(queue,al_get_timer_event_source(timer));
   
    while (programa_completo)
    {
        menu=true;
        corriendo=false;
        while(menu)
        {
            seleccion=mostrar_menu(display,fuente_titulo,fuente_opciones,queue,&mis_imagenes);
        
            if(seleccion==0)
            {
                pedir_nombre(nombre_jugador,display,fuente_nombre,queue,&mis_imagenes);
                corriendo=true;
                menu=false;
            }
            if(seleccion==1)
            {
                mostrar_informacion(display,fuente_opciones,queue,&mis_imagenes);
            }
            if(seleccion==2)
            {
                ranking("",0,0,display,fuente_ranking,queue,&mis_imagenes,false);
            }
            if(seleccion==3)
            {
                corriendo=false;
                menu=false;
                programa_completo=false;
            }
        }       
        if(corriendo)
        {
            al_start_timer(timer);
        }
        while (corriendo)
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(queue, &evento);

            if(evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                corriendo=false;
                programa_completo=false;
            }

            else if(evento.type==ALLEGRO_EVENT_KEY_DOWN)
            {
                switch (evento.keyboard.keycode)
                {
                    case ALLEGRO_KEY_UP:
                    {
                        if(colision(heroe.x,heroe.y+1.0))
                        {   
                        velocidad_caida=-9.0;
                        }
                        al_play_sample(mis_imagenes.sonido_saltar,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;
                    }

                    case ALLEGRO_KEY_SPACE:
                    {
                        disparar_proyectil(&heroe,MUNICION_LASER,direccion_mirada,&mis_imagenes);
                        break;
                    }
            
                    case ALLEGRO_KEY_V:
                    {
                        disparar_proyectil(&heroe,MUNICION_VENENO,direccion_mirada,&mis_imagenes);
                        break;
                    }

                    case ALLEGRO_KEY_B:
                    {
                        disparar_proyectil(&heroe,MUNICION_BOMBA,direccion_mirada,&mis_imagenes);
                        break;
                    }

                    case ALLEGRO_KEY_M:
                    {
                        if(heroe.tiene_mapita) 
                        {
                            llamar_mapa=!llamar_mapa;
                        } 
                        else 
                        {
                            printf("Aún no has encontrado al mapa!\n");
                        }
                        break;
                    }

                    case ALLEGRO_KEY_ESCAPE:
                    {
                        corriendo=false;
                        break;
                    }
                }
            }
            else if(evento.type==ALLEGRO_EVENT_KEY_UP)
            {
                switch (evento.keyboard.keycode)
                {
                    case ALLEGRO_KEY_SPACE:
                    {
                        heroe.ataque=false;
                        break;
                    }
                }
            }
            else if(evento.type==ALLEGRO_EVENT_TIMER)
            {
                ALLEGRO_KEYBOARD_STATE estado_teclado;
                al_get_keyboard_state(&estado_teclado);
                tecla_izq=al_key_down(&estado_teclado, ALLEGRO_KEY_LEFT);
                tecla_der=al_key_down(&estado_teclado,ALLEGRO_KEY_RIGHT);

                if(tecla_der)
                {
                    direccion_mirada=1.0;
                }
                if(tecla_izq)
                {
                    direccion_mirada=-1.0;
                }
                if(heroe.vida>0)
                {
                    mover_personaje(&heroe,tecla_izq,tecla_der,&velocidad_caida,&mi_nivel);
                }

                actualizar_estado_heroe(&heroe,tecla_izq,tecla_der,&velocidad_caida,max_cuadros_heroe,&mis_imagenes);
                r_x=heroe.cuadro_actual*ANCHO_SPRITE;
                mover_enemigos(mi_nivel.enemigos,mi_nivel.total_enemigos,&mis_imagenes,&mi_nivel);
                mover_municion_heroe(heroe.proyectiles,MAX_ITEMS,&mi_nivel);

                if(mi_nivel.suelo_destruido)
                {
                    mi_nivel.temporizador_suelo--;
                    if(mi_nivel.temporizador_suelo<=0)
                    {
                        mi_nivel.suelo_destruido=false; 
                    }
                }

                if(heroe.y>alto_pantalla)
                {
                    heroe.vida=0;
                }
            
                for(i=0;i<mi_nivel.total_enemigos;i++)
                {
                mover_municion_enemigos(mi_nivel.enemigos[i].laser_enemigos,MAX_LASERS);
                }

                recolectar_items(mi_nivel.mis_items,MAX_ITEMS,&mis_imagenes,&mi_nivel);
                trampitas(&heroe,&velocidad_caida,&mi_nivel);
                portalito(&heroe,&portal_abierto,&cambiar_nivel,&mis_imagenes,&mi_nivel);
                animacion_entorno(&contador_animacion_moneda,&cuadro_animacion_moneda,&movimiento_y_items,&velocidad_flotacion,&contador_animacion_bomba,&cuadro_animacion_bombita);

                casilla_x=(heroe.x+20)/TAM_TILE;
                casilla_y=(heroe.y+20)/TAM_TILE;

                if(casilla_y>=0&&casilla_y<fila&&casilla_x>=0&&casilla_x<columna)
                {
                    if(mapa[casilla_y][casilla_x]=='C')
                    {
                        heroe.spawn_x=TAM_TILE*casilla_x;
                        heroe.spawn_y=TAM_TILE*casilla_y;
                        mapa[casilla_y][casilla_x]='c';
                        printf("Progreso guardado\n");
                    }

                    if(mapa[casilla_y][casilla_x]=='L')
                    {
                        mapa[casilla_y][casilla_x]='l'; 
                        al_play_sample(mis_imagenes.sonido_palanca,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    
                        for(i=casilla_y-3;i<=casilla_y+3;i++)
                        {
                            for(j=casilla_x-3;j<=casilla_x+3;j++)
                            {
                                if(i>=0&&i<fila&&j>=0&&j<columna) 
                                {
                                    if(mapa[i][j]=='D') 
                                    {
                                        mapa[i][j]='A';
                                    }
                                }
                            }
                        }
                        al_play_sample(mis_imagenes.sonido_puerta,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        printf("Palanca activada! Se abrió la puerta.\n");
                    }
                }

                if(cambiar_nivel) 
                {
                    cambios_de_niveles(&nivel_actual,&corriendo,&portal_abierto,&velocidad_caida,&mi_nivel,display,fuente_titulo,fuente_opciones,queue,&mis_imagenes);
                    cambiar_nivel=false;
                }

                if(mi_nivel.juego_terminado) 
                {
                    printf("Felicidades! Has escapado.\n FIN DEL JUEGO.\n");
                    heroe.puntos+=1000; 
                    the_final(display,fuente_titulo,queue,&mis_imagenes);
                    corriendo=false; 
                }

                dibujar=true;
            }

            if(dibujar&&al_is_event_queue_empty(queue))
            {
                dibujar=false;

                ALLEGRO_BITMAP *img_actual=NULL;

                if(heroe.estado==ESTADO_IDLE) 
                {
                    img_actual=mis_imagenes.img_idle;
                }
                else if(heroe.estado==ESTADO_CAMINAR) 
                {
                    img_actual=mis_imagenes.img_caminar;
                }
                else if(heroe.estado==ESTADO_SALTO) 
                {
                    img_actual=mis_imagenes.img_salto;
                }
                else if(heroe.estado==ESTADO_DISPARO) 
                {
                    img_actual=mis_imagenes.img_disparo;
                }
                else if(heroe.estado==ESTADO_ATAQUE) 
                {
                    img_actual=mis_imagenes.img_ataque;
                }
                else if(heroe.estado==ESTADO_DANO) 
                {
                    img_actual=mis_imagenes.img_dano;
                }
                else if(heroe.estado==ESTADO_CORRER) 
                {
                    img_actual=mis_imagenes.img_correr;
                }
                else if(heroe.estado==ESTADO_RECARGA) 
                {
                    img_actual=mis_imagenes.img_recarga;
                }
                else if(heroe.estado==ESTADO_MUERTE)
                {
                    img_actual=mis_imagenes.img_muerte;
                }

                if(direccion_mirada==-1.0)
                {
                    voltear=ALLEGRO_FLIP_HORIZONTAL;
                }
                else
                {
                    voltear=0;
                }

                contexto_dibujo mis_graficos;
                mis_graficos.catalogo=&mis_imagenes;
                mis_graficos.img_heroe=img_actual;
                mis_graficos.r_x=r_x;
                mis_graficos.espejo=voltear;
                mis_graficos.cuadro_moneda=cuadro_animacion_moneda; 
                mis_graficos.cuadro_portal=mi_nivel.cuadro_portal;
                mis_graficos.portal_abierto=portal_abierto;
                mis_graficos.mostrar_mapa=llamar_mapa;
                mis_graficos.cuadro_moneda=cuadro_animacion_moneda; 
                mis_graficos.movimiento_items=movimiento_y_items;
                mis_graficos.cuadro_bomba=cuadro_animacion_bombita;
                mis_graficos.nivel = &mi_nivel;
                mis_graficos.fuente=fuente_texto;
                mis_graficos.nivel_actual=nivel_actual;
            
                dibujar_juego(&mis_graficos);
            }
        }
        al_stop_timer(timer);
        if(programa_completo&&heroe.puntos>0&&strlen(nombre_jugador)>0)
        {
            ranking(nombre_jugador,heroe.puntos,nivel_actual,display,fuente_ranking,queue,&mis_imagenes,true);
            heroe.puntos=0;
        }
    }

    al_destroy_font(fuente_titulo);
    al_destroy_font(fuente_opciones);
    al_destroy_font(fuente_ranking);
    al_destroy_font(fuente_nombre);
    limpieza(&mis_imagenes,fuente_texto,timer,queue,display);

    return 0;
}

bool colision(float x,float y)
{
    int izq=x/TAM_TILE;
    int der=(x+39)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+39)/TAM_TILE;

    if(x<0||x+39>=ancho_pantalla)
    {
        return true;
    }
    
    if(izq<0||der>=columna||arr<0||aba>=fila)
    {
        return true;
    }

    if (mapa[arr][izq]=='1'||mapa[arr][izq]=='D'||mapa[arr][izq]=='3'||mapa[arr][izq]=='$') 
    {
        return true;
    }

    if (mapa[arr][der]=='1'||mapa[arr][der]=='D'||mapa[arr][der]=='3'||mapa[arr][der]=='$') 
    {
        return true;
    }

    if (mapa[aba][izq]=='1'||mapa[aba][izq]=='D'||mapa[aba][izq]=='3'||mapa[aba][izq]=='$') 
    {
        return true;
    }

    if (mapa[aba][der]=='1'||mapa[aba][der]=='D'||mapa[aba][der]=='3'||mapa[aba][der]=='$') 
    {
        return true;
    }

    return false;
}

bool cargar_mapa(const char *nombre_archivo, estado_juego *nivel)
{
    int i,j,k;
    char estado_luz;
    char texto_secuencia[20];
    (*nivel).total_venenos_bombas=0;
    (*nivel).total_venenos_bombas=0;

    FILE *archivo=fopen(nombre_archivo, "r");

    if(archivo==NULL)
    {
        printf("Error al abrir el archivo");
        return false;
    }

    fscanf(archivo," %c",&estado_luz);
    if (estado_luz=='1') 
    {
        (*nivel).luz_apagada=1;
    } 
    else 
    {
        (*nivel).luz_apagada = 0;
    }

    fscanf(archivo," %s",texto_secuencia);
    (*nivel).total_llaves_secuencia=strlen(texto_secuencia);
    (*nivel).indice_llave_actual=0;
    
    for(i=0;i<(*nivel).total_llaves_secuencia;i++) 
    {
        if(texto_secuencia[i]=='k') 
        {
            (*nivel).secuencia_llaves[i]=ITEM_LLAVE_ROJA;
        }
        else if(texto_secuencia[i]=='g') 
        {
            (*nivel).secuencia_llaves[i]=ITEM_LLAVE_VERDE;
        }
        else if(texto_secuencia[i]=='y') 
        {
            (*nivel).secuencia_llaves[i]=ITEM_LLAVE_AMARILLA;
        }
        else if(texto_secuencia[i]=='b') 
        {
            (*nivel).secuencia_llaves[i]=ITEM_LLAVE_AZUL;
        }
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
                heroe.spawn_x=heroe.x;
                heroe.spawn_y=heroe.y;
                mapa[i][j]=0;
            }

            else if(mapa[i][j]=='2'||mapa[i][j]=='5'||mapa[i][j]=='6'||mapa[i][j]=='9')
            {
                if((*nivel).total_enemigos<MAX_ENEMIGOS)
                {
                    inicializar_enemigo(&(*nivel).enemigos[(*nivel).total_enemigos],mapa[i][j],TAM_TILE*j,TAM_TILE*i);
                    (*nivel).total_enemigos++;
                }
                mapa[i][j]=0;
            }

            else if(mapa[i][j]=='#')
            {
                (*nivel).x_amigo=TAM_TILE*j;
                (*nivel).y_amigo=TAM_TILE*i;
                (*nivel).amigo_activo=true;
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='v')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_VENENO;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=20;
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='k'||mapa[i][j]=='g'||mapa[i][j]=='y'||mapa[i][j]=='b')
            {
                for(k=0; k<MAX_ITEMS; k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=1;
                    
                        if(mapa[i][j]=='k') 
                        {
                            (*nivel).mis_items[k].tipo=ITEM_LLAVE_ROJA;
                        }
                        if(mapa[i][j]=='g') 
                        {
                            (*nivel).mis_items[k].tipo=ITEM_LLAVE_VERDE;
                        }
                        if(mapa[i][j]=='y') 
                        {
                            (*nivel).mis_items[k].tipo=ITEM_LLAVE_AMARILLA;
                        }
                        if(mapa[i][j]=='b') 
                        {
                            (*nivel).mis_items[k].tipo=ITEM_LLAVE_AZUL;
                        }
                        break;
                    }
                }
                mapa[i][j] = 0;
            }
            else if(mapa[i][j]=='p'||mapa[i][j]=='q')
            {
                for(k=0;k<MAX_TRAMPAS;k++)
                {
                    if(!(*nivel).mis_trampas[k].activado)
                    {
                        (*nivel).mis_trampas[k].activado=true;
                        (*nivel).mis_trampas[k].x=TAM_TILE*j;
                        (*nivel).mis_trampas[k].y=TAM_TILE*i;
                        (*nivel).mis_trampas[k].tipo=mapa[i][j];
                        (*nivel).mis_trampas[k].tiempo=0;
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='O') 
            {
                (*nivel).portal_activo = true;
                (*nivel).x_portal = TAM_TILE * j;
                (*nivel).y_portal = TAM_TILE * i;
                mapa[i][j] = 0; 
            }
            else if(mapa[i][j]=='.') 
            {
                for(k=0; k<MAX_ITEMS; k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_PUNTOS;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=10; 
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='m')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_MUNICION;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=5; 
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='h')
            {
                for(k=0; k<MAX_ITEMS; k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_VIDA;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=25; 
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='x')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_BOMBA;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=50; 
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='?')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_MINIMAPA;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=1; 
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='+')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_ESCUDO;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=10; 
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='%')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!(*nivel).mis_items[k].activo)
                    {
                        (*nivel).mis_items[k].activo=true;
                        (*nivel).mis_items[k].tipo=ITEM_LINTERNA;
                        (*nivel).mis_items[k].x=TAM_TILE*j+5;
                        (*nivel).mis_items[k].y=TAM_TILE*i+10;
                        (*nivel).mis_items[k].velocidad_y=0;
                        (*nivel).mis_items[k].valor=3600;
                        break;
                    }
                }
                mapa[i][j]=0;
            }
        }
    }

    fclose(archivo);
    return true;
}

void inicializar_personaje(personaje *entidad,int vida_base,float velocidad_base)
{
    int i;

    (*entidad).velocidad=velocidad_base;
    (*entidad).vida=vida_base;
    (*entidad).energia=100;
    (*entidad).ataque=false;
    (*entidad).tiempo_dano=0;
    (*entidad).escudo=0;
    (*entidad).contador_animacion=0;
    (*entidad).cuadro_actual=0;
    (*entidad).puntos=0;
    (*entidad).municion=10;
    (*entidad).duracion_linterna=0;
    (*entidad).tiene_mapita=false;
    (*entidad).y_azote=0.0;
    for(i=0;i<MAX_COSAS;i++)
    {
        (*entidad).inventario[i]=0;
    }
    for(i=0;i<MAX_ITEMS;i++)
    {
        (*entidad).proyectiles[i].activo=false;
    }
}

void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial)
{
    int i;

    for(i=0;i<MAX_LASERS;i++)
    {
        (*villano).laser_enemigos[i].activo=false;
    }
    (*villano).tipo=tipo_char;
    (*villano).x=x_inicial;
    (*villano).y=y_inicial;
    (*villano).velocidad_y=0.0;
    (*villano).ataque=false;
    (*villano).estado=ESTADO_CAMINAR;
    (*villano).cuadro_actual=0;
    (*villano).contador_animacion=0;
    (*villano).direccion_mirada=1.0;

    if(tipo_char=='2')
    {
        (*villano).vida=50;
        (*villano).velocidad=0.5;
        (*villano).herida=5;
        (*villano).alcance=50;
    }

    if(tipo_char=='5')
    {
        (*villano).vida=100;
        (*villano).velocidad=1.0;
        (*villano).herida=15;
        (*villano).alcance=400;
        (*villano).energia=0;
    }

    if(tipo_char=='6')
    {
        (*villano).vida=150;
        (*villano).velocidad=2.5;
        (*villano).herida=20;
        (*villano).alcance=400;
        (*villano).energia=0;
    }

    if(tipo_char=='9')
    {
        (*villano).vida=2000;
        (*villano).velocidad=2.0;
        (*villano).herida=40;
        (*villano).alcance=600;
        (*villano).energia=0;
    }
}

void mover_personaje(personaje *p,bool izq,bool der,float *vel_caida,estado_juego *nivel)
{
    float trayecto;
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

    if(*vel_caida<=0)
    {
        (*p).y_azote=(*p).y;
    }

    *vel_caida+=0.5;
    (*p).y+=*vel_caida;

    if(colision((*p).x,(*p).y))
    {
        if(*vel_caida>0)
        {
            if(!(*nivel).suelo_destruido)
            {
                (*p).y=((int)((*p).y+39)/TAM_TILE)*TAM_TILE-39.1;
                
                trayecto=(*p).y-(*p).y_azote;
                
                if(trayecto>=(8*TAM_TILE))
                {
                    if((*p).escudo>0)
                    {
                        (*p).escudo--;
                        (*p).tiempo_dano=30;
                        printf("Caída fuerte! Tu escudo te salvó de romperte las piernas.\n");
                    }
                    else
                    {
                        (*p).vida-=20;
                        (*p).tiempo_dano=30;
                        printf("Daño por caída de %d bloques.\n",(int)(trayecto/TAM_TILE));
                    }
                }

                *vel_caida=0;
            }
        }
        else if (*vel_caida<0) 
        {
            (*p).y=((int)(*p).y/TAM_TILE)*TAM_TILE+TAM_TILE+0.1;
            *vel_caida=0;
        }
    }
}

void mover_enemigos(enemigo ejercito[],int total,recursos *sonidos,estado_juego *nivel)
{
    int max_cuadros=8;
    int limite_cuadros=8;
    int i,j,frente_x,pies_y,check_x,check_y,enfriamiento,probabilidad_salto; 
    float dist_x_abs,dist_y_abs,dist_x,dist_y,velocidad_boss;
    bool puede_moverse,enfurecido,heroe_en_rango,es_puerta,choca_pared;

    for(i=0;i<total;i++)
    {
        if(ejercito[i].vida<=0) 
        {
            ejercito[i].estado=ESTADO_MUERTE;
            ejercito[i].velocidad=0; 
            ejercito[i].ataque=false;
        }
        else
        {
            dist_x=heroe.x-ejercito[i].x;
            dist_y=heroe.y-ejercito[i].y;

            if (dist_x>0)
            {
                dist_x_abs=dist_x;
            }
            else
            {
                dist_x_abs=-dist_x;
            }

            if (dist_y>0)
            {
                dist_y_abs=dist_y;
            }
            else
            {
                dist_y_abs=-dist_y;
            }

            heroe_en_rango=(heroe.vida>0&&dist_x_abs<=ejercito[i].alcance&&dist_y_abs<40.0);

            if(ejercito[i].tipo=='2')
            {
                if(heroe_en_rango)
                {
                    ejercito[i].ataque=true;
                    if(dist_x>0) 
                    { 
                        ejercito[i].direccion_mirada=1.0; 
                    }
                    else 
                    { 
                        ejercito[i].direccion_mirada=-1.0; 
                    }

                    if(dist_x_abs>1.5) 
                    {
                        if(dist_x > 0) 
                        { 
                            ejercito[i].velocidad=1.0; 
                        }
                        else 
                        { 
                            ejercito[i].velocidad=-1.0; 
                        }
                    }
                    else
                    {
                        ejercito[i].velocidad=0.0; 
                    }
                }
                else
                {
                    ejercito[i].ataque=false;
                    if(ejercito[i].velocidad==0.0) 
                    {
                        ejercito[i].velocidad=1.0; 
                    }
                }

                if (ejercito[i].velocidad>0) 
                {
                    check_x=(ejercito[i].x+45.0)/TAM_TILE;
                } 
                else 
                {
                    check_x=(ejercito[i].x-5.0)/TAM_TILE;
                }
                check_y=(ejercito[i].y+20.0)/TAM_TILE;
                es_puerta=false;

                if(check_y>=0&&check_y<fila&&check_x>=0&&check_x<columna)
                {
                    if(mapa[check_y][check_x]=='D'||mapa[check_y][check_x]=='A')
                    {
                        es_puerta=true;
                    }
                }

                if (es_puerta)
                {
                    ejercito[i].velocidad=-ejercito[i].velocidad;
                }
                else if(colision(ejercito[i].x+(ejercito[i].velocidad*5.0),ejercito[i].y)&&ejercito[i].velocidad_y==0.0&&!es_puerta)
                {
                    ejercito[i].velocidad_y=-9.0;
                }
            }
            
            else if(ejercito[i].tipo=='5')
            {
                if(heroe_en_rango)
                {
                    ejercito[i].ataque=true;
                    if (dist_x>0)
                    {
                        ejercito[i].direccion_mirada=1.0;
                    }
                    else
                    {
                        ejercito[i].direccion_mirada=-1.0;
                    }
            
                    if(ejercito[i].energia<=0)
                    {
                        ejercito[i].ataque=true;
                    }
                }
                else
                {
                    if(ejercito[i].energia>0)
                    {
                        ejercito[i].ataque=false;
                    }
                }
                
                if(ejercito[i].ataque) 
                {
                    if(ejercito[i].cuadro_actual==3&&ejercito[i].contador_animacion==0)
                    {
                        for(j= 0;j<MAX_LASERS;j++)
                        {
                            if(!ejercito[i].laser_enemigos[j].activo)
                            {
                                ejercito[i].laser_enemigos[j].activo=true;
                                ejercito[i].laser_enemigos[j].tipo=MUNICION_LASER;
                                ejercito[i].laser_enemigos[j].distancia_recorrida=0.0;
                                ejercito[i].laser_enemigos[j].x=ejercito[i].x+20;
                                ejercito[i].laser_enemigos[j].y=ejercito[i].y+15;
                                ejercito[i].laser_enemigos[j].velocidad_x=10.0*ejercito[i].direccion_mirada;
                                ejercito[i].laser_enemigos[j].velocidad_y=0.0;
                                al_play_sample((*sonidos).disparo_alien5,0.8,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                                break;
                            }
                        }
                        ejercito[i].energia=90;
                        ejercito[i].ataque=false; 
                    }
                }
                else
                {   
                    if(ejercito[i].energia>0) 
                    {
                        ejercito[i].energia--;
                    }
                }

                if(ejercito[i].velocidad>0) 
                {
                    frente_x=(ejercito[i].x+45)/TAM_TILE; 
                }
                else 
                {
                    frente_x=(ejercito[i].x-5)/TAM_TILE; 
                }
                
                pies_y=(ejercito[i].y+45)/TAM_TILE; 
                
                if(pies_y<fila&&frente_x>=0&&frente_x<columna)
                {
                    if(mapa[pies_y][frente_x]!='1'&&mapa[pies_y][frente_x]!='3'&&mapa[pies_y][frente_x]!='$')
                    {
                        ejercito[i].velocidad=-ejercito[i].velocidad;
                    }
                }
            }
            else if(ejercito[i].tipo=='6')
            {
                if(dist_x>0) 
                {
                    ejercito[i].direccion_mirada=1.0;
                }
                else 
                {
                    ejercito[i].direccion_mirada=-1.0;
                }

                if(heroe.vida>0&&dist_y_abs<100.0) 
                {
                    if(dist_x_abs<150.0)
                    {
                        ejercito[i].ataque=false;
                        if(dist_x>0) 
                        {
                            ejercito[i].velocidad=-3.0; 
                        }
                        else 
                        {
                            ejercito[i].velocidad=3.0;  
                        }          
                    }
                    else if(dist_x_abs>300.0&&dist_x_abs<ejercito[i].alcance)
                    {
                        ejercito[i].ataque=false;
                        if(dist_x>0) 
                        {
                            ejercito[i].velocidad=2.5; 
                        }
                        else 
                        {
                            ejercito[i].velocidad=-2.5;
                        }
                    }
                    else if(dist_x_abs>=150.0&&dist_x_abs<=300.0)
                    {
                        ejercito[i].velocidad=0;
                        ejercito[i].ataque=true;

                        if(ejercito[i].energia<=0)
                        {
                            for(j=0;j<MAX_LASERS;j++)
                            {
                                if(!ejercito[i].laser_enemigos[j].activo)
                                {
                                    ejercito[i].laser_enemigos[j].activo=true;
                                    ejercito[i].laser_enemigos[j].tipo=MUNICION_LASER;
                                    ejercito[i].laser_enemigos[j].distancia_recorrida=0.0;
                                    if(ejercito[i].direccion_mirada==1.0) 
                                    {
                                        ejercito[i].laser_enemigos[j].x=ejercito[i].x+45; 
                                    } 
                                    else 
                                    {
                                        ejercito[i].laser_enemigos[j].x=ejercito[i].x-5;  
                                    }
                                    ejercito[i].laser_enemigos[j].y = ejercito[i].y+16;
                                    ejercito[i].laser_enemigos[j].velocidad_x=15.0*ejercito[i].direccion_mirada; 
                                    ejercito[i].laser_enemigos[j].velocidad_y=0.0;
                                    al_play_sample((*sonidos).disparo_alien6,0.8,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                                    break;
                                }
                            }
                            ejercito[i].energia=30;
                        }
                    }
                }
                else
                {
                    ejercito[i].ataque=false;
                    if(ejercito[i].velocidad==0) 
                    {
                        ejercito[i].velocidad=1.5;
                    }
                }

                if(ejercito[i].energia>0) 
                {
                    ejercito[i].energia--;
                }

                if(ejercito[i].velocidad>0)
                {
                    frente_x=(ejercito[i].x+45)/TAM_TILE;
                }
                else
                {
                    frente_x=(ejercito[i].x-5)/TAM_TILE;
                }
                pies_y=(ejercito[i].y+45)/TAM_TILE;

                if(pies_y<fila&&frente_x>=0&&frente_x<columna)
                {
                    if((mapa[pies_y][frente_x]!='1'&&mapa[pies_y][frente_x]!='3'&&mapa[pies_y][frente_x]!='$')||colision(ejercito[i].x+ejercito[i].velocidad,ejercito[i].y))
                    {
                        ejercito[i].velocidad=-ejercito[i].velocidad;
                    }
                }
            }
            else if(ejercito[i].tipo=='9')
            {
                mover_jefe(&ejercito[i],sonidos,nivel);
            }

            if(ejercito[i].ataque) 
            {
                ejercito[i].estado=ESTADO_ATAQUE;
            } 
            else if(ejercito[i].velocidad!=0) 
            {
                ejercito[i].estado=ESTADO_CAMINAR;
                if(ejercito[i].velocidad>0) 
                {
                    ejercito[i].direccion_mirada=1.0;
                }
                if(ejercito[i].velocidad<0) 
                {
                    ejercito[i].direccion_mirada=-1.0;
                }
            } 
            else 
            {
                ejercito[i].estado=ESTADO_IDLE;
            }

            puede_moverse=true;
            if(ejercito[i].tipo=='5') 
            {
                if(ejercito[i].ataque) 
                {
                    puede_moverse=false; 
                }
                else if(ejercito[i].energia>0) 
                {
                    puede_moverse=false;
                    ejercito[i].cuadro_actual=0; 
                    ejercito[i].contador_animacion=0;
                }
            }

            if(ejercito[i].ataque)
            {
                ejercito[i].estado=ESTADO_ATAQUE;
            }
            else if(ejercito[i].velocidad!=0&&puede_moverse)
            {
                ejercito[i].estado=ESTADO_CAMINAR;
                if(ejercito[i].velocidad>0) 
                {
                    ejercito[i].direccion_mirada=1.0;
                }
                if(ejercito[i].velocidad<0) 
                {
                    ejercito[i].direccion_mirada=-1.0;
                }

                ejercito[i].x+=ejercito[i].velocidad;

                choca_pared = false;
                if(ejercito[i].tipo=='9') 
                {
                    choca_pared=colision_jefe(ejercito[i].x,ejercito[i].y);
                } 
                else 
                {
                    choca_pared=colision(ejercito[i].x,ejercito[i].y);
                }

                if(choca_pared)
                {
                    ejercito[i].x-=ejercito[i].velocidad;

                    if(ejercito[i].tipo!='9') 
                    {
                        ejercito[i].velocidad=-ejercito[i].velocidad; 
                    }
                }
            }
            else
            {
                ejercito[i].estado=ESTADO_IDLE;
            }

            if(ejercito[i].tipo=='5')
            {
                if(ejercito[i].estado==ESTADO_IDLE) 
                {
                    limite_cuadros=6;
                }
                else if(ejercito[i].estado==ESTADO_ATAQUE) 
                {
                    limite_cuadros=6;
                }
                else if(ejercito[i].estado==ESTADO_CAMINAR) 
                {
                    limite_cuadros=6;
                }
                else if(ejercito[i].estado==ESTADO_MUERTE) 
                {
                    limite_cuadros=5;
                }
            }

            if(ejercito[i].cuadro_actual>=limite_cuadros)
            {
                ejercito[i].cuadro_actual=0;
            }

            if(ejercito[i].tipo=='2')
            {
                ejercito[i].velocidad_y+=0.5; 
                ejercito[i].y+=ejercito[i].velocidad_y;

                if(colision(ejercito[i].x,ejercito[i].y))
                {
                    if(ejercito[i].velocidad_y>0)
                    {
                        ejercito[i].y=((int)(ejercito[i].y+39)/TAM_TILE)*TAM_TILE-39.1;
                        ejercito[i].velocidad_y=0.0;
                    }
                    else if(ejercito[i].velocidad_y<0) 
                    {
                        ejercito[i].y=((int)ejercito[i].y/TAM_TILE)*TAM_TILE+TAM_TILE+0.1;
                        ejercito[i].velocidad_y=0.0;
                    }
                }
            }
            else if(ejercito[i].tipo!='9') 
            {
                ejercito[i].y+=4.0;
                if(colision(ejercito[i].x,ejercito[i].y))
                {
                    ejercito[i].y=((int)(ejercito[i].y+39)/TAM_TILE)*TAM_TILE-39.1;
                }
            }

            if(ejercito[i].estado==ESTADO_ATAQUE&&ejercito[i].cuadro_actual==3&&ejercito[i].contador_animacion==0)
            {
                if(heroe.tiempo_dano==0&&(ejercito[i].tipo=='2'))
                {
                    heroe.vida-=ejercito[i].herida;
                    heroe.tiempo_dano=30;
                }
            }
        }

        ejercito[i].contador_animacion++;

        if(ejercito[i].contador_animacion>=6) 
        {
            ejercito[i].contador_animacion=0;
            ejercito[i].cuadro_actual++;
            
            if(ejercito[i].estado==ESTADO_IDLE) 
            {
                max_cuadros=3;
            }
            else if(ejercito[i].estado==ESTADO_MUERTE) 
            {
                max_cuadros=5;
            }
            else if(ejercito[i].estado==ESTADO_ATAQUE)
            {
                max_cuadros=4;
            }
            else if(ejercito[i].estado==ESTADO_CAMINAR)
            {
                max_cuadros=6;
            }

            if(ejercito[i].tipo=='5'&&ejercito[i].estado==ESTADO_IDLE)
            {
                max_cuadros=3;
            }
            
            if(ejercito[i].cuadro_actual>=max_cuadros)
            {
                if(ejercito[i].estado==ESTADO_MUERTE) 
                {
                    ejercito[i].cuadro_actual=max_cuadros-1; 
                } 
                else 
                {
                    ejercito[i].cuadro_actual=0;
                }
            }
        }
    }    
}

void mover_municion_heroe(municion balas[],int maximo,estado_juego *nivel)
{
    enemigo *enemigos=(*nivel).enemigos;
    item *mis_items=(*nivel).mis_items;
    int total_enemigos=(*nivel).total_enemigos;
    int i,j,k,dano;
    float hit_x_min;
    float hit_x_max;
    float hit_y_min;
    float hit_y_max;
    float cabeza_y_max; 
    bool es_jefe;

    for(i=0;i<maximo;i++)
    {
        if(balas[i].activo)
        {
            balas[i].x+=balas[i].velocidad_x;
            balas[i].y+=balas[i].velocidad_y;
            
            if(balas[i].tipo==MUNICION_VENENO||balas[i].tipo==MUNICION_BOMBA) 
            {
                balas[i].velocidad_y+=0.5;
            }
            
            if(balas[i].velocidad_x>0) 
            {
                balas[i].distancia_recorrida+=balas[i].velocidad_x;
            }
            else 
            {
                balas[i].distancia_recorrida-=balas[i].velocidad_x;
            }

            if(balas[i].tipo==MUNICION_LASER&&balas[i].distancia_recorrida>=350.0)
            {
                balas[i].activo=false;
            }

            if(colision_lasers(balas[i].x,balas[i].y)||balas[i].x>ancho_pantalla||balas[i].x<0)
            {
                balas[i].activo=false;
            }

            if(balas[i].activo)
            {
                for(j=0;j<total_enemigos;j++)
                {
                    if(enemigos[j].vida>0)
                    {
                        hit_x_min=enemigos[j].x;
                        hit_x_max=enemigos[j].x+40.0;
                        hit_y_min=enemigos[j].y;
                        hit_y_max=enemigos[j].y+40.0;
                        cabeza_y_max = hit_y_min;
                        es_jefe=(enemigos[j].tipo=='9');

                        if (es_jefe)
                        {
                            hit_x_min=enemigos[j].x-80.0; 
                            hit_x_max=enemigos[j].x+120.0; 
                            hit_y_min=enemigos[j].y-180.0; 
                            hit_y_max=enemigos[j].y+40.0;  
                            cabeza_y_max=hit_y_min+80.0; 
                        }
                        if(balas[i].x<hit_x_max&&balas[i].x+15>hit_x_min&&balas[i].y<hit_y_max&&balas[i].y+15>hit_y_min)
                        {
                            if(balas[i].tipo==MUNICION_LASER)
                            {
                                dano=25;
                            }
                            else
                            {
                                dano=40;
                            }
                            
                            if (es_jefe)
                            {
                                if (balas[i].y<cabeza_y_max)
                                {
                                    dano*=3;
                                    printf("GOLPE CRÍTICO! Daño en la cabeza: %d\n",dano);
                                }
                                else
                                {
                                    printf("Golpe al cuerpo. Daño: %d\n",dano);
                                }
                            }
                            
                            enemigos[j].vida-=dano;
                            balas[i].activo=false;
                            
                            if(enemigos[j].vida<=0&&enemigos[j].tipo=='2')
                            {
                                heroe.puntos+=25;
                            }
                            else if(enemigos[j].vida<=0&&enemigos[j].tipo=='5')
                            {
                                heroe.puntos+=50;
                                for(k=0;k<MAX_ITEMS;k++)
                                {
                                    if(!mis_items[k].activo)
                                    {
                                        mis_items[k].activo=true; 
                                        mis_items[k].tipo=ITEM_VIDA;
                                        mis_items[k].x=enemigos[j].x+10; 
                                        mis_items[k].y=enemigos[j].y;
                                        mis_items[k].velocidad_y=-8.0; 
                                        mis_items[k].valor=25;
                                        break;
                                    }
                                }
                            }
                            else if(enemigos[j].vida<=0&&enemigos[j].tipo=='6')
                            {
                                heroe.puntos+=100;
                            }
                            else if(enemigos[j].vida<=0&&enemigos[j].tipo=='9')
                            {
                                heroe.puntos += 500;
                                for(k=0; k<MAX_ITEMS; k++)
                                {
                                    if(!mis_items[k].activo)
                                    {
                                        mis_items[k].activo=true; 
                                        mis_items[k].tipo=ITEM_LLAVE_FINAL;
                                        mis_items[k].x=enemigos[j].x+40; 
                                        mis_items[k].y=enemigos[j].y;
                                        mis_items[k].velocidad_y=-8.0; 
                                        mis_items[k].valor=0;
                                        printf("El jefe solto la llave para liberar a tu amiguito!\n");
                                        break;
                                    }
                                }
                            }
                            
                            break;
                        }
                    }
                }
            }
        }
    }
}

void mover_municion_enemigos(municion balas[],int maximo)
{
    int i;
    for(i=0;i<maximo;i++)
    {
        if(balas[i].activo)
        {
            balas[i].x+=balas[i].velocidad_x;
            balas[i].y+=balas[i].velocidad_y;

            if(balas[i].velocidad_x>0) 
            {
                balas[i].distancia_recorrida+=balas[i].velocidad_x;
            }
            else 
            {
                balas[i].distancia_recorrida-=balas[i].velocidad_x;
            }

            if(balas[i].tipo==MUNICION_LASER&&balas[i].distancia_recorrida>=350.0) 
            {
                balas[i].activo=false;
            }

            if(colision_lasers(balas[i].x,balas[i].y)||balas[i].x>ancho_pantalla||balas[i].x<0) 
            {
                balas[i].activo=false;
            }

            if(balas[i].activo&&heroe.vida>0)
            {
                if(balas[i].x<heroe.x+40&&balas[i].x+15>heroe.x&&balas[i].y<heroe.y+40&&balas[i].y+15>heroe.y)
                {
                    if(heroe.tiempo_dano==0)
                    {
                        if(heroe.escudo>0)
                        {
                            heroe.escudo--; 
                            heroe.tiempo_dano=15;
                            printf("¡Escudo golpeado! Resistencia: %d\n",heroe.escudo);
                        }
                        else
                        {
                            heroe.vida-=15;
                            heroe.tiempo_dano=30;
                        }
                    }
                    balas[i].activo=false;
                }
            }
        }
    }
}

void dibujar_juego(contexto_dibujo *graficos)
{
    estado_juego *n=(*graficos).nivel;
    int i,j,k,recorte_x,recorte_y,recorte_portal_x,recorte_moneda_x,ancho_frame,alto_frame,ancho_puerta,alto_puerta,fila_y_alien6;
    int salto_y,salto_x,fila_y,ancho_tile,alto_tile,fila_y_alien5,m_y,m_x,max_cuadros_boss,imagen;
    int total_enemigos=(*n).total_enemigos;
    int luz_apagada=(*n).luz_apagada;
    int espejo_enemigo=0;
    float x_bloque,y_bloque,ajuste_y_alien5,ajuste_piso,ancho_dibujo_alien,alto_dibujo_alien,ajuste_x_centro,ajuste_centro,y_flotante;
    float centro_x,centro_y,radio_vision,grosor_oscuridad,mini_tile,pos_mapa_x,pos_mapa_y,rect_x,rect_y,mini_enemigo_x,mini_enemigo_y;
    float mini_portal_x,mini_portal_y,mini_heroe_x,mini_heroe_y,x_actual,ancho_llave,espaciado,total_ancho,pos_x_llaves,pos_y_llaves;
    float mini_item_x,mini_item_y,temblor_x,temblor_y,ancho_mapa_rial,alto_mapa_rial,inicio_x_mapa,inicio_y_mapa,margen_x,margen_y;
    float ancho_dibujo=60.0;
    float alto_dibujo=85.0;
    float ajuste_x=(ancho_dibujo-40.0)/2.0; 
    float ajuste_y=alto_dibujo-40.0;
    float ajuste_alien_x=(64.0-40.0)/2.0;
    float ajuste_alien_y=10.0;
    float x_portal=(*n).x_portal;
    float y_portal=(*n).y_portal;
    char nombre_archivo[30],texto_secuencia[20],estado_luz,caracter_leido;
    bool portal_activo=(*n).portal_activo;
    enemigo *enemigos=(*n).enemigos;
    item *mis_items=(*n).mis_items;
    trampas *mis_trampas=(*n).mis_trampas;
    recursos *imgs=(*graficos).catalogo;
    
    al_clear_to_color(al_map_rgb(20,30,50));

    ALLEGRO_TRANSFORM camara;
    al_identity_transform(&camara);

    if((*n).suelo_destruido)
    {
        temblor_x=(rand()%15)-7.0; 
        temblor_y=(rand()%15)-7.0;
        al_translate_transform(&camara,temblor_x,temblor_y);
    }
    al_use_transform(&camara);

    if((*imgs).fondo_nivel1!=NULL)
    {
        al_draw_scaled_bitmap((*imgs).fondo_nivel1,0,0,al_get_bitmap_width((*imgs).fondo_nivel1),al_get_bitmap_height((*imgs).fondo_nivel1),0,0,columna*TAM_TILE,fila*TAM_TILE,0);
    }

    for(i=0;i<fila;i++)
    {
        for(j=0;j<columna;j++)
        {
            x_bloque=j*TAM_TILE;
            y_bloque=i*TAM_TILE; 

            if(mapa[i][j]=='1')
            {
                if((*imgs).textura_luna!=NULL)
                {
                    recorte_x=96;
                    recorte_y=0;
                    al_draw_scaled_bitmap((*imgs).textura_luna,recorte_x,recorte_y,48,48,x_bloque,y_bloque,TAM_TILE,TAM_TILE,0);
                }
            }
            else if(mapa[i][j]=='3')
            {
                if((*imgs).textura_luna!=NULL) 
                {
                    recorte_x=0;
                    recorte_y=80;
                    al_draw_scaled_bitmap((*imgs).textura_luna,recorte_x,recorte_y,48,48,x_bloque,y_bloque,TAM_TILE,TAM_TILE,0);
                }
            }
            else if(mapa[i][j]=='$')
            {
                if((*imgs).textura_luna!=NULL) 
                {
                    recorte_x=0;
                    recorte_y=160;
                    al_draw_scaled_bitmap((*imgs).textura_luna,recorte_x,recorte_y,48,48,x_bloque,y_bloque,TAM_TILE,TAM_TILE,0);
                }
            }
            else if((mapa[i][j]=='D'||mapa[i][j]=='A')&&(*imgs).door!=NULL)
            {
                alto_puerta=48;
                ancho_puerta=48;
                recorte_x=0;
                recorte_y=0;
                if(mapa[i][j]=='A')
                {
                    recorte_x=144;
                }
                al_draw_scaled_bitmap((*imgs).door,recorte_x,recorte_y,ancho_puerta,alto_puerta,x_bloque,y_bloque,TAM_TILE,TAM_TILE*2,0);
            }
            else if(mapa[i][j]=='L'&&(*imgs).palanca!=NULL) 
            {
                ancho_frame=al_get_bitmap_width((*imgs).palanca)/5;
                alto_frame=al_get_bitmap_height((*imgs).palanca);
                
                al_draw_scaled_bitmap((*imgs).palanca,0,0,ancho_frame,alto_frame,x_bloque-10,y_bloque-20,60,60,0);
            }
            else if(mapa[i][j]=='l'&&(*imgs).palanca!=NULL) 
            {
                ancho_frame=al_get_bitmap_width((*imgs).palanca)/5;
                alto_frame=al_get_bitmap_height((*imgs).palanca);
                recorte_x=4*ancho_frame; 
                al_draw_scaled_bitmap((*imgs).palanca,recorte_x,0,ancho_frame,alto_frame,x_bloque-10,y_bloque-20,60,60,0);
            }
            else if((mapa[i][j]=='C'||mapa[i][j]=='c')&&(*imgs).checkpoint!=NULL)
            {
                ancho_frame=16;
                alto_frame=48;
                recorte_y=0;
                
                if(mapa[i][j]=='C')
                {
                    recorte_x=0;
                }
                else
                {
                    recorte_x=8*ancho_frame;
                }
                
                al_draw_scaled_bitmap((*imgs).checkpoint,recorte_x,recorte_y,ancho_frame,alto_frame,x_bloque-10,y_bloque-5,60,60,0);
            }
        }
    }

    for(i=0;i<MAX_TRAMPAS;i++)
    {
        if(mis_trampas[i].activado)
        {
            if(mis_trampas[i].tipo =='p'&&(*imgs).puas!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).puas,0,0,al_get_bitmap_width((*imgs).puas),al_get_bitmap_height((*imgs).puas),mis_trampas[i].x,mis_trampas[i].y,TAM_TILE,TAM_TILE,0);
            }
            else if(mis_trampas[i].tipo=='q'&&(*imgs).img_quema!=NULL)
            {
                ancho_frame=16;
                alto_frame=64;
                
                imagen=mis_trampas[i].tiempo/14;
                if(imagen>8)
                {
                    imagen=8;
                }
                
                recorte_x=imagen*ancho_frame;
                al_draw_scaled_bitmap((*imgs).img_quema,recorte_x,0,ancho_frame,alto_frame,mis_trampas[i].x,mis_trampas[i].y,TAM_TILE,160,0);
            }
        }
    }

    if((*graficos).img_heroe!=NULL)
    {
        al_draw_scaled_bitmap((*graficos).img_heroe,(*graficos).r_x,0,ANCHO_SPRITE,ALTO_SPRITE,heroe.x-AJUSTE_X,heroe.y-AJUSTE_Y,TAM_DIBUJO,TAM_DIBUJO,(*graficos).espejo);
        if(heroe.escudo>0&&(*imgs).proteccion!=NULL)
        {
            al_draw_tinted_scaled_bitmap((*imgs).proteccion,al_map_rgba_f(1.0,1.0,1.0,0.7),0,0,al_get_bitmap_width((*imgs).proteccion),al_get_bitmap_height((*imgs).proteccion),heroe.x-AJUSTE_X,heroe.y-AJUSTE_Y,TAM_DIBUJO,TAM_DIBUJO,0);
        }
    }

    for(i=0;i<total_enemigos;i++)
    {
        if(enemigos[i].tipo=='2') 
        { 
            if((*imgs).img_basico!=NULL)
            {
                espejo_enemigo=0;
                fila_y=0;
                if(enemigos[i].direccion_mirada==-1.0)
                {
                    espejo_enemigo=ALLEGRO_FLIP_HORIZONTAL;
                }

                if(enemigos[i].estado==ESTADO_IDLE) 
                {
                    fila_y=0;
                }

                else if(enemigos[i].estado==ESTADO_CAMINAR) 
                {
                    fila_y=1;
                }

                else if(enemigos[i].estado==ESTADO_ATAQUE) 
                {
                    fila_y=2;
                }

                else if(enemigos[i].estado==ESTADO_DANO) 
                {
                    fila_y=3;
                }

                else if(enemigos[i].estado==ESTADO_MUERTE) 
                {
                    fila_y=4;
                }

                recorte_x=enemigos[i].cuadro_actual*64;
                recorte_y=fila_y*64;

                al_draw_scaled_bitmap((*imgs).img_basico,recorte_x,recorte_y,64,64,enemigos[i].x-ajuste_alien_x,enemigos[i].y-ajuste_alien_y,64,64,espejo_enemigo);
            }
        }

        if(enemigos[i].tipo=='5'&&(*imgs).alien5!=NULL) 
        {
            int espejo_enemigo=0;
            if(enemigos[i].direccion_mirada==-1.0) 
            {
                espejo_enemigo = ALLEGRO_FLIP_HORIZONTAL;
            }

            ancho_frame=48; 
            alto_frame=48; 
            recorte_x=enemigos[i].cuadro_actual*48;
            recorte_y=0;

            if(enemigos[i].estado==ESTADO_IDLE) 
            {
                recorte_y=0;
                ajuste_piso=20.0;
            }
            else if(enemigos[i].estado==ESTADO_CAMINAR) 
            {
                recorte_y=42;  
                ajuste_piso=14.0;
            }
            else if(enemigos[i].estado==ESTADO_ATAQUE) 
            {
                recorte_y=73; 
                ajuste_piso=28.0;
            }
            else if(enemigos[i].estado==ESTADO_MUERTE) 
            {
                recorte_y=192; 
                ajuste_piso=14.0;
            }

            ancho_dibujo=80.0;
            alto_dibujo=85.0;
            ajuste_centro=20.0; 

            al_draw_scaled_bitmap((*imgs).alien5,recorte_x,recorte_y,ancho_frame,alto_frame,enemigos[i].x-ajuste_centro,enemigos[i].y-ajuste_piso,ancho_dibujo,alto_dibujo,espejo_enemigo);
        }

        if(enemigos[i].tipo=='6'&&(*imgs).intenemy!=NULL) 
        {
            espejo_enemigo=0;
            if(enemigos[i].direccion_mirada==-1.0) 
            {
                espejo_enemigo=ALLEGRO_FLIP_HORIZONTAL;
            }
            ancho_frame=64; 
            alto_frame=70; 
            recorte_x=enemigos[i].cuadro_actual*ancho_frame;
            recorte_y=0;

            if(enemigos[i].estado==ESTADO_IDLE) 
            {
                recorte_y=0;    
                ajuste_piso=3.0;
            }
            else if(enemigos[i].estado==ESTADO_CAMINAR) 
            {
                recorte_y=70;
                ajuste_piso = 3.0; 
            }
            else if(enemigos[i].estado==ESTADO_ATAQUE) 
            {
                recorte_y=120;  
                ajuste_piso=18.0; 
            }
            else if(enemigos[i].estado==ESTADO_MUERTE) 
            {
                recorte_y=280;  
                ajuste_piso=-8.0; 
            }
            ancho_dibujo=64.0;
            alto_dibujo=70.0;
            ajuste_centro=ajuste_alien_x; 

            al_draw_scaled_bitmap((*imgs).intenemy,recorte_x,recorte_y,ancho_frame,alto_frame,enemigos[i].x-ajuste_centro,enemigos[i].y-ajuste_piso,ancho_dibujo,alto_dibujo,espejo_enemigo);
        }

        if(enemigos[i].tipo=='9'&&(*imgs).boss!=NULL)
        {
            espejo_enemigo=0;
            if(enemigos[i].direccion_mirada==-1.0) 
            {
                espejo_enemigo=ALLEGRO_FLIP_HORIZONTAL;
            }
            ancho_frame=48; 
            alto_frame=48; 
            recorte_x=enemigos[i].cuadro_actual*ancho_frame;
            recorte_y=0;

            if(enemigos[i].estado==ESTADO_IDLE) 
            {
                recorte_y=0*alto_frame; 
                max_cuadros_boss=3;
                ajuste_piso=-66.0;
            }
            else if(enemigos[i].estado==ESTADO_CAMINAR) 
            {
                recorte_y=1*alto_frame;
                max_cuadros_boss=8;
                ajuste_piso=-66.0;
            }
            else if(enemigos[i].estado==ESTADO_ATAQUE) 
            {
                recorte_y=2*alto_frame; 
                max_cuadros_boss=8;
                ajuste_piso=-66.0;
            }
            else if(enemigos[i].estado==ESTADO_MUERTE) 
            {
                recorte_y=4*alto_frame; 
                max_cuadros_boss=6;
                ajuste_piso=-66.0;
            }

            if(enemigos[i].cuadro_actual>=max_cuadros_boss)
            {
                enemigos[i].cuadro_actual=0;
            }
            recorte_x=enemigos[i].cuadro_actual*ancho_frame;
            ancho_dibujo=240.0; 
            alto_dibujo=240.0;
            ajuste_centro=(ancho_dibujo-40.0)/2.0; 

            al_draw_scaled_bitmap((*imgs).boss,recorte_x,recorte_y,ancho_frame,alto_frame,enemigos[i].x-ajuste_centro,enemigos[i].y-ajuste_piso-(alto_dibujo-40.0),ancho_dibujo,alto_dibujo,espejo_enemigo);
        }
    }

    for(i=0;i<total_enemigos;i++)
    {
        for(j=0;j<MAX_LASERS;j++)
        {
            if(enemigos[i].laser_enemigos[j].activo)
            {
                al_draw_filled_rectangle(enemigos[i].laser_enemigos[j].x,enemigos[i].laser_enemigos[j].y,enemigos[i].laser_enemigos[j].x+20,enemigos[i].laser_enemigos[j].y+5,al_map_rgb(180,0,255));
            }
        }
    }

    for(i=0;i<MAX_BOMBARDEO;i++)
    {
        if((*n).bombas_entorno[i].activo)
        {
            ancho_frame=48;
            alto_frame=48;
            recorte_x=0;
            recorte_y=0;
            al_draw_scaled_bitmap((*imgs).bomba_enemiga,recorte_x,recorte_y,ancho_frame,alto_frame,(*n).bombas_entorno[i].x,(*n).bombas_entorno[i].y,30,30,0);
        }
    }

    for(i=0;i<MAX_ITEMS;i++)
    {
        if(mis_items[i].activo)
        {
            y_flotante=mis_items[i].y-(*graficos).movimiento_items;   

            if(mis_items[i].tipo==ITEM_VIDA&&(*imgs).pocion_vida!= NULL)
            {
                al_draw_scaled_bitmap((*imgs).pocion_vida,0,0,al_get_bitmap_width((*imgs).pocion_vida),al_get_bitmap_height((*imgs).pocion_vida),mis_items[i].x,y_flotante,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_VENENO&&(*imgs).veneno!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).veneno,0,0,al_get_bitmap_width((*imgs).veneno),al_get_bitmap_height((*imgs).veneno),mis_items[i].x,y_flotante,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_BOMBA&&(*imgs).bombita!=NULL)
            {
                ancho_frame=516/2; 
                alto_frame=1968/8; 
                recorte_x=((*graficos).cuadro_bomba%2)*ancho_frame; 
                recorte_y=((*graficos).cuadro_bomba/2)*alto_frame;
                al_draw_scaled_bitmap((*imgs).bombita,recorte_x,recorte_y,ancho_frame,alto_frame,mis_items[i].x,y_flotante,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_LLAVE_ROJA||mis_items[i].tipo==ITEM_LLAVE_VERDE||mis_items[i].tipo==ITEM_LLAVE_AMARILLA||mis_items[i].tipo==ITEM_LLAVE_AZUL)
            {
                if(mis_items[i].tipo==ITEM_LLAVE_ROJA&&(*imgs).llave_roja!=NULL)
                {
                    al_draw_scaled_bitmap((*imgs).llave_roja,0,0,al_get_bitmap_width((*imgs).llave_roja),al_get_bitmap_height((*imgs).llave_roja),mis_items[i].x,y_flotante,30,30,0);
                }
                else if(mis_items[i].tipo==ITEM_LLAVE_VERDE&&(*imgs).llave_verde!=NULL)
                {
                    al_draw_scaled_bitmap((*imgs).llave_verde,0,0,al_get_bitmap_width((*imgs).llave_verde),al_get_bitmap_height((*imgs).llave_verde),mis_items[i].x,y_flotante,30,30,0);
                }
                else if(mis_items[i].tipo==ITEM_LLAVE_AMARILLA&&(*imgs).llave_amarilla!=NULL)
                {
                    al_draw_scaled_bitmap((*imgs).llave_amarilla,0,0,al_get_bitmap_width((*imgs).llave_amarilla),al_get_bitmap_height((*imgs).llave_amarilla),mis_items[i].x,y_flotante,30,30,0);
                }
                else if(mis_items[i].tipo==ITEM_LLAVE_AZUL&&(*imgs).llave_azul!=NULL)
                {
                    al_draw_scaled_bitmap((*imgs).llave_azul,0,0,al_get_bitmap_width((*imgs).llave_azul),al_get_bitmap_height((*imgs).llave_azul),mis_items[i].x,y_flotante,30,30,0);
                }     
            }
            else if(mis_items[i].tipo==ITEM_LLAVE_FINAL&&(*imgs).llave_final!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).llave_final,0,0,al_get_bitmap_width((*imgs).llave_final),al_get_bitmap_height((*imgs).llave_final),mis_items[i].x,y_flotante,40,40,0);
            }
            else if(mis_items[i].tipo==ITEM_PUNTOS&&(*imgs).punto!=NULL)
            {
                recorte_moneda_x=(*graficos).cuadro_moneda*563; 
                y_flotante=mis_items[i].y-(*graficos).movimiento_items;
                al_draw_scaled_bitmap((*imgs).punto,recorte_moneda_x,0,563,565,mis_items[i].x,y_flotante,25,25,0);
            }
            else if(mis_items[i].tipo==ITEM_MUNICION&&(*imgs).municion!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).municion,0,0,al_get_bitmap_width((*imgs).municion),al_get_bitmap_height((*imgs).municion),mis_items[i].x,y_flotante,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_ESCUDO&&(*imgs).proteccion!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).proteccion,0,0,al_get_bitmap_width((*imgs).proteccion),al_get_bitmap_height((*imgs).proteccion),mis_items[i].x,y_flotante,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_MINIMAPA&&(*imgs).soy_mapa!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).soy_mapa,0,0,300,200,mis_items[i].x,y_flotante+5.0,30,20,0);
            }
            else if(mis_items[i].tipo==ITEM_LINTERNA&&(*imgs).linternita!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).linternita,0,0,al_get_bitmap_width((*imgs).linternita),al_get_bitmap_height((*imgs).linternita),mis_items[i].x,y_flotante,30,30,0);
            }
        }
    }

    for(i=0;i<MAX_ITEMS;i++)
    {
        if(heroe.proyectiles[i].activo)
        {
            if(heroe.proyectiles[i].tipo==MUNICION_LASER)
            {
                al_draw_filled_rectangle(heroe.proyectiles[i].x,heroe.proyectiles[i].y,heroe.proyectiles[i].x+15,heroe.proyectiles[i].y+5,al_map_rgb(255,0,0));
            }
            else if(heroe.proyectiles[i].tipo==MUNICION_VENENO&&(*imgs).veneno!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).veneno,0,0,al_get_bitmap_width((*imgs).veneno),al_get_bitmap_height((*imgs).veneno),heroe.proyectiles[i].x,heroe.proyectiles[i].y,30,30,0);
            }
            else if(heroe.proyectiles[i].tipo==MUNICION_BOMBA&&(*imgs).bombita!=NULL)
            {
                if((*n).bombas_entorno[i].activo&&(*imgs).bomba_enemiga!=NULL)
                {
                    ancho_frame=48;
                    alto_frame=48;
                    recorte_x=0;
                    recorte_y=0;
                    al_draw_scaled_bitmap((*imgs).bomba_enemiga,recorte_x,recorte_y,ancho_frame,alto_frame,(*n).bombas_entorno[i].x,(*n).bombas_entorno[i].y,30,30,0);
                }
            }
        }
    }

    if(portal_activo&&(*imgs).portal!=NULL)
    {
        if((*graficos).portal_abierto)
        {
            recorte_portal_x=(*graficos).cuadro_portal*32; 
            al_draw_scaled_bitmap((*imgs).portal,recorte_portal_x,0,32,32,x_portal,y_portal-TAM_TILE,TAM_TILE,TAM_TILE*2,0);
        }
        else
        {
            al_draw_scaled_bitmap((*imgs).portal,0,0,32,32,x_portal,y_portal-TAM_TILE,TAM_TILE,TAM_TILE*2,0);
        }
    }

    if(luz_apagada==1)
    {
        centro_x=heroe.x+20.0; 
        centro_y=heroe.y+20.0;
        
        if(heroe.duracion_linterna>0) 
        {
            radio_vision=300.0; 
        } 
        else 
        {
            radio_vision=100.0;
        }
    
        grosor_oscuridad=4000.0;
        
        al_draw_circle(centro_x,centro_y,radio_vision+(grosor_oscuridad/2.0),al_map_rgb(0,0,0),grosor_oscuridad);
    }

    if((*graficos).fuente!=NULL)
    {
        al_draw_filled_rectangle(10, 10, 220, 110, al_map_rgba(0,0,0,180));
        al_draw_textf((*graficos).fuente,al_map_rgb(255,255,255),20,20,0,"VIDA: %d",heroe.vida);
        al_draw_textf((*graficos).fuente,al_map_rgb(255,215,0),20,50,0,"PUNTOS: %d",heroe.puntos);
        al_draw_textf((*graficos).fuente,al_map_rgb(100,200,255),20,80,0,"MUNICION: %d",heroe.municion);

        if((*graficos).nivel_actual==4)
        {
            for(k=0;k<(*n).total_enemigos;k++)
            {
                if((*n).enemigos[k].tipo=='9'&&(*n).enemigos[k].vida>0)
                {
                    al_draw_filled_rectangle(ancho_pantalla-320,10,ancho_pantalla-20,50,al_map_rgba(0,0,0,180));
                    al_draw_textf((*graficos).fuente,al_map_rgb(255,50,50),ancho_pantalla-40,18,ALLEGRO_ALIGN_RIGHT, "VIDA JEFE: %d",(*n).enemigos[k].vida);
                    break;
                }
            }
        }
        
        if((*n).total_llaves_secuencia>0) 
        {
            ancho_llave=30.0;
            espaciado=15.0;
            total_ancho=((*n).total_llaves_secuencia*ancho_llave)+(((*n).total_llaves_secuencia-1)*espaciado);
            pos_x_llaves=(ancho_pantalla/2.0)-(total_ancho/2.0);
            pos_y_llaves=20.0;

            al_draw_text((*graficos).fuente,al_map_rgb(255,255,255),ancho_pantalla/2.0,0,ALLEGRO_ALIGN_CENTER,"ORDEN:");

            for(k=0;k<(*n).total_llaves_secuencia;k++) 
            {
                ALLEGRO_BITMAP *img_llave=NULL;
                if((*n).secuencia_llaves[k]==ITEM_LLAVE_ROJA) 
                {
                    img_llave=(*imgs).llave_roja;
                }
                else if((*n).secuencia_llaves[k]==ITEM_LLAVE_VERDE) 
                {
                    img_llave=(*imgs).llave_verde;
                }
                else if((*n).secuencia_llaves[k]==ITEM_LLAVE_AMARILLA) 
                {
                    img_llave=(*imgs).llave_amarilla;
                }
                else if((*n).secuencia_llaves[k]==ITEM_LLAVE_AZUL) 
                {
                    img_llave=(*imgs).llave_azul;
                }

                if(img_llave!=NULL) 
                {
                    x_actual=pos_x_llaves+(k*(ancho_llave + espaciado));
                    
                    if(k<(*n).indice_llave_actual) 
                    {
                        al_draw_tinted_scaled_bitmap(img_llave, al_map_rgba_f(0.2,0.2,0.2,0.5),0,0,al_get_bitmap_width(img_llave),al_get_bitmap_height(img_llave),x_actual,pos_y_llaves,ancho_llave,ancho_llave,0);
                    } 
                    else 
                    {
                        al_draw_scaled_bitmap(img_llave,0,0,al_get_bitmap_width(img_llave),al_get_bitmap_height(img_llave),x_actual,pos_y_llaves,ancho_llave,ancho_llave,0);
                    }
                    
                    if(k==(*n).indice_llave_actual) 
                    {
                        al_draw_rectangle(x_actual-3,pos_y_llaves-3,x_actual+ancho_llave+3,pos_y_llaves+ancho_llave+3,al_map_rgb(255,255,0),2.0);
                    }
                }
            }
        }
    }

    if((*graficos).mostrar_mapa)
    {
        mini_tile=15.0; 
        ancho_mapa_rial=columna*mini_tile;
        alto_mapa_rial=fila*mini_tile;        
        inicio_x_mapa=(ancho_pantalla-ancho_mapa_rial)/2.0;
        inicio_y_mapa=(alto_pantalla-alto_mapa_rial)/2.0;

        if((*imgs).big_map != NULL)
        {
            margen_x=270.0;
            margen_y=240.0;
            
            al_draw_scaled_bitmap((*imgs).big_map,0,0,al_get_bitmap_width((*imgs).big_map),al_get_bitmap_height((*imgs).big_map),inicio_x_mapa-margen_x,inicio_y_mapa-margen_y,ancho_mapa_rial+(margen_x*2),alto_mapa_rial+(margen_y*2),0);
        }

        snprintf(nombre_archivo,sizeof(nombre_archivo),"nivel%d.txt",(*graficos).nivel_actual);
        
        FILE *archivo_mapa=fopen(nombre_archivo,"r");
        if(archivo_mapa!=NULL)
        {
            fscanf(archivo_mapa," %c",&estado_luz);
            fscanf(archivo_mapa," %s",texto_secuencia);

            for(i=0;i<fila;i++) 
            {
                for(j=0;j<columna;j++) 
                {
                    fscanf(archivo_mapa," %c",&caracter_leido);
                    
                    rect_x=inicio_x_mapa+(j*mini_tile);
                    rect_y=inicio_y_mapa+(i* mini_tile);

                    if(caracter_leido=='1'||caracter_leido=='3'||caracter_leido=='$') 
                    {
                        al_draw_filled_rectangle(rect_x,rect_y,rect_x+mini_tile,rect_y+mini_tile,al_map_rgb(100,70,40)); 
                        al_draw_rectangle(rect_x,rect_y,rect_x+mini_tile,rect_y+mini_tile,al_map_rgba(50,30,10,150),1.0); 
                    } 
                    else if(caracter_leido=='D'||caracter_leido=='A') 
                    {
                        al_draw_filled_rectangle(rect_x,rect_y,rect_x+mini_tile,rect_y+mini_tile,al_map_rgb(160,90,40)); 
                    }
                }
            }
            fclose(archivo_mapa);

            for(i=0;i<total_enemigos;i++) 
            {
                if(enemigos[i].vida>0) 
                { 
                    mini_enemigo_x=inicio_x_mapa+(enemigos[i].x/TAM_TILE)*mini_tile;
                    mini_enemigo_y=inicio_y_mapa+(enemigos[i].y/TAM_TILE)*mini_tile;
                    al_draw_filled_circle(mini_enemigo_x+(mini_tile/2.0), mini_enemigo_y+(mini_tile/2.0),mini_tile/2.5,al_map_rgb(150,0,0));
                }
            }

            for(i=0; i<MAX_ITEMS; i++) 
            {
                if(mis_items[i].activo) 
                {
                    mini_item_x=inicio_x_mapa+(mis_items[i].x/TAM_TILE)*mini_tile;
                    mini_item_y=inicio_y_mapa+(mis_items[i].y/TAM_TILE)*mini_tile;
                
                    if(mis_items[i].tipo==ITEM_LLAVE_ROJA) 
                    {
                        al_draw_filled_circle(mini_item_x+(mini_tile/2.0),mini_item_y+(mini_tile/2.0),mini_tile/3.0,al_map_rgb(255,50,50));
                    } 
                    else if(mis_items[i].tipo==ITEM_LLAVE_VERDE) 
                    {
                        al_draw_filled_circle(mini_item_x+(mini_tile/2.0),mini_item_y+(mini_tile/2.0),mini_tile/3.0,al_map_rgb(50,255,50));
                    } 
                    else if(mis_items[i].tipo==ITEM_LLAVE_AMARILLA) 
                    {
                        al_draw_filled_circle(mini_item_x+(mini_tile/2.0),mini_item_y+(mini_tile/2.0),mini_tile/3.0,al_map_rgb(255,255,0));
                    } 
                    else if(mis_items[i].tipo==ITEM_LLAVE_AZUL) 
                    {
                        al_draw_filled_circle(mini_item_x+(mini_tile/2.0),mini_item_y+(mini_tile/2.0),mini_tile/3.0,al_map_rgb(50,150,255));
                    }
                    else if(mis_items[i].tipo==ITEM_LLAVE_FINAL) 
                    {
                        al_draw_filled_circle(mini_item_x+(mini_tile/2.0),mini_item_y+(mini_tile/2.0),mini_tile/2.0,al_map_rgb(255,215,0)); 
                    }
                }
            }

            mini_heroe_x=inicio_x_mapa+(heroe.x/TAM_TILE)*mini_tile;
            mini_heroe_y=inicio_y_mapa+(heroe.y/TAM_TILE)*mini_tile;
        
            al_draw_filled_circle(mini_heroe_x+(mini_tile/2.0),mini_heroe_y+(mini_tile/2.0),mini_tile/1.5,al_map_rgb(0,255,0));
            al_draw_circle(mini_heroe_x+(mini_tile/2.0),mini_heroe_y+(mini_tile/2.0),mini_tile/1.5,al_map_rgb(255,255,255),2.0);
        }
    }

    if((*n).suelo_destruido)
    {
        al_identity_transform(&camara);
        al_use_transform(&camara);
    }

    al_flip_display();
}

bool colision_lasers(float x,float y)
{
    int izq=x/TAM_TILE;
    int der=(x+15)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+5)/TAM_TILE;

    if(izq<0||der>=columna||arr<0||aba>=fila)
    {
        return true;
    }

    if (mapa[arr][izq]=='1'||mapa[arr][izq]=='D'||mapa[arr][izq]=='3'||mapa[arr][izq]=='$') 
    {
        return true;
    }

    if (mapa[arr][der]=='1'||mapa[arr][der]=='D'||mapa[arr][der]=='3'||mapa[arr][der]=='$') 
    {
        return true;
    }

    if (mapa[aba][izq]=='1'||mapa[aba][izq]=='D'||mapa[aba][izq]=='3'||mapa[aba][izq]=='$') 
    {
        return true;
    }

    if (mapa[aba][der]=='1'||mapa[aba][der]=='D'||mapa[aba][der]=='3'||mapa[aba][der]=='$') 
    {
        return true;
    }

    return false;
}

void recolectar_items(item lista_items[],int maximo,recursos *sonidos,estado_juego *nivel)
{
    int i,j,k,llave_esperada;
    bool guardado;
    for(i=0;i<maximo;i++)
    {
        if(lista_items[i].activo)
        {
            lista_items[i].velocidad_y+=0.5;
            lista_items[i].y+=lista_items[i].velocidad_y;
            if(colision(lista_items[i].x, lista_items[i].y))
            {               
                lista_items[i].y =((int)(lista_items[i].y+39)/TAM_TILE)*TAM_TILE-39.1;
                lista_items[i].velocidad_y=0;
            }
            if(heroe.x<lista_items[i].x+40&&heroe.x+40>lista_items[i].x&&heroe.y<lista_items[i].y+40&&heroe.y+40>lista_items[i].y)
            {
                if(lista_items[i].tipo==ITEM_VIDA)
                {
                    heroe.vida+=lista_items[i].valor;
                    if(heroe.vida>100) 
                    {
                        heroe.vida=100; 
                    }
                    printf("Vida recogida, vida actual: %d\n",heroe.vida);
                    al_play_sample((*sonidos).sonido_vida,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    lista_items[i].activo=false;
                }
                else if(lista_items[i].tipo==ITEM_LLAVE_FINAL)
                {
                    al_play_sample((*sonidos).sonido_llaves,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    lista_items[i].activo=false;
                    (*nivel).amigo_activo=false; 
                    (*nivel).juego_terminado=true;
                }
                else if(lista_items[i].tipo==ITEM_VENENO||lista_items[i].tipo==ITEM_LLAVE_ROJA||lista_items[i].tipo==ITEM_LLAVE_VERDE||lista_items[i].tipo==ITEM_LLAVE_AMARILLA||lista_items[i].tipo==ITEM_LLAVE_AZUL||lista_items[i].tipo==ITEM_BOMBA)
                {
                    if(lista_items[i].tipo!=ITEM_VENENO&&lista_items[i].tipo!=ITEM_BOMBA) 
                    {
                        llave_esperada=(*nivel).secuencia_llaves[(*nivel).indice_llave_actual];
                        if(lista_items[i].tipo!=llave_esperada) 
                        {
                            continue; 
                        }
                    }
                    guardado=false;
                    for(j=0;j<MAX_COSAS;j++)
                    {
                        if(heroe.inventario[j]==0)
                        {
                            heroe.inventario[j]=lista_items[i].tipo; 
                            printf("Objeto guardado en el bolsillo %d\n",j);
                            lista_items[i].activo=false; 
                            guardado=true;
                            if(lista_items[i].tipo==ITEM_VENENO||lista_items[i].tipo==ITEM_BOMBA) 
                            {
                                al_play_sample((*sonidos).sonido_equipar,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                                
                                (*nivel).recolectados_venenos_bombas++;
                                
                                if((*nivel).recolectados_venenos_bombas>=(*nivel).total_venenos_bombas)
                                {
                                    printf("El jefe se ha debilitado por las bombas y venenos!\n");
                                    for(k=0;k<(*nivel).total_enemigos;k++)
                                    {
                                        if((*nivel).enemigos[k].tipo=='9') 
                                        {
                                            (*nivel).enemigos[k].vida=500;
                                        }
                                    }
                                }
                            }
                            else 
                            {
                                (*nivel).indice_llave_actual++;
                                al_play_sample((*sonidos).sonido_llaves,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                            }
                            break;
                        }
                    }
                    if(!guardado)
                    {
                        printf("Inventario lleno\n");
                    }
                }
                else if(lista_items[i].tipo==ITEM_PUNTOS)
                {
                    heroe.puntos+=lista_items[i].valor;
                    printf("Punto recogido, puntuación: %d\n",heroe.puntos);
                    al_play_sample((*sonidos).sonido_moneda,0.7,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    lista_items[i].activo=false;
                }
                else if(lista_items[i].tipo==ITEM_MUNICION)
                {
                    heroe.municion+=lista_items[i].valor;
                    printf("Munición recogida, tienes %d tiros.\n",heroe.municion);
                    al_play_sample((*sonidos).sonido_municion,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    lista_items[i].activo=false;
                }
                else if(lista_items[i].tipo==ITEM_LINTERNA)
                {
                    heroe.duracion_linterna+=lista_items[i].valor; 
                    printf("Recogiste una linterna! Visión aumentada por 1 minuto.\n");
                    al_play_sample((*sonidos).sonido_equipar,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    lista_items[i].activo=false;
                }
                else if(lista_items[i].tipo==ITEM_ESCUDO)
                {
                    heroe.escudo+=lista_items[i].valor;
                    
                    if(heroe.escudo>10) 
                    {
                        heroe.escudo=10;
                    }
                    
                    printf("Escudo recogido! Resistencia actual: %d\n",heroe.escudo);
                    al_play_sample((*sonidos).sonido_equipar,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    lista_items[i].activo=false;
                }
                else if(lista_items[i].tipo==ITEM_MINIMAPA)
                {
                    heroe.tiene_mapita=true;
                    printf("¡Mapa recogido!\n");
                    al_play_sample((*sonidos).sonido_equipar,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    lista_items[i].activo=false;
                }
            }
        }
    }
}

void animacion_entorno(int *cont_moneda,int *cuad_moneda,float *mov_items,float *vel_flote,int *cont_bomba,int *cuadro_bombita) 
{
    (*cont_moneda)++;
    if((*cont_moneda)>=5) 
    {
        *cont_moneda=0;
        (*cuad_moneda)++;
        if((*cuad_moneda)>=10) 
        {
            *cuad_moneda=0;
        }
    }
    (*cont_bomba)++;
    if((*cont_bomba)>=5) 
    {
        *cont_bomba=0;
        (*cuadro_bombita)++;
        if((*cuadro_bombita)>=15)
        {
            *cuadro_bombita=0;
        }
    }
    *mov_items+=*vel_flote;
    if(*mov_items>6.0||*mov_items<0.0) 
    {
        *vel_flote=-(*vel_flote);
    }
}

void actualizar_estado_heroe(personaje *h,bool izq,bool der,float *vel_caida,int max_cuadros,recursos *imgs) 
{
    if((*h).tiempo_dano>0) 
    {
        (*h).tiempo_dano--; 
        (*h).estado=ESTADO_DANO;
    } 
    else if((*h).vida<=0) 
    {
        (*h).estado=ESTADO_MUERTE; 
    } 
    else 
    {
        (*h).estado=ESTADO_IDLE;
        if(izq||der) 
        {
            (*h).estado=ESTADO_CAMINAR;
        }
        if(*vel_caida!=0) 
        {
            (*h).estado=ESTADO_SALTO;
        }
        if((*h).ataque) 
        {
            (*h).estado=ESTADO_DISPARO;
        }
    }

    (*h).contador_animacion++;
    if((*h).contador_animacion>=5) 
    {
        (*h).contador_animacion=0;
        (*h).cuadro_actual++;
        
        if((*h).estado==ESTADO_CAMINAR&&((*h).cuadro_actual==1||(*h).cuadro_actual==3)) 
        {
            al_play_sample((*imgs).sonido_pasos,0.4,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
        
        if((*h).cuadro_actual>=max_cuadros) 
        {
            if(h->estado == ESTADO_MUERTE) 
            {
                (*h).vida=100;
                (*h).x=(*h).spawn_x;
                (*h).y=(*h).spawn_y;
                (*h).estado=ESTADO_IDLE;
                *vel_caida=0.0;
                printf("Has revivido en el último checkpoint!\n");
            } 
            else 
            {
                (*h).cuadro_actual=0; 
            }
        }
    }
    if((*h).duracion_linterna > 0)
    {
        (*h).duracion_linterna--;
    }
}

void trampitas(personaje *h,float *vel_caida,estado_juego *nivel) 
{
    int i,recuadro;
    float fuego;
    bool trampita_mala;
    trampas *mis_trampas=(*nivel).mis_trampas;

    for(i=0;i<MAX_TRAMPAS;i++) 
    {
        if(mis_trampas[i].activado) 
        {
            if(mis_trampas[i].tipo=='q')
            {
                mis_trampas[i].tiempo++;
                if(mis_trampas[i].tiempo>120) 
                {
                    mis_trampas[i].tiempo=0;
                }
            }
            trampita_mala=false;
            fuego=TAM_TILE;

            if(mis_trampas[i].tipo=='p')
            {
                trampita_mala=true;
            }
            else if(mis_trampas[i].tipo=='q')
            {
                fuego=160.0;
                recuadro=mis_trampas[i].tiempo/14;
                
                if(recuadro==6||recuadro==7)
                {
                    trampita_mala=true;
                }
            }

            if(trampita_mala&&(*h).x<mis_trampas[i].x+TAM_TILE&&(*h).x+40>mis_trampas[i].x&&(*h).y<mis_trampas[i].y+fuego&&(*h).y+40>mis_trampas[i].y) 
            {
                if((*h).tiempo_dano==0) 
                {
                    if((*h).escudo>0&&mis_trampas[i].tipo=='q')
                    {
                        (*h).escudo--;
                        (*h).tiempo_dano=30;
                        printf("El escudo absorbió el fuego! Resistencia: %d\n", (*h).escudo);
                    }
                    else
                    {
                        if(mis_trampas[i].tipo=='p')
                        {
                            (*h).vida=0;
                            *vel_caida=-6.0; 
                            printf("Caiste en las púas!\n");
                        }
                        else if(mis_trampas[i].tipo=='q')
                        {
                            (*h).vida-=20; 
                            (*h).tiempo_dano=30; 
                            *vel_caida=-4.0;
                            printf("Te quemaste!, fijate por donde andas\n");
                        }
                    }
                }
            }
        }
    }
    if(rand()%60==0) 
    {
        for(i=0;i<MAX_BOMBARDEO;i++)
        {
            if(!(*nivel).bombas_entorno[i].activo)
            {
                (*nivel).bombas_entorno[i].activo=true;
                (*nivel).bombas_entorno[i].tipo=MUNICION_BOMBA;
                (*nivel).bombas_entorno[i].x=rand()%ancho_pantalla; 
                (*nivel).bombas_entorno[i].y=-30.0; 
                (*nivel).bombas_entorno[i].velocidad_y=5.0; 
                (*nivel).bombas_entorno[i].velocidad_x=0.0;
                break;
            }
        }
    }
    for(i=0;i<MAX_BOMBARDEO;i++)
    {
        if((*nivel).bombas_entorno[i].activo)
        {
            (*nivel).bombas_entorno[i].y+=(*nivel).bombas_entorno[i].velocidad_y;

            if(colision_bombas_enemigas((*nivel).bombas_entorno[i].x,(*nivel).bombas_entorno[i].y)||(*nivel).bombas_entorno[i].y>alto_pantalla)
            {
                (*nivel).bombas_entorno[i].activo=false;
            }
            if((*nivel).bombas_entorno[i].activo&&(*h).vida>0)
            {
                if((*nivel).bombas_entorno[i].x<(*h).x+40&&(*nivel).bombas_entorno[i].x+30>(*h).x&&(*nivel).bombas_entorno[i].y<(*h).y+40&&(*nivel).bombas_entorno[i].y+30>(*h).y)
                {
                    if((*h).tiempo_dano==0)
                    {
                        (*h).vida-=15; 
                        (*h).tiempo_dano=30; 
                        printf("El cielo se cae!\n");
                    }
                    (*nivel).bombas_entorno[i].activo=false; 
                }
            }
        }
    }
}

void portalito(personaje *h,bool *portal_abierto,bool *cambiar_nivel,recursos *imgs,estado_juego *nivel)
{
    int i;
    float x_portal=(*nivel).x_portal;
    float y_portal=(*nivel).y_portal;
    bool portal_activo=(*nivel).portal_activo;

    if(portal_activo) 
    {
        (*nivel).contador_portal++;
        if((*nivel).contador_portal>=5) 
        {
            (*nivel).contador_portal=0;
            (*nivel).cuadro_portal++;
            if((*nivel).cuadro_portal>=4) 
            {
                (*nivel).cuadro_portal=0;
            }
        }

        if(!(*portal_abierto)) 
        {
            if((*nivel).indice_llave_actual==(*nivel).total_llaves_secuencia&&(*nivel).total_llaves_secuencia>0) 
            {
                *portal_abierto=true;
                al_play_sample((*imgs).sonido_portal,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                printf("Todas las llaves recogidas, portal activado.\n");
                for(i=0;i<MAX_COSAS;i++) 
                {
                    if((*h).inventario[i]==ITEM_LLAVE_ROJA||(*h).inventario[i]==ITEM_LLAVE_VERDE||(*h).inventario[i]==ITEM_LLAVE_AMARILLA||(*h).inventario[i]==ITEM_LLAVE_AZUL) 
                    {
                        (*h).inventario[i]=0; 
                    }
                }
            }
        }

        if((*h).x<x_portal+TAM_TILE&&(*h).x+40>x_portal&&(*h).y<y_portal+TAM_TILE&&(*h).y+40>y_portal-TAM_TILE) 
        {
            if(*portal_abierto)
            {
                printf("Portal alcanzado, nivel completado.\n");
                *cambiar_nivel=true;
            }
        }
    }
}

void cambios_de_niveles(int *nivel_actual,bool *corriendo,bool *portal_abierto,float *vel_caida,estado_juego *nivel,ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_titulo,ALLEGRO_FONT *f_opciones,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs)
{
    int i;
    char nombre_archivo[30];
    (*nivel).total_enemigos=0;
    (*nivel).portal_activo=false;
    *portal_abierto=false;
    *vel_caida=0.0;
    
    for(i=0;i<MAX_ITEMS;i++) 
    {
        (*nivel).mis_items[i].activo=false;
    }
    for(i=0;i<MAX_TRAMPAS;i++) 
    {
        (*nivel).mis_trampas[i].activado=false;
    }
    for(i=0;i<MAX_ITEMS;i++) 
    {
        heroe.proyectiles[i].activo=false;
    }

    mostrar_pantalla_carga(display,f_titulo,f_opciones,queue,imgs);

    (*nivel_actual)++;

    if (*nivel_actual<=4) 
    {
        snprintf(nombre_archivo,sizeof(nombre_archivo),"nivel%d.txt",*nivel_actual);
        
        if(!cargar_mapa(nombre_archivo,nivel)) 
        {
            *corriendo=false;
        }
    }
    else 
    {
        printf("Has completado el juego\n");
        *corriendo=false;
    }
}

void disparar_proyectil(personaje *h,int tipo_municion,float direccion_mirada,recursos *imgs)
{
    int i,bolsillo;
    bool puede_disparar=false;

    if(tipo_municion==MUNICION_LASER&&(*h).municion>0) 
    {
        puede_disparar=true;
    }
    else if(tipo_municion==MUNICION_VENENO) 
    {
        for(bolsillo=0;bolsillo<MAX_COSAS;bolsillo++) 
        {
            if((*h).inventario[bolsillo]==ITEM_VENENO) 
            {
                puede_disparar=true;
                break;
            }
        }
    }
    else if(tipo_municion==MUNICION_BOMBA) 
    {
        for(bolsillo=0;bolsillo<MAX_COSAS;bolsillo++) 
        {
            if((*h).inventario[bolsillo]==ITEM_BOMBA) 
            {
                puede_disparar=true;
                (*h).inventario[bolsillo]=0;
                break;
            }
        }
    }

    if(puede_disparar) 
    {
        for(i=0;i<MAX_ITEMS;i++) 
        {
            if(!(*h).proyectiles[i].activo) 
            {
                (*h).proyectiles[i].activo=true;
                (*h).proyectiles[i].tipo=tipo_municion;
                (*h).proyectiles[i].distancia_recorrida=0;

                if(tipo_municion==MUNICION_LASER) 
                {
                    (*h).ataque=true;
                    (*h).proyectiles[i].x=(*h).x+20;
                    (*h).proyectiles[i].y=(*h).y+15;
                    (*h).proyectiles[i].velocidad_x=15.0*direccion_mirada;
                    (*h).proyectiles[i].velocidad_y=0.0; 
                    (*h).municion--; 
                    printf("Disparo, munición restante: %d\n",(*h).municion);
                    al_play_sample((*imgs).disparo_prota,0.8,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                else if(tipo_municion==MUNICION_VENENO) 
                {
                    (*h).proyectiles[i].x=(*h).x+20;
                    (*h).proyectiles[i].y=(*h).y+10;
                    (*h).proyectiles[i].velocidad_x=12.0*direccion_mirada;
                    (*h).proyectiles[i].velocidad_y=-8.0;
                    printf("Veneno lanzado!\n");
                    al_play_sample((*imgs).sonido_proyectil,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                else if(tipo_municion==MUNICION_BOMBA) 
                {
                    (*h).proyectiles[i].x=(*h).x+20;
                    (*h).proyectiles[i].y=(*h).y+10;
                    (*h).proyectiles[i].velocidad_x=10.0*direccion_mirada;
                    (*h).proyectiles[i].velocidad_y=-10.0;
                    printf("Bomba lanzada!\n");
                    al_play_sample((*imgs).sonido_proyectil,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                break;
            }
        }
    }
    else
    {
        if(tipo_municion==MUNICION_LASER) 
        {
            printf("Sin munición, necesitas una recarga.\n");
        }
        else 
        {
            printf("No tienes veneno en el inventario!\n");
        }
    }
}

int mostrar_menu(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_titulo,ALLEGRO_FONT *f_opciones,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs)
{
    int mouse_x,mouse_y,i,pos_y,y_max,y_min,x_max,x_min;
    int opcion_seleccionada=0;
    int total_opciones=4;
    char opciones[4][15]={"Jugar","Informacion","Ranking","Salir"};
    bool en_menu=true;

    al_flush_event_queue(queue);

    while (en_menu) 
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue, &evento);

        if (evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE) 
        {
            return 3; 
        }
        
        else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) 
        {
            mouse_x=evento.mouse.x;
            mouse_y=evento.mouse.y;

            opcion_seleccionada=-1; 
            
            for (i=0;i<total_opciones;i++) 
            {
                pos_y=(alto_pantalla/2)+i*60; 
                y_min=pos_y;
                y_max=pos_y+40; 
                x_min=(ancho_pantalla/2)-100;
                x_max=(ancho_pantalla/2)+100;
                if (mouse_x>=x_min&&mouse_x<= x_max&&mouse_y>=y_min&&mouse_y<= y_max) 
                {
                    opcion_seleccionada=i; 
                }
            }
        }
        else if (evento.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) 
        {
            if (evento.mouse.button==1) 
            {
                if (opcion_seleccionada!=-1) 
                {
                    return opcion_seleccionada; 
                }
            }
        }

        if (al_is_event_queue_empty(queue)) 
        {
            al_clear_to_color(al_map_rgb(20,30,50)); 
            if ((*imgs).fondo_menu != NULL)
            {
                al_draw_scaled_bitmap((*imgs).fondo_menu,0,0,al_get_bitmap_width((*imgs).fondo_menu),al_get_bitmap_height((*imgs).fondo_menu),0,0,ancho_pantalla,alto_pantalla,0);
            }
            al_draw_text(f_titulo,al_map_rgb(255,255,255),ancho_pantalla/2,alto_pantalla/4,ALLEGRO_ALIGN_CENTER,"AstroKey");

            for (i=0;i<total_opciones;i++) 
            {
                ALLEGRO_COLOR color_texto;
                
                if (i==opcion_seleccionada) 
                {
                    color_texto=al_map_rgb(255,215,0); 
                    al_draw_text(f_opciones, color_texto,(ancho_pantalla/2)-150,(alto_pantalla/2)+i*60,ALLEGRO_ALIGN_RIGHT,"->");
                } 
                else 
                {
                    color_texto=al_map_rgb(200,200,200); 
                }
                
                al_draw_text(f_opciones,color_texto,ancho_pantalla/2,(alto_pantalla/2)+i*60,ALLEGRO_ALIGN_CENTER,opciones[i]);
            }

            al_flip_display();
        }
    }
    return 3;
}

void pedir_nombre(char *nombre,ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_nombre,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs)
{
    int contador_letras=0;
    char nueva_letra;
    bool escribiendo_nombre=true;

    al_flush_event_queue(queue);

    while (escribiendo_nombre) 
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue,&evento);

        if (evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE) 
        {
            escribiendo_nombre=false;
        }
        else if (evento.type==ALLEGRO_EVENT_KEY_CHAR) 
        {
            if (evento.keyboard.keycode==ALLEGRO_KEY_ENTER&&contador_letras>0) 
            {
                escribiendo_nombre=false;
            }
            else if (evento.keyboard.keycode==ALLEGRO_KEY_BACKSPACE&&contador_letras>0) 
            {
                contador_letras--;
                nombre[contador_letras]=0;
            }
            else if (evento.keyboard.unichar>=32&&evento.keyboard.unichar<=126&&contador_letras<19) 
            {
                if (evento.keyboard.unichar==32) 
                {
                    nueva_letra='_';
                }
                else 
                {
                    nueva_letra=(char)evento.keyboard.unichar;
                }
                snprintf(&nombre[contador_letras],20-contador_letras,"%c",nueva_letra);
                contador_letras++;
            }
        }

        if (al_is_event_queue_empty(queue)) 
        {
            al_clear_to_color(al_map_rgb(20,30,50));
            if ((*imgs).fondo_menu!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).fondo_menu,0,0,al_get_bitmap_width((*imgs).fondo_menu),al_get_bitmap_height((*imgs).fondo_menu),0,0,ancho_pantalla,alto_pantalla,0);
            }
            al_draw_text(f_nombre,al_map_rgb(255,215,0),ancho_pantalla/2,200,ALLEGRO_ALIGN_CENTER,"NUEVA PARTIDA");
            al_draw_text(f_nombre,al_map_rgb(200,200,200),ancho_pantalla/2,400,ALLEGRO_ALIGN_CENTER,"Ingresa tu nombre para jugar y presiona ENTER:");
            al_draw_text(f_nombre,al_map_rgb(100,255,100),ancho_pantalla/2,500,ALLEGRO_ALIGN_CENTER,nombre);
            al_flip_display();
        }
    }

    if (contador_letras==0) 
    {
        strcpy(nombre,"Jugador");
    }
}

void mostrar_informacion(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_texto,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs)
{
    bool en_info=true;
    al_flush_event_queue(queue);

    while(en_info) 
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue,&evento);

        if(evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE||(evento.type==ALLEGRO_EVENT_KEY_DOWN&&evento.keyboard.keycode==ALLEGRO_KEY_ESCAPE)) 
        {
            en_info=false;
        }

        if(al_is_event_queue_empty(queue)) 
        {
            al_clear_to_color(al_map_rgb(20,30,50));
            if ((*imgs).fondo_menu!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).fondo_menu,0,0,al_get_bitmap_width((*imgs).fondo_menu),al_get_bitmap_height((*imgs).fondo_menu),0,0,ancho_pantalla,alto_pantalla,0);
            }
            
            al_draw_text(f_texto,al_map_rgb(255,215,0),ancho_pantalla/2,40,ALLEGRO_ALIGN_CENTER,"INFORMACION DEL Astrokey");
            al_draw_text(f_texto,al_map_rgb(100,200,255),ancho_pantalla/2,100,ALLEGRO_ALIGN_CENTER,"CONTROLES");
            al_draw_text(f_texto,al_map_rgb(255,255,255),ancho_pantalla/2,135,ALLEGRO_ALIGN_CENTER,"Movimiento: Flechas, Saltar: Arriba, Disparar: Espacio");
            al_draw_text(f_texto,al_map_rgb(255,255,255),ancho_pantalla/2,165,ALLEGRO_ALIGN_CENTER,"Acciones: V (Veneno), B (Bomba), M (Minimapa)");
            al_draw_text(f_texto,al_map_rgb(100,200,255),ancho_pantalla/2,215,ALLEGRO_ALIGN_CENTER,"ENEMIGOS");
            if((*imgs).img_basico!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).img_basico,0,0,64,64,450,250,50,50,0);
            }
            al_draw_text(f_texto,al_map_rgb(255,255,255),520,260,ALLEGRO_ALIGN_LEFT,"Alien Basico: Patrulla zonas y te ataca de cerca.");

            if((*imgs).alien5!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).alien5,0,0,48,48,450,320,50,50,0);
            }
            al_draw_text(f_texto,al_map_rgb(255,255,255),520,330,ALLEGRO_ALIGN_LEFT,"Alien Disparador: Te ataca disparando rayos laser.");

            if((*imgs).intenemy!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).intenemy,0,0,64,70,450,390,50,50,0);
            }
            al_draw_text(f_texto,al_map_rgb(255,255,255),520,400,ALLEGRO_ALIGN_LEFT,"Alien Intermedio: Se desplaza rápido y dispara proyectiles.");
            al_draw_text(f_texto,al_map_rgb(100,200,255),ancho_pantalla/2,470,ALLEGRO_ALIGN_CENTER,"JEFE FINAL");
            if((*imgs).boss!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).boss,0,0,48,48,450,510,60,60,0);
            }
            al_draw_text(f_texto,al_map_rgb(255,255,255),520,525,ALLEGRO_ALIGN_LEFT,"Alien Boss: Muy resistente. Debilitalo con bombas y venenos.");
            al_draw_text(f_texto,al_map_rgb(150,150,150),ancho_pantalla/2,alto_pantalla-80,ALLEGRO_ALIGN_CENTER,"Presiona ESC para volver al menu");
            
            al_flip_display();
        }
    }
}

void mover_jefe(enemigo *boss,recursos *sonidos,estado_juego *nivel)
{
    int j,enfriamiento,probabilidad_salto; 
    float dist_x_abs,dist_y_abs,dist_x,dist_y,velocidad_boss;
    bool enfurecido;

    dist_x=heroe.x-(*boss).x;
    dist_y=heroe.y-(*boss).y;

    if(dist_x>0) 
    { 
        dist_x_abs=dist_x; 
    } 
    else 
    { 
        dist_x_abs=-dist_x; 
    }
    
    if(dist_y>0) 
    { 
        dist_y_abs=dist_y; 
    } 
    else 
    { 
        dist_y_abs=-dist_y; 
    }

    if(dist_x>0) 
    { 
        (*boss).direccion_mirada=1.0; 
    }
    else 
    { 
        (*boss).direccion_mirada=-1.0; 
    }

    enfurecido=false;
    if((*boss).vida<=500) 
    { 
        enfurecido=true; 
    }

    enfriamiento=80;
    velocidad_boss=1.5;

    if(enfurecido)
    {
        enfriamiento=40;
        velocidad_boss=3.0;
    }

    if(heroe.vida>0&&dist_x_abs<=(*boss).alcance) 
    {
        if(dist_x_abs<120.0&&dist_y_abs<150.0) 
        {
            (*boss).velocidad=0;
            (*boss).ataque=true;
            
            if((*boss).cuadro_actual==3&&(*boss).contador_animacion==0)
            {
                if(heroe.tiempo_dano==0)
                {
                    heroe.vida-=(*boss).herida;
                    heroe.tiempo_dano=30;
                }
            }
        }
        else 
        {
            (*boss).ataque=false;
            if(dist_x>0) 
            { 
                (*boss).velocidad=velocidad_boss; 
            }
            else 
            { 
                (*boss).velocidad=-velocidad_boss; 
            }

            if((*boss).energia<=0)
            {
                (*boss).ataque=true;
                (*boss).velocidad=0;
                
                for(j=0;j<MAX_LASERS;j++)
                {
                    if(!(*boss).laser_enemigos[j].activo)
                    {
                        (*boss).laser_enemigos[j].activo=true;
                        (*boss).laser_enemigos[j].tipo=MUNICION_LASER;
                        (*boss).laser_enemigos[j].distancia_recorrida=0.0;
                        (*boss).laser_enemigos[j].x=(*boss).x+40; 
                        (*boss).laser_enemigos[j].y=(*boss).y + 60;
                        
                        if(enfurecido) 
                        {
                            (*boss).laser_enemigos[j].velocidad_x=20.0*(*boss).direccion_mirada;
                        } 
                        else 
                        {
                            (*boss).laser_enemigos[j].velocidad_x=12.0*(*boss).direccion_mirada;
                        }
                        (*boss).laser_enemigos[j].velocidad_y=0.0;
                        break;
                    }
                }
                (*boss).energia=enfriamiento;
            }
        }
    }
    else 
    {
        (*boss).ataque=false;
        (*boss).velocidad=0;
    }

    if((*boss).energia>0) 
    { 
        (*boss).energia--; 
    }

    if(enfurecido) 
    { 
        probabilidad_salto=100; 
    } 
    else 
    { 
        probabilidad_salto=250; 
    }

    if((*boss).velocidad_y==0.0&&(rand()%probabilidad_salto)==0&&dist_x_abs<500.0) 
    {
        (*boss).velocidad_y=-14.0;
    }

    (*boss).velocidad_y+=0.5; 
    (*boss).y+=(*boss).velocidad_y;

    if(colision_jefe((*boss).x,(*boss).y))
    {
        if((*boss).velocidad_y>0)
        {
            (*boss).y=((int)((*boss).y+39)/TAM_TILE)*TAM_TILE-39.1;
            
            if((*boss).velocidad_y>8.0) 
            {
                if(rand()%4==0) 
                {
                    (*nivel).suelo_destruido=true;
                    (*nivel).temporizador_suelo=60;
                    printf("TERREMOTO!\n");
                }
            }
            (*boss).velocidad_y=0.0;
        }
        else if((*boss).velocidad_y<0) 
        {
            (*boss).y=((int)((*boss).y-200.0)/TAM_TILE)*TAM_TILE+200.0+TAM_TILE+0.1;
            (*boss).velocidad_y=0.0;
        }
    }
}

bool colision_jefe(float x,float y)
{
    int izq,der,aba,arr,i,j;
    char tile;

    if(x-80.0<0||x+120.0>=ancho_pantalla)
    {
        return true;
    }

    izq=(x-80.0)/TAM_TILE;
    der=(x+120.0)/TAM_TILE;
    arr=(y-200.0)/TAM_TILE;
    aba=(y+39.0)/TAM_TILE; 

    if(izq<0||der>=columna||arr<0||aba>=fila)
    {
        return true;
    }

    for(i=arr;i<=aba;i++)
    {
        for(j=izq;j<=der;j++)
        {
            tile=mapa[i][j];
            
            if(tile=='$'||tile=='1'||tile=='D'||tile=='d') 
            {
                return true;
            }
        }
    }
    return false;
}

bool colision_bombas_enemigas(float x, float y)
{
    int izq=x/TAM_TILE;
    int der=(x+30)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+30)/TAM_TILE;

    if(izq<0||der>=columna||arr<0||aba>=fila) 
    {
        return true;
    }
    if(mapa[arr][izq]=='1'||mapa[arr][izq]=='D'||mapa[arr][izq]=='3') 
    {
        return true;
    }
    if(mapa[arr][der]=='1'||mapa[arr][der]=='D'||mapa[arr][der]=='3') 
    {
        return true;
    }
    if(mapa[aba][izq]=='1'||mapa[aba][izq]=='D'||mapa[aba][izq]=='3') 
    {
        return true;
    }
    if(mapa[aba][der]=='1'||mapa[aba][der]=='D'||mapa[aba][der]=='3') 
    {
        return true;
    }

    return false;
}

void mostrar_pantalla_carga(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_titulo,ALLEGRO_FONT *f_opciones,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs)
{
    char mensaje_actual[100]="Cargando siguiente nivel...";
    char lineas[10][100];
    int total_lineas=0;
    int indice;
    
    FILE *archivo = fopen("mensajitos.txt","r");
    if (archivo != NULL)
    {
        while(fscanf(archivo,"%s",lineas[total_lineas])==1)
        {
            total_lineas++;
            if(total_lineas>=8) 
            {
                break;
            }
        }
        fclose(archivo);

        if(total_lineas>0)
        {
            indice=rand()%total_lineas;
            printf("%d\n",indice);
            strcpy(mensaje_actual,lineas[indice]);
        }
    }

    al_flush_event_queue(queue);
    al_clear_to_color(al_map_rgb(20,30,50));

    if((*imgs).fondo_menu!=NULL)
    {
        al_draw_scaled_bitmap((*imgs).fondo_menu,0,0,al_get_bitmap_width((*imgs).fondo_menu),al_get_bitmap_height((*imgs).fondo_menu),0,0,ancho_pantalla,alto_pantalla,0);
    }

    al_draw_text(f_titulo,al_map_rgb(255,215,0),ancho_pantalla/2,alto_pantalla/3,ALLEGRO_ALIGN_CENTER,"CARGANDO...");
    al_draw_text(f_opciones,al_map_rgb(200,200,200),ancho_pantalla/2,(alto_pantalla/3)+100,ALLEGRO_ALIGN_CENTER,mensaje_actual);
    al_draw_text(f_opciones,al_map_rgb(150,150,150),ancho_pantalla/2,alto_pantalla-150,ALLEGRO_ALIGN_CENTER,"Por favor espera...");
    
    al_flip_display();
    al_rest(3.0); 
}

void ranking(char *nombre,int puntaje,int nivel,ALLEGRO_DISPLAY *display,ALLEGRO_FONT *f_ranking,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs,bool modo_guardar)
{
    datos_ranking top[6];
    datos_ranking temp;
    int total_registros=0;
    int i,j,resultado,posicion,limite,mostrar_limite;
    char linea_texto[120];
    bool  en_ranking;
    
    FILE *archivo=fopen("ranking.txt","r");
    if (archivo!=NULL) 
    {
        while(total_registros<10) 
        {
            resultado=fscanf(archivo,"%s %d %d",top[total_registros].nombre,&top[total_registros].puntaje,&top[total_registros].nivel);
            if(resultado!=3) 
            {
                break;
            }

            total_registros++;
        }
        fclose(archivo);
    }
    if (modo_guardar)
    {
        if (puntaje>0&&strlen(nombre)>0)
        {
            posicion=total_registros;

            for(i=0;i<total_registros;i++)
            {
                if(puntaje>top[i].puntaje)
                {
                    posicion=i;
                    break;
                }
            }

            if(posicion<5)
            {
                for(j=total_registros;j>posicion;j--)
                {
                    if (j<5)
                    {
                        top[j]=top[j-1];
                    }
                }

                strcpy(top[posicion].nombre,nombre);
                top[posicion].puntaje=puntaje;
                top[posicion].nivel=nivel;

                if (total_registros<5)
                {
                    total_registros++;
                }
            }

            archivo=fopen("ranking.txt","w");
            if(archivo!=NULL) 
            {
                if(total_registros<5) 
                {
                    limite=total_registros;
                } 
                else 
                {
                    limite=5;
                }
                for (i=0;i<limite;i++) 
                {
                    fprintf(archivo,"%s %d %d\n",top[i].nombre,top[i].puntaje,top[i].nivel);
                }
                fclose(archivo);
            }
            printf("Puntaje guardado en el ranking!\n");
        }
        return; 
    }

    al_flush_event_queue(queue);
    en_ranking=true;

    while(en_ranking) 
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue,&evento);

        if(evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE||(evento.type==ALLEGRO_EVENT_KEY_DOWN&&evento.keyboard.keycode==ALLEGRO_KEY_ESCAPE)) 
        {
            en_ranking=false;
        }

        if(al_is_event_queue_empty(queue)) 
        {
            al_clear_to_color(al_map_rgb(20,30,50));
            if((*imgs).fondo_menu!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).fondo_menu,0,0,al_get_bitmap_width((*imgs).fondo_menu),al_get_bitmap_height((*imgs).fondo_menu),0,0,ancho_pantalla, alto_pantalla,0);
            }
            al_draw_text(f_ranking,al_map_rgb(255,215,0),ancho_pantalla/2,100,ALLEGRO_ALIGN_CENTER,"MEJORES PUNTUACIONES");
            
            if (total_registros==0) 
            {
                al_draw_text(f_ranking,al_map_rgb(255,255,255),ancho_pantalla/2,250,ALLEGRO_ALIGN_CENTER,"No existe el ranking aun.");
            }
            else 
            {
                if (total_registros<5) 
                {
                    mostrar_limite=total_registros;
                } 
                else 
                {
                    mostrar_limite=5;
                }
                for (i=0;i<mostrar_limite;i++) 
                {
                    snprintf(linea_texto,sizeof(linea_texto),"%d. %s  -  %d pts  (Nivel %d)",i+1,top[i].nombre,top[i].puntaje,top[i].nivel);
                    al_draw_text(f_ranking,al_map_rgb(255,255,255),ancho_pantalla/2,220+(i*50),ALLEGRO_ALIGN_CENTER,linea_texto);
                }
            }
            
            al_draw_text(f_ranking,al_map_rgb(150,150,150),ancho_pantalla/2,alto_pantalla-100,ALLEGRO_ALIGN_CENTER,"Presiona ESC para volver");
            al_flip_display();
        }
    }
}

void the_final(ALLEGRO_DISPLAY *display,ALLEGRO_FONT *fuente_titulo,ALLEGRO_EVENT_QUEUE *queue,recursos *imgs)
{
    al_flush_event_queue(queue);
    al_clear_to_color(al_map_rgb(0,0,0));

    if((*imgs).img_final!=NULL)
    {
        al_draw_scaled_bitmap((*imgs).img_final,0,0,al_get_bitmap_width((*imgs).img_final),al_get_bitmap_height((*imgs).img_final),0,0,ancho_pantalla,alto_pantalla,0);
    }
    al_draw_text(fuente_titulo,al_map_rgb(255,215,0),ancho_pantalla/2,alto_pantalla/2-50,ALLEGRO_ALIGN_CENTER,"HAS SIDO LIBERADO DE LOS ALIENS!");

    al_flip_display();
    al_rest(5.0); 
}

bool cargar_recursos(recursos *imgs)
{
    (*imgs).fondo_menu=al_load_bitmap("fondo_menu.png");
    if(!(*imgs).fondo_menu)
    {
        printf("Error fatal: No se encontro 'fondo_menu.png'\n");
        return false;
    }
    (*imgs).fondo_nivel1 = al_load_bitmap("Fondo1.png");
    if(!(*imgs).fondo_nivel1)
    { 
        printf("Error fatal: No se encontro 'Fondo1.png'\n"); 
        return false; 
    }

    (*imgs).img_ataque = al_load_bitmap("prota_ataque.png");
    if(!(*imgs).img_ataque) 
    { 
        printf("Error fatal: No se encontro 'prota_ataque.png'\n"); 
        return false; 
    }

    (*imgs).img_dano = al_load_bitmap("prota_auch.png");
    if(!(*imgs).img_dano) 
    { 
        printf("Error fatal: No se encontro 'prota_auch.png'\n"); 
        return false; 
    }

    (*imgs).img_caminar = al_load_bitmap("prota_caminar.png");
    if(!(*imgs).img_caminar) 
    { 
        printf("Error fatal: No se encontro 'prota_caminar.png'\n"); 
        return false; 
    }

    (*imgs).img_correr = al_load_bitmap("prota_corre.png");
    if(!(*imgs).img_correr) 
    { 
        printf("Error fatal: No se encontro 'prota_corre.png'\n"); 
        return false; 
    }

    (*imgs).img_disparo = al_load_bitmap("prota_disparo.png");
    if(!(*imgs).img_disparo) 
    { 
        printf("Error fatal: No se encontro 'prota_disparo.png'\n"); 
        return false; 
    }

    (*imgs).img_muerte = al_load_bitmap("prota_muerte.png");
    if(!(*imgs).img_muerte) 
    { 
        printf("Error fatal: No se encontro 'prota_muerte.png'\n"); 
        return false; 
    }

    (*imgs).img_recarga = al_load_bitmap("prota_recarga.png");
    if(!(*imgs).img_recarga) 
    { 
        printf("Error fatal: No se encontro 'prota_recarga.png'\n"); 
        return false; 
    }

    (*imgs).img_salto = al_load_bitmap("prota_salto.png");
    if(!(*imgs).img_salto) 
    { 
        printf("Error fatal: No se encontro 'prota_salto.png'\n"); 
        return false; 
    }

    (*imgs).img_idle = al_load_bitmap("prota_Idle.png");
    if(!(*imgs).img_idle) 
    { 
        printf("Error fatal: No se encontro 'prota_Idle.png'\n"); 
        return false; 
    }

    (*imgs).img_basico = al_load_bitmap("alien_basico.png");
    if(!(*imgs).img_basico) 
    { 
        printf("Error fatal: No se encontro 'alien_basico.png'\n"); 
        return false; 
    }

    (*imgs).textura_luna=al_load_bitmap("tile_lunar.png");
    if(!(*imgs).textura_luna) 
    { 
        printf("Error fatal: No se encontro 'tile_lunar.png'\n"); 
        return false; 
    }

    (*imgs).pocion_vida=al_load_bitmap("vida.png");
    if(!(*imgs).pocion_vida) 
    { 
        printf("Error fatal: No se encontro 'vida.png'\n"); 
        return false; 
    }

    (*imgs).alien5=al_load_bitmap("alien5.png");
    if(!(*imgs).alien5)
    {
        printf("Error fatall: No se encontro 'alien5.png'\n");
        return false;
    }

    (*imgs).veneno=al_load_bitmap("veneno.png");
    if(!(*imgs).veneno)
    {
        printf("Error fata: No se encontro 'veneno.png'\n");
        return false;
    }

    (*imgs).llave_roja=al_load_bitmap("llave_roja.png");
    if(!(*imgs).llave_roja)
    {
        printf("Error fatal: No se encontro'llave_roja.png'\n");
        return false;
    }

    (*imgs).llave_final=al_load_bitmap("llave_final.png");
    if(!(*imgs).llave_final)
    {
        printf("Error fatal: No se encontro 'llave_final.png'\n");
        return false;
    }

    (*imgs).puas=al_load_bitmap("puas.png");
    if(!(*imgs).puas)
    {
        printf("Error fatal: No se encontro 'puas.png'\n");
        return false;
    }

    (*imgs).portal=al_load_bitmap("portal.png");
    if(!(*imgs).portal)
    {
        printf("Error fatal: No se encontro 'portal.png'\n");
        return false;
    }

    (*imgs).punto=al_load_bitmap("point.png");
    if(!(*imgs).punto)
    {
        printf("Error fatal: No se encontro 'point.png'\n");
        return false;
    }

    (*imgs).municion=al_load_bitmap("recarga.png");
    if(!(*imgs).municion)
    {
        printf("Error fatal: No se encontro 'recarga.png'\n");
        return false;
    }

    (*imgs).door=al_load_bitmap("puerta.png");
    if(!(*imgs).door)
    {
        printf("Error fatal: No se encontro 'puerta.png'\n");
        return false;
    }

    (*imgs).palanca=al_load_bitmap("palanca.png");
    if(!(*imgs).palanca)
    {
        printf("Error fatal: No se encontro 'palanca.png'\n");
        return false;
    }

    (*imgs).llave_verde=al_load_bitmap("llave_verde.png");
    if(!(*imgs).llave_verde)
    {
        printf("Error fatal: No se encontro'llave_verde.png'\n");
        return false;
    }

    (*imgs).llave_azul=al_load_bitmap("llave_azul.png");
    if(!(*imgs).llave_azul)
    {
        printf("Error fatal: No se encontro'llave_azul.png'\n");
        return false;
    }

    (*imgs).llave_amarilla=al_load_bitmap("llave_amarilla.png");
    if(!(*imgs).llave_amarilla)
    {
        printf("Error fatal: No se encontro 'llave_amarilla.png'\n");
        return false;
    }

    (*imgs).intenemy=al_load_bitmap("alien6.png");
    if(!(*imgs).intenemy)
    {
        printf("Error fatal: No se encontro 'alien6.png'\n");
        return false;
    }

    (*imgs).boss=al_load_bitmap("alienboss.png");
    if(!(*imgs).boss)
    {
        printf("Error fatal: No se encontro 'alienboss.png'\n");
        return false;
    }

    (*imgs).checkpoint=al_load_bitmap("checkpoint.png");
    if(!(*imgs).checkpoint)
    {
        printf("Error fatal: No se encontro 'checkpoint.png'\n");
        return false;
    }

    (*imgs).bombita=al_load_bitmap("bomba.png");
    if(!(*imgs).bombita)
    {
        printf("Error fatal: No se encontro 'bombita.png'\n");
        return false;
    }

    (*imgs).img_quema=al_load_bitmap("quemador.png");
    if(!(*imgs).img_quema)
    {
        printf("Error fatal: No se encontro 'quemador.png'\n");
        return false;
    }

    (*imgs).proteccion=al_load_bitmap("escudo.png");
    if(!(*imgs).proteccion)
    {
        printf("Error fatal: No se encontro 'escudo.png'\n");
        return false;
    }

    (*imgs).linternita=al_load_bitmap("Linterna.png");
    if(!(*imgs).linternita)
    {
        printf("Error fatal: No se encontro 'Linterna.png'\n");
        return false;
    }

    (*imgs).soy_mapa=al_load_bitmap("minimapa.png");
    if(!(*imgs).soy_mapa)
    {
        printf("Error fatal: No se encontro 'minimapa.png'\n");
        return false;
    }

    (*imgs).bomba_enemiga=al_load_bitmap("bombotas.png");
    if(!(*imgs).bomba_enemiga)
    {
        printf("Error fatal: No se encontro 'bombotas.png'\n");
        return false;
    }

    (*imgs).big_map=al_load_bitmap("map.png");
    if(!(*imgs).big_map)
    {
        printf("Error fatal: No se encontro 'map.png'\n");
        return false;
    }

    (*imgs).img_final=al_load_bitmap("Final.jpg");
    if(!(*imgs).img_final)
    {
        printf("Error fatal: No se encontro 'Final.jpg'\n");
        return false;
    }

    (*imgs).disparo_prota=al_load_sample("laser_prota.ogg");
    if(!(*imgs).disparo_prota)
    {
        printf("Error fatal: No se encontro 'laser_prota.ogg'\n");
        return false;
    }

    (*imgs).disparo_alien5=al_load_sample("laser_alien5.ogg");
    if(!(*imgs).disparo_alien5)
    {
        printf("Error fatal: No se encontro 'laser_alien5.ogg'\n");
        return false;
    }

    (*imgs).disparo_alien6=al_load_sample("laser_alien6.ogg");
    if(!(*imgs).disparo_alien6)
    {
        printf("Error fatal: No se encontro 'laser_alien6.ogg'\n");
        return false;
    }

    (*imgs).sonido_equipar=al_load_sample("equipar.ogg");
    if(!(*imgs).sonido_equipar)
    {
        printf("Error fatal: No se encontro 'equipar.ogg'\n");
        return false;
    }

    (*imgs).sonido_proyectil=al_load_sample("impacto_proyectil.ogg");
    if(!(*imgs).sonido_proyectil)
    {
        printf("Error fatal: No se encontro 'impacto_proyectil.ogg'\n");
        return false;
    }

    (*imgs).sonido_llaves=al_load_sample("llaves.ogg");
    if(!(*imgs).sonido_llaves)
    {
        printf("Error fatal: No se encontro 'llaves.ogg'\n");
        return false;
    }

    (*imgs).sonido_moneda=al_load_sample("moneda.ogg");
    if(!(*imgs).sonido_moneda)
    {
        printf("Error fatal: No se encontro 'moneda.ogg'\n");
        return false;
    }

    (*imgs).sonido_municion=al_load_sample("municion.ogg");
    if(!(*imgs).sonido_municion)
    {
        printf("Error fatal: No se encontro 'municion.ogg'\n");
        return false;
    }

    (*imgs).sonido_portal=al_load_sample("portal.ogg");
    if(!(*imgs).sonido_portal)
    {
        printf("Error fatal: No se encontro 'portal.ogg'\n");
        return false;
    }

    (*imgs).sonido_saltar=al_load_sample("saltar.ogg");
    if(!(*imgs).sonido_saltar)
    {
        printf("Error fatal: No se encontro 'saltar.ogg'\n");
        return false;
    }

    (*imgs).sonido_vida=al_load_sample("vida.ogg");
    if(!(*imgs).sonido_vida)
    {
        printf("Error fatal: No se encontro 'vida.ogg'\n");
        return false;
    }

    (*imgs).sonido_pasos=al_load_sample("pasos.ogg");
    if(!(*imgs).sonido_pasos)
    {
        printf("Error fatal: No se encontro 'pasos.ogg'\n");
        return false;
    }

    (*imgs).sonido_palanca=al_load_sample("palanca.ogg");
    if(!(*imgs).sonido_palanca)
    {
        printf("Error fatal: No se encontro 'palanca.ogg'\n");
        return false;
    }

    (*imgs).sonido_puerta=al_load_sample("door_open.ogg");
    if(!(*imgs).sonido_puerta)
    {
        printf("Error fatal: No se encontro 'door_open.ogg'\n");
        return false;
    }

    (*imgs).pantalla_carga=al_load_sample("carga.ogg");
    if(!(*imgs).sonido_puerta)
    {
        printf("Error fatal: No se encontro 'carga.ogg'\n");
        return false;
    }

    return true; 
}

void limpieza(recursos *imgs,ALLEGRO_FONT *fuente,ALLEGRO_TIMER *timer,ALLEGRO_EVENT_QUEUE *queue,ALLEGRO_DISPLAY *display)
{
    al_destroy_bitmap((*imgs).fondo_menu);
    al_destroy_bitmap((*imgs).fondo_nivel1);
    al_destroy_bitmap((*imgs).img_ataque);
    al_destroy_bitmap((*imgs).img_dano);
    al_destroy_bitmap((*imgs).img_caminar);
    al_destroy_bitmap((*imgs).img_correr);
    al_destroy_bitmap((*imgs).img_disparo);
    al_destroy_bitmap((*imgs).img_muerte);
    al_destroy_bitmap((*imgs).img_recarga);
    al_destroy_bitmap((*imgs).img_salto);
    al_destroy_bitmap((*imgs).img_idle);
    al_destroy_bitmap((*imgs).img_basico);
    al_destroy_bitmap((*imgs).pocion_vida);
    al_destroy_bitmap((*imgs).textura_luna);
    al_destroy_bitmap((*imgs).alien5);
    al_destroy_bitmap((*imgs).veneno);
    al_destroy_bitmap((*imgs).llave_final);
    al_destroy_bitmap((*imgs).llave_roja);
    al_destroy_bitmap((*imgs).llave_verde);
    al_destroy_bitmap((*imgs).llave_azul);
    al_destroy_bitmap((*imgs).llave_amarilla);
    al_destroy_bitmap((*imgs).portal);
    al_destroy_bitmap((*imgs).punto);
    al_destroy_bitmap((*imgs).puas);
    al_destroy_bitmap((*imgs).municion);
    al_destroy_bitmap((*imgs).palanca);
    al_destroy_bitmap((*imgs).door);
    al_destroy_bitmap((*imgs).intenemy);
    al_destroy_bitmap((*imgs).big_map);
    al_destroy_bitmap((*imgs).img_final);
    al_destroy_sample((*imgs).disparo_prota);
    al_destroy_sample((*imgs).disparo_alien5);
    al_destroy_sample((*imgs).disparo_alien6);
    al_destroy_sample((*imgs).sonido_portal);
    al_destroy_sample((*imgs).sonido_municion);
    al_destroy_sample((*imgs).sonido_moneda);
    al_destroy_sample((*imgs).sonido_llaves);
    al_destroy_sample((*imgs).sonido_proyectil);
    al_destroy_sample((*imgs).sonido_equipar);
    al_destroy_sample((*imgs).sonido_palanca);
    al_destroy_sample((*imgs).sonido_puerta);
    al_destroy_sample((*imgs).sonido_pasos);
    al_destroy_font(fuente);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
}