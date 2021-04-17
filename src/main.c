#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef FORCA_H
#define FORCA_H
  #include "forca.h"
#endif

int main() {
  //declaracao de variaveis
  int n, escolha, posicaoAcerto[26];
  unsigned short int continuar_jogando = 1;
  char alfabeto[26];
  char **segredos, linhas[40];
  int palavra, tentativas = 0, ganhou = 0;

  do {
    prepara_alfabeto(linhas, posicaoAcerto, alfabeto);

    modo_de_jogo(&escolha);
    flush_stdin();

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

      if (auth_chute(posicaoAcerto, segredos[palavra], linhas))
        continue;

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

    do {
      printf("Quer continuar jogando? (1)SIM (0)NÃO: ");
      scanf(" %ud", &continuar_jogando);
      flush_stdin();
    } while(!(continuar_jogando >= 0 && continuar_jogando < 2));

  } while(continuar_jogando == 1);

  return 0;
}
