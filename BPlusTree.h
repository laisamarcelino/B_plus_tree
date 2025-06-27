#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#define ORDEM 3

//estrutura para armazenar os dados de um automóvel
typedef struct {
    int chave; //chave única (renavam)
    char modelo[20];  //modelo do veículo
    int ano; //ano de fabricação
    char cor[20]; //cor do veículo
} registro_t;

//estrutura de um nó da árvore B+
typedef struct nodo_t {
    int chaves[ORDEM - 1]; //chaves armazenadas no nó
    struct nodo_t *filhos[ORDEM]; //ponteiros para os filhos
    registro_t *registros[ORDEM - 1]; //registros associados às chaves
    struct nodo_t *proximo; //ponteiro para o próximo nó
    char numChaves; //número de chaves atuais no nó
    char folha; //indica se o nó é folha (1) ou não (0) 
} nodo_t;

//estrutura da árvore B+
typedef struct {
    nodo_t *raiz; //ponteiro para a raiz da árvore
    int numNodos; //número total de nós na árvore
} BPlusTree_t;

registro_t *criarRegistro(int chave, const char modelo[20], int ano, const char cor[20]); //protótipo de função para criar um novo registro
nodo_t *criarNodo(int folha); //protótipo de função para criar um novo nó (folha ou interno)
BPlusTree_t *criarArvoreBPlus(); //protótipo de função para criar uma nova árvore B+
void inserir(BPlusTree_t *arvore, registro_t registro); //protótipo de função para inserir um registro na árvore B+
registro_t *buscar(BPlusTree_t *arvore, int chave); //protótipo de função para buscar um registro na árvore B+
void imprimeArvore(BPlusTree_t *arvore); //protótipo de função para imprimir a árvore B+ (para depuração).

#endif //BPlusTree.h