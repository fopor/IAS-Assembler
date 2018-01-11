#ifndef  DIRETIVA_SOLVER_H
#define DIRETIVA_SOLVER_H
#include "main.h"

int diretivas_solver(char *map_buffer, int *map_buffer_pos_pointer, char memory_map[][15], char label_buffer[][64], char label_value[][64],  int memory_map_source_line[]);
int number_treat(char numberString[]);
int number_treat_(char numberString[], int flag);

#endif