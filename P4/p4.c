#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#define MAXSIZE 2048000

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros
       pseudo-aleatorios */
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void printv(int v[], int n) {
    int i;// iterador
    printf("[");
    for (i = 0; i < n; i++){
        printf("%3d", v[i]);
        if (i != n - 1){
            printf(",");
        }
    }
    printf("]\n");
}

typedef struct {
    int vec[MAXSIZE];
    int ultimo;
} monticulo;

void intercambiar(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void test_dijkstra(){
    int vector[25];
    printf("Test algoritmo ordenación:\n");
    printf("\tVector ordenado:\n");
    ascendente(vector,25);
    printv(vector,25);
    heapsort(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    if (estaOrdenado(vector,25)) printf("Vector ordenado correctamente\n");
    else printf("ERROR: Ordenación del vector");
    printf("\n");

    printf("\tVector ordenado al revés:\n");
    descendente(vector,25);
    printv(vector,25);
    heapsort(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    if (estaOrdenado(vector,25)) printf("Vector ordenado correctamente\n");
    else printf("ERROR: Ordenación del vector");
    printf("\n");

    printf("\tVector aleatorio:\n");
    aleatorio(vector,25);
    printv(vector,25);
    heapsort(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    if (estaOrdenado(vector,25)) printf("Vector ordenado correctamente\n");
    else printf("ERROR: Ordenación del vector");
    printf("\n");
}

double medir_tiempo(void (* algoritmo)(int v[], int tam),
                    void (* f_init)(int v[], int tam), int tam, int k){
//K es el número de repeticiones en caso de tener que medir tiempos pequeños
    double t_inicio = 0.0; //tiempo de inicio
    double t_fin = 0.0; //tiempo de fin
    double t_test = 0.0; //tiempo del test
    double t_test_init = 0.0; //tiempo de test+inicializacion
    double t_init = 0.0; //tiempo de solo inicialización
    int i; //Iterador
    //f_init es la función de inicialización
    //algoritmo es la función de ordenación
    int *v;
    v = malloc(tam*sizeof(int));
    f_init(v, tam); //genera un vector de [tam] enteros
    t_inicio = microsegundos();
    algoritmo(v, tam); //aplica el algoritmo indicado
                       //por parámetro al vector generado
    t_fin = microsegundos();
    t_test = t_fin - t_inicio;
    if (t_test < 500){
        printf("(*)");
        t_inicio = microsegundos();
        for (i = 0; i < k; i++){
            f_init(v, tam);
            algoritmo(v,tam);
        }
        t_fin = microsegundos();
        t_test_init = t_fin-t_inicio;
        t_inicio = microsegundos();
        for (i = 0; i<k; i++){
            f_init(v, tam);
        }
        t_fin = microsegundos();
        t_init = t_fin - t_inicio;
        t_test = (t_test_init - t_init) /k;
    }
    free(v); return t_test;
}

void print_dijkstra(){
    int k = 1000;
    double tiempo = 0.0;
    int n; //Iterador //Algoritmo Dijkstra:
    printf("Ordenación por Montículos: \n\tVector de entrada ordenado:\n");
    printf("\t       n\t\t\t   t(n)\t   t(n)/(n^0.8)*log2(n)"
    "\t       t(n)/(n^0.98*log2(n))\t   t(n)/(n^1.2)*log2(n)\n");
    for (n=500; n<=256000; n*=2){
        tiempo = medir_tiempo(heapsort, ascendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)*log2(n)),
               tiempo/(pow(n,0.98)*log2(n)),
               tiempo/(pow(n,1.2)*log2(n)));
    }
    printf("\n\tVector de entrada ordenado al revés:\n");
    printf("\t       n\t\t\t   t(n)\t   t(n)/(n^0.8)*log2(n)"
    "\t       t(n)/(n^0.98*log2(n))\t   t(n)/(n^1.2)*log2(n)\n");
    for (n=500; n<=256000; n*=2){
        tiempo = medir_tiempo(heapsort, descendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)*log2(n)),
               tiempo/(pow(n,0.98)*log2(n)),
               tiempo/(pow(n,1.2)*log2(n)));
    }
    printf("\n\tVector de entrada aleatorio:\n");
    printf("\t       n\t\t\t   t(n)\t   t(n)/(n^0.8)*log2(n)"
    "\t       t(n)/(n^1.02*log2(n))\t   t(n)/(n^1.2)*log2(n)\n");
    for (n=500; n<=512000; n*=2){
        tiempo = medir_tiempo(heapsort, aleatorio, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)*log2(n)),
               tiempo/(pow(n,1.02)*log2(n)),
               tiempo/(pow(n,1.2)*log2(n)));
    }
    printf("\n");
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
}

int main(int argc, char const *argv[]){
    inicializar_semilla();
    test_crearmonticulo();
    test_heapsort();
    print_crearmonticulo();
    print_heapsort();
    return 0;
}
