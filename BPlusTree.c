#include <stdlib.h>
#include <stdio.h>
#include "BPlusTree.h"

//cria um novo registro
registro_t *criarRegistro(int chave, const char modelo[20], int ano, const char cor[20]);
//cria um novo nó (folha ou interno)
nodo_t *criarNodo(int folha);
//cria uma nova árvore B+ 
BPlusTree_t *criarArvoreBPlus();
//insere um registro na árvore B+
void inserir(BPlusTree_t *arvore, registro_t registro);
//busca um registro na árvore B+ 
registro_t *buscar(BPlusTree_t *arvore, int chave);
//imprime a árvore B+ em ordem (para depuração) 
void imprimeArvore(BPlusTree_t *arvore); 