// forca.c
// Jogo da forca implementado com linked lists
// jaz 30/12/25

// importacao de bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

// definicao de constantes de manipulacao de cores no terminal
// exemplo de uso - printf(CVERM);
#define CRESET "\x1B[0m"    // reset
#define CVERM  "\x1B[31m"   // tudo a seguir escrito em vermelho
#define CVERD  "\x1B[32m"   // tudo a seguir escitro em verde

// definicao de constantes para estado do chute nas structs Alfabeto
#define CHUTE_RESET 0
#define CHUTE_CERTO 1
#define CHUTE_ERRADO 2

// definicao de constante de numero de erros maximos no jogo
#define N_VIDAS 5

// definicao da estrutura base de linked list
typedef struct alfab {
    char letra;                 // letra salva no node
    int chutado;                // info se letra foi chutada ou nao, ver constantes acima
    struct alfab *prox;         // pointeiro para o proximo node
} Alfabeto;

// declaracao de prototipos de funcoes
Alfabeto *criar_letra(char letra); Alfabeto *criar_alfabeto(); void append_letra(Alfabeto *head, Alfabeto *nova_letra);
void printar_alfabeto(Alfabeto *head); Alfabeto *encontrar_letra(Alfabeto *head, char letra); Alfabeto *criar_alfabeto();
Alfabeto *criar_segredo(char s_segredo[]); char maiusculador(char letra); void auth_segredo(Alfabeto *head, char chute);
int acertou_segredo(Alfabeto *head); void printar_segredo(Alfabeto *head);
int auth_chute(Alfabeto *alfabeto, Alfabeto *segredo, char chute); void free_nodes(Alfabeto *head);
char *escolher_segredo(char *segredo_escolhido);

int main(void){
    
    // declaracao de variaveis
    int vidas, continuar_jogando = 1;
    char *s_segredo; 
    char chute;
    
    do {
        // escolher o segredo
        s_segredo = escolher_segredo(s_segredo);
        
        // declaracao das estruturas alfabeto e segredo
        Alfabeto *alfabeto = criar_alfabeto();
        Alfabeto *segredo = criar_segredo(s_segredo);
        
        // reseta o numero de vidas
        vidas = N_VIDAS;    
    
        do {
            system("clear");
    
            // printa segredo em forma de _ _ _ _ e alfabeto com os chutes disponivieis
            printar_segredo(segredo);
            printar_alfabeto(alfabeto);
            printf("\n");
            printf("Numero de chutes: %d\n", vidas);
    
            // aceita chute
            printf("Chute uma letra: ");
            scanf(" %c", &chute);
    
            // mesmo que input seja minusculo, funcao transforma em maiuscula
            chute = maiusculador(chute);
    
            // funcao verifica o chute, altera o alfabato e o segredo e retorna a alteracao da vida
            // chute errado --> vida diminui
            // chute certo --> vida nao se altera
            vidas += auth_chute(alfabeto, segredo, chute);
        } while (!acertou_segredo(segredo) && vidas > 0);
        // enquanto o jogador nao acertou o segredo e há vidas
        // --> o jogo continua
        // daqui para frente ou o segredo foi descoberto ou as tentativas acabaram
        
        system("clear");
        
        printf("FIM DE JOGO! -----> ");
    
        // mostra segredo no estado que saiu do loop anterior
        printar_segredo(segredo);
    
        // parabeniza ou nao o jogador
        if (acertou_segredo(segredo)) {
            printf("Parabéns, você descobriu o segredo!\n");
        } else {
            printf("Poxa, não foi dessa vez.\n");
        }

        // evitando memory leak
        free_nodes(alfabeto);
        free_nodes(segredo);

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

// funcao cria node com a letra para se inserir no alfabeto ou no segredo
// recebe p char letra e retorna o node Alfabeto *
Alfabeto *criar_letra(char letra){
    // alocando memoria para o node
    Alfabeto *nova_letra = (Alfabeto *) malloc(sizeof(Alfabeto));
    // malloc usado --> memoria alocada manualmente --> perigo de memory leak

    // limpa area da memoria para evitar garbage
    memset(nova_letra, 0, sizeof(*nova_letra));

    // insere letra no node
    nova_letra->letra = letra;
    // no inicio, todas as letras estao no estado inicial de nao terem sido chutadas
    nova_letra->chutado = CHUTE_RESET;
    // este node novo é o último
    nova_letra->prox = NULL;

    // retorna node novo
    return nova_letra;
}

// funcao que adiciona node ao final da linked lista, seja o alfabeto ou o segredo
void append_letra(Alfabeto *head, Alfabeto *nova_letra) {
    Alfabeto *iter = head;

    // enquanto o iter ainda é um item da linked lista
    while (iter != NULL) {
        // se for o último node da lista, append o novo node
        if (iter->prox == NULL) {
            iter->prox = nova_letra;
            nova_letra->prox = NULL;
            break;
        }
        // se nao for o ultimo node, passa para o proximo
        iter = iter->prox;
    }
}


// funcao para printar o alfabeto
void printar_alfabeto(Alfabeto *head) {
    // node iter para nao perder o head
    Alfabeto *iter = head;

    // enquanto o iter nao é o último node da linked list
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

            default: // significa que nao foi chutado ainda --> branco
                printf("%c ", iter->letra);
        }

        // passa o iter para o proximo node
        iter = iter->prox;
    }

    puts("");
}

// funcao para procurar node dentro da linked lista
Alfabeto *encontrar_letra(Alfabeto *head, char letra) {
    // criar node iter para nao perder o head
    Alfabeto *iter = head;

    // varre a linked lista
    while (iter != NULL) {
        // se a letra for a procura, retorna este node
        if (iter->letra == letra) {
            return iter;
        }
        iter = iter->prox;
    }
    return NULL;
}

Alfabeto *criar_alfabeto() {
    Alfabeto *iter, *head;

    iter = criar_letra(65);
    head = iter;

    for (int i = 1; i < 26; i++) {
        append_letra(iter, criar_letra(65 + i));
    }

    return head;
}

Alfabeto *criar_segredo(char s_segredo[]) {
    Alfabeto *iter, *segredo;

    segredo = criar_letra(s_segredo[0]);
    iter = segredo;

    for (int i = 1; i <= strlen(s_segredo); i++) {
        append_letra(iter, criar_letra(s_segredo[i]));
    }

    return segredo;
}

char maiusculador(char letra) {
    if (letra >= 97 && letra <= 122) {
        return (letra - 97) + 65;
    } else {
        return letra;
    }
}

void auth_segredo(Alfabeto *head, char chute) {
    Alfabeto *iter = head;

    while (iter->prox != NULL) {
        if (iter->letra == chute && iter->chutado != CHUTE_CERTO) {
            iter->chutado = CHUTE_CERTO;
        }
        iter = iter->prox;
    }
}

int acertou_segredo(Alfabeto *head) {
    Alfabeto *iter = head;

    while (iter->prox != NULL) {
        if (iter->chutado == CHUTE_RESET) { // ainda ha chutes a serem feitos
            return 0;
        }
        iter = iter->prox;
    }

    // o segredo ja foi descoberto
    return 1;
}

void printar_segredo(Alfabeto *head) {
    Alfabeto *iter = head;

    while (iter->prox != NULL) {
        if (iter->chutado == CHUTE_RESET) {
            printf("_ ");
        } else {
            printf("%c ", iter->letra);
        }
        iter = iter->prox;
    }

    puts("");
}

int auth_chute(Alfabeto *alfabeto, Alfabeto *segredo, char chute) {
    if (encontrar_letra(segredo, chute) != NULL) {
        encontrar_letra(alfabeto, chute)->chutado = CHUTE_CERTO;
        auth_segredo(segredo, chute);
        return 0;
    } else {
        encontrar_letra(alfabeto, chute)->chutado = CHUTE_ERRADO;
        return -1;
    }
}

// lidando com a memoria alocada manualmente
void free_nodes(Alfabeto *head) {
    Alfabeto *iter = head;
    Alfabeto *prev;

    while (iter->prox != NULL) {
        prev = iter;
        iter = iter->prox;

        free(prev);
    }

    free(iter);
}

char *escolher_segredo(char *segredo_escolhido) {

    // declaracao da string segredo_escolhido que será retornada
    segredo_escolhido = calloc(10, sizeof(char));

    // declaracao de variaveis
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

	//determinar numero de segredos
	char c;
	while (!feof(arq)) {
    	fscanf(arq, "%c", &c);
    	if (c == '\n') {
      		n++;
    	}
  	}

	//	reseta o ponteiro do arquivo para a posicao inicial
	rewind(arq);

	// alocar dinamicamente a quantidade de segredos
	segredos = (char **) calloc(n, sizeof(char *));
	for (int i = 0; i < n; i++) {
		segredos[i] = (char *) calloc(10, sizeof(char *));
	}

	// ler no vetor segredos os segredos do arq
	fscanf(arq, "%s", segredos[0]);
	for (int i = 1; i < n; i++) {
		fscanf(arq, "\n%s", segredos[i]);
	}

	// gerar posicao aleatoria na array segredos e copiar em segredo_escolhido
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