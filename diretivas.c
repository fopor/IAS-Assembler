#include "diretivas.h"

/*copia uma instrucao*/
int copia_instrucao(char string_recipiente[], char string_entrada[]){
    int i;
    
    i = 0;
    while(i < 2){
        string_recipiente[i] = string_entrada[i];
        i++;
    }
    
    return 0;    
}

/*copia endereco de instrucao*/
int copia_endereco(char string_recipiente[], char string_entrada[]){
    int i = 0;
    
    while(i < 3){
        string_recipiente[i] = string_entrada[i];
        i++;
    }
    
    return 0;    
}

/*constroi matrix auxiliar com os comandos*/
int constroi_matriz_auxiliar(char *map_buffer, int *map_buffer_pos_pointer, char command_matrix_buffer[][286]){
    int i, j, k;

    /*inicializa a matrix de comandos*/
    for (i = 0; i < 1024; i++){
        for(j = 0; j < 286; j++){
            command_matrix_buffer[i][j] = 0;
        }
    }

    /*coloca os comandos na matriz auxiliar, um comando por linha*/
    i = 0;
    j = 0;
    while(i < (*map_buffer_pos_pointer) && j< 1024){
        k=0;
        while(map_buffer[i] != 0) {
            if(map_buffer[i] == -1)
                i++;
            command_matrix_buffer[j][k] = map_buffer[i];
            k++;
            i++;
        }
        j++;
        i++;
    }
    
    if(j >=1024) {
        return -1;
    }
    
    return 0;
}

/*calcula potencia de 10*/
unsigned long long int potencia(int x) {
    int i = 0;
    unsigned long long int resposta = 1;
    
    for (i = 0; i < x; i++){
        resposta *= 10;
    }
    
    return resposta;
}

/*calcula potencia de 16*/
int potencia_16(int x) {
    int i = 0;
    unsigned long long int resposta = 1;
    
    for (i = 0; i < x; i++) {
        resposta *= 16;
    }
    
    return resposta;
}

/*caso comum do number treat*/
int number_treat(char numberString[]){
    return number_treat_(numberString, 0);
}

/*transforma em hexadecimais*/
int number_treat_(char numberString[], int flag){
    int number_size;
    char vet_aux[13];
    int i, j;
    int number_value, pos_value;
    
    number_size = strlen(numberString); 
    
    /*remove as aspas antes de tratar o numero*/
    if(numberString[0] == '"') {
        if(flag == 0){
            printf("Neste caso numero nao pode ser fornecido entre aspas!\n");
            
            return -1;
        }
        
        strcpy(vet_aux, numberString);
        strcpy(numberString,(vet_aux+1));
        numberString[number_size-2] = '\0';
        
        /*atualiza o tamanho*/
        number_size = strlen(numberString);
    }

    /*se for decimal coloca em hexadecimal*/
    if(numberString[1] != 'x') {
        /*se for um numero decimal negativo*/
        if(numberString[0] == '-'){
            number_value = 0;
            
            /*retira o sinal inicialmente para calcularmos o valor do numero*/
            numberString[0] = '0';
            
            /*acumula o valor negativo do numero*/
            for(i = 0; i < number_size; i++){
                pos_value = numberString[i] - '0';

                j= number_size - i -1;

                number_value += potencia(j) * pos_value;
            }
            
            /*pega o valor negativo desse numero*/
            number_value = (-1)*number_value;
            
            sprintf(numberString, "%010X",  number_value);
            numberString[0] = 'F';
            numberString[1] = 'F';
            
            return 0;
        }
        
        
        /*percorre numberString e guarda o valor representado*/
        number_value = 0;
        for(i = 0; i < number_size; i++){
            pos_value = numberString[i] - '0';

            j= number_size - i -1;

            number_value += potencia(j) * pos_value;
        }

        /*coloca o valor com 10 digitos na matriz*/
        sprintf(numberString, "%010X", number_value);

        return 0;
    }
    
    /*se for hexadecimal, remove o '0x'*/
    if(numberString[1] == 'x'){
        strcpy(vet_aux, numberString);
        strcpy(numberString, (vet_aux+2));
        
        return 0;
    }

    return 0;
}

/*chama o solve_mnemonico com menos argumentos*/
int solve_mnemonico_(char string_entrada[], char string_saida[], char label_buffer[][64], char label_value[][64]){
    int aux;
    
    aux = solve_mnemonico(string_entrada, string_saida, 10, NULL, NULL, 0, 
    label_buffer,  label_value, 0);
    
    return aux;
}

/*transforma string de hex em endereco*/
int hex_to_adress(char numberString[]){
    int pos_value;
    int i, j;
    int numero_size;
    int valor;
   
    numero_size = strlen(numberString);
    
    /*percorre numberString e guarda o valor representado*/
    valor = 0;
    for(i = 0; i < numero_size; i++){
        if(numberString[i] >= '0' && numberString[i] <= '9'){
            pos_value = numberString[i] - '0';
        }
        
        /*se for uma letra*/
        else if(numberString[i] >= 'A' && numberString[i] <='F'){
            pos_value = 10 + numberString[i] - 'A';
        }
    
        j= numero_size - i -1;
        
        valor += potencia_16(j) * pos_value;
    }
    
    /*se for um tamanho invlalido*/
    if(valor < 0){
        return -1;
    }
    
    return valor;
}

int diretivas_solver(char *map_buffer, int *map_buffer_pos_pointer, char memory_map[][15], char label_buffer[][64], char label_value[][64],   int memory_map_source_line[]){
	char command_matrix_buffer[1024][286];
    int adress = 0;
    int pos = ESQUERDA;
    int i, j;
    int aux;
    int comando;
    char saida[64];

    
    /*constroi a matrix de comandos auxiliar*/
    aux = constroi_matriz_auxiliar(map_buffer, map_buffer_pos_pointer, command_matrix_buffer);
    
    if(aux == -1){
        printf("Tamanho de mapa de memoria invalido!\n");
        
        return 0;
    }
    
    /*troca todos os numeros por hexadecimais de 10 digitos*/
    i = 0;
    while(command_matrix_buffer[i][0] != 0){
        /*se for um numero entre aspas*/
        if(command_matrix_buffer[i][0] == '"') {
            /*verifica se o valor anterior aceita aspas*/
                comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
                if(!(comando == INSTRUCTION || comando == CON_INSTRUCTION || comando == NO_ADRESS_INS)){
                    printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                    printf("Argumento nao pode ser fornecido entre aspas!\n");
                
                return 0;
                }
            
            number_treat(command_matrix_buffer[i]);
        }
        
        /*se for um numero sem as aspas*/
        else if(command_matrix_buffer[i][0] >= '0' && command_matrix_buffer[i][0]<='9') {
            number_treat(command_matrix_buffer[i]);
        }
        
         /*se for um decimal negativo*/
         else if(command_matrix_buffer[i][0] == '-'){
             number_treat(command_matrix_buffer[i]);
         }
        
        i++;
    }   
    
    /*comeca a montar o mapa de memoria*/
    i = 0;
    while(command_matrix_buffer[i][0] != 0){
        comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
        
        /*se ler um .org*/
        if(comando == ORG){
            /*le a proxima instrucao*/
            i++;
            
            /*verifica se ha proxima instrucao esperada*/
            if(command_matrix_buffer[i][0] == 0) {
                printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                printf("Argumento esperado nao encontrado!\n");
                
                return 0;
            }
            
            /*verifica se o proximo valor eh um numero*/
            comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
            
            if(comando != NUMERO){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Numero nao encontrado!\n");
                
                return 0;
            }
            
            aux = hex_to_adress(command_matrix_buffer[i]);
            
            if(aux < 0 || aux >= 1024){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf(".org chamado para valor invalido!\n");
                
                return 0;
            }
            
             /*se tudo esta correto, vamos pra posicao esquerda do endereco dado*/
             pos = ESQUERDA;
             adress = aux;
             
            /*da um passo na matriz de comandos*/
            i++;
        }
        
        else if(comando == WFILL){
            /*le a proxima instrucao*/
            i++;
            
            /*verifica se ha proxima instrucao esperada*/
            if(command_matrix_buffer[i][0] == 0) {
                printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                printf("Argumento esperado nao encontrado![1]\n");
                
                return 0;
            }
            
            if(pos == DIREITA){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf(".wfill chamada com o mapa na direita!\n");
                
                return 0;
            }
            
            /*verifica se o proximo valor eh um numero*/
            comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
            if(comando != 9) {
                printf("Valor %s invalido\n", command_matrix_buffer[i]);
                
                return 0;
            }
            
            aux = hex_to_adress(command_matrix_buffer[i]);
            
            if(aux == -1 || (aux+i > 1025)){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Solicitado quantidade de .wfill invalida!\n");
                
                return 0;
            }
            
            /*le qual palavra deve ser escrita 'aux' vezes*/
            i++;
            comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);

            /*verifica se ha proxima instrucao esperada*/
            if(command_matrix_buffer[i][0] == 0) {
                printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                printf("Argumento esperado nao encontrado![2]\n");
                
                return 0;
            }
            
            /*verifica se eh um numero*/
            if(comando != NUMERO && comando != LABEL) {
                printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                printf("Argumento nao eh um numero!\n");
                
                return 0;
            }
            
            /*escreve aux vezes a palavra encontrada*/
             while(aux > 0){
                 /*habilita escrita dessa linha*/
                 if(comando == NUMERO){
                     if(memory_map[adress][0] != -1)
                        memory_map[adress][0] = 1;
                     strcpy((memory_map[adress]+1), command_matrix_buffer[i]);
                     
                     adress++;
                 }
                 
                 if(comando == LABEL){
                     j = (find_label(label_buffer, command_matrix_buffer[i]));
                       
                     memory_map[adress][0] = -1;
                     memory_map[adress][9]= 0;
                     memory_map[adress][8]= (-1) * j; 
                     
                     adress++;
                 }
                 aux--;
          
             }
             
             /*ja deu um passo adiante para continuar, em [i] e em [adress]*/
             i++;
        }
    
        /*resolve .ALIGN */
        else if(comando == ALIGN){
            /*le a proxima instrucao*/
            i++;
            
            /*verifica se ha proxima instrucao esperada*/
            if(command_matrix_buffer[i][0] == 0) {
                printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                printf("Argumento esperado nao encontrado![1]\n");
                
                return 0;
            }
            
            /*verifica se o proximo valor eh um numero*/
            comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
            if(comando != NUMERO) {
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Valor %s invalido\n", command_matrix_buffer[i]);
              
                return 0;
            }
            
            /*guarda o numero do .align*/
            aux = hex_to_adress(command_matrix_buffer[i]);
            
            if(pos == DIREITA){
                adress++;
            }
            
            if(aux <= 0){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Impossivel alinhar com zero ou valores negativos!\n");
              
                return 0;
            }
            
            /*vai ate o multiplo solicitado*/
            while(adress % aux) {
                adress++;
            }
            
            if(adress >= 1024){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Tamanho de mapa de memoria excedido!!\n");
              
                return 0;
            }
            
            /*passa a aprondar pra esqueda*/
            pos = ESQUERDA;
            
            /*da um passo na instrucoes*/
            i++;
        }
        
        /*resolve a diretiva WORD*/        
        else if(comando == WORD) {
            /*nao escreve palavra na direita*/
            if(pos == DIREITA){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Tentativa de escrever palavra na posicao DIREITA!\n");
                
                return 0;
            }
            
            if(adress >= 1024) {
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Tamanho do mapa de memoria excedido![2]\n");
                
                return 0;
            }
        
  
            /*ve qual eh o proximo comando*/
            i++;
            
            /*verifica se ha proxima instrucao esperada*/
            if(command_matrix_buffer[i][0] == 0) {
                printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                printf("Argumento esperado nao encontrado![1]\n");
                
                return 0;
            }
            
            /*le o proximo mnemonico*/
            comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);

            /*se nao for do formato valido, emite erro*/
            if(comando != NUMERO && comando != LABEL){
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Argumento de .word invalido!\n");
                
                return 0;
            }
  
            /*escreve a palavra/numero na esquerda*/
            if(comando == NUMERO){
                /*escreve no mapa de memoria*/
                strcpy((memory_map[adress]+1), command_matrix_buffer[i]);
                
                /*habilita a linha escrita*/
                if(memory_map[adress][0] != -1)
                    memory_map[adress][0] = 1;
            }
            
            /*se for um rotulo, apenas copia seu valor*/
            else if(comando == LABEL){ 
                 aux = find_label(label_buffer, command_matrix_buffer[i]);
                 
                 /*habilita a linha escrita*/
                memory_map[adress][0] = -1;
                memory_map[adress][9] = 0;
                memory_map[adress][8] = -1*(aux);
            }
            
            /*da um passo para a proxima linha a ser escrita*/
            adress++;
            
            /*da um passo na instrucoes*/
            i++;
        }
        
        /*resolve os mnemonicos de instrucao*/
        else if(comando == INSTRUCTION || comando == CON_INSTRUCTION || comando == NO_ADRESS_INS){
            if(adress >= 1024) {
                printf("ERROR on line %d\n", memory_map_source_line[i]);
                printf("Tamanho do mapa de memoria excedido![3]\n");
                
                return 0;
            }
            
            
            /*se for um comando que nao depende dos rotulos, escreve*/
            if(comando == INSTRUCTION){
                /*habilita linha do mapa de memoria para ser escrita*/
                if(memory_map[adress][0] != -1)
                    memory_map[adress][0] = 1;
                
                /*se estamos na direita, escreve na direita*/
                if(pos == DIREITA) {
                    copia_instrucao((memory_map[adress]+6), saida);
                }
                
                /*se estamos na esquerda, escreve na esquerda*/
                if(pos == ESQUERDA){
                    copia_instrucao((memory_map[adress]+1), saida);
                }
                
                /*procura endereco da da instrucao*/
                i++;
                
                /*verifica se ha proxima instrucao esperada*/
                if(command_matrix_buffer[i][0] == 0) {
                    printf("ERROR on line %d\n", memory_map_source_line[i-1]);
                    printf("Argumento esperado nao encontrado![1]\n");
                    
                    return 0;
                }
                
                /*le o proximo mnemonico*/
                comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
                
                /*se nao for do formato valido, emite erro*/
                if(comando != NUMERO && comando != LABEL){
                    printf("ERROR on line %d\n", memory_map_source_line[i]);
                    printf("Endereco da instrucao invalido![4]\n");
                    
                    return 0;
                }
                
                /*como nao depende se eh esquerda ou direita*/
                if(comando == LABEL) {
                    memory_map[adress][0] = -1;
                    
                    aux = find_label(label_buffer, command_matrix_buffer[i]);
                    
                    if(pos == ESQUERDA){
                        memory_map[adress][3] = (-1)*aux;
                        memory_map[adress][4] =  0;
                    }
                    
                    if(pos == DIREITA) {
                        memory_map[adress][8] = (-1)*aux;
                        memory_map[adress][9] = 0;
                    }
                }
                
                /*resolve para o caso de endereco numerico*/
                if(comando == NUMERO){
                    aux = hex_to_adress(command_matrix_buffer[i]);
                    
                    /*em caso de tamanho de endereco invalido*/
                    if(aux == -1){
                        
                        printf("Endereco de instrucao invalido!\n");
                        
                        return 0;
                    }
                    
                    /*se estiver tudo certo, escreve o endereco*/
                    /*se estamos na direita, escreve na direita*/
                    j = 0;
                    if(pos == DIREITA) {
                        /*copia o endereco para o mapa de memoria*/
                        while(j < 3){
                            memory_map[adress][8+j] = command_matrix_buffer[i][7+j]; 
                            
                            j++;
                        }
                    }
                   
                    /*se estamos na esquerda, escreve na esquerda*/
                    if(pos == ESQUERDA){
                        /*copia o endereco para o mapa de memoria*/
                           while(j < 3){
                            memory_map[adress][3+j] = command_matrix_buffer[i][7+j]; 
                            j++;
                        }  
                    }
                }
            
                
            }
            
            /*se o comando depende dos rotulos, verifica se eh direita ou esquerda*/
            else if(comando == CON_INSTRUCTION){
                /*habilita linha do mapa de memoria para ser escrita*/
                if(memory_map[adress][0] != -1)
                    memory_map[adress][0] = 1;
                
                /*se estamos na direita, escreve na direita*/
                if(pos == DIREITA) {
                    copia_instrucao((memory_map[adress]+6), saida);
                    
                }
                
                /*se estamos na esquerda, escreve na esquerda*/
                if(pos == ESQUERDA){
                    copia_instrucao((memory_map[adress]+1), saida);
                }
                
                /*procura endereco da da instrucao*/
                i++;
                
                /*verifica se ha proxima instrucao esperada*/
                if(command_matrix_buffer[i][0] == 0) {
                    /*printf("Argumento esperado nao encontrado![1]\n");*/
                    
                    return 0;
                }
                
                /*le o proximo mnemonico*/
                comando = solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
                
                /*se nao for do formato valido, emite erro*/
                if(comando != NUMERO && comando != LABEL){
                    /*printf("Endereco da instrucao invalido![4]\n");*/
                    
                    return 0;
                }
                
                /*resolve para o caso de endereco numerico*/
                if(comando == NUMERO){
                    aux = hex_to_adress(command_matrix_buffer[i]);
                    
                    /*em caso de tamanho de endereco invalido*/
                    if(aux == -1){
                        printf("ERROR on line %d\n", memory_map_source_line[i]);
                        printf("Endereco de instrucao invalido!\n");
                        
                        return 0;
                    }
                    
                    /*se estiver tudo certo, escreve o endereco*/
                    /*se estamos na direita, escreve na direita*/
                    j = 0;
                    if(pos == DIREITA) {
                        /*copia o endereco para o mapa de memoria*/
                        while(j < 3){
                            memory_map[adress][8+j] = command_matrix_buffer[i][7+j]; 
                            
                            j++;
                        }
                    }
                   
                    /*se estamos na esquerda, escreve na esquerda*/
                    if(pos == ESQUERDA){
                        /*copia o endereco para o mapa de memoria*/
                           while(j < 3){
                            memory_map[adress][3+j] = command_matrix_buffer[i][7+j]; 
                            j++;
                        }  
                    }
                }
                
                if(comando == LABEL){
                    aux = find_label(label_buffer, command_matrix_buffer[i]);
                    memory_map[adress][0] = -1;
                    
                    /*ve qual era a instrucao anterior*/
                    solve_mnemonico_(command_matrix_buffer[i-1], saida, label_buffer, label_value);
                    
                    /*JMP*/
                    if(strcmp(saida, "0D") == 0) {    
                        if(pos == ESQUERDA){
                            memory_map[adress][3] = (-1)*aux;
                            memory_map[adress][4] = 1;
                        }
                        
                        if(pos == DIREITA){
                            memory_map[adress][9] = 1;
                            memory_map[adress][8] = (-1)*aux;
                        }
                    }
                    
                    /*JUMP+*/
                    else if(strcmp(saida, "0F") == 0) {
                        if(pos == ESQUERDA){
                            memory_map[adress][3] = (-1)*aux;
                            memory_map[adress][4] = 2;
                        }
                        
                        if(pos == DIREITA){
                            memory_map[adress][9] = 2;
                            memory_map[adress][8] = (-1)*aux;
                        }
                    }
                    
                    /*STAADR*/
                    else if(strcmp(saida, "12") == 0) {
                        if(pos == ESQUERDA){
                            memory_map[adress][3] = (-1)*aux;
                            memory_map[adress][4] = 3;
                        }
                        
                        if(pos == DIREITA){
                            memory_map[adress][9] = 3;
                            memory_map[adress][8] = (-1)*aux;
                        }
                    }
                    
                }
            }
            
            /*se eh um comando que nao precisa de endereco*/
            else if(comando == NO_ADRESS_INS){
                /*habilita linha do mapa de memoria para ser escrita*/
                if(memory_map[adress][0] != -1)
                    memory_map[adress][0] = 1;
                
                /*se estamos na direita, escreve na direita*/
                if(pos == DIREITA) {
                    copia_instrucao((memory_map[adress]+6), saida);

                }
                
                /*se estamos na esquerda, escreve na esquerda*/
                if(pos == ESQUERDA){
                    copia_instrucao((memory_map[adress]+1), saida);

                }
                
                /*da um passo para verificar se encontra um endereco*/
                i++;
                
                /*verifica oq vem a seguir se nao estamos no fim*/
                if(command_matrix_buffer[i][0] != 0){
                    /*verifica qual eh o proximo conteudo*/
                    comando =  solve_mnemonico_(command_matrix_buffer[i], saida, label_buffer, label_value);
                    
                    if((comando == NUMERO || comando == LABEL) && command_matrix_buffer[i][0] != 0){
                        printf("ERROR on line %d\n", memory_map_source_line[i]);
                        printf("Endereco nao necessario fornecido!\n");
                        
                        return 0;
                    }              
                }       
                
                /*retorna se nao existir endereco*/
                i--;
            }
         
            /*se estamos na direita, move pra baixa e pra esquerda*/
            if(pos == DIREITA){
                pos = ESQUERDA;
                
                /*passa pra proxima linha*/
                adress++;
            }
            
            /*se estamos na esquerda, */
            else if(pos == ESQUERDA){
                pos = DIREITA;
            }
            
            /*da um passo na instrucoes*/
            i++;
        }
        
        /*resolve a primeira etapa dos rotulos*/
        else if(comando == LABEL || comando == LABEL_DECLARATION){    
            /*caso encontre a declaracao de um rotulo*/
            if(comando == LABEL_DECLARATION){
                /*encontra em qual linha do label_buffer esta essa palavra*/
                
                j = find_label(label_buffer, saida);
                
                /*escreve o endereço atual na matriz de valors na linha correspondente*/
                sprintf(label_value[j], "%03X", adress);
                
                /*registra a posicao*/
                label_value[j][63] = pos;
                
                /*da um passo nas instrucoes*/
                i++;
            }
            
            /*caso encontre uma referencia de um rotulo*/
            else if(comando == LABEL){
                /*marca na linha que devemos tratar o rotulo*/
                memory_map[adress][0] = -1;
                
                /*indica que soh eh preciso resolver o valor do rotulo*/
                memory_map[adress][11] = 4;
                
                /*da um passo na instrucoes*/
                i++;
            }
        }
    
        else {
            printf("ERROR on line %d\n", memory_map_source_line[i]);
            printf("%s recebeu argumento invalido\n", saida);
                
            return 0;
        }
    }
    
    /*troca os rotulos pelo valor da matriz*/
    for(i = 0; i < 1024; i++){
        /*procura valores marcados pra trocar por rotulos*/
        if(memory_map[i][0] == -1){
            /*habilita linha para impressao*/
            memory_map[i][0] = 1;            
            
            /*resolve rotulos colocados na esquerda*/
            if(memory_map[i][3] < 0 || memory_map[i][3] == 0){
            
                aux = (-1)*memory_map[i][3];
                
                /*se nao eh preciso mudar a instrucao por causa do endereco*/
                if(memory_map[i][4] == 0){
                    copia_endereco((memory_map[i]+3), label_value[aux]);
                }
                
                /*altera a instrucao dependendo do endereco*/
                /*JMP*/
                if (memory_map[i][4] == 1){
                  if(label_value[aux][63] == DIREITA){
                        memory_map[i][1] = '0';
                        memory_map[i][2] = 'E';    
                  }
                }
                
                /*JUMP+*/
                if (memory_map[i][4] == 2){
                    if(label_value[aux][63] == DIREITA){
                        memory_map[i][1] = '1';
                        memory_map[i][2] = '0';  
                  }  
                }
               
               /*STAADR*/
               if (memory_map[i][4] == 3){
                    if(label_value[aux][63] == DIREITA){
                        memory_map[i][1] = '1';
                        memory_map[i][2] = '3';    
                  }
                }
                
                /*finalmente copia o endereco*/
                copia_endereco((memory_map[i]+3), label_value[aux]);
            }
            
            /*resolve rotulos colocados na direita*/
            if(memory_map[i][8] < 0 || memory_map[i][8] == 0){
                aux = (-1)*memory_map[i][8];
                
                if(memory_map[i][9] == 0){
                    copia_endereco((memory_map[i]+8), label_value[aux]);
                }
                
                /*altera a instrucao dependendo do endereco*/
                /*JMP*/
                else if (memory_map[i][9] == 1){
                  if(label_value[aux][63] == DIREITA){
                        memory_map[i][6] = '0';
                        memory_map[i][7] = 'E';    
                  }
                }
                
                /*JUMP+*/
                else if (memory_map[i][9] == 2){
                    if(label_value[aux][63] == DIREITA){
                        memory_map[i][6] = '1';
                        memory_map[i][7] = '0';    
                  }  
                }
               
               /*STAADR*/
               else if (memory_map[i][9] == 3){
                    if(label_value[aux][63] == DIREITA){
                        memory_map[i][6] = '1';
                        memory_map[i][7] = '3';    
                  }
                }
                
                copia_endereco((memory_map[i]+8), label_value[aux]);
            }
        }
    }
	return 1;
}