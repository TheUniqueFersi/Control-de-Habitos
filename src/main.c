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
void IUSD();
void llamarINSERT();
void llamarUPDATE();
void llamarSELECT();
void llamarDELETE();
void creacionEstructuras();
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
    //al_draw_rectangle(30, 250, 150, 300, al_map_rgb(255, 0, 0), 3);
    al_draw_filled_rectangle(0, 0, 100, 700, al_map_rgb(255, 0, 0));
    al_draw_scaled_bitmap(HABITOSROSA, 0, 0, 100, 175, 0, 0,100, 175, 0);
    al_draw_scaled_bitmap(CALENDARIO, 0, 0, 100, 175, 0, 175,100, 175, 0);
    al_draw_scaled_bitmap(RECORDS, 0, 0, 100, 175, 0, 350,100, 175, 0);
    al_draw_scaled_bitmap(AJUSTES, 0, 0, 100, 175, 0, 525,100, 175, 0);
    al_draw_filled_rectangle(1000, 0, 1200, 700, al_map_rgb(255, 255, 255));
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
                    break;
            }
        }
    }
}
void IUSD(){
    int opcion;
    printf("1.Insert.\n2.Update\n3.Select\n4.Delete.");
    scanf("%i", &opcion);
    switch (opcion) {
        case 1:
            llamarINSERT();
            break;
        case 2:
            llamarUPDATE();
            break;
        case 3:
            llamarSELECT();
            break;
        case 4:
            llamarDELETE();
            break;
        default:
            break;
    }
}
void llamarINSERT(){}

void pedirDatosUPDATE(int opcion, int id){
    /*HABITO habit1 ={1, "HABITO 4 NUEVO OWO", "NOTA PARA 4", "2", 4, '\0', '\0', 44, '\0', '\0'};
    habit1.ID_habito = manejarAUTOINCREMENT("./data/usuarios/1/habito.dat");*/
    HABITO newHab={0};
    newHab.ID_habito = manejarAUTOINCREMENT("./data/usuarios/1/habito.dat");
    switch (opcion) {
        case 1:
            getchar();
            printf("NOMBRE DEL HABITO:");
            fgets(newHab.nombre, sizeof(newHab.nombre), stdin);
            printf("Nota del habito:");
            fgets(newHab.nota, sizeof(newHab.nota), stdin);
            printf("Repeticion semanal:");
            fgets(newHab.repeticion_semanal, sizeof(newHab.repeticion_semanal), stdin);

            printf("Racha:");
            char racha[2];
            sprintf(racha,"%c", newHab.racha);
            fgets(racha, sizeof(newHab.racha), stdin);
            newHab.racha = atoi(racha);

            UPDATE("./data/usuarios/1/habito.dat", &newHab, sizeof(HABITO), 1, id);


    }

}
void llamarUPDATE(){
    HABITO habNULL={0};
    USUARIO usuNULL={0};
    HORARIO  horNULL={0};
    RECORDATORIOS recNULL={0};
    int opcion, id;
    char *ruta[] ={};


    printf("Estructura:\n1.Habito\n2.Usuario\n3.Horario\n4.Recordatorio\n");
    scanf("%i", &opcion);

    printf("ID: ");
    scanf("%i", &id);
    switch (opcion) {
        case 1:
//            *ruta = "./data/usuarios/1/habito.dat";
//          DELETE("./data/usuarios/1/habito.dat", &habNULL, sizeof(HABITO), 1, id);
//            SELECT("./data/usuarios/1/habito.dat", &habNULL, sizeof(HABITO), 1, id);
            pedirDatosUPDATE(opcion, id);


            break;
        case 2:
            *ruta = "./data/usuarios/1/usuario.dat";

            break;

        case 3:
            *ruta = "./data/usuarios/1/horario.dat";
            break;

        case 4:
            *ruta = "./data/usuarios/1/recordatorio.dat";
            break;

        default:
            break;
    }

}
void llamarSELECT(){
    HABITO habNULL={0};
    USUARIO usuNULL={0};
    HORARIO  horNULL={0};
    RECORDATORIOS recNULL={0};
    int opcion, id;
    char *ruta[] ={};

    printf("Estructura:\n1.Habito\n2.Usuario\n3.Horario\n4.Recordatorio\n");
    scanf("%i", &opcion);

    printf("ID: ");
    scanf("%i", &id);
    switch (opcion) {
        case 1:
//            *ruta = "./data/usuarios/1/habito.dat";
//            DELETE("./data/usuarios/1/habito.dat", &habNULL, sizeof(HABITO), 1, id);
            SELECT("./data/usuarios/1/habito.dat", &habNULL, sizeof(HABITO), 1, id);
            break;
        case 2:
            *ruta = "./data/usuarios/1/usuario.dat";

            break;

        case 3:
            *ruta = "./data/usuarios/1/horario.dat";
            break;

        case 4:
            *ruta = "./data/usuarios/1/recordatorio.dat";
            break;

        default:
            break;
    }


}
void llamarDELETE(){
    HABITO habNULL={0};
    USUARIO usuNULL={0};
    HORARIO  horNULL={0};
    RECORDATORIOS recNULL={0};
    int opcion, id;
    char *ruta[] ={};


    printf("Estructura:\n1.Habito\n2.Usuario\n3.Horario\n4.Recordatorio\n");
    scanf("%i", &opcion);

    printf("ID: ");
    scanf("%i", &id);
    switch (opcion) {
        case 1:
//            *ruta = "./data/usuarios/1/habito.dat";
            DELETE("./data/usuarios/1/habito.dat", &habNULL, sizeof(HABITO), 1, id);
//            SELECT("./data/usuarios/1/habito.dat", &habNULL, sizeof(HABITO), 1, id);
            break;
        case 2:
            *ruta = "./data/usuarios/1/usuario.dat";

            break;

        case 3:
            *ruta = "./data/usuarios/1/horario.dat";
            break;

        case 4:
            *ruta = "./data/usuarios/1/recordatorio.dat";
            break;

        default:
            break;
    }
}

void creacionEstructuras(){
    /*Dificultad*/
    DIFICULTAD dif1 = {1, "Muy facil"};
    DIFICULTAD dif2 = {2, "Facil"};
    DIFICULTAD dif3 = {3, "Intermedio"};
    DIFICULTAD dif4 = {4, "Dificil"};
    DIFICULTAD dif5 = {5, "Muy dificil"};

    /*USUARIO*/
    USUARIO usu1 = {1, "Luillilol"};
    USUARIO usu2 = {2, "Fersa"};
    USUARIO usu3 = {3, "José"};
    USUARIO usu4 = {4, "Arias"};
    USUARIO usu5 = {5, "Raz"};

    /*TIPO */
    TIPO tip1 = {1, "Personal"};
    TIPO tip2 = {2, "Escuela"};
    TIPO tip3 = {3, "Salud"};

    /*HABITO*/
    time_t tiempo= time(NULL);
    struct tm *infoTiempo;
    time(&tiempo);
    infoTiempo = localtime(&tiempo);
    FECHA miFecha = *infoTiempo;
    printf("Fecha actual: %d/%02d/%02d %02d:%02d:%02d\n",
           miFecha.tm_year + 1900, miFecha.tm_mon + 1, miFecha.tm_mday,
           miFecha.tm_hour, miFecha.tm_min, miFecha.tm_sec);

    HABITO hab1 = {1, "Ir al Gym", "Llevar toalla", "0010110", 1, &tip3, &dif3, 12, tiempo, miFecha};
    HABITO hab2 = {1, "Krunkear", "Un ratito", "1010010", 5, &tip1, &dif1, 35,  tiempo, miFecha};
    HABITO hab3 = {1, "Hacer la tarea", "Pa mañana", "0000001", 7, &tip2, &dif3, 2,  tiempo, miFecha};
    HABITO hab4 = {1, "Una paja a la crema", "Es una buena paja", "1111111", 5, &tip1, &dif1, 100,  tiempo, miFecha};

    /*REGISTROH-HABITOS*/
    time_t tiempo2= time(NULL);
    struct tm *infoTiempo2;
    time(&tiempo2);
    infoTiempo2= localtime(&tiempo2);
    FECHA miFecha2 = *infoTiempo2;
    printf("Fecha actual: %d/%02d/%02d %02d:%02d:%02d\n",
           miFecha2.tm_year + 1900, miFecha2.tm_mon + 1, miFecha2.tm_mday,
           miFecha2.tm_hour, miFecha2.tm_min, miFecha2.tm_sec);

    REGISTRO_HABITOS reg_hab1 = {1, &hab1, tiempo2, miFecha2, 1, 0};
    REGISTRO_HABITOS reg_hab2 = {2, &hab2, tiempo2, miFecha2, 4, 1};
    REGISTRO_HABITOS reg_hab3 = {3, &hab3, tiempo2, miFecha2, 1, 0};
    REGISTRO_HABITOS reg_hab4 = {4, &hab4, tiempo2, miFecha2, 5, 0};

    /*HORARIO*/
    HORARIO horario1 = {1, "Algebra", "1000100", &tip2, tiempo, miFecha2, miFecha2, miFecha2};
    HORARIO horario2 = {2, "Curso Progra", "1010101", &tip2, tiempo, miFecha2, miFecha2, miFecha2};
    HORARIO horario3 = {3, "Calculo", "0111000", &tip2, tiempo, miFecha2, miFecha2, miFecha2};

    /*HORA_HORARIO*/
    HORA_HORARIO hor_hor1={1, &horario1, tiempo, miFecha2, miFecha2};
    HORA_HORARIO hor_hor2={2, &horario1, tiempo, miFecha2, miFecha2};
    HORA_HORARIO hor_hor3={3, &horario2, tiempo, miFecha2, miFecha2};
    HORA_HORARIO hor_hor4={4, &horario2, tiempo, miFecha2, miFecha2};

    /*RECORDATORIOS*/
    RECORDATORIOS recor1 = {1, "EXAMEN FINAL PIÑA", &tip2, tiempo, miFecha2, 0};
    RECORDATORIOS recor2 = {2, "Serie Algebra", &tip2, tiempo, miFecha2, 1};
    RECORDATORIOS recor3 = {3, "Salida con amigos", &tip1, tiempo, miFecha2, 0};
    RECORDATORIOS recor4 = {4, "Cita con Doctor", &tip3, tiempo, miFecha2, 1};

    /*PRODUCTIVIDAD*/
    PRODUCTIVIDAD product1= {1, tiempo, miFecha2, 7, 6};
    PRODUCTIVIDAD product2= {2, tiempo, miFecha2, 8, 7};
    PRODUCTIVIDAD product3= {3, tiempo, miFecha2, 12, 10};















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
        HABITO habit1 ={1, "HABITO 4 NUEVO OWO", "NOTA PARA 4", "2", 4, '\0', '\0', 44, '\0', '\0'};
        habit1.ID_habito = manejarAUTOINCREMENT("./data/usuarios/1/habito.dat");
//        UPDATE("./data/usuarios/1/habito.dat", &habit1, sizeof(HABITO), 1, 4);
//      DELETE("./data/usuarios/1/habito.dat", &habit1, sizeof(HABITO), 1, 1);
//      SELECT("./data/usuarios/1/habito.dat", &habit1, sizeof(HABITO), 1, 4);
//        IUSD();
//        EJEMPLO ej1 ={"AQUI EJEMPLO", 208};
//        UPDATE("./data/usuarios/1/ejemplo.dat", &ej1, sizeof(EJEMPLO), 1);

        /*CREACIÓN DE ESTRUCTURAS*/
        creacionEstructuras();



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

