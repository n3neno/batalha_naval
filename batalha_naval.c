#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define AGUA_DESCONHECIDA 7
#define EMBARCACAO 5
#define ERRO_AGUA 0
#define TIRO_IMPACTO 1
#define TIRO_AFUNDOU 2

typedef struct {
    int tam;
    int qntd_embarcacao;
}config;
// funçoes
int turno_jogador(int **campo_computador, int tam) {
    printf("Sua vez de atacar(3 Tiros)\n");

    int linha = 0, coluna = 0, resultado = 0;
    for (int tiros_dados = 0; tiros_dados < 3; tiros_dados++) {
        do {
            
            printf("Digite a linha e a coluna (ou -1 -1 para SALVAR E SAIR) [Exemplo = 2 4]:\n");
            scanf("%d %d", &linha, &coluna);

            
            if (linha == -1 && coluna == -1) {
                return -1;
            }
            if (linha < 1 || linha > tam || coluna < 1 || coluna > tam) {
                printf("Posicao invalida! Fora dos limites do tabuleiro.\n");
            }

        } while(linha < 1 || linha > tam || coluna < 1 || coluna > tam);

        resultado = tiro(campo_computador, tam, linha - 1, coluna - 1);
        printf("\n===================================\n");
        printf("       TABULEIRO INIMIGO ATUALIZADO\n");
        printf("===================================\n");
        exibir_campo(campo_computador, tam, 1);
        printf("\n");
    }
    
    return 0; 
}
int sobrou_navio(int **campo, int tam) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (campo[i][j] == EMBARCACAO) {
                return 1; 
            }
        }
    }
    return 0; 
}
void turno_computador(int **campo_jogador, int tam){
    printf("\nTurno do Computador(3 Tiros)\n");
    int linha, coluna, resultado = 0;
    for (int i = 0; i < 3; i++){
        linha = rand() % tam;
        coluna = rand() % tam;

        
        printf("\n[Computador Tiro %d] Atirou na linha %d, coluna %d -> \n", i + 1, linha + 1, coluna + 1);
        resultado = tiro(campo_jogador, tam, linha, coluna);
    }
}
int tiro(int **campo, int tam, int linha, int coluna){ // 1 = tiro computado, 0= tiro invalido
    if (campo[linha][coluna] != AGUA_DESCONHECIDA && campo[linha][coluna] != EMBARCACAO){
        printf("Você já atirou aqui! Perdeu o tiro\n");
        return 0;
    }
    if (campo[linha][coluna] == AGUA_DESCONHECIDA){
        campo[linha][coluna] = ERRO_AGUA;
        printf("Água\n");
        return 1;
    }
    if (campo[linha][coluna] == EMBARCACAO){
        campo[linha][coluna] = TIRO_IMPACTO;
        printf("Você atingiu uma embarcação!");

        if (linha + 1 < tam && campo[linha + 1][coluna] == TIRO_IMPACTO) {
            campo[linha][coluna] = TIRO_AFUNDOU;
            campo[linha + 1][coluna] = TIRO_AFUNDOU;
            printf("Embarcação afundada!!\n");
        }

        else if (linha - 1 >= 0 && campo[linha - 1][coluna] == TIRO_IMPACTO) {
            campo[linha][coluna] = TIRO_AFUNDOU;
            campo[linha - 1][coluna] = TIRO_AFUNDOU;
            printf("Embarcação afundada!!\n");
        }
        else if (coluna + 1 < tam && campo[linha][coluna + 1] == TIRO_IMPACTO) {
            campo[linha][coluna] = TIRO_AFUNDOU;
            campo[linha][coluna + 1] = TIRO_AFUNDOU;
            printf("Embarcação afundada!!\n");
        }
        else if (coluna - 1 >= 0 && campo[linha][coluna - 1] == TIRO_IMPACTO) {
            campo[linha][coluna] = TIRO_AFUNDOU;
            campo[linha][coluna - 1] = TIRO_AFUNDOU;
            printf("Embarcação afundada!!\n");
        }

    }
    return 1;

}
void exibir_campo(int ** campo, int tam, int ecmp){
    for (int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if (ecmp == 1 && campo[i][j] == EMBARCACAO) {
                printf("%d ", AGUA_DESCONHECIDA);
            } 
            
            else {
                printf("%d ", campo[i][j]);
            }
            
        }
        printf("\n");
        }
       
    }

int **alocar_campo(int tam){
    int **campo = (int**)malloc(tam * (sizeof(int*)));

    for (int i = 0; i < tam; i++){
        campo[i] = (int*)malloc(tam * (sizeof(int)));
    }
    for (int i = 0; i < tam; i++){
        for (int j = 0; j < tam; j++){
            campo[i][j] = AGUA_DESCONHECIDA;
        }
    }
    return campo;
}

void posicionar_embarcacoes(int **campo, int tam, int qntd_embarcacao) {
    for (int i = 0; i < qntd_embarcacao; i++) {
        int valido;
        int linha, coluna, direcao;
        
        do {
            valido = 1;
            linha = rand() % tam;
            coluna = rand() % tam;
            direcao = rand() % 2;

            if (campo[linha][coluna] != AGUA_DESCONHECIDA) {
                valido = 0;
            } else {
                if (direcao == 0) { // horizontal
                    if (coluna + 1 >= tam || campo[linha][coluna + 1] != AGUA_DESCONHECIDA) {
                        valido = 0;
                    }
                } else { // vertical
                    if (linha + 1 >= tam || campo[linha + 1][coluna] != AGUA_DESCONHECIDA) {
                        valido = 0;
                    }
                }
            }
        } while(valido == 0); 

        campo[linha][coluna] = 5;
        if (direcao == 0) {
            campo[linha][coluna + 1] = 5; 
        } else {
            campo[linha + 1][coluna] = 5; 
        }
    }
}

int ler_save(config *c){
    FILE * abrir = fopen("../historico.bin", "rb");
    if (abrir == NULL){
        printf("Erro ao abrir arquivo binário");
        return 1;
    }

    fread(c, sizeof(config), 1, abrir);
    fclose(abrir);

    return 0;
}

int adicionar_save(config *c){
    FILE * salvar = fopen("../historico.bin", "wb");
    if (salvar == NULL){
        printf("Erro ao abrir o arquivo binário");
        return 1;
    }

    fwrite(c, sizeof(config), 1, salvar);
    fclose(salvar);

    return 0;
}
int salvar_partida(config *settings, int **campo_jogador, int **campo_computador) {
    FILE *arquivo = fopen("../partida_salva.bin", "wb");
    
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar o arquivo de save.\n");
        return 0; 
    }

    fwrite(settings, sizeof(config), 1, arquivo);

    for (int i = 0; i < settings->tam; i++) {
        fwrite(campo_jogador[i], sizeof(int), settings->tam, arquivo);
    }

   
    for (int i = 0; i < settings->tam; i++) {
        fwrite(campo_computador[i], sizeof(int), settings->tam, arquivo);
    }

    fclose(arquivo);
    printf("\nPartida salva com sucesso\n");
    return 1; 
}

int main(){
    srand(time(NULL));
    int opcao_menu = 0;

    config *settings = malloc(sizeof(config));
    int **campo_jogador = NULL;
    int **campo_computador = NULL;

    printf("BATALHA NAVAL\n");
    printf("1 - Novo Jogo\n");
    printf("2 - Novo Jogo com as configurações do último jogo completo\n");
    printf("3 - Carregar Jogo Salvo\n");
    printf("4 - Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao_menu);

    if (opcao_menu == 4){
        printf("Saindo do jogo\n");
        free(settings);
        return 0; 
    } 
    else if (opcao_menu == 3) {
        FILE *arquivo = fopen("../partida_salva.bin", "rb");
        if (arquivo == NULL) {
            printf("Erro: Nenhum jogo salvo encontrado!\n");
            free(settings);
            return 1;
        }
        
        fread(settings, sizeof(config), 1, arquivo);
        
        campo_jogador = alocar_campo(settings->tam);
        campo_computador = alocar_campo(settings->tam);
        
        for (int i = 0; i < settings->tam; i++) {
            fread(campo_jogador[i], sizeof(int), settings->tam, arquivo);
        }
        for (int i = 0; i < settings->tam; i++) {
            fread(campo_computador[i], sizeof(int), settings->tam, arquivo);
        }
        fclose(arquivo);
        printf("Jogo carregado com sucesso!\n");
    } 
    else {
        if (opcao_menu == 2){
            ler_save(settings);
        } 
        else if (opcao_menu == 1) {
            
            do {
                printf("Digite um número para definir o número de linhas e colunas do campo aquático [Minímo é 8]\n");
                scanf("%d", &settings->tam);

                if (settings->tam < 8){
                    printf("Digite um tamanho válido, este valor deve ser 8 ou superior.\n");
                }
            } while (settings->tam < 8);
            
            
            do {
                printf("Digite o número de embarcações [Minímo é 4]\n");
                scanf("%d", &settings->qntd_embarcacao);

                if (settings->qntd_embarcacao < 4){
                    printf("Digite um número válido, este valor deve ser 4 ou superior.\n");
                }
            } while(settings->qntd_embarcacao < 4);

            adicionar_save(settings);
            printf("Configurações salvas com sucesso!\n");
        }

        printf("Iniciando novo jogo..\n");
        printf("\nAGUA_DESCONHECIDA = 7 \n EMBARCACAO = 5 \nERRO_AGUA = 0 \nTIRO_IMPACTO = 1 \nTIRO_AFUNDOU = 2\n");
        campo_jogador = alocar_campo(settings->tam);
        campo_computador = alocar_campo(settings->tam);

        posicionar_embarcacoes(campo_jogador, settings->tam, settings->qntd_embarcacao);
        posicionar_embarcacoes(campo_computador, settings->tam, settings->qntd_embarcacao);
    }

    
    while(sobrou_navio(campo_computador, settings->tam) && sobrou_navio(campo_jogador, settings->tam)){
        printf("\n===================================\n");
        printf("         SEU TABULEIRO\n");
        printf("===================================\n");
        exibir_campo(campo_jogador, settings->tam, 0);

        printf("\n===================================\n");
        printf("       TABULEIRO INIMIGO\n");
        printf("===================================\n");
        exibir_campo(campo_computador, settings->tam, 1);

        int status_turno = turno_jogador(campo_computador, settings->tam);
        if (status_turno == -1) {
            printf("\nSalvando a partida...\n");

            salvar_partida(settings, campo_jogador, campo_computador);
            break; 
        }
       
        if (!sobrou_navio(campo_computador, settings->tam)) {
            printf("\nPARABÉNS! VOCÊ DESTRUIU A FROTA INIMIGA!\n");
            break;
        }

        turno_computador(campo_jogador, settings->tam);

        if(!sobrou_navio(campo_jogador, settings->tam)){
            printf("Você perdeu!!\n");
            break;
        }
    }

    for (int i = 0; i < settings->tam; i++) {
        free(campo_computador[i]);
    }
    free(campo_computador);
    
    for (int i = 0; i < settings->tam; i++) {
        free(campo_jogador[i]);
    }
    free(campo_jogador);
    
    free(settings);
    return 0;
}

