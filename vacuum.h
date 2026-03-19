
#include <stdbool.h>
#include <stdlib.h>

#define MAX_BATTERY 45

#ifndef VACUUM_H
#define VACUUM_H

//Propriedades de cada lugar do ambiente
typedef struct{
    int row, col; //indice do lugar em um grid
    bool dirt; //estado sujo
} place;

//O ambiente é uma matriz de lugares (grid)
typedef struct{
    int h, w;
    place** grid;
} enviroment;

enviroment newEnviroment(int h, int w);
void delEnviroment(enviroment* E);
void initEnviromentDirt(enviroment E, int numDirt);
bool isNeighbor(place p, place q);
place* getPlace(enviroment E, int i, int j);

typedef struct{
    place* whereCleaner;
    place* whereCharger;
    int battery;
    int numActions;
} cleaner;

cleaner newCleaner(enviroment);
bool move(cleaner* C, enviroment E, place* p);
bool goTarget(cleaner* C, enviroment E, place* target);
bool charge(cleaner* C, enviroment E);
void clean(cleaner* C);

void printSimulation(cleaner C, enviroment E);

#endif