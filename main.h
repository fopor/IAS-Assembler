#ifndef MAIN_H
#define MAIN_H


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "mnemonicos.h"
#include "line_solver.h"
#include "diretivas.h"

#define DIREITA 1
#define ESQUERDA 2
#define SAIDA_PADRAO 2
#define SAIDA_ARQUIVO 3

#define NUMERO_ASPAS 8
#define NUMERO 9
#define ORG 10
#define WFILL 11
#define ALIGN 12
#define WORD 13
#define LABEL 14

#define INSTRUCTION 20
#define CON_INSTRUCTION 21
#define NO_ADRESS_INS 22
#define LABEL_DECLARATION 24

#define MONTA_LABEL_TABLE 64

#endif