#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

/*
 * Algoritmos
 * Práctica 1
 * Carlos Torres (carlos.torres)
 * Daniel Vega (d.s.vega)
 * Grupo 4.3
*/

int sumasubMax1(int v[], int tam){
    int i,j;
    int sumaMax = 0, estaSuma = 0;
    for (i = 0; i < tam; i++){
        estaSuma = 0;
        for (j = i; j < tam; j++){
            estaSuma += v[j];
            if (estaSuma > sumaMax){
                sumaMax = estaSuma;
            }
        }
    }
    return sumaMax;
}

int sumasubMax2(int v[], int tam){
    int estaSuma = 0; int sumaMax = 0;
    int i;
    for (i = 0; i < tam;  i++){
      estaSuma += v[i];
      if (estaSuma > sumaMax) {
          sumaMax = estaSuma;
      }
      else if (estaSuma < 0){
          estaSuma = 0;
      }
    }
    return sumaMax;
}

void inicializar_semilla() {
    srand(time(NULL));
  /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n */
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void printv(int v[], int n) {
    int i;
    printf("[");
    for (i = 0; i < n; i++){
        printf("%3d", v[i]);
        if (i != n - 1){
            printf(",");
        }   
    }
    printf("]");
}

void test1(){
    int v[6][5] = {{-9,2,-5,-4,6},{4,0,9,2,5},{-2,-1,-9,-7,-1},
    {9,-2,1,-7,-8},{15,-2,-5,-4,16},{7,-5,6,7,-7}};
    int i;
    printf("\t\t\tsumasubMax1\tsumasubMax2\n");
    for (i = 0; i < 6; i++){
        printv(v[i],5);
        printf("    ");
        printf("%5d",sumasubMax1(v[i],5));
        printf("         ");
        printf("%5d\n",sumasubMax2(v[i],5));
    }
}

void test2() {
    int v[9];
    int i;
    printf("\t\t\t\t\tsumasubMax1\tsumasubMax2\n");
    for (i = 0; i < 10; i++){
        aleatorio(v,9);
        printv(v,9);
        printf("    ");
        printf("%5d",sumasubMax1(v,9));
        printf("         ");
        printf("%5d\n",sumasubMax2(v,9));
    }
}

double medir_tiempo(int (* algoritmo)(int v[], int tam) , int tam, int k) {
//K es el número de repeticiones en caso de tener que medir tiempos pequeños
    double t_inicio = 0.0; //tiempo de inicio
    double t_fin = 0.0; //tiempo de fin
    double t_test = 0.0; //tiempo del test
    double t_test_init = 0.0; //tiempo de test+inicializacion
    double t_init = 0.0; //tiempo de solo inicialización
    int i; //iterador de los bucles for
    int *v;

    v = malloc(tam*sizeof(int));
    aleatorio(v, tam); //rellena un vector de [tam] enteros aleatorios
    t_inicio = microsegundos();
    algoritmo(v, tam);
    t_fin = microsegundos();
    t_test = t_fin - t_inicio;
    if (t_test < 500){
        printf("(*)");
        t_inicio = microsegundos();
        for (i = 0; i < k; i++){
            aleatorio(v, tam);
            algoritmo(v,tam);
        }
        t_fin = microsegundos();
        t_test_init = t_fin-t_inicio;
        t_inicio = microsegundos();
        for (i = 0; i<k; i++){
            aleatorio(v, tam);
        }
        t_fin = microsegundos();
        t_init = t_fin - t_inicio;
        t_test = (t_test_init - t_init) /k;
    }
    free(v);
    return t_test;
}

void test3(){
    int k = 1000; //ejecuciones del algoritmo para tiempos pequeños
    double tiempo = 0.0;
    int n;
        //Algoritmo 1
    printf("Algoritmo SumaSubMax 1: \n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^1.8\t\t  "
    "   t(n)/n^2.0\t\t     t(n)/n^2.2\n");
    for (n=500; n<=64000; n*=2){
        tiempo = medir_tiempo(sumasubMax1, n, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
        tiempo, tiempo/(pow(n,1.8)), tiempo/(pow(n,2)), tiempo/(pow(n,2.2)));
    }
    printf("\n");

        //Algoritmo 2
    printf("Algoritmo SumaSubMax 2: \n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^1.8\t\t  "
    "   t(n)/n^2.0\t\t     t(n)/n^2.2\n");
    for (n=500; n<=2048000; n*=2){
        tiempo = medir_tiempo(sumasubMax2, n, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
        n, tiempo, tiempo/(pow(n,0.8)), tiempo/n, tiempo/(pow(n,1.2)));
    }
    printf("\n");
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
}

int main(int argc, char const *argv[])
{
    inicializar_semilla();
    printf("TEST 1:\n");
    test1();
    printf("\nTEST 2:\n");
    test2();
    printf("\nTEST 3:\n");
    test3();
    return 0;
}
