#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SMAX 20

typedef struct jogador {
  char nome[4];
  float pontuacao;
  unsigned long epochInicial;
  unsigned long epochFinal;
} sJogadorInfo;

//prototipo de funcoes
char **LeSegredos(char **segredos, int qualArquivo); int voltaN(int qualArquivo);
void DesenhaCabecalho(); void TrocaCor(int n); void DesenhaForca(int situacao);
void salvaJogador(sJogadorInfo jaux);

int main() {
  //declaracao de variaveis
  int n, escolha, posicaoAcerto[26];
  int diff;
  char alfabeto[26];
  char **segredos;
  char chute;
  char linhas[] = "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _";
  int palavra, tentativas = 5, ganhou = 0;
  sJogadorInfo jaux;

  DesenhaCabecalho();

  //pedindo nome do jogador
  printf("INSIRA SEU NICK: ");
  fgets(jaux.nome, 4, stdin);
  jaux.epochInicial = time(NULL); //lu

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
    system("clear");
  } while(escolha != 1 && escolha != 2);

  //quero saber o numero de segredos, jogo a escolha pra decidir qual arquivo abrir
  n = voltaN(escolha);
  segredos = LeSegredos(segredos, escolha); //segredos ta alocado dinamicamente para o numero de palavras

  //escolhe um segredo aleatoriamente
  srand(time(NULL));
  palavra = rand() % n;

  //define o fim da string que faz os _ _ _ _ _
  linhas[2 * strlen(segredos[palavra])] = '\0';

  if (escolha == 2) {
    tentativas = 7;
  }

  jaux.pontuacao = 0;

  do {
    DesenhaCabecalho();
    puts("");
    DesenhaForca(tentativas);
    puts("");

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

    printf("\n\n");
    puts("\t-----------------------------  ");
    printf("\t|  Tentativas restantes: %d  |\n", tentativas);
    puts("\t-----------------------------  ");
    printf("\n%s\t\t\t-----> %ld letras\n\n", linhas, strlen(segredos[palavra]));
    printf("Pontuacao: %.2f\n", jaux.pontuacao);
    printf("Chute: ");
    scanf(" %c", &chute);


    if (escolha == 2) {
      tentativas = 7;
    } else {
      tentativas = 5;
    }

    jaux.pontuacao = 0;

    //nao importa o chute esse codigo faz ele ser maiusculo
    if (chute >= 97 && chute <= 122) {
      diff = chute - 97;
      chute = 65 + diff;
    }

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

    //para cada flag marcando erro, tiro 1 de tentativas e tira  pontos
    for (int i = 0; i < 26; i++) {
      if (posicaoAcerto[i] == 2) {
        tentativas--;
        jaux.pontuacao -= 15;
      } else if (posicaoAcerto[i]) {
        jaux.pontuacao += 50;
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

  jaux.epochFinal = time(NULL); //lu

  salvaJogador(jaux);

  if (ganhou == 1) {
    DesenhaCabecalho();
    puts("");
    TrocaCor(1);
    printf("%s\n\n", linhas);
    TrocaCor(0);
    printf("VOCE GANHOU, PARABENS!! ---> SUA PONTUACAO: %.2f\n\a", jaux.pontuacao);
    DesenhaForca(10);
  } else {
    DesenhaCabecalho();
    puts("");
    printf("Voce perdeu ;( sua pontuacao foi: %.2f\n\n", jaux.pontuacao);
    DesenhaForca(0);
    puts("");
  }

  //evitando memory leak
  for (int i = 0; i < n; i++) {
    free(segredos[i]);
  }
  free(segredos);
  return 0;
}

void salvaJogador(sJogadorInfo jaux) {
  FILE *arq;

  arq = fopen("placar.txt", "a+");

  if (arq == NULL) {
    printf("NAO FOI POSSIVEL SALVAR JOGADOR\n");
    exit(0);
  }

  fprintf(arq, "Nick: %s | Placar: %.2f | Tempo: %lu segundos\n", jaux.nome, jaux.pontuacao, jaux.epochFinal - jaux.epochInicial);

  fclose(arq);
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

void DesenhaCabecalho() {
  system("clear");
  puts("++++++++++++++++++++++++++++++++++++++");
  puts("+       FORCA                        +");
  puts("+                  FASCINANTE        +");
  puts("++++++++++++++++++++++++++++++++++++++");
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
