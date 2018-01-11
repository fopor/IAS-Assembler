#include "main.h"

 /*inicializa o buffer*/
void inicializa_buffer(char *buffer);

/*fecha os arquivos abertos*/
void close_files(FILE *input_file, FILE *output_file);

int main( int argc, char *argv[] ){

    /*apontadores para os arquivos de entrada e saida*/
    FILE *input_file = NULL;
    FILE *output_file = NULL;

    /*variavel auxiliar para o tipo de saida*/
    int tipo_saida;
    
    /*buffer para as instrucoes*/
    char buffer[150];
    char buffer2[150];
    char string_saida[150];
	char map_buffer[4096];
    char alias_buffer[500][64];
    char alias_value[500][64];
    char label_buffer[500][64];
    char label_value[500][64];
    char memory_map[1024][15];
    int memory_map_source_line[8000];
    
    /*guarda em qual posica do map_buffer estamos*/
	int map_buffer_pos = 0;
	int *map_buffer_pos_pointer = &map_buffer_pos;
    
    /*conta em qual linha estamos no arquivo de entrada*/
    int line_number = 0;
    int label_line = 0;
    int alias_line = 0;
    
    /*variaveis para percorrer vetores e lacos*/
    int i,j,k;

    /*variaveis para controle de numero de linha*/
    int *label_line_adress = &label_line;
    int *alias_line_adress = &alias_line;
    
    /*contadores auxiliar*/
    int n_diretivas;
    int n_rotulos;
    int n_aspas;
    
    /*passa o tipo de saida para a variavel auxiliar*/
    tipo_saida = argc;
    
    /*verifica se o numero de argumentos passados eh valido*/
    if (argc != 2 && argc != 3){
        printf("Numero de argumentos invalido\n");
        return 0;
    }

    /*verifica se o arquivo pode ser aberto*/
    input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Arquivo de entrada nao encontrado.\n");
        return 0;
    }
    
    /*cria o arquivo de saida, se solicitado*/
    if(argc == 3) {
        output_file = fopen(argv[2], "w+");
        
        /*interrompe se o arquivo de saida nao pode ser criado*/
        if (output_file == NULL){
            printf("Nao foi possivel criar o arquivo de saida\n");
			
			/*fecha o arquivo de entrada*/
			close_files(input_file, output_file);
            return 0;
        }
    }
    
	/*inicializa map_buffer*/
	for(i = 0; i < 4096; i++){
		map_buffer[i] = 0;
	}
    
    /*inicializa o marcador de linhas*/
    for(i = 0; i < 8000; i++){
        memory_map_source_line[i] = -1;
    }
	
    /*inicializa os buffers*/
    for(i = 0; i < 500; i++){
        for(j = 0; j < 64; j++){
            alias_buffer[i][j] = 0;
            alias_value[i][j] = 0;
            label_buffer[i][j] = 0;
            label_value[i][j] = 0;
        }
    }

    /*inicializa o memory_map de sadia*/
    for(i = 0; i < 1024; i++){
        memory_map[i][0] = 0;
        
        for(j = 1; j < 15; j++){
            memory_map[i][j] = '0';
        }
    }
    
    /*monta primeiro o dicionario de labels*/
    line_number = 0;
    /*le o arquivo de entrada linha a linha*/
    do{
        /*conta em qual linha esta o solver*/
        line_number += 1;
        
        /*inicializa o buffer e a saida*/
        inicializa_buffer(buffer);
        inicializa_buffer(buffer2);
		string_saida[0] = '\0';
        
        /*coloca o conteudo da linha*/
        fgets(buffer, 150, input_file);
        
        /*verifica se temos duas diretivas na mesma linha*/
        j = 0;
        n_diretivas = 0;
        n_rotulos = 0;
        n_aspas = 0;
        while(buffer[j] != '\0'  && buffer[j] != '#'){
            if(buffer[j]=='.'){
                n_diretivas++;
            }
            
            if(buffer[j]==':'){
                n_rotulos++;
            }
            
            if(buffer[j] == '"'){
                n_aspas++;
            }
            
            j++;
        }
        
        /*somente instrucoes podem aceitar argumentos com aspas*/
        if(n_aspas && n_diretivas){
            printf("ERROR on line %d\n", line_number);
            printf("Diretivas nao podem receber argumentos entre aspas!\n");
            
            close_files(input_file, output_file);
            
            return 0;
        }
        
        /*emite erro se houver mais de uma declaracao de rotulo na linha*/
        if(n_diretivas>1){
            printf("ERROR on line %d\n", line_number);
            printf("Mais de uma diretiva na mesma linha!\n");
            
            close_files(input_file, output_file);
            
            return 0;
        }
                
        /*emite erro se houver mais de uma declaracao de rotulo na linha*/
        if(n_rotulos >1){
            printf("ERROR on line %d\n", line_number);
            printf("Mais de um rotulo declarado na mesma linha!\n");
            
            close_files(input_file, output_file);
            
            return 0;
        }
        
        j = 0;
        /*verific se ha algo antes dos rotulo*/
        while(buffer[j] != '\0'  && buffer[j] != '#'){
            
            /*se encontrar um rotulo, verifica se ha algo antes dele*/
            if(buffer[j] == ':'){                
                /*vai ate o comeco do rotulo*/
                while(j >= 0 && !(isspace(buffer[j]))){
                    j--;
                }
                
                /*verifica se ha algo antes do rotulo*/
                while(j >= 0){
                     if(!isspace(buffer[j])){
                        
                        printf("ERROR on line %d\n", line_number);
                        printf("Os rotulos devem ser declarados no inicio da linha!\n");
                        
                        close_files(input_file, output_file);
                        return 0;
                    }
                    j--;
                }
                /*nao eh preciso procurar mais, porque soh eh possivel 1 rotulo por linha*/
                break;
            }
            j++;
        }
        
        /*coloca os rotulos no dicionario*/
        j = 0;
        k = 0;        
        while(buffer[j] != '\0'  && buffer[j] != '#'){
            if(buffer[j] == ':'){
                /*volta ate o comeco da declaracao do rotulo*/
                while(buffer[j] != ' ' && j >= 0 && buffer[j] != '\t' && buffer[j] != '\r'){
                    k++;
                    j--;
                }
                
                if(k == 0){
                    printf("ERROR on line %d\n", line_number);
                    printf("Rotulo vazio declarado!\n");
                    
                    close_files(input_file, output_file);
                    return 0;
                }
                
                /*copia o label declarado para o buffer 2*/
                while(k > 0){
                    buffer2[k-1] = buffer[j+k];
                    k--;
                }
                
                /*verifica se o rotulo ja nao foi declarado*/
                /*tira o ':' para testar*/
                k = strlen(buffer2);
                buffer2[k-1] = '\0';
                if(find_label(label_buffer, buffer2) != -1){ 
                    printf("ERROR on line %d\n", line_number);
                    printf("Declaracao de rotulo com nome ja utilizado!\n");
                    
                    close_files(input_file, output_file);
                    return 0;
                }
                
                /*devolve os ':' antes de inserir no dicionario*/
                buffer2[k-1] = ':';
                buffer2[k] = '\0';
                
                /*insere mnemonico na lista*/
                solve_mnemonico(buffer2, string_saida, MONTA_LABEL_TABLE, alias_buffer, alias_value, alias_line, label_buffer, label_value, label_line);
                break;
            }
            j++;
        }
		
               
    }while(!feof(input_file));
    
    rewind(input_file);
    
    line_number = 0;
    /*le o arquivo de entrada linha a linha*/
    do{
        /*conta em qual linha esta o solver*/
        line_number += 1;
        
        /*inicializa o buffer e a saida*/
        inicializa_buffer(buffer);
		string_saida[0] = '\0';
        
        /*coloca o conteudo da linha*/
        fgets(buffer, 150, input_file);
        
        /*verifica se houve um comando invalido*/
        if (solve_entrada(buffer, string_saida, map_buffer,  map_buffer_pos_pointer, alias_buffer, alias_value, label_buffer, label_value, 
        label_line_adress, alias_line_adress, line_number, memory_map_source_line) == 0){
            printf("ERROR on line %d\n", line_number);
            j = 0;
			
            printf("%s nao eh uma entrada valida\n", string_saida);
			
			close_files(input_file, output_file);
    
            return 0;
        }
               
    }while(!feof(input_file));
    
	/*resolve as diretivas*/
	if(diretivas_solver(map_buffer, map_buffer_pos_pointer, memory_map, label_buffer, label_value,  memory_map_source_line) == 0) {
        
        /*em caso de erro*/
        close_files(input_file, output_file);
        return 0;
    }
	
    /*verifica se tem instrucao e diretiva na mesma linha*/
     for(i = 0; memory_map_source_line[i] != -1; i++){
         k = 0;
         while(memory_map_source_line[i+1] == memory_map_source_line[i]){
             k++;
             i++;
         }
         if(k >=4){
             printf("ERROR on line %d\n", memory_map_source_line[i]);
             printf("Muitas entradas na mesma linha!\n");
             
             close_files(input_file, output_file);
             return 0;
         }
     }
    
    /*coloca todos os hexadecimais em uppercase*/
    for(i = 0; i < 1024; i++){
        j = 0;
        while(memory_map[i][j]){
            if(memory_map[i][j] >= 'a' && memory_map[i][j]<= 'z'){
                memory_map[i][j] -= 32;
            }
            j++;
        }
    }
    
    
    /*imprime o resultado na saida*/
    if(tipo_saida == SAIDA_PADRAO){
        /*imprime a saida na saida padrao*/
        for(i = 0; i < 1024; i++){
            if(memory_map[i][0]){
                j = 1;
                
                /*coloca o endereco*/
                fprintf(stdout, "%03X ", i);
                
                /*coloca a palavra*/
                while(j < 3) {
                    fprintf(stdout, "%c", memory_map[i][j]);
                    j++;
                }
                
                fprintf(stdout, " ");
                while(j < 6) {
                    fprintf(stdout, "%c", memory_map[i][j]);
                    
                    j++;
                }
                
                fprintf(stdout, " ");
                while(j < 8) {
                    fprintf(stdout, "%c", memory_map[i][j]);
                    
                    j++;
                }
                
                fprintf(stdout, " ");
                while(j < 11) {
                    fprintf(stdout, "%c", memory_map[i][j]);
                    
                    j++;
                }
                
                fprintf(stdout, "\n");
            }
        }
        
        /*fecha o arquivo de entrada*/
        close_files(input_file, output_file);
        
        return 0;
    }
    
    /*imprime o resultado no arquivo de saida*/
    if(tipo_saida == SAIDA_ARQUIVO) {
        /*imprime a saida no arquivo de saida*/
        for(i = 0; i < 1024; i++){
            if(memory_map[i][0]){
                j = 1;
                
                /*coloca o endereco*/
                fprintf(output_file, "%03X ", i);
                
                /*coloca a palavra*/
                while(j < 3) {
                    fprintf(output_file, "%c", memory_map[i][j]);
                    j++;
                }
                
                fprintf(output_file, " ");
                while(j < 6) {
                    fprintf(output_file, "%c", memory_map[i][j]);
                    
                    j++;
                }
                
                fprintf(output_file, " ");
                while(j < 8) {
                    fprintf(output_file, "%c", memory_map[i][j]);
                    
                    j++;
                }
                
                fprintf(output_file, " ");
                while(j < 11) {
                    fprintf(output_file, "%c", memory_map[i][j]);
                    
                    j++;
                }
                
                fprintf(output_file, "\n");
            }
        }
        
        /*fecha o arquivo do entrada e de saida*/
        close_files(input_file, output_file);
        
        return 0;
    }
	
    return 0;
}

 /*inicializa o buffer*/
void inicializa_buffer(char *buffer){
    for(int i = 0; i < 150; i++){
        buffer[i] = 0;
    }
}

/*fecha os arquivos abertos*/
void close_files(FILE *input_file, FILE *output_file){
	/*fecha o arquivo de entrada*/
    fclose(input_file);
    
    /*fecha o arquivo de saida, se existir*/
    if(output_file != NULL) {
        fclose (output_file);
    }
}