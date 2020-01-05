#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SMAX 20

//prototipo de funcoes
char **LeSegredos(char **segredos, int qualArquivo); int voltaN(int qualArquivo);
void DesenhaCabecalho(); void TrocaCor(int n); void DesenhaForca(int situacao);

int main() {
  //declaracao de variaveis
  int n, escolha, posicaoAcerto[26];
  int diff;
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

  //escolha do modo de jogo, condicao evita um modo que nao existe
  do {
    puts("Qual o modo de jogo?");
    printf("(1)FRUTAS ou (2)CARROS: ");
    scanf("%d", &escolha);
  } while(escolha != 1 && escolha != 2);

  //quero saber o numero de segredos, jogo a escolha pra decidir qual arquivo abrir
  n = voltaN(escolha);
  segredos = LeSegredos(segredos, escolha); //segredos ta alocado dinamicamente para o numero de palavras

  //escolhe um segredo aleatoriamente
  srand(time(NULL));
  palavra = rand() % n;

  //define o fim da string que faz os _ _ _ _ _
  linhas[2 * strlen(segredos[palavra])] = '\0';

  do {
    DesenhaCabecalho();
    puts("");
    DesenhaForca(tentativas);
    puts("");

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

    printf("\n\nTentativas restantes: %d", tentativas);
    printf("\n%s\t\t\t-----> %ld letras\n\n", linhas, strlen(segredos[palavra]));
    printf("Chute: ");
    scanf(" %c", &chute);

    if (chute >= 97 && chute <= 122) {
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

    if (escolha == 2) {
      tentativas = 7;
    } else {
      tentativas = 5;
    }

    for (int i = 0; i < 26; i++) {
      if (posicaoAcerto[i] == 2) {
        tentativas--;
      }
    }

    ganhou = 1;

    for (int i = 0; i < strlen(linhas); i++) {
      if (linhas[i] == '_') {
        ganhou = 0;
      }
    }

  } while(!ganhou && tentativas > 0);

  if (ganhou == 1) {
    DesenhaCabecalho();
    puts("");
    TrocaCor(1);
    printf("%s\n\n", linhas);
    TrocaCor(0);
    printf("VOCE GANHOU, PARABENS!!\n\a");
    DesenhaForca(10);
  } else {
    DesenhaCabecalho();
    puts("");
    printf("Voce tentativas ;( o segredo era: ");
    printf("%s\n\n", segredos[palavra]);
    DesenhaForca(0);
    puts("");
  }

  for (int i = 0; i < n; i++) {
    free(segredos[i]);
  }
  free(segredos);
  return 0;
}

char **LeSegredos(char **segredos, int qualArquivo) {
  //n vai ser o numero de segredos no arquivo
  int n;
  FILE *arq1, *arq2;
  arq1 = fopen("frutas.txt", "r");
  arq2 = fopen("carros.txt", "r");

  if (arq1 == NULL || arq2 == NULL) {
    puts("ERRO 101!");
    exit(0);
  }

  switch (qualArquivo) {
    case 1:
      fscanf(arq1, "%d", &n);
      segredos = (char **) calloc(n, sizeof(char *));
      for (int i = 0; i < n; i++) {
        segredos[i] = (char *) calloc(SMAX, sizeof(char *));
      }

      fscanf(arq1, "\n%s", segredos[0]);
      for (int i = 1; i < n; i++) {
        fscanf(arq1, "\n%s", segredos[i]);
      }
      break;
    case 2:
      fscanf(arq2, "%d", &n);
      segredos = (char **) calloc(n, sizeof(char *));
      for (int i = 0; i < n; i++) {
        segredos[i] = (char *) calloc(SMAX, sizeof(char *));
      }

      fscanf(arq2, "\n%s", segredos[0]);
      for (int i = 1; i < n; i++) {
        fscanf(arq2, "\n%s", segredos[i]);
      }
      break;
  }
  fclose(arq1);
  fclose(arq2);

  return segredos;
}

int voltaN(int qualArquivo) {
  int n;
  FILE *arq1, *arq2;
  arq1 = fopen("frutas.txt", "r");
  arq2 = fopen("carros.txt", "r");

  if (arq1 == NULL || arq2 == NULL) {
    puts("ERRO 101!");
    exit(0);
  }

  switch (qualArquivo) {
    case 1:
      fscanf(arq1, "%d", &n);
      break;
    case 2:
      fscanf(arq2, "%d", &n);
      break;
  }

  fclose(arq1);
  fclose(arq2);
  return n;
}

void DesenhaCabecalho() {
  system("clear");
  puts("|--------JOGO------------------------|");
  puts("|-----------------DA-----------------|");
  puts("|---------------------------FORCA----|");
}

void DesenhaForca(int situacao) {
  switch (situacao) {
    case 5:
      puts("||==========");
      puts("||      |   ");
      puts("||          ");
      puts("||          ");
      puts("||          ");
      puts("||          ");
      break;
    case 4:
      puts("||==========");
      puts("||      |   ");
      puts("||      O   ");
      puts("||          ");
      puts("||          ");
      puts("||          ");
      break;
    case 3:
      puts("||==========");
      puts("||      |   ");
      puts("||      O   ");
      puts("||    --|-- ");
      puts("||          ");
      puts("||          ");
      break;
    case 2:
      puts("||==========");
      puts("||      |   ");
      puts("||      O   ");
      puts("||    --|-- ");
      puts("||      |   ");
      puts("||          ");
      break;
    case 1:
      puts("||==========");
      puts("||      |   ");
      puts("||      O   ");
      puts("||    --|-- ");
      puts("||      |   ");
      puts("||     / \\ ");
      break;
    case 0:
      TrocaCor(2);
      puts("||==========");
      puts("||      |   ");
      puts("||      X   ");
      puts("||    --|-- ");
      puts("||      |   ");
      puts("||     / \\ ");
      TrocaCor(0);
      break;
    case 10:
      TrocaCor(1);
      puts("     |  O    ");
      puts("     ---|--- ");
      puts("        |   |");
      puts("        |    ");
      puts("       / \\  ");
      TrocaCor(0);
      break;
    case 11:
      TrocaCor(1);
      puts("        O  | ");
      puts("     ---|--- ");
      puts("     |  |    ");
      puts("        |    ");
      puts("       / \\  ");
      TrocaCor(0);
      break;
    default:
      puts("||==========");
      puts("||      |   ");
      puts("||          ");
      puts("||          ");
      puts("||          ");
      puts("||          ");
      break;
  }
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
