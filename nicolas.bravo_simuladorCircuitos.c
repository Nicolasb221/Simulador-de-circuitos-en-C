#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX 100



typedef enum STATE STATE;
enum STATE {NetList, Determinante, Verifica, Cramer};
void mostrar_matriz(double matriz_de_admitancias[][MAX], int n_dnodos);
void mostrar_matriz_1(double matriz_tmp2_admitancias[][MAX], int n_dnodos);
double determinante(double matriz_de_admitancias[][MAX], int n_dnodos);
double determinante_1(double matriz_tmp2_admitancias[][MAX], int n_dnodos);
double cofactor(double matriz_de_admitancias[][MAX], int n_dnodos, int fila, int columna);
double cofactor_1(double matriz_tmp2_admitancias[][MAX], int n_dnodos, int fila, int columna);

int main()
{
    int fila;
    int columna;
    int i,j,l,k,i2,j2,i3,j3,fdcorrientes,n_dnodos,n_dr,n;
    printf("********************************************SIMULADOR DE CIRCUITOS RESISTIVOS******************************************\n");
    printf("\t\t\t\tGND no es un nodo, tenerlo en cuenta en toda la simulacion");
    printf("\n\t\t\t\t\t\t Ingrese el numero de nodos \n");
    printf("=====>  ");
    scanf("%d", &n_dnodos);
    double lect_I,cofactor,corriente,adm_tmp,admitancias,resistencias,det,det_principal,matriz_de_admitancias[MAX][MAX],vector_corrientes[n_dnodos],matriz_tmp_admitancias[n_dnodos][n_dnodos],matriz_tmp2_admitancias[n_dnodos][n_dnodos],vector_det[n_dnodos],vector_voltajes[n_dnodos];

    STATE state=NetList;
    while(1)
    {
        switch(state)
        {
            case NetList:
            for(i=0;i<n_dnodos;i++){
                for(j=0;j<n_dnodos;j++){
                    if(i==j){
                        printf("\n ingrese el numero de resistencias conectadas al nodo [%d] == ",i+1);
                        scanf("%d", &n_dr);
                        for(k=0;k<n_dr;k++){
                            printf("R[%d] = ",k+1);
                            scanf("%lf", &resistencias);
                            if(i==j){
                                admitancias=1/resistencias;
                                adm_tmp=adm_tmp+admitancias;
                            }
                            else{
                                admitancias=-1/resistencias;
                                adm_tmp=adm_tmp+admitancias;
                            }
                        }
                        matriz_de_admitancias[i][j]=adm_tmp;
                        matriz_tmp2_admitancias[i][j]=adm_tmp;
                        adm_tmp=0;
                    }
                    else{
                        printf("\n ingrese el numero de resistencias conectadas entre el nodo [%d] y nodo [%d] == ",i+1,j+1);
                        scanf("%d", &n_dr);
                        for(k=0;k<n_dr;k++){
                            printf("R[%d] = ",k+1);
                            scanf("%lf",&resistencias);
                                if(i==j){
                                    admitancias=1/resistencias;
                                    adm_tmp=adm_tmp+admitancias;
                                }
                                else{
                                    admitancias=-1/resistencias;
                                    adm_tmp=adm_tmp+admitancias;
                                }
                        }
                        matriz_de_admitancias[i][j]=adm_tmp;
                        matriz_tmp2_admitancias[i][j]=adm_tmp;
                        adm_tmp=0;
                    }
                }
            }
            printf("\n ingrese el valor de cada fuente en amperios \n");
            for(i=0;i<n_dnodos;i++){
                salto:
                printf("ingrese el valor de la fuente de corriente conectada al nodo [%d] == \n ",i+1);
                scanf("%lf",&corriente);

                if(corriente==0){
                    vector_corrientes[i]=0;
                }
                if(corriente!=0){
                    printf("la fuente entra al nodo o sale \n");
                    printf("1)entra \n");
                    printf("2)sale \n");
                    scanf("%d",&n);
                    if(n==1){
                        vector_corrientes[i]=corriente;
                    }
                    if(n==2){
                        vector_corrientes[i]=-corriente;
                    }

                    if(n!=1 && n!=2){
                        goto salto;
                    }
                }
                //printf("I%lf",vector_corrientes[i]);
            }
            state=Determinante;
            break;


            case Determinante:
            printf("\nMostrando la matriz ingresada:\n");
            mostrar_matriz(matriz_de_admitancias, n_dnodos);
            printf("\nEl determinante es: %lf\n", determinante(matriz_de_admitancias, n_dnodos));
            det_principal=determinante(matriz_de_admitancias, n_dnodos);


            state=Verifica;
            break;

            case Verifica:
            if(determinante(matriz_de_admitancias, n_dnodos)!=0){
                state=Cramer;
            }else{
                state=NetList;
            }
            break;

            case Cramer:
            for(i=0;i<n_dnodos;i++){
                for(j=0;j<n_dnodos;j++){
                    matriz_tmp2_admitancias[i][j]=matriz_de_admitancias[i][j];
                }
            }
            for(i=0;i<n_dnodos;i++){
                for(j=0;j<n_dnodos;j++){
                    matriz_de_admitancias[j][i]=vector_corrientes[j];
                }
                //det de cramer
                printf("\nMostrando la matriz ingresada:\n");
                mostrar_matriz(matriz_de_admitancias, n_dnodos);
                printf("\nEl determinante es: %lf\n", determinante(matriz_de_admitancias, n_dnodos));
                vector_det[i]=determinante(matriz_de_admitancias, n_dnodos);

                //

                //se reasigna la matriz inicial
                for(i2=0;i2<n_dnodos;i2++){
                    for(j2=0;j2<n_dnodos;j2++){
                        matriz_de_admitancias[i2][j2]=matriz_tmp2_admitancias[i2][j2];
                    }
                }
            }
            for(i=0;i<n_dnodos;i++){
                vector_voltajes[i]=vector_det[i]/det_principal;
                printf("\n el valor del voltaje en el nodo [%d] es = ",i+1);
                printf("%lf", vector_voltajes[i]);
            }
            printf("\n\n");
            printf("ingrese el numero de nodos del circuito \n");
            scanf("%d", &n_dnodos);
            printf("\n\n");
            state=NetList;
            break;
        }
    }
}
void mostrar_matriz(double matriz_de_admitancias[][MAX], int n_dnodos)
{
	int i, j;
	printf("Orden: %d\n", n_dnodos);
    for (i = 0; i <n_dnodos; i++) {
        for (j = 0; j <n_dnodos; j++) {
            printf("\t%lf", matriz_de_admitancias[i][j]);
        }
        printf("\n");
    }
}
void mostrar_matriz_1(double matriz_tmp2_admitancias[][MAX], int n_dnodos)
{
	int i, j;
    printf("Orden: %d\n", n_dnodos);
    for (i = 0; i <n_dnodos; i++) {
        for (j = 0; j <n_dnodos; j++) {
            printf("\t%lf", matriz_tmp2_admitancias[i][j]);
        }
        printf("\n");
    }
}


double determinante(double matriz_de_admitancias[][MAX], int n_dnodos)
{
   double det = 0.0;
   int j;

   if (n_dnodos == 1) {
      det = matriz_de_admitancias[0][0];
    }
   else{
      for (j = 0; j < n_dnodos; j++) {
         det = det + matriz_de_admitancias[0][j] * cofactor(matriz_de_admitancias, n_dnodos, 0, j);
      }
   }
   return det;
}
double determinante_1(double matriz_tmp2_admitancias[][MAX], int n_dnodos)
{
   double det_1 = 0.0;
   int j;
   if (n_dnodos == 1) {
      det_1 = matriz_tmp2_admitancias[0][0];
   }
   else{
      for(j = 0; j < n_dnodos; j++){
         det_1 = det_1 + matriz_tmp2_admitancias[0][j] * cofactor_1(matriz_tmp2_admitancias, n_dnodos, 0, j);
      }
   }
   return det_1;
}
double cofactor(double matriz_de_admitancias[][MAX], int n_dnodos, int fila, int columna)
{
   double submatriz[MAX][MAX];
   int n = n_dnodos - 1;
   int i, j;
   int x = 0;
   int y = 0;
   for (i = 0; i < n_dnodos; i++) {
      for (j = 0; j < n_dnodos; j++) {
         if (i != fila && j != columna) {
            submatriz[x][y] = matriz_de_admitancias[i][j];
            y++;
            if (y >= n) {
               x++;
               y = 0;
            }
         }
      }
   }
   return pow(-1.0, fila + columna) * determinante(submatriz, n);
}
double cofactor_1(double matriz_tmp2_admitancias[][MAX], int n_dnodos, int fila, int columna)
{
   double submatriz_1[MAX][MAX];
   int n = n_dnodos - 1;
   int i, j;
   int x = 0;
   int y = 0;
   for (i = 0; i < n_dnodos; i++) {
      for (j = 0; j < n_dnodos; j++) {
         if (i != fila && j != columna) {
            submatriz_1[x][y] = matriz_tmp2_admitancias[i][j];
            y++;
            if (y >= n) {
               x++;
               y = 0;
            }
         }
      }
   }
   return pow(-1.0, fila + columna) * determinante_1(submatriz_1, n);
}




