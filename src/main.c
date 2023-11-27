#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
//#include "../include/structs.c"
#include "../include/CRUD.c"
#include "../include/resources.c"
/* ---->  <---- */
/* ----> Prototipos <---- */
int inicializar_allegro();//INICIALIZA TODO LO NECESARIO PARA QUE ALLEGRO FUNCIONÉ
void main_habitus(int);
void actualizar_display();
int init_resources();
//termina;
/* ----> ALLEGRO {TIPO DE DATOS} <---- */
// Displays
ALLEGRO_DISPLAY *disp;

// Eventos
ALLEGRO_EVENT_QUEUE *cola_eventos;
ALLEGRO_EVENT evento;

//SONIDO
ALLEGRO_SAMPLE_ID id_SAMPLE;
ALLEGRO_SAMPLE *SAMPLE;

//BITMAPS:
ALLEGRO_BITMAP *BITMAP;

//TIMERS
ALLEGRO_TIMER *AFK;

//COLORES
ALLEGRO_FONT *fuente1;





/* ---- termina; ---- */

/* ----> VARIABLES GLOBALES <---- */
/* ----> General <---- */
int fin=0;
int momento=0; /*0: Inicio primera vez
 *              1:
 *
 *
 *
 *              */
/* ----> Nombres de archivos que manejar <---- */


char N_T_DIFICULTAD[100] = {"./../data/"};
//termina;


/* ----> ARCHIVOS <---- */
//termina;
void Dia(int dia){
    al_draw_text(lexend_regular[15], texto_black, 1015, 375, ALLEGRO_ALIGN_LEFT, "Lu Ma Mi  Ju  Vi  Sa  Do");
    for (int i = 0; i < 7; ++i) {
        if (i == dia-1) {
            al_draw_filled_rectangle(1015 + i * 25, 395, 1035 + i * 25, 415, secundario_pastel_magenta);
        } else {
            al_draw_filled_rectangle(1015 + i * 25, 395, 1035 + i * 25, 415, fondo_gris1);
        }
    }
}
void Pendientes(){
    //-ALLEGRO_PI/2.0 SE UTILIZA PARA INICIAR EN LA PARTE SUPERIOR DE LA CIRCUNFERENCIA
    //theta se trabaja en radianes
    //PasoHabitos=(-2*ALLEGRO_PI)/CantHabitosHoy
    char CantHabitosHoy[2]="05";
    //PasoRecordatorios=(-2*ALLEGRO_PI)/CantRecordatoriosHoy
    al_draw_arc(1100,75, 50,-ALLEGRO_PI/2.0,-ALLEGRO_PI,principal_pale_chestnut,15.0);
    al_draw_text(lexend_regular[30],texto_black,1100,55,ALLEGRO_ALIGN_CENTER,CantHabitosHoy);
    al_draw_arc(1100,225, 50,-ALLEGRO_PI/2.0,-ALLEGRO_PI,neutro1_tinta_de_pulpo,15.0);
    al_draw_text(lexend_regular[30], texto_black, 1100, 205, ALLEGRO_ALIGN_CENTER, CantHabitosHoy);
}
void calendario(int dia, int dia_mes, int mes){
    int FILAS = 6;
    int COLUMNAS = 7;
    int CELDA=25;
    int tipomes=mes%2;
    int dias_en_mes;
    if(mes==2){
        dias_en_mes=28;
    }else if(tipomes==0){
        dias_en_mes=31;
    }else if(tipomes==1){
        dias_en_mes=30;
    }
    float transparencia;
    for (int fila = 0; fila < FILAS; ++fila) {
        for (int columna = 0; columna < COLUMNAS; ++columna) {
            int dia_calendario = fila * COLUMNAS + columna + 1 - dia_mes;
            //Aqui va la lógica para poder hacer la transparencia en base a la cantidad de actividades que tuvo,solo que ocupo la cantidad
            if(dia_calendario%4==0)transparencia=0;
            else if(dia_calendario%4==1)transparencia=255/2;
            else if(dia_calendario%4==2)transparencia=255*3/4;
            else if(dia_calendario%4==3)transparencia=255*4/5;
            if (dia_calendario >= 1 && dia_calendario <= dias_en_mes) {
                // Dibujar calendario
                    al_draw_filled_rectangle(1012 + columna * CELDA+3, 500 + 3 + fila * CELDA,
                                             1012 + (columna + 1) * CELDA, 500 + (fila + 1) * CELDA,
                                             fondo_gris1);
                if (transparencia != 0) {
                    al_draw_filled_rectangle(1012 + columna * CELDA+3, 500+ 3 + fila * CELDA,
                                             1012 + (columna + 1) * CELDA, 500 + (fila + 1) * CELDA,
                                             al_map_rgba(248,107,234,transparencia));
                }

                // Puedes agregar el número del día del mes
                char texto_dia[3];
                snprintf(texto_dia, sizeof(texto_dia), "%d", dia_calendario);
                al_draw_text(roboto_black[10], texto_black,
                             1012 + 3 + columna * CELDA + CELDA / 2, 495 + fila * CELDA + CELDA / 2,
                             ALLEGRO_ALIGN_CENTER, texto_dia);
            }
        }
    }
}
void ObtenerHora(){
    char hora_formateada[9];
    time_t HoraActual = time(NULL);
    struct tm *info_tiempo = localtime(&HoraActual);
    char dia_formateado[60];
    strftime(hora_formateada, sizeof(hora_formateada), "%H:%M", info_tiempo);
    int dia = info_tiempo->tm_mday;
    int dia_semana = info_tiempo->tm_wday;
    int mes = info_tiempo->tm_mon + 1;  // tm_mon es 0-indexado, por lo que se suma 1
    int anio = info_tiempo->tm_year + 1900;
    sprintf(dia_formateado,"%02d/%02d/%d",dia,mes,anio);
    Pendientes();
    Dia(dia_semana);
    calendario(dia, dia_semana,mes);
    al_draw_text(lexend_regular[59], texto_black, 1100, 310, ALLEGRO_ALIGN_CENTER, hora_formateada);
    al_draw_text(lexend_regular[20], texto_black, 1100, 420, ALLEGRO_ALIGN_CENTER, dia_formateado);
}

void actualizar_display(){
    //FIGURAS PRIMITAVAS
    al_draw_filled_rectangle(0, 0, 100, 700, al_map_rgb(255, 0, 0));
    al_draw_scaled_bitmap(HABITOSROSA, 0, 0, 100, 175, 0, 0,100, 175, 0);
    al_draw_scaled_bitmap(CALENDARIOBLANCO, 0, 0, 100, 175, 0, 175,100, 175, 0);
    al_draw_scaled_bitmap(RECORDS, 0, 0, 100, 175, 0, 350,100, 175, 0);
    al_draw_scaled_bitmap(AJUSTES, 0, 0, 100, 175, 0, 525,100, 175, 0);

    al_draw_filled_rectangle(1000, 0, 1200, 700, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(101, 0, 1000, 720, al_map_rgb(47, 50, 58));
    /*al_draw_filled_rectangle(300, 90, 500, 120, al_map_rgb(214, 164, 226));
    al_draw_filled_rectangle(600, 90, 800, 120, al_map_rgb(214, 164, 226));
    //l_draw_filled_rectangle(150, 150, 950, 600, al_map_rgb(119, 86, 122));
    /*al_draw_filled_rectangle(175, 175, 925, 400, al_map_rgb(74, 63, 75));
    al_draw_filled_rectangle(190, 190, 910, 235, al_map_rgb(227, 158, 193));//227, 158, 193, 1
    al_draw_filled_rectangle(190, 250, 540, 370, al_map_rgb(227, 158, 193)); //(225, 0, 129));
    al_draw_filled_rectangle(550, 250, 910, 370, al_map_rgb(227, 158, 193));
    //al_draw_filled_rectangle(550, 250, 910, 290, al_map_rgb(225, 0, 129));
    al_draw_filled_rectangle(550, 330, 910, 370, al_map_rgb(225, 0, 129));
    al_draw_filled_rectangle(220, 270, 290, 340, al_map_rgb(225, 0, 129));
    al_draw_filled_rectangle(440, 270, 510, 340, al_map_rgb(225, 0, 129));
    al_draw_scaled_bitmap(LOGO, 0, 0, 516, 484, 488, 0,125, 125, 0);
    al_draw_filled_rectangle(340, 285, 390, 330, al_map_rgb(225, 0, 129));
    al_draw_scaled_bitmap(NUEVOHABITO, 0, 0, 738, 740, 100, 496,75, 68, 0);
    al_draw_scaled_bitmap(EDITARHABITO, 0, 0, 740, 744, 100, 564,75, 68, 0);
    al_draw_scaled_bitmap(BORRARHABITO, 0, 0, 744, 740, 100, 632,75, 68, 0);
    //al_draw_filled_circle(100, 510, 50, al_map_rgb(74, 63, 75));*/
    al_draw_filled_rounded_rectangle(325, 150, 800, 720, 100, 100, al_map_rgb(227, 218, 201));
    al_draw_filled_circle(562, 400, 238, al_map_rgb(227, 218, 201));//255, 134, 0, 1
    //al_draw_filled_pieslice(0,0,40, al_map_rgb(74, 63, 75));
    al_draw_pieslice(475, 400, 120,2.9, 3.6, al_map_rgb(255, 255, 255), 4);
    al_draw_scaled_bitmap(NUEVOHABITO, 0, 0, 738, 740, 100, 496,75, 68, 0);
    al_draw_scaled_bitmap(EDITARHABITO, 0, 0, 740, 744, 100, 564,75, 68, 0);
    al_draw_scaled_bitmap(BORRARHABITO, 0, 0, 744, 740, 100, 632,75, 68, 0);
    al_draw_scaled_bitmap(BORRARHABITO, 0, 0, 744, 740, 100, 632,75, 68, 0);


    al_draw_arc(475, 400, 120, 2.95, 0.55, al_map_rgba(255, 0, 0, 500), 20);
    al_draw_arc(475, 400, 120, 3.6, 0.6, al_map_rgba(255, 134, 0, 500), 20);//255, 134, 0, 1
    al_draw_arc(475, 400, 120, 4.3, 0.8, al_map_rgba(255, 255, 0, 500), 20);
    al_draw_arc(475, 400, 120, 5.2, 0.7, al_map_rgba(69, 183, 30, 500), 20);
    al_draw_arc(475, 400, 120, 5.95, 0.5, al_map_rgba(22, 82, 1, 500), 20);//22, 82, 1, 1
    al_draw_filled_circle(475, 400, 15, al_map_rgb(0, 0, 0));//255, 134, 0, 1
    al_draw_filled_triangle(470, 400, 480, 400, 475, 300, al_map_rgb(0, 0, 0));
    al_draw_filled_rounded_rectangle(450, 175, 675, 250, 25, 25, al_map_rgb(222, 186, 201));//222, 186, 192, 1
    al_draw_scaled_bitmap(LOGO, 0, 0, 516, 484, 360, 175,80, 80, 0);
    al_draw_filled_rounded_rectangle(625, 270, 780, 635, 25, 25, al_map_rgb(222, 186, 201));//222, 186, 192, 1
    al_draw_filled_rounded_rectangle(650, 290, 755, 330, 10, 10, al_map_rgb(146, 98, 107));//146, 98, 107, 1
    al_draw_filled_rounded_rectangle(650, 335, 755, 403, 10, 10, al_map_rgb(146, 98, 107));//146, 98, 107, 1
    al_draw_filled_rounded_rectangle(650, 408, 755, 476, 10, 10, al_map_rgb(146, 98, 107));//146, 98, 107, 1
    al_draw_filled_rounded_rectangle(650, 481, 755, 549, 10, 10, al_map_rgb(146, 98, 107));//146, 98, 107, 1
    al_draw_filled_rounded_rectangle(650, 554, 755, 622, 10, 10, al_map_rgb(146, 98, 107));//146, 98, 107, 1

    ObtenerHora();
    al_flip_display();
}
void main_habitus(int verif_iniciador_primera_vez){
    int pantalla_requiere_actualizacion=1;
    momento=verif_iniciador_primera_vez;//Si es 0, es que no se ha iniciado la aplicacion ni una vez
    while(fin!=1){
        if(al_event_queue_is_empty(cola_eventos) && pantalla_requiere_actualizacion){
            pantalla_requiere_actualizacion=0;
            actualizar_display();
        }
        else if(!al_event_queue_is_empty(cola_eventos)){
            pantalla_requiere_actualizacion = 1;
        }

        //EVENTOS SUCEDIENDO:
        al_wait_for_event(cola_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            //funcion de confirmacion() --TODO
            fin = 1;
        }else if(evento.type == ALLEGRO_EVENT_TIMER){
            actualizar_display();
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){//Evento de que perdiste el foco de la ventana
            printf("PERDISTE EL FOCO\n");
            USUARIO usuario = {98, "FERSA"}, usuarioprueba = {0};

            usuario.ID_usuario = manejarAUTOINCREMENT("./data/usuarios.dat");
            INSERT("./data/usuarios.dat", &usuario, sizeof(USUARIO), 1);

            printf("ID: %i\n\n", usuario.ID_usuario);
            //printf("\nEl tamaño al final fue de: %i", contadorBytesArch("./data/usuarios.dat"));
            printf("Apoco %lli \n", sizeof(USUARIO));
            SELECT("./data/usuarios.dat", &usuarioprueba, sizeof(USUARIO), 1, 12);
            printf("\nIDd: %i USERNAME: %s", usuarioprueba.ID_usuario, usuarioprueba.nombre);
            //contadorBytesArch("./data/app.dat");
            //FILE *archh = fopen("./data/app.dat", "rb");
            //manejarAUTOINCREMENT("./data/app.dat");
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN){//Evento de que retomaste el foco de la ventana
            //printf("RECUPERASTE EL FOCO\n");
        }
        else{//Si no fueron eventos generales de la ventana:
            switch (momento) {
                case 0:
                    switch (evento.type) {

                    }
                    break;
                case 1:
                    switch (evento.type) {

                    }
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                default:

            }
        }
    }
}
int main() {
    int acceso;

    if(inicializar_allegro()){

        disp = al_create_display(1200, 700);
        AFK= al_create_timer(30);
        al_set_window_title(disp, "Hábitus");
        //al_set_display_icon(disp, n); // --TODO
        cola_eventos = al_create_event_queue();

        al_register_event_source(cola_eventos,al_get_timer_event_source(AFK)); // FUENTE: eventos de tipo temporizador
        al_register_event_source(cola_eventos, al_get_display_event_source(disp)); // FUENTE: eventos de la ventana
        al_register_event_source(cola_eventos, al_get_keyboard_event_source());// FUENTE: eventos del teclado

        //Prueba función UPDATE
        HABITO habit1 ={1, "HABITO PARA REGISTRO 1 ", "NOTITA 1 WOW", "2", 5, '\0', '\0', 85, '\0', '\0'};
        habit1.ID_habito = manejarAUTOINCREMENT("./data/usuarios/1/habito.dat");
        //UPDATE("./data/usuarios/1/habito.dat", &habit1, sizeof(HABITO), 1, 1);
        //DELETE("./data/usuarios/1/habito.dat", &habit1, sizeof(HABITO), 1, 1);
        SELECT("./data/usuarios/1/habito.dat", &habit1, sizeof(HABITO), 1, 2);
        EJEMPLO ej1 ={"AQUI EJEMPLO", 208};
//        UPDATE("./data/usuarios/1/ejemplo.dat", &ej1, sizeof(EJEMPLO), 1);



        //ACCEDE AL ARCHIVO QUE TIENE LA INFORMACION DE INICIO DE APP POR PRIMERA VEZ
        // = acceso --TODO
        al_start_timer(AFK);
        main_habitus(0);
        al_destroy_event_queue(cola_eventos);
        al_destroy_display(disp);
        al_destroy_timer(AFK);
    }
    else{
        printf("\nOcurrio un error");// --TODO hacer un archivo txt que registre todos los errores
    }
    return 0;
}
int inicializar_allegro(){
    int verif_todo_ok=1;
    if(!al_init()) {
        printf("No se pudo iniciar Allegro");
    }
    if(!al_init_primitives_addon()){
        printf("No se iniciaron las primitivas");
        verif_todo_ok = 0;
    }
    if(!al_install_keyboard()){
        printf("No se instalo el teclado");
        verif_todo_ok = 0;
    }
    if(!al_init_image_addon()){
        printf("No se inicio image addon");
        verif_todo_ok = 0;
    }
    if(!al_install_audio()){//SONIDO
        printf("No se cargo el complemento de audio");
        verif_todo_ok = 0;
    }
    if(!al_init_acodec_addon()){//SONIDO
        printf("No se pudo cargar el complemento de codex");
        verif_todo_ok = 0;
    }
    if(!al_init_font_addon() || !al_init_ttf_addon()){
        printf("No se pudo cargar el complemento de fuentes");
        verif_todo_ok = 0;
    }
    if(!init_resources()){
        printf("Error al iniciar los recursos fuentes");
        verif_todo_ok = 0;
    }
    return verif_todo_ok;
}

