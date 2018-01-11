#ifndef  LINE_SOLVER_H
#define LINE_SOLVER_H
#include "main.h"

int solve_entrada(char *comando_entrada, char *string_saida, char *map_buffer,
	int  *map_buffer_pos_pointer, char aliasb[][64],  char aliasv[][64],  char labelb[][64], char labelv[][64], int *labe_l, int *alias_l, int line_number,  int memory_map_source_line[]);

int insere_comando_valido(char *mnemonico_ready, int *map_buffer_pos_pointer, char *map_buffer);

#endif