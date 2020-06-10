#define SMAX 20

//prototipo de funcoes
char **LeSegredos(char **segredos, int qualArquivo); int voltaN(int qualArquivo);
void DesenhaCabecalho(); void TrocaCor(int n); void DesenhaForca(int situacao);
void modoDeJogo(int *escolha); void pintarAlfabeto(char *alfabeto, int *posicaoAcerto);
void tentativasRestantes(int tentativas, char *linhas, int numeroDeLetras);
void maiusculador(char *letra);
