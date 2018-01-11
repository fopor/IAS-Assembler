#ifndef  MNEMONICOS_H
#define MNEMONICOS_H
#include "main.h"

int solve_mnemonico(char *entrada, char *saida, int flag,  char alias_buffer[][64], char alias_value[][64], int alias_line, char label_buffer[][64], char label_value[][64], int label_line);
int find_label(char label_buffer[][64], char label[]);
#endif