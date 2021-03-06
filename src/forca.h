#define SMAX 20

//prototipo de funcoes
void prepara_alfabeto(char *linhas, int *posicaoAcerto, char *alfabeto);
char **ler_segredos(char **segredos, int qualArquivo); int voltar_n(int qualArquivo);
void desenhar_cabecalho(); void trocar_cor(int n); void desenhar_forca(int situacao);
void modo_de_jogo(int *escolha); void pintar_alfabeto(char *alfabeto, int *posicaoAcerto);
void tentativas_restantes(int tentativas, char *linhas, int numeroDeLetras);
void maiusculador(int *letra); int auth_chute(int *posicaoAcerto, char *palavraSecreta, char *linhas);
int verificar_linhas(char *linhas);
void flush_stdin();
