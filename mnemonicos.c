#include "mnemonicos.h"

/*encontra o rotulo na label_buffer*/
int find_label(char label_buffer[][64], char label[]){
    int i;
    int j;
    int label_size = strlen(label);
    int label_line_size;
    
    for(i = 0; label_buffer[i][0] != 0; i++){
        label_line_size = strlen(label_buffer[i]);
        
        /*no dicionario de labels nao temos aspas*/
        if(label_line_size == (label_size+1)){
            j=0;
            while(label_buffer[i][j] == label[j] && j != label_line_size && label_buffer[i][j] != '\0' && label[j] != '\0'){
                
                j++;
            }
            
            /*sao iguais exceto pelos ':'*/
            if(j == label_size){
                return i;
            }
        }
    }
    
    /*retorna -1 se nao encontrar*/
    return -1;
}

/*resolve qual comando eh representado pelo mnemonico de entrada*/
int solve_mnemonico(char *entrada, char *saida, int flag, 
char alias_buffer[][64], char alias_value[][64], int alias_line, char label_buffer[][64], 
char label_value[][64], int label_line) {
    int i, j;
    int entrada_size;
    char c_aux;
    
    /*variavel auxiliar para o tamanho da string entrada*/
    entrada_size = strlen(entrada);
     
    /*procura qual o codigo correspondente ao mnemonico*/
    if ((strcmp(entrada, "LD"))== 0){
		strcpy(saida, "LD");
        
        if(flag == 10){
            strcpy(saida, "01");
            return INSTRUCTION;
        }
        
        return 5;
    }
    
    if ((strcmp(entrada, "LD-"))== 0){
		strcpy(saida, "LD-");
        
        if(flag == 10){
            strcpy(saida, "02");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "LD|"))== 0){
		strcpy(saida, "LD|");
        
        if(flag == 10){
            strcpy(saida, "03");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "LDmq"))== 0){
		strcpy(saida, "LDmq");
        
        if(flag == 10){
            strcpy(saida, "0A");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "LDmq_mx"))== 0){
		strcpy(saida, "LDmq_mx");
        
        if(flag == 10){
            strcpy(saida, "09");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "ST"))== 0){
		strcpy(saida, "ST");
        
        if(flag == 10){
            strcpy(saida, "21");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "JMP"))== 0){
		strcpy(saida, "JMP");
        
        /*implementar essa diferenciacao*/
        if(flag == 10){
            strcpy(saida, "0D");
            return CON_INSTRUCTION;
        }
        
        if(flag == 11){
            strcpy(saida, "0E");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "JUMP+"))== 0){
		strcpy(saida, "JUMP+");
        
        if(flag == 10){
            strcpy(saida, "0F");
            return CON_INSTRUCTION;
        }
        
        if(flag == 11){
            strcpy(saida, "10");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "ADD"))== 0){
		strcpy(saida, "ADD");
        
        if(flag == 10){
            strcpy(saida, "05");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "ADD|"))== 0){
		strcpy(saida, "ADD|");
        
        if(flag == 10){
            strcpy(saida, "07");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "SUB"))== 0){
		strcpy(saida, "SUB");
        
        if(flag == 10){
            strcpy(saida, "06");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "SUB|"))== 0){
		strcpy(saida, "SUB|");
        
        if(flag == 10){
            strcpy(saida, "08");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "MUL"))== 0){
		strcpy(saida, "MUL");
        
        if(flag == 10){
            strcpy(saida, "0B");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "DIV"))== 0){
		strcpy(saida, "DIV");
        
        if(flag == 10){
            strcpy(saida, "0C");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "LSH"))== 0){
		strcpy(saida, "LSH");
        
        if(flag == 10){
            strcpy(saida, "14");
            return NO_ADRESS_INS;
        }
        
        return 5;
    }
	
	if ((strcmp(entrada, "RSH"))== 0){
		strcpy(saida, "RSH");
        
        if(flag == 10){
            strcpy(saida, "15");
            return NO_ADRESS_INS;
        }
        
        return 5;
    }
    
	if ((strcmp(entrada, "STaddr"))== 0){
		strcpy(saida, "STaddr");
        
        if(flag == 10){
            strcpy(saida, "12");
            return CON_INSTRUCTION;
        }
        
        if(flag == 11){
            strcpy(saida, "13");
            return INSTRUCTION;
        }
        
        return 5;
    }
	
	/*verfica se eh uma diretiva*/
	if(strcmp(entrada, ".org") == 0){
   		strcpy(saida, entrada);

        if(flag == 10){
            return ORG;
        }
        
        return 1;
	}
    
    if(strcmp(entrada,  ".set") == 0){
   		strcpy(saida, entrada);

        return 3;
	}
    
    if(strcmp(entrada,  ".word") == 0){
   		strcpy(saida, entrada);

        if(flag == 10){
            return WORD;
        }
        
        return 1;
	}
    
    if(strcmp(entrada, ".wfill") == 0){
   		strcpy(saida, entrada);
        
        if(flag == 10) {
            return WFILL;
        }

        return 1;
	}
    
     if(strcmp(entrada, ".align") == 0){
   		strcpy(saida, entrada);

        if(flag == 10){
            return ALIGN;
        }
        
        return 1;
	}
    
    /*verifica se eh um hex de 10 digitos (processado)*/
    if(entrada_size == 10 && (flag == 10)){
        
        return NUMERO;
    }
	
	/*verifica se eh um rotulo*/
	if(entrada[entrada_size-1] == ':'){
        if(flag == 10){
            
            strcpy(saida, entrada);
            saida[entrada_size-1] = '\0';
            return LABEL_DECLARATION;
        }
        
        /*verifica se comeca com um numero*/
        if(entrada[0] >= 48 && entrada[0] <= 57){
            strcpy(saida, entrada);
    
            printf("Rotulos nao podem comecar com numeros!\n");
    
            return -1;
        }
        
        /*verifica se : aparece mais de uma vez*/
        for(i = 0; i < (entrada_size-1); i++){
            if(entrada[i] == ':'){
           		strcpy(saida, entrada);
                
                /*printf("Rotulos nao podem conter ':' mais de uma vez!\n");*/
                
                return -1;
            }
        }
        
        /*escreve o label na saida*/
   		strcpy(saida, entrada);
        
        /*escreve o label  no dicionario*/
        j = 0;
        /*encontra a proxima linha vazinha do dicionario*/
        while(label_buffer[j][0] != '\0'){
            j++;
        }
        
        /*coloca o rotulo no dicionario*/
        if(flag == MONTA_LABEL_TABLE){
            strcpy(label_buffer[j], entrada);
            
            /*remove os ':'*/
            label_buffer[j][entrada_size] = '\0';            
        }
         
        /*nao reescreve os rotulos na matriz auxiliar*/
        return 1;
	}
    
    /*verifica se eh um rotulo ja registrado*/
    if(find_label(label_buffer, entrada) != -1){
            
            strcpy(saida, entrada);
            
            if(flag == 10){
                return LABEL;
            }
            
            return 1;
    }
    
    /*resolve valores entre aspas*/
	if(entrada[0] == '"'){    
        /*se o formato estiver incorreto, interrompe execucao*/
        if(entrada[entrada_size-1] != '"'){
            strcpy(saida, "Entrada sem fechamento para aspas");
            
            /*retorna -1 para indicar erro*/
            return -1;
        }
        
        /*se for um numero hexadecimal de tamanho incorreto*/
        if((entrada_size != (14)) && (entrada[2] == 'x')){
            strcpy(saida, entrada);
            
            /*retorna -1 para indicar erro*/
            return -1;
        }

        /*copia sem as aspas*/
        i = 0;
        while(i < entrada_size-2){
            saida[i] = entrada[i+1];
            i++;
        }
        saida[i] = '\0';
        
        if(flag == 10){
            return NUMERO_ASPAS;
        }
        
        /*adiciona alias value na lista*/
        if(flag == 2) {
            /*colcoa o valor do alias na lista*/        
            strcpy(saida, "Valor do .set entre aspas");
        
            return 0;
        }
        
        return 1;
	}
    
    /*verifica se eh um hexadecimal sem aspas*/
    if(entrada[0] == '0' && entrada[1] == 'x'){
        
        /*verifica se tem o tamanho certo*/
        if(entrada_size != 12){
            strcpy(saida, entrada);
            
            
            return -1;
        }
        
        /*verifica se todos os valores sao corretos*/
        for(i = 2; i < entrada_size; i++){
            c_aux = entrada[i];
            
            if(!((c_aux >= '0' && c_aux <= '9' ) || (c_aux >= 'a' && c_aux <= 'f') || (c_aux >= 'A' && c_aux <= 'F'))){
                strcpy(saida, "hexadecimal com caractere invalido");
            
                return -1;
            }
        }
        
        if(flag == 10){
            strcpy(saida, entrada);
            
            return NUMERO;
        }
        
        /*adiciona alias value na lista*/
        if(flag == 2) {
            /*colcoa o valor do alias na lista*/
            strcpy(alias_value[alias_line], entrada);
        
            return 6;
        }
        
         /*retorna o numero valido*/
        strcpy(saida, entrada);
        return 1;
    }
    
    /*verifica se eh um numero decimal negativo*/
    if(entrada[0] == '-'){
        /*verifica se eh um decimal negativo valido*/
        for(i = 1; i < entrada_size; i++){
            if(entrada[i]<'0' || entrada[i] > '9'){
                strcpy(saida, entrada);
                
                return -1;
            }
        }
        
        /*retorna o numero valido*/
        strcpy(saida, entrada);
        
        if(flag == 10){
            return NUMERO;
        }
        
        return 1;
    }
    
    /*verifica se eh um numero decimal*/
    if(entrada[0] >= '0' && entrada[0] <= '9' && flag != 10) {
        /*verifica se eh um numero valido*/
        for (i = 0; i < entrada_size-1; i++){
            /*se nao for um numero, interrompe execucao*/
            if(entrada[i] < 48 || entrada[i] > 57){
                strcpy(saida, entrada);
                
                return -1;
            }
        }
        
        strcpy(saida, entrada);
    
        if(flag == 10){
            strcpy(saida, entrada);
            
            return NUMERO;
        }
    
        /*adiciona alias value na lista*/
        if(flag == 2) {
            /*colcoa o valor do alias na lista*/
            strcpy(alias_value[alias_line], entrada);
        
            return 6;
        }
        
        return 1;
    }

    /*verifica se eh um alias do .set*/ 
    for(i = 0; flag != 10 && alias_buffer[i][0] != 0; i++){
        /*se achar no dicionario, troca pelo valor do alias*/        
        if(strcmp(alias_buffer[i], entrada) == 0){
            strcpy(saida, alias_value[i]);
            
            return 1;
        }
    }
    
    /*adiciona o alias na lista sem as aspas*/
    if(flag == 1) {
        strcpy(saida, entrada);
        
        strcpy(alias_buffer[alias_line], entrada);
        return 4;
    }
    
    /*em caso de erro, a saida sera o comando que deu errado*/
    strcpy(saida, entrada);
    
    return -1;
}