#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SMAX 20

char **TiraSegredo(char **segredos); int voltaN();
void cabecalho(); void cores(int n);

int main() {
  int n, posicaoAcerto[26];
  char alfabeto[26];
  char **segredos;
  char chute;
  char linhas[40] = "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _";
  int palavra, perdeu = 5, ganhou = 0;

  for (int i = 0; i < 26; i++) {
    posicaoAcerto[i] = 0;
  }
  for (int i = 65; i < 91; i++) {
    alfabeto[i - 65] = i;
  }
  alfabeto[26] = '\0';

  n = voltaN();
  segredos = TiraSegredo(segredos);

  srand(time(NULL));
  palavra = rand() % n;

  linhas[2 * strlen(segredos[palavra])] = '\0';

  do {
    cabecalho();
    puts("");

    for (int i = 0; i <= strlen(alfabeto); i++) {
      switch (posicaoAcerto[i]) {
        case 0:
          printf("%c ", alfabeto[i]);
          break;
        case 1:       //acerto
          cores(1);
          printf("%c ", alfabeto[i]);
          cores(0);
          break;
        case 2:       //erro
          cores(2);
          printf("%c ", alfabeto[i]);
          cores(0);
          break;
      }
    }

    printf("\n\nTentativas restantes: %d", perdeu);
    printf("\n%s\t\t\t%ld letras\n\n", linhas, strlen(segredos[palavra]));
    printf("Chute: ");
    scanf(" %c", &chute);

    if (chute >= 97 && chute <= 122) {
      int diff;
      diff = chute - 97;
      chute = 65 + diff;
    }

    posicaoAcerto[chute - 65] = 2; //erro

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

    perdeu = 5;
    for (int i = 0; i < 26; i++) {
      if (posicaoAcerto[i] == 2) {
        perdeu--;
      }
    }

    if (perdeu > 5) {
      perdeu = 5;
    }
    ganhou = 1;

    for (int i = 0; i < strlen(linhas); i++) {
      if (linhas[i] == '_') {
        ganhou = 0;
      }
    }

  } while(!ganhou && perdeu > 0);

  if (ganhou == 1) {
    cabecalho();
    puts("");
    cores(1);
    printf("%s\n\n", linhas);
    cores(0);
    printf("VOCE GANHOU, PARABENS!!\n\a");
  } else {
    cabecalho();
    puts("");
    printf("Voce perdeu ;( o segredo era: ");
    printf("%s\n\n", segredos[palavra]);
  }

  return 0;
}

char **TiraSegredo(char **segredos) {
  //n vai ser o numero de segredos no arquivo
  int n;
  FILE *arq;
  arq = fopen("segredos.txt", "r");

  if (arq == NULL) {
    puts("ERRO 101!");
    exit(0);
  }

  fscanf(arq, "%d", &n);
  segredos = (char **) calloc(n, sizeof(char *));
  for (int i = 0; i < n; i++) {
    segredos[i] = (char *) calloc(SMAX, sizeof(char *));
  }

  fscanf(arq, "\n%s", segredos[0]);
  for (int i = 1; i < n; i++) {
    fscanf(arq, "\n%s", segredos[i]);
  }
  fclose(arq);

  return segredos;
}

int voltaN() {
  int n;
  FILE *arq;
  arq = fopen("segredos.txt", "r");
  if (arq == NULL) {
    puts("ERRO 101!");
    exit(0);
  }
  fscanf(arq, "%d", &n);
  fclose(arq);
  return n;
}

void cabecalho() {
  system("clear");
  puts("|--------JOGO------------------------|");
  puts("|-----------------DA-----------------|         versao: frutas");
  puts("|---------------------------FORCA----|");
}

void cores(int n) {
  switch (n) {
    case 0:
      printf("\033[0m");
      break;
    case 1:
      printf("\033[0;32m");
      break;
    case 2:
      printf("\033[1;31m");
      break;
  }
}
