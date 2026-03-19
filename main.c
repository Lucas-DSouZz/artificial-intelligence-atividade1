#include "vacuum.h"
#include "plan.h"
#include <stdio.h>

int main(){

    int h, w, numDirt;
    printf("Digite as dimensoes do ambiente (linhas e colunas): ");
    scanf("%d %d", &h, &w);
    printf("Digite a quantidade de sujeira: ");
    scanf("%d",&numDirt);

    //Inicialização do ambiente
    enviroment E = newEnviroment(h,w);
    initEnviromentDirt(E,numDirt);
    
    //Inicialização do agente
    cleaner C = newCleaner(E);

    //Limpeza do ambiente
    cleanEnviroment(&C,E);
    printf("Numero total de acoes executadas pelo agente: %d \n",C.numActions);

    delEnviroment(&E);
    return 0;
}