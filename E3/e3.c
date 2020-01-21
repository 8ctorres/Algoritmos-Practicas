/*
  EXAMEN ALGORITMOS
  21 ENERO 2020, GRUPO 3 (16:30)

  CARLOS TORRES PAZ
  carlos.torres@udc.es
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

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

int max(int a, int b, int c){
  if (a>b){
    if (b>c) return a; //a mayor que b, b mayor que c
    else{
      if (a>c) return a; //a mayor que b, a mayor que c
      else return c; //a mayor que b, c mayor que a
    }
  }
  else{
    if (c>b) return c; //b mayor que a, c mayor que b
    else return b; //b mayor que a, b mayor que c
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

int sumaSubMaxAux (int v[], int izq, int der){
  int centro, sumDer, sumIzq, sumBordeDer, sumBordeIzq, sumAux, i, j;
  if (izq==der) {
    if (v[izq] > 0) return v[izq];
    else return 0;
  }//if izq==der
  else{
    centro = (izq+der) / 2;
    sumDer = sumaSubMaxAux(v, centro+1, der);
    sumIzq = sumaSubMaxAux(v, izq, centro);
    sumBordeDer = 0;
    sumAux = 0;
    for (i = centro+1; i<= der; i++){
      sumAux += v[i];
      if (sumAux > sumBordeDer) sumBordeDer = sumAux;
    }
    sumAux = 0;
    sumBordeIzq = 0;
    for (j = centro; j>=izq; j--){
      sumAux += v[j];
      if (sumAux > sumBordeIzq) sumBordeIzq = sumAux;
    }
    return max(sumIzq, sumDer, sumBordeIzq+sumBordeDer);
  }//if not izq==der
}

int sumaSubMax3(int v[], int n){
  return (sumaSubMaxAux(v,0,n-1));
}

void test1(){
    int v[6][5] = {{-9,2,-5,-4,6},{4,0,9,2,5},{-2,-1,-9,-7,-1},
    {9,-2,1,-7,-8},{15,-2,-5,-4,16},{7,-5,6,7,-7}};
    int i;
    printf("secuencia\t\tresultado\n");
    for (i = 0; i < 6; i++){
        printv(v[i],5);
        printf("    ");
        printf("%5d\n",sumaSubMax3(v[i],5));
    }
}

void test2() {
    int v[9];
    int i;
    printf("\t\t\t\t\tsumasubMax1\tsumasubMax2\tsumasubMax3\n");
    for (i = 0; i < 10; i++){
        aleatorio(v,9);
        printv(v,9);
        printf("\t");
        printf("%5d",sumasubMax1(v,9));
        printf("\t\t");
        printf("%5d",sumasubMax2(v,9));
	printf("\t\t");
	printf("%5d\n",sumaSubMax3(v,9));
    }
}

void test3(){
    int k = 1000; //ejecuciones del algoritmo para tiempos pequeños
    double tiempo = 0.0;
    int n;
    printf("Tabla de tiempos del algoritmo SumaSubMax 3: \n\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^0.8\t\t "
    "   t(n)/n^1.03\t\t     t(n)/n^1.2\n");
    for (n=1000; n<=16384000; n*=2){
        tiempo = medir_tiempo(sumaSubMax3, n, k);
        printf("\t%10d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
	       tiempo, tiempo/(pow(n,0.8)), tiempo/(pow(n,1.03)),
           tiempo/(pow(n,1.2)));
    }
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
}

int main(){
  inicializar_semilla();
  printf("Test algoritmo SumaSubMax3:\n\n");
  test1();
  printf("\n\nTest comparando con los algoritmos de la P1:\n\n");
  test2();
  printf("\n\n");
  test3();
}
