#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cabecalho();

int main() {

  char segredos[3][20] = {{"MELANCIA"}, {"BANANA"}, {"PERA"}};
  char chute;
  char linhas[40] = "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _";
  int palavra, perdeu = 5, ganhou = 0;

  srand(time(NULL));
  palavra = rand() % 3;

  linhas[2 * strlen(segredos[palavra])] = '\0';

  do {
    cabecalho();
    puts("");
    printf("Tentativas restantes: %d", perdeu);
    printf("\n%s\n\n", linhas);
    printf("Chute: ");
    scanf(" %c", &chute);

    for (int i = 0; i < strlen(segredos[palavra]); i++) {
      if (chute == segredos[palavra][i]) {
        if (i == 0) {
          linhas[0] = segredos[palavra][0];
          perdeu++;
        } else {
          linhas[i * 2] = segredos[palavra][i];
          perdeu++;
        }
      }
    }

    perdeu--;

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
    printf("\n%s\n\n", linhas);
    printf("VOCE GANHOU, PARABENS!!\n");
  } else {
    cabecalho();
    puts("");
    printf("Voce perdeu ;( o segredo era: ");
    printf("%s\n\n", segredos[palavra]);
  }

  return 0;
}

int cabecalho() {
  system("clear");
  puts("|--------JOGO------------------------|");
  puts("|-----------------DA-----------------|         versao: frutas");
  puts("|---------------------------FORCA----|");
}
