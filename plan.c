#include "plan.h"
#include "stdio.h"

static int manhattan(place* a, place* b){
    return abs(a->row - b->row) + abs(a->col - b->col);
}

static bool hasDirt(enviroment E){
    for (int i = 0; i < E.h; i++){
        for (int j = 0; j < E.w; j++){
            if (E.grid[i][j].dirt)
                return true;
        }
    }
    return false;
}

static bool inside(enviroment E, int row, int col){
    return row >= 0 && row < E.h && col >= 0 && col < E.w;
}

static place* nearestDirt(cleaner* C, enviroment E){
    int baseRow = C->whereCleaner->row;
    int baseCol = C->whereCleaner->col;
    int maxDist = (E.h - 1) + (E.w - 1);

    for (int dist = 0; dist <= maxDist; dist++){
        for (int di = -dist; di <= dist; di++){
            int dj = dist - abs(di);
            int row = baseRow + di;
            int col = baseCol + dj;

            if (inside(E, row, col) && E.grid[row][col].dirt)
                return &E.grid[row][col];

            if (dj != 0){
                col = baseCol - dj;
                if (inside(E, row, col) && E.grid[row][col].dirt)
                    return &E.grid[row][col];
            }
        }
    }

    return NULL;
}

void cleanEnviroment(cleaner* C, enviroment E){
/*
    Plano de limpeza para um agente que conhece o ambiente, mas não sabe onde 
    está a sujeira a ser limpada.
*/
    while (hasDirt(E)){
        printSimulation(*C,E);

        //Se houver sujeira na posição atual, limpa antes de buscar novo alvo.
        if (C->whereCleaner->dirt){
            if (C->battery <= 0){
                if (!charge(C,E))
                    return;
            }
            clean(C);
            continue;
        }

        place* target = nearestDirt(C,E);
        if (target == NULL)
            break;

        int toTarget = manhattan(C->whereCleaner, target);
        int toChargerFromTarget = manhattan(target, C->whereCharger);
        int requiredBattery = toTarget + 1 + toChargerFromTarget;

        //Garante ida ao alvo, limpeza e retorno para recarga.
        if (requiredBattery > MAX_BATTERY){
            printf("Nao ha bateria suficiente para limpar com seguranca este alvo.\n");
            return;
        }

        if (C->battery < requiredBattery){
            if (!charge(C,E))
                return;
            continue;
        }

        if (!goTarget(C,E,target))
            return;

        clean(C);
    }

    //Ao final, retorna ao carregador se possivel.
    if (C->whereCleaner != C->whereCharger){
        int toCharger = manhattan(C->whereCleaner, C->whereCharger);
        if (toCharger <= C->battery)
            goTarget(C,E,C->whereCharger);
    }
}