#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "forca.h"

int main() {
  //declaracao de variaveis
  int n, escolha, posicaoAcerto[26];
  char alfabeto[26];
  char **segredos;
  char chute;
  char linhas[40] = "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _";
  int palavra, tentativas = 5, ganhou = 0;

  //enche o array de flags com zeros
  for (int i = 0; i < 26; i++) {
    posicaoAcerto[i] = 0;
  }

  //preenche o alfabeto usando ASCII
  for (int i = 65; i < 91; i++) {
    alfabeto[i - 65] = i;
  }
  alfabeto[26] = '\0';

  modoDeJogo(&escolha);

  //quero saber o numero de segredos, jogo a escolha pra decidir qual arquivo abrir
  n = voltaN(escolha);
  segredos = LeSegredos(segredos, escolha); //segredos ta alocado dinamicamente para o numero de palavras

  //escolhe um segredo aleatoriamente
  srand(time(NULL));
  palavra = rand() % n;

  //define o fim da string que faz os _ _ _ _ _
  linhas[2 * strlen(segredos[palavra])] = '\0';

  tentativas = (escolha == 2) ? 7 : 5;

  do {
    DesenhaCabecalho();
    puts("");
    DesenhaForca(tentativas);
    puts("");

    pintarAlfabeto(alfabeto, posicaoAcerto);

    tentativasRestantes(tentativas, linhas, strlen(segredos[palavra]));

    printf("Chute: ");
    scanf(" %c", &chute);

    tentativas = (escolha == 2) ? 7 : 5;

    maiusculador(&chute);

    //assume que o chute é um erro
    posicaoAcerto[chute - 65] = 2;

    //verifica se o chute é de fato um erro
    for (int i = 0; i < strlen(segredos[palavra]); i++) {
      if (chute == segredos[palavra][i]) {
        if (i == 0) {
          linhas[0] = segredos[palavra][0];
          posicaoAcerto[chute - 65] = 1;
        } else {
          linhas[i * 2] = segredos[palavra][i];
          posicaoAcerto[chute - 65] = 1;
        }
      }
    }

    //para cada flag marcando erro, tiro 1 de tentativas
    for (int i = 0; i < 26; i++) {
      if (posicaoAcerto[i] == 2) {
        tentativas--;
      }
    }

    //assumo que todos as letras foram descobertas
    ganhou = 1;

    //verifica se falta alguma letra
    for (int i = 0; i < strlen(linhas); i++) {
      if (linhas[i] == '_') {
        ganhou = 0;
      }
    }

  } while(!ganhou && tentativas > 0);

  if (ganhou) {
    DesenhaCabecalho();
    puts("");
    TrocaCor(1);
    printf("%s\n\n", linhas);
    TrocaCor(0);
    printf("VOCE GANHOU, PARABENS!!\n\a");
    TrocaCor(1);
    DesenhaForca(10);
    TrocaCor(0);
  } else {
    DesenhaCabecalho();
    puts("");
    printf("Voce perdeu ;( o segredo era: ");
    printf("%s\n\n", segredos[palavra]);
    TrocaCor(2);
    DesenhaForca(0);
    TrocaCor(0);
    puts("");
  }

  //evitando memory leak
  for (int i = 0; i < n; i++) {
    free(segredos[i]);
  }
  free(segredos);
  return 0;
}

void maiusculador(char *letra) {
  int diff;
  //nao importa o letra esse codigo faz ele ser maiusculo
  if (*letra >= 97 && *letra <= 122) {
    diff = *letra - 97;
    *letra = 65 + diff;
  }
}

void tentativasRestantes(int tentativas, char *linhas, int numeroDeLetras) {
  printf("\n\n");
  puts("\t-----------------------------  ");
  printf("\t|  Tentativas restantes: %d  |\n", tentativas);
  puts("\t-----------------------------  ");
  printf("\n%s\t\t\t-----> %ld letras\n\n", linhas, numeroDeLetras);
}

void pintarAlfabeto(char *alfabeto, int *posicaoAcerto) {
  //varre o alfabeto e sua array de flags para verificar os acertos e erros
  for (int i = 0; i <= strlen(alfabeto); i++) {
    switch (posicaoAcerto[i]) {
      case 0:
      printf("%c ", alfabeto[i]);
      break;
      case 1:       //acerto
      TrocaCor(1);
      printf("%c ", alfabeto[i]);
      TrocaCor(0);
      break;
      case 2:       //erro
      TrocaCor(2);
      printf("%c ", alfabeto[i]);
      TrocaCor(0);
      break;
    }
  }
}

void modoDeJogo(int *escolha) {
  //escolha do modo de jogo, condicao evita um modo que nao existe
  do {
    puts("Qual o modo de jogo?");
    printf("(1)FRUTAS ou (2)CARROS: ");
    scanf("%d", escolha);
  } while(*escolha != 1 && *escolha != 2);
}

char **LeSegredos(char **segredos, int qualArquivo) {
  char nomeDoArquivo[10];
  int n;
  FILE *arq;

  n = voltaN(qualArquivo);

  if (qualArquivo == 1) {
    strncpy(nomeDoArquivo, "frutas.txt", 10);
  } else {
    strncpy(nomeDoArquivo, "carros.txt", 10);
  }

  arq = fopen(nomeDoArquivo, "r");

  if (arq == NULL) {
    puts("ERRO 101!");
    exit(0);
  }

  //aloca dinamicamente a quantidade de segredos
  segredos = (char **) calloc(n, sizeof(char *));
  for (int i = 0; i < n; i++) {
    segredos[i] = (char *) calloc(SMAX, sizeof(char *));
  }

  //le no vetor os segredos
  fscanf(arq, "%s", segredos[0]);
  for (int i = 1; i < n; i++) {
    fscanf(arq, "\n%s", segredos[i]);
  }

  fclose(arq);

  return segredos;
}

int voltaN(int qualArquivo) {
  char nomeDoArquivo[10];
  char c;
  int n = -1;
  FILE *arq;

  if (qualArquivo == 1) {
    strncpy(nomeDoArquivo, "frutas.txt", 10);
  } else {
    strncpy(nomeDoArquivo, "carros.txt", 10);
  }

  arq = fopen(nomeDoArquivo, "r");

  if (arq == NULL) {
    puts("ERRO 101!");
    exit(0);
  }

  while (!feof(arq)) {
    fscanf(arq, "%c", &c);
    if (c == '\n') {
      n++;
    }
  }

  fclose(arq);

  return n;
}

void DesenhaCabecalho() {
  system("clear");
  puts("|--------JOGO------------------------|");
  puts("|-----------------DA-----------------|");
  puts("|---------------------------FORCA----|");
}

void DesenhaForca(int situacao) {
  printf("||==========\n");
  printf("||      |       \n");
  printf("||      %c      \n", (situacao < 5 ? 'o' : ' '));
  printf("||    %c%c%c%c  \n", (situacao < 4 ? '-' : ' '), (situacao < 4 ? '-' : ' '), (situacao < 4 ? '|' : ' '), (situacao < 4 ? '-' : ' '), (situacao < 4 ? '-' : ' '));
  printf("||      %c      \n", (situacao < 3 ? '|' : ' '));
  printf("||     %c %c    \n", (situacao < 2 ? '/' : ' '), (situacao < 2 ? '\\' : ' '));
  printf("\n");
}

void TrocaCor(int n) {
  switch (n) {
    case 0:
      printf("\033[0m");    //reseta
      break;
    case 1:
      printf("\033[0;32m"); //verde
      break;
    case 2:
      printf("\033[1;31m"); //vermelho
      break;
  }
}
