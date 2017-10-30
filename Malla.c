/****************************************BIBLIOTECAS****************************************/
#include <stdio.h> //Contiene prototipos de funciones para manipular datos de entrada y salida
#include <graphics.h> //Contiene prototipos de funciones para preparar y manipular gráficos en el entorno MS-DOS
#include <stdio.h> //Contiene prototipos de funciones para utilidades de uso general
#include <time.h> //Contiene prototipos de funciones para uso de tiempo
/*******************************************************************************************/

/****************************************CONSTANTES****************************************/
#define X 1280 //Limite para la creacion de nodos en posicion horizontal
#define Y 760 //Limite para la creacion de nodos en posicion vertical
#define L 40 //Tamaño de cada cuadro
#define T X-200
/******************************************************************************************/

/****************************************ESTRUCTURAS****************************************/
typedef struct mal{ //Declaracion de estructura
    int x,y; //Coordenadas para tamaño de cuadro
    struct mal *sig,*ant,*up,*dw; //Nodos arriba,abajo,dercha e izquierda
    struct mal *obj; //Nodo auxiliar para saber si hay un enemigo u obstaculo sera NULL en caso de no haber nada
}MALLA; //Nombre de estructura

typedef struct{ //Declaracion de estructura
    MALLA *pos; //Nodo para moverse por la malla
    int vidas; //Numero de vidas
}JUGADOR; //Nombre de estructura

typedef struct nod{ //Declaracion de estructura
    int x,y; //Coordenadas para graficar enemigo
    int tp; //Tipo de enemigo
    int vida; //Vida de enemigo
    struct nod *sig,*ant,*obj; //Nodos para recorrer o eliminar enemigos
}NODOD; //Nombre de estructura

typedef struct{ //Declaracion de estructura
    NODOD *prim,*ult; //Nodos derecho e izquierdo y nodo que apunta a la malla
}ENEMIGO; //Nombre de estructura
/*******************************************************************************************/

/****************************************VARIABLES GLOBALES***************************************************/
int ie=0; //Variable para cambio de orientacion de personaje 0-3 (arriba,abajo,izquierda,derecha)
int iem=0;
int level=0; //Variable para saber el nivel actual
/*******************************************************************************************/

/****************************************PROTOTIPOS_DE_FUNCIONES****************************************/
MALLA* CreaNodo(int d,int e); //Ingresa datos en un nodo
void iniListaD(ENEMIGO *E); //Inicializa los nodos para creacion de enemigos
NODOD* CreaNodoD(int d,int e,int t, int v); //Ingresa datos en un nodo para una lista doblemente enlazada
void CreaEnemigo(ENEMIGO *cab,int d,int e,int t,int v);
void CreaMalla(MALLA **cab); //Creacion de malla de nodos
void DibujaMalla(MALLA *cab); //Graficacion de malla de nodos*/
void movimiento(MALLA *cab,JUGADOR *P,ENEMIGO *E); //Captura de teclas para mover personaje
void abrir(int col[L-1][L-1]); //Apertura de archivo de personaje principal
void abriren(int col[L-1][L-1]); //Apertura de archivo de enemigo
void borra(int x,int y); //Cambio de color de personaje al de fondo
void dibuja(int col[L-1][L-1],int x,int y); //Graficacion de personaje
void dibujae(int col[L-1][L-1],int x,int y); //Graficacion de enemigo
void movenemigos(MALLA *cab,ENEMIGO *E);
/*******************************************************************************************************/

/****************************************FUNCION_PRINCIPAL****************************************/
int main(){ //Inicio de funcion principal
    initwindow(1280,800,"MALLANODOS"); //Inicializacion de modo grafico
    rectangle(T,0,X-5,Y); //Tamaño de tablero de datos de partia en curso
    rectangle(0,0,T,Y); //Tamaño de ventana de juego
    MALLA *M=NULL; //Declaracion de malla
    JUGADOR  P; //Declaracion de jugador
    ENEMIGO E;//Declaracion de enemigo
    CreaMalla(&M); //Llamado a funcion de creacion de malla
    iniListaD(&E);
    movimiento(M,&P,&E); //Llamado a funcion de movimiento
    system("pause"); //Pausa del sistema para visualizacion del modo grafico
} //Fin de funcion principal
/*************************************************************************************************/

/****************************************FUNCIONES****************************************/
MALLA* CreaNodo(int d,int e){ //Inicio de funcion para creacion de nodo paso por valor de coordenadas x,y
    MALLA *nuevo; //Declaracion de nodo
    nuevo=(MALLA*)malloc(sizeof(MALLA)); //Creacion de nodo
    if(nuevo){ //Comprobación de la existencia de nodo nuevo
        nuevo->x=d; //Inicialización de coordenada en x
        nuevo->y=e; //Inicialización de coordena en y
        nuevo->sig=nuevo->ant=nuevo->dw=nuevo->up=NULL; //Declaracion para saber donde continua el nodo siguiente
        nuevo->obj=NULL; //Nodo para saber si hay un enemigo u objeto bloqueando el paso
    } //Fin de comprobación
    return(nuevo); //Retorno de nodo con datos
} //Fin de funcion

void iniListaD(ENEMIGO *E){ //Inicio de funcion para inicialzar lista de enemigos
    E->prim=NULL; //Nodo izquierdo inicializado como nulo
    E->ult=NULL; //Nodo derecho inicializado como nulo
} //Fin de funcion

NODOD* CreaNodoD(int d,int e,int t, int v){ //Inicio de funcion para creacion de nodo paso por valor de coordenadas x,y
    NODOD *nuevo; //Declaracion de nodo
    nuevo=(NODOD*)malloc(sizeof(NODOD)); //Creacion de nodo
    if(nuevo){ //Comprobación de la existencia de nodo nuevo
        nuevo->x=d; //Declaracion de coordenada en x
        nuevo->y=e; //Declaracion de coordenada en y
        nuevo->tp=t; //Declaracion de  tipo de enemigo
        nuevo->vida=v; //Declaracion de vida de enemigo
        nuevo->sig=nuevo->ant=NULL; //Declaracion para saber donde continua el nodo siguiente o el anterior
    } //Fin de comprobación
    return(nuevo); //Retorno de nodo con datos
} //Fin de funcion

void CreaMalla(MALLA **cab){ //Inicio de funcion para creacion de datos de la malla de nodos
    int i,j,n,m,x,y; //Declaracion de variables de contadores
    m=(T)/L; //Numero maximo de nodos en horizontal
    n=Y/L; //Numero maximo de nodos en vertical
    x=y=1; //Inicializacion de x e y(Inicializado en 1 para evitar borrar los margenes de ventana de juego y de informacion de partida)
    MALLA *nuevo; //Declaracion de nodo principal
    MALLA *auxarriba=NULL; //Declaracion de nodo auxiliar para vinculacion de nodo arriba
    MALLA *auxanterior=NULL; //Declaracion de nodo auxiliar para vinculacion de nodo anterior
    MALLA *aux=NULL; //Declaracion de nodo auxiliar para recorrido de nodos
    MALLA *aux2=NULL;
    for(i=0;i<n;i++){ //Inicio de ciclo para recorrido en y
        x=1; //Inicializacion de x para evitar borrar limite de ventana de juego
        if(i==18){ //Comprobacion en recorrido vertical para evitar borrar limite de la ventana de juego
            y=-1; //Se le resta uno a y para no llegar al limite de la ventana de juego
        } //Fin de comprobacion
        for(j=0;j<m;j++){ //Inicio de ciclo para recorrido en x
            if(j==25){ //Limite en x para evitar borrar limite de ventana de juego
                x=-1; //Se le resta uno a x para no llegar al limite de la ventana de juego
            }//Fin de comprobacion
            nuevo=CreaNodo(x+(j*L),y+(i*L)); //Llamado a funcion para crear nodo
            if(nuevo){ //Comprobacion de la existencia del nodo para empezar a crear nodos
                if(!*cab){ //Iteracion para enlazamiento de nodo
                    *cab=nuevo; //Enlazamiento de cabecera
                    aux=nuevo; //Declaracion de nodo auxiliar para ir al siguiente nodo
                    auxarriba=nuevo; //Declaracion de auxiliar para recorrer hilera de arriba
                    aux2=nuevo; //Declaracion de inicio de hilera de arriba
                    auxanterior=nuevo; //Declaracion de nodo anterior
                } //Fin de iteracion
                else{ //Iteracion para enlazamiento de nodos
                    nuevo->ant=auxanterior; //Enlazamiento con nodo anterior
                    auxanterior=nuevo; //Enlazamiento de nodo anterior con nodo actual;
                    auxanterior->sig=NULL; //Creacion de nodo siguiente del nodo anterior
                    aux->sig=nuevo; //Creacion de nodo siguiente
                    aux=aux->sig; //Declaracion de nodo siguiente
                    if(j!=0 && i==0){ //Iteracion creacion de hilera de arriba
                        auxarriba->sig=nuevo; //Creacion del siguiente de hilera de arriba
                        auxarriba=auxarriba->sig; // Declaracion de hilera de arriba
                    } //Fin de iteracion para creacion de hilera de arriba
                    if(j==0){ //Iteracion para inicio de auxarriba;
                        auxarriba=aux2; //Inicio de recorrido de hilera arriba
                    } //Fin de iteracion para inicializacion de ciclo
                    if(auxarriba && nuevo && i>0){ //Iteracion para recorido y enlazamiento d hilera arriba
                        auxarriba->dw=nuevo; //Enlazamiento de nodo arriba con nodo actual
                        //printf("%d\t%d\n",auxarriba->x,auxarriba->y);
                        nuevo->up=auxarriba; //Enlazamiento de nodo actual con nodo arriba
                        auxarriba=auxarriba->sig; //Recorrido de nodo arriba
                    } //Fin de iteracion para recorrido de hilera arriba
                    if(j==0){ //Iteracion para iniciar hilera de arriba
                        aux2=nuevo; //Creacion de inicio de hilera de arriba
                    } //fin de iteracion para iniciar hilera de arriba
                } //Fin de iteracion para enlazamiento de nodos
            } //Fin de creacion de nodos
        } //Fin de ciclo de recorrido en x
    } //Fin de ciclo de recorrido en y
} //Fin de funcion

void abrir(int col[L-1][L-1]){ //funcion para abrir archivo de personaje principal
    int i,j,cl,N,M; //Declaracion de contadores, N y M(valores no usados debido a que ya se sabe el tamaño del personaje; el cual es igual al de un cuadro de la malla
    char nombre[5]={'t','a','n','k','p'}; //Nombre del archivo de personje principal
    FILE *arch; //Declaracion de un puntero de tipo archivo
    arch=fopen(nombre,"rb");//Apertura de archivo
    if(arch){ //Comprobacion de apertura de archivo para paso de valores
        fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
        fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
        for(i=0;i<L-1;i++){ //Ciclo en vertical para asignar los colores del personaje
            for(j=0;j<L-1;j++){ //Ciclo en horizontal para asignar los colores del personaje
                fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
            } //Fin de recorrido horizontal
        } //Fin de recorrido vertical
    } //Fin de paso de valores del archivo
    fclose(arch); //Cierre de archivo
} //Fin de funcion

void CreaEnemigo(ENEMIGO *cab,int d,int e,int t,int v){ //Inicio de funcion para crear enemigos(Insercion al final)
    NODOD *nuevo; //Nodo principal
    nuevo=CreaNodoD(d,e,t,v); //Llamada a funcion CreaNodoD para ingresas datos en nodo
    if(nuevo){ //Enlazamiento de nodos
        if(!cab->ult){ //Enlazamiento de cabecera
            cab->prim=nuevo; //Declaracion de inicio por izquierda de nodo
            cab->ult=nuevo; //Declaracion de inicio por derecha de nodo
        } //Fin de enlazamiento de cabecera
        else{ //Recorrido de nodo
            cab->ult->sig=nuevo; //Recorrido de nodo
        }//Fin de recorrido de nodo
        nuevo->ant=cab->ult; //Enlazamiento de nodo anterior
        cab->ult=nuevo; //Declaracion de nodo
    } //Fin de Enlazamientos
} //Fin de funcion

void abriren(int cole[L-1][L-1]){ //funcion para abrir archivo de personaje principal
    int i,j,cl,N,M; //Declaracion de contadores, N y M(valores no usados debido a que ya se sabe el tamaño del personaje; el cual es igual al de un cuadro de la malla
    char nombre[5]={'t','a','n','k','e'}; //Nombre del archivo de personje principal
    FILE *arch; //Declaracion de un puntero de tipo archivo
    arch=fopen(nombre,"rb");//Apertura de archivo
    if(arch){ //Comprobacion de apertura de archivo para paso de valores
        fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
        fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
        for(i=0;i<L-1;i++){ //Ciclo en vertical para asignar los colores del personaje
            for(j=0;j<L-1;j++){ //Ciclo en horizontal para asignar los colores del personaje
                fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                cole[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
            } //Fin de recorrido horizontal
        } //Fin de recorrido vertical
    } //Fin de paso de valores del archivo
    fclose(arch); //Cierre de archivo
} //Fin de funcion

void movimiento(MALLA *cab,JUGADOR *P,ENEMIGO *E){ //Funcion de movimiento
    int tecla; //Variable para capturar una tecla
    int col[L-1][L-1]; //Arreglo para colores de personaje principal
    P->pos=cab; //Enlace de personaje con la malla
    for(int i=0;i<8;i++){ //Recorrido para ubicar al personaje en determinda posicion horizontal
        P->pos=P->pos->sig; //Declaracion de nodo como nodo siguiente
    } //Fin de recorrido
    for(P->pos=P->pos->dw->up;P->pos->dw!=NULL;P->pos=P->pos->dw); //Ciclo para ubicar al personaje en determinada posicion vertical
    abrir(col); //Llamado a funcion para abrir el archivo del personaje principal
    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar un personaje(personaje principal en este caso)
    do{ //Ciclo para capturar teclas
        tecla=getch(); //Captura de tecla
        switch(tecla){ //Inicio de opciones
        case 72: //Tecla flecha arriba
            if((P->pos)->up){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=0){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=0;  //Cambio de orientacion a arriba
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra orientado
                P->pos=P->pos->up; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                } //Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        case 80: //Tecla flecha abajo
            if(P->pos->dw){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=1){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=1; //Cambio de orientacion a abajo
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra orientado
                P->pos=P->pos->dw; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                }//Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        case 75: //Tecla Flecha izquierda
            if(P->pos->ant){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=2){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=2; //Cambio de orientacion a izquierda
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra en la posicion
                P->pos=P->pos->ant; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                } //Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        case 77: //Tecla Flecha derecha
            if(P->pos->sig){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=3){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=3; //Cambio de orientacion a derecha
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra en la posicion
                P->pos=P->pos->sig; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                } //Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        } //Fin de opciones para la variable tecla
    }while(tecla!=27); //Fin de ciclo el cual se rompe hasta presionar la tecla Esc
    closegraph(); //Cierre de ventana de moo grafico
} //Fin de funcion

void borra(int x,int y){ //Funcion para borrar personaje recibe posicion de pixel inicial
    int i,j,ax,ay; //Declaracion de varibles de contadores y auxiliares para coordenadas
    ax=x; //Inicializacion de auxiliar para x
    ay=y; //Inicializacion de auxiliar para y
    for(i=0;i<L;i++){ //Recorrido en vertical
        ax=x; //Inicializacion de x
        for(j=0;j<L;j++){ //Recorrido en horizontal
            putpixel(ax,ay,0); //Graficado de un pixel igual al color del fondo
            ax+=1; //Aumento en x
        } //Fin de recorrido horizontal
        ay+=1; //Aumento en y
    } //Fin de recorrido vertical
} //Fin de funcion
void movenemigos(MALLA *cab,ENEMIGO *E){
    NODOD *aux;
    int n,m=5,tp=0; //Variables para numero de enemigos, posicion de enemigos y tipo de enemigos
    int cole[L-1][L-1]; //Arreglo para colores de enemigos
    time_t t; //Variable de tiempo para posiciones aleatorias de enemigos
    srand((unsigned) time(&t)); //Semilla para posiciones aleatorias
    abriren(cole);
    if(level>0 && level <8) //Comprobacion de nivel y aumento de enemigos
        m+=3; //Aumento de enemigos
    for(int i=0;i<m;i++){
        n=rand()%L-1;
        for(int j=0;j<n;j++){
            cab=cab->sig;
        }
        if(level>5 && level <8 && i==m-2) //Comprobacion de nivel y cambio en el tipo de enemigos
        tp+=1;
        CreaEnemigo(E,cab->x,cab->y,tp,tp);
    }
    aux=E->prim;
    for(int i=0;i<m;i++){
        dibujae(cole,aux->x,aux->y);
        aux=aux->sig;
    }
}

void dibuja(int col[L-1][L-1],int x,int y){ //Funcion para graficar personaje u enemigo en diferentes orientaciones
    int i,j,ax,ay; //Declaracion de contadores y auxiliares para coordenadas
    ax=x; //Iniciliazacion de auxiliar para x
    ay=y; //Inicializacion de auxiliar para y
    if(ie==0){ //Comprobacion de orientacion arriba y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){  //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion arriba
    if(ie==1){ //Comprobacion de orientacion abajo y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion abajo
    if(ie==2){ //Comprobacion de orientacion izquierda y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
    if(ie==3){ //Comprobacion de orientacion derecha y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
} //Fin de funcion

void dibujae(int col[L-1][L-1],int x,int y){ //Funcion para graficar personaje u enemigo en diferentes orientaciones
    int i,j,ax,ay; //Declaracion de contadores y auxiliares para coordenadas
    ax=x; //Iniciliazacion de auxiliar para x
    ay=y; //Inicializacion de auxiliar para y
    if(iem==1){ //Comprobacion de orientacion arriba y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){  //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion arriba
    if(iem==0){ //Comprobacion de orientacion abajo y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion abajo
    if(iem==2){ //Comprobacion de orientacion izquierda y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
    if(iem==3){ //Comprobacion de orientacion derecha y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
} //Fin de funcion
