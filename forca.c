/* 
--------------------------------------------------------------------------|
| Programa: forca_fascinante.c  										  |
| Objetivo: criar um jogo de forca usando listas encadeadas               |
| Autor: João Pedro de Azevedo Zonzini @ 30/12/25						  |
--------------------------------------------------------------------------|
*/ 

// importação de bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

// definição de constantes de manipulação de cores no terminal
// exemplo de uso - printf(CVERM);
#define CRESET "\x1B[0m"    // reset
#define CVERM  "\x1B[31m"   // tudo a seguir escrito em vermelho
#define CVERD  "\x1B[32m"   // tudo a seguir escitro em verde

// definição de constantes para estado do chute nas structs Alfabeto
#define CHUTE_RESET 0
#define CHUTE_CERTO 1
#define CHUTE_ERRADO 2

// definição de constante de número de erros máximos no jogo
#define N_TENTATIVAS 5

// definição da estrutura base de lista encadeada
typedef struct alfab {
    char letra;                 // letra salva no node
    int chutado;                // info se letra foi chutada ou não, ver constantes acima
    struct alfab *prox;         // pointeiro para o próximo node
} Alfabeto;

// declaração de protótipos de funções
Alfabeto *criar_letra(char letra); Alfabeto *criar_alfabeto(); void append_letra(Alfabeto *head, Alfabeto *nova_letra);
void printar_alfabeto(Alfabeto *head); Alfabeto *encontrar_letra(Alfabeto *head, char letra); Alfabeto *criar_alfabeto();
Alfabeto *criar_segredo(char s_segredo[]); void auth_segredo(Alfabeto *head, char chute);
int acertou_segredo(Alfabeto *head); void printar_segredo(Alfabeto *head);
int auth_chute(Alfabeto *alfabeto, Alfabeto *segredo, char chute); void free_nodes(Alfabeto *head);
char *escolher_segredo(char *segredo_escolhido); void desenhar_cabecalho(int tentativas, char *segredo);

int main(void){
    
    // declaração de variaveis
    int tentativas, continuar_jogando = 1;
    char *s_segredo;
    char chute;
    char temp;
    
    do {
        // escolher o segredo
        s_segredo = escolher_segredo(s_segredo);
        
        // declaração das estruturas alfabeto e segredo
        Alfabeto *alfabeto = criar_alfabeto();
        Alfabeto *segredo = criar_segredo(s_segredo);
        Alfabeto *chute_hist = criar_letra(64);
        
        // reseta o número de tentativas
        tentativas = N_TENTATIVAS;    
    
        do {
            system("clear");
            
            puts("----> forca_fascinante <----\n\n");
            // printa segredo em forma de _ _ _ _ e alfabeto com os chutes disponíveis
            printf("\t");
            printar_segredo(segredo);
            puts("");
            printar_alfabeto(alfabeto);
            puts("");
            desenhar_cabecalho(tentativas, s_segredo);
    
            // aceita chute
            printf("Chute uma letra: ");
            scanf(" %c", &chute);
    
            // mesmo que input seja minúsculo, transforma em maiúsculo
            // também garante que o input é uma letra válida
            if (chute >= 97 && chute <= 122) {
                chute = (chute - 97) + 65;
            } else if (chute < 65 || chute > 90) {
                continue;
            }
            
            if (encontrar_letra(chute_hist, chute) == NULL) {
                // chute novo
                append_letra(chute_hist, criar_letra(chute));
            } else {
                // chute já feito antes
                continue;
            }
    
            // função verifica o chute, altera o alfabeto e o segredo e retorna a alteração da vida
            // chute errado --> vida diminui
            // chute certo --> vida não se altera
            tentativas += auth_chute(alfabeto, segredo, chute);
        } while (!acertou_segredo(segredo) && tentativas > 0);
        // enquanto o jogador não acertou o segredo e há tentativas
        // --> o jogo continua
        // daqui para frente ou o segredo foi descoberto ou as tentativas acabaram
        
        system("clear");
        
        printf("FIM DE JOGO! -----> ");
    
        // mostra segredo no estado que saiu do loop anterior
        printar_segredo(segredo);
    
        // parabeniza ou não o jogador
        if (acertou_segredo(segredo)) {
            printf("Parabéns, você descobriu o segredo!\n");
        } else {
            printf("Poxa, não foi dessa vez.\n");
        }

        // evitando memory leak
        free_nodes(alfabeto);
        free_nodes(segredo);
        free_nodes(chute_hist);

        // continuar jogando? tem que ser 0 ou 1 --> do-while verifica isso
        do {
            printf("Quer continuar jogando? (1)SIM (0)NÃO: ");
            scanf(" %d", &continuar_jogando);
        } while (!(continuar_jogando >= 0 && continuar_jogando < 2));

    } while (continuar_jogando);
    // se continuar jogando --> comeca tudo de novo

    free(s_segredo);

	return 0;
}

// função cria node com a letra para se inserir no alfabeto ou no segredo
// recebe char letra e retorna o node Alfabeto
Alfabeto *criar_letra(char letra){
    // alocando memória para o node
    Alfabeto *nova_letra = (Alfabeto *) malloc(sizeof(Alfabeto));
    // malloc usado --> memória alocada manualmente --> perigo de memory leak

    // limpa área da memória para evitar garbage
    memset(nova_letra, 0, sizeof(*nova_letra));

    // insere letra no node
    nova_letra->letra = letra;
    // no início, todas as letras estão no estado inicial de não terem sido chutadas
    nova_letra->chutado = CHUTE_RESET;
    // este node novo é o último
    nova_letra->prox = NULL;

    // retorna node novo
    return nova_letra;
}

// função que adiciona node ao final da lista encadeada, seja o alfabeto ou o segredo
void append_letra(Alfabeto *head, Alfabeto *nova_letra) {
    Alfabeto *iter = head;

    // enquanto o iter ainda é um item da lista encadeada
    while (iter != NULL) {
        // se for o último node da lista, append o novo node
        if (iter->prox == NULL) {
            iter->prox = nova_letra;
            nova_letra->prox = NULL;
            break;
        }
        // se não for o último node, passa para o proximo
        iter = iter->prox;
    }
}

// função para printar o alfabeto
void printar_alfabeto(Alfabeto *head) {
    // node iter para não perder o head
    Alfabeto *iter = head;

    // enquanto o iter não é o último node da lista encadeada
    while (iter != NULL) {

        // verifica a variavel iter->chutado para ver qualquer pintar a tela
        switch (iter->chutado) {
            case CHUTE_CERTO:
                // verde
                printf(CVERD);
                printf("%c ", iter->letra);
                printf(CRESET);
                break;

            case CHUTE_ERRADO:
                //vermelho
                printf(CVERM);
                printf("%c ", iter->letra);
                printf(CRESET);
                break;

            default: // significa que não foi chutado ainda --> branco
                printf("%c ", iter->letra);
        }

        // passa o iter para o próximo node
        iter = iter->prox;
    }

    puts("");
}

// função para procurar node dentro da lista encadeada
Alfabeto *encontrar_letra(Alfabeto *head, char letra) {
    // criar node iter para não perder o head
    Alfabeto *iter = head;

    // varre a lista encadeada
    while (iter != NULL) {
        // se a letra for encontrada, retorna este node
        if (iter->letra == letra) {
            return iter;
        }
        iter = iter->prox;
    }
    // se a letra não foi encontrada, retorna um ponteiro nulo
    return NULL;
}

// função para programaticamente popular a struct alfabeto
// com as letras de A a Z
Alfabeto *criar_alfabeto() {
    // criar ponteiro para o primeiro node --> *head
    // *iter vai ser usado para "varrer" a lista
    Alfabeto *iter, *head;

    // inicializando a lista
    iter = criar_letra(65);
    head = iter;

    // para cada letra do alfabeto inserir na lista
    for (int i = 1; i < 26; i++) {
        append_letra(iter, criar_letra(65 + i));
    }

    return head;
}

// função para transformar a string s_segredo em lista encadeada
Alfabeto *criar_segredo(char s_segredo[]) {
    // criar ponteiro para o primeiro node --> *segredo
    // *iter vai ser usado para "varrer" a lista
    Alfabeto *iter, *segredo;

    // inicializando a lista
    segredo = criar_letra(s_segredo[0]);
    iter = segredo;

    // para cada letra da string s_segredo,
    // inserir uma letra nova no segredo
    for (int i = 1; i <= strlen(s_segredo); i++) {
        append_letra(iter, criar_letra(s_segredo[i]));
    }

    return segredo;
}

// função para verificar se um chute está correto
// recebe struct *segredo e char chute
void auth_segredo(Alfabeto *head, char chute) {
    // *iter vai ser usado para "varrer" a lista e não perder o head
    Alfabeto *iter = head;

    // enquanto a lista não acabar, verificar chute
    while (iter->prox != NULL) {
        if (iter->letra == chute && iter->chutado != CHUTE_CERTO) {
            iter->chutado = CHUTE_CERTO;
        }
        // ausência de else para forçar a varredura completa da lista
        // assim duas letras iguais necessariamente são verificadas
        iter = iter->prox;
    }
}

// função para verificar se todas as letras do segredo foram descobertas
int acertou_segredo(Alfabeto *head) {
    // *iter vai ser usado para "varrer" a lista
    Alfabeto *iter = head;

    while (iter->prox != NULL) {
        if (iter->chutado == CHUTE_RESET) { // ainda há chutes a serem feitos
            return 0;
        }
        iter = iter->prox;
    }

    // o segredo já foi descoberto
    return 1;
}

// função para imprimir na tela o segredo com a letra aparente ou como underline
void printar_segredo(Alfabeto *head) {
    // *iter vai ser usado para "varrer" a lista
    Alfabeto *iter = head;

    while (iter->prox != NULL) {
        if (iter->chutado == CHUTE_RESET) { // não foi chutado ainda --> _
            printf("_ ");
        } else {
            printf("%c ", iter->letra); // já se sabe a letra --> printa ela
        }
        iter = iter->prox;
    }

    puts("");
}

// função para verificar se chute está no alfabeto e no segredo
int auth_chute(Alfabeto *alfabeto, Alfabeto *segredo, char chute) {
    // verificar se a letra está no segredo
    // fatorar codigo !!!
    if (encontrar_letra(segredo, chute) != NULL) {
        encontrar_letra(alfabeto, chute)->chutado = CHUTE_CERTO; // 
        auth_segredo(segredo, chute);
    } else { // chute errado
        if (encontrar_letra(alfabeto, chute)->chutado == CHUTE_RESET) {
            encontrar_letra(alfabeto, chute)->chutado = CHUTE_ERRADO;     // marca chute no alfabeto como errado (pinta de vermelho)
            return -1;                                                    // tira uma tentativa do jogador e sai da função
        }
    }
    return 0;  // não entrou no if do chute errado e saiu --> chute certo --> não tira tentativa do jogador
}

// função para lidar com a memória alocada manualmente nas structs / lista encadeada
void free_nodes(Alfabeto *head) {
    // *iter vai ser usado para "varrer" a lista
    Alfabeto *iter = head;
    Alfabeto *prev;

    // cada node foi alocado manualmente --> cada node deve ser desalocado 
    while (iter->prox != NULL) {
        prev = iter;
        iter = iter->prox;

        free(prev);
    }

    free(iter);
}

// função para ler o arquivo de segredos e escolher um aleatoriamente
char *escolher_segredo(char *segredo_escolhido) {

    // declaração da string segredo_escolhido que será retornada
    segredo_escolhido = calloc(10, sizeof(char));

    // declaração de variáveis
	char **segredos;
	int n = -1;
	int pos_segredo;

	// alocando o primeiro ptr para um segredo_escolhido no array
	segredos = (char **) calloc(10, sizeof(char *));

	// ponteiro para arquivo de segredos
    FILE *arq;

    // abre arquivo de segredos
    arq = fopen("segredos.txt", "r");

    // se arq for null algo na abertura do arquivo deu errado
    if (arq == NULL) {
        puts("ERRO NA LEITURA DOS SEGREDOS");
        exit(1);
    }    

	// determinar número de segredos
	char c;
	while (!feof(arq)) {
    	fscanf(arq, "%c", &c);
    	if (c == '\n') {
      		n++;
    	}
  	}

	// reseta o ponteiro do arquivo para a posição inicial
	rewind(arq);

	// alocar dinamicamente a quantidade de segredos
	segredos = (char **) calloc(n, sizeof(char *));
	for (int i = 0; i < n; i++) {
		segredos[i] = (char *) calloc(10, sizeof(char *));
	}

	// ler no vetor segredos os segredos do arquivo
	fscanf(arq, "%s", segredos[0]);
	for (int i = 1; i < n; i++) {
		fscanf(arq, "\n%s", segredos[i]);
	}

	// gerar posição aleatória na array segredos e copiar em segredo_escolhido
	srand(time(NULL));
	pos_segredo = rand() % n;
	strncpy(segredo_escolhido, segredos[pos_segredo], 9);

	// evitando memory leak
	for (int i = 0; i < n; i++) {
		free(segredos[i]);
	}
	free(segredos);

    // tudo que é aberto tem que ser fechado
    fclose(arq);
    
    return segredo_escolhido;
}

// função para desenhar o número de tentativas e o número de letras do segredo
void desenhar_cabecalho(int tentativas, char *segredo) {
    int n_letras = 0;

    n_letras = strlen(segredo);

    printf("--> %d letras\n\n", n_letras);
    puts("\t-----------------------------");
    printf("\t|  Tentativas restantes: %d  |\n", tentativas);
    puts("\t-----------------------------\n");
}
