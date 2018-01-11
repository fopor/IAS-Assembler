#include "line_solver.h"

/*resolve a linha recebida*/
int solve_entrada(char *comando_entrada, char *string_saida, char *map_buffer,
	int  *map_buffer_pos_pointer, char alias_buffer[][64],  char alias_value[][64],  
    char label_buffer[][64], char label_value[][64], int *label_l, int *alias_l, int line_number, int memory_map_source_line[]) {
    int i,  j, aux, aux_, aux_counter,k;
    char command_buffer[150];
    char mnemonico_ready[150];
    int label_line = *label_l;
    int alias_line = *alias_l;
    int n_instrucoes = 0;
    int command_counter = 0;
    
    /*trata tudo qeu eh separado por espacos, ante encontrar umas
    quebra de linha ou o inicio de um comentario*/
	 i = 0;
     aux_ = -1;
     aux_counter = -1;
    while ((comando_entrada[i] != '#') && (comando_entrada[i] != '\n')
    && (comando_entrada[i] != '\0')) {
		j = 0;

        /*pula os espacos*/
		while(isspace(comando_entrada[i])){
            i++;
        }
		
		/*se apos s spacos temos o fim da linha, linha resolvida*/
		if(comando_entrada[i] == '\n' || comando_entrada[i] == '\0')
			return 1;

		/*copia os comandos separadamente para vetor auxiliar*/
        while(!(isspace(comando_entrada[i+j])) && comando_entrada[i+j] != '#'  && comando_entrada[i+j] != '\0'){
            command_buffer[j] = comando_entrada[i+j];
            j++;
        }
		
		/*se nao ha comando, encerra a busca*/
        if (j == 0) {
            /*linha resolvida*/
            return 1;
        }

        command_buffer [j] = '\0';

        /*resolve o comando encontrado, ou pula se estiver resolvendo o .set*/
        if(aux_counter == -1) {          
            aux = solve_mnemonico(command_buffer, mnemonico_ready, 0, alias_buffer, alias_value, alias_line, label_buffer, label_value, label_line);
        }
        
        /*verifica se houve erro na resolucao do comando*/
        if(aux == -1) {
            strcpy(string_saida, mnemonico_ready);
            return 0;
        }

        /*insere o alias do .set na lista*/
        if(aux == 3) {
           /*recupera o alias*/
            if(aux_counter == 0) {
                aux_ = solve_mnemonico(command_buffer, mnemonico_ready, 1, alias_buffer, alias_value , alias_line, label_buffer, label_value, label_line);
                     
                /*em caso de  nome invalido, encerra execucao*/
                if(aux_ != 4 || (mnemonico_ready[0] >= '0' && mnemonico_ready[0]<='9')) {
                   strcpy(string_saida, mnemonico_ready);
                   
                    return 0;
                }
            
                aux_counter++;
            }
            
            /*recupera o valor do alias*/
            else if( aux_counter == 1) {
                aux_ = solve_mnemonico(command_buffer, mnemonico_ready, 2, alias_buffer, alias_value , alias_line, label_buffer, label_value, label_line);
                
                /*em caso de valor invalido, encerra execucao*/
                 if(aux_ != 6 && aux_ != 1) {

                    strcpy(string_saida, mnemonico_ready);
     
                    return 0;
                }
                
                aux_counter++;
            }
            
            else if(aux_counter == 2) {
                alias_line++;            
                aux_ = -1;
                aux_counter = -1;
            }
            
            else if(aux_counter == -1) {
                aux_counter++;
            }
        }

        /*salva comando na saida se o mnemonico for uma instrucao*/
        if(aux == 1 || aux == 5)  {
            if(aux == 5){
                n_instrucoes++;
            }

            /*guarda o numero da linha na proxima posicao de vetor de numeros de linha*/
            k = 0;
            while(memory_map_source_line[k] != -1){
                k++;
            }
            
            /*anota a linha da inscricao*/
            memory_map_source_line[k] = line_number;
            command_counter++;
            
            if(command_counter >= 4) {
                strcpy(string_saida, "Duas instrucoes na mesma linha");
        
                return 0;
            }
            
          
            insere_comando_valido(mnemonico_ready, map_buffer_pos_pointer, map_buffer);
        }

        i+= j;
    }
	
    /*verifica se nao houve mais de uma instrucao na mesma linha*/
    if(n_instrucoes > 1){
        strcpy(string_saida, "Duas instrucoes na mesma linha");
        
        return 0;
    }
    
	/*se a linha foi bem resolvida, retorna 1*/
	return 1;
}

/*guarda todos os comandos no buffer de entrada*/
int insere_comando_valido(char *mnemonico_ready, int *map_buffer_pos_pointer,
char *map_buffer){

	for(int i = 0; mnemonico_ready[i] != 0; i++){
		map_buffer[(*map_buffer_pos_pointer)] = mnemonico_ready[i];
		(*map_buffer_pos_pointer) += 1;
	}	
	
	/*coloca -1 como separador entre comandos no buffer*/
	map_buffer[++(*map_buffer_pos_pointer)] = -1;
	
	/*indica que a posicao que pode ser escrita esta no proximo espaco do vetor buffer*/
	(*map_buffer_pos_pointer) += 1;
	
	return 0;
}