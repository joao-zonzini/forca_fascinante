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
  char linhas[40] = "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _";
  int palavra, tentativas = 0, ganhou = 0;

  prepara_alfabeto(posicaoAcerto, alfabeto);

  modo_de_jogo(&escolha);

  //quero saber o numero de segredos, jogo a escolha pra decidir qual arquivo abrir
  n = voltar_n(escolha);
  segredos = ler_segredos(segredos, escolha); //segredos ta alocado dinamicamente para o numero de palavras

  //escolhe um segredo aleatoriamente
  srand(time(NULL));
  palavra = rand() % n;

  //define o fim da string que faz os _ _ _ _ _
  linhas[2 * strlen(segredos[palavra])] = '\0';

  tentativas = (escolha == 2) ? 7 : 5;

  do {
    desenhar_cabecalho();
    puts("");
    desenhar_forca(tentativas);
    puts("");

    pintar_alfabeto(alfabeto, posicaoAcerto);

    tentativas_restantes(tentativas, linhas, strlen(segredos[palavra]));

    auth_chute(posicaoAcerto, segredos[palavra], linhas);

    tentativas = (escolha == 2) ? 7 : 5;

    //para cada flag marcando erro, tiro 1 de tentativas
    for (int i = 0; i < 26; i++) {
      if (posicaoAcerto[i] == 2) {
        tentativas--;
      }
    }

    ganhou = verificar_linhas(linhas);

  } while(!ganhou && tentativas > 0);

  desenhar_cabecalho();
  puts("");
  if (ganhou) {
    trocar_cor(1);
    printf("%s\n\n", linhas);
    trocar_cor(0);
    printf("   VOCE GANHOU, PARABENS!!\n\a");
    trocar_cor(3);
		printf("        ___________      \n");
		printf("       '._==_==_=_.'     \n");
		printf("       .-\\:      /-.    \n");
		printf("      | (|:.     |) |    \n");
		printf("       '-|:.     |-'     \n");
		printf("         \\::.    /      \n");
		printf("          '::. .'        \n");
		printf("            ) (          \n");
		printf("          _.' '._        \n");
		printf("         '-------'       \n\n");
    trocar_cor(0);
  } else {
    printf("Voce perdeu ;( o segredo era: ");
    printf("\t%s\n\n", segredos[palavra]);
    trocar_cor(2);
    desenhar_forca(0);
    trocar_cor(0);
    puts("");
  }

  //evitando memory leak
  for (int i = 0; i < n; i++) {
    free(segredos[i]);
  }
  free(segredos);
  return 0;
}

void prepara_alfabeto(int *posicaoAcerto, char *alfabeto){
  //enche o array de flags com zeros
  for (int i = 0; i < 26; i++) {
    posicaoAcerto[i] = 0;
  }

  //preenche o alfabeto usando ASCII
  for (int i = 65; i < 91; i++) {
    alfabeto[i - 65] = i;
  }

  posicaoAcerto[26] = '\0';
  alfabeto[26] = '\0';
}

int verificar_linhas(char *linhas) {
    //verifica se falta alguma letra
  for (int i = 0; i < strlen(linhas); i++) {
    if (linhas[i] == '_') {
      return 0;
    }
  }
  return 1;
}

void auth_chute(int *posicaoAcerto, char *palavraSecreta, char *linhas) {
  char chute;

  //pega chute:
  printf("Chute: ");
  chute = getchar();
  maiusculador(&chute);

  //assume que o chute é um erro
  posicaoAcerto[chute - 65] = 2;

  //verifica se o chute é de fato um erro
  for (int i = 0; i < strlen(palavraSecreta); i++) {
    if (chute == palavraSecreta[i]) {
      printf("\a");
      if (i == 0) {
        linhas[0] = palavraSecreta[0];
        posicaoAcerto[chute - 65] = 1;
      } else {
        linhas[i * 2] = palavraSecreta[i];
        posicaoAcerto[chute - 65] = 1;
      }
    }
  }
}

void maiusculador(char *letra) {
  int diff;
  //nao importa o letra esse codigo faz ele ser maiusculo
  if (*letra >= 97 && *letra <= 122) {
    diff = *letra - 97;
    *letra = 65 + diff;
  }
}

void tentativas_restantes(int tentativas, char *linhas, int numeroDeLetras) {
  printf("\n\n");
  puts("\t-----------------------------  ");
  printf("\t|  Tentativas restantes: %d  |\n", tentativas);
  puts("\t-----------------------------  ");
  printf("\n%s\t\t\t-----> %ld letras\n\n", linhas, numeroDeLetras);
}

void pintar_alfabeto(char *alfabeto, int *posicaoAcerto) {
  //varre o alfabeto e sua array de flags para verificar os acertos e erros
  for (int i = 0; i <= strlen(alfabeto); i++) {
    switch (posicaoAcerto[i]) {
      case 0:
      printf("%c ", alfabeto[i]);
      break;
      case 1:       //acerto
      trocar_cor(1);
      printf("%c ", alfabeto[i]);
      trocar_cor(0);
      break;
      case 2:       //erro
      trocar_cor(2);
      printf("%c ", alfabeto[i]);
      trocar_cor(0);
      break;
    }
  }
}

void modo_de_jogo(int *escolha) {
  //escolha do modo de jogo, condicao evita um modo que nao existe
  do {
    puts("Qual o modo de jogo?");
    printf("(1)FRUTAS ou (2)CARROS: ");
    scanf("%d", escolha);
  } while(*escolha != 1 && *escolha != 2);
}

char **ler_segredos(char **segredos, int qualArquivo) {
  char nomeDoArquivo[10];
  int n;
  FILE *arq;

  n = voltar_n(qualArquivo);

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

int voltar_n(int qualArquivo) {
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

void desenhar_cabecalho() {
  system("clear");
  puts("|--------JOGO------------------------|");
  puts("|-----------------DA-----------------|");
  puts("|---------------------------FORCA----|");
}

void desenhar_forca(int situacao) {
  printf("||==========\n");
  printf("||      |       \n");
  printf("||      %c      \n", (situacao < 5 ? 'o' : ' '));
  printf("||    %c%c%c%c  \n", (situacao < 4 ? '-' : ' '), (situacao < 4 ? '-' : ' '), (situacao < 4 ? '|' : ' '), (situacao < 4 ? '-' : ' '), (situacao < 4 ? '-' : ' '));
  printf("||      %c      \n", (situacao < 3 ? '|' : ' '));
  printf("||     %c %c    \n", (situacao < 2 ? '/' : ' '), (situacao < 2 ? '\\' : ' '));
  printf("\n");
}

void trocar_cor(int n) {
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
    case 3:
      printf("\033[1;33m"); //amarelo
      break;
  }
}
