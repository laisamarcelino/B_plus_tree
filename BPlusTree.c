#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BPlusTree.h"

//função para criar um novo registro
registro_t *criarRegistro(int chave, char modelo[20], int ano, char cor[20]){
    registro_t *registro = (registro_t *)malloc(sizeof(registro_t));
    if (!registro) {
        perror("Erro ao alocar memória para o registro");
        exit(EXIT_FAILURE);
    }
    registro->chave = chave;
    strcpy(registro->modelo, modelo);
    registro->ano = ano;
    strcpy(registro->cor, cor);
    return registro;
}

//função para destruir um registro
void destroirRegistro(registro_t *registro){
    if (registro)
        free(registro);
}
//função para criar um novo nó (folha ou interno)
nodo_t *criarNodo(){
    nodo_t *novoNodo = (nodo_t *)malloc(sizeof(nodo_t));
    if (!novoNodo) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }

    //Inicializa o novo nó
    novoNodo->numChaves = 0;
    novoNodo->folha = 1;
    novoNodo->proximo = NULL;

    //incializa as chaves, filhos e registros
    for (int i = 0; i < ORDEM; i++) {
        novoNodo->filhos[i] = NULL;
        novoNodo->registros[i] = NULL; // apenas folhas têm registros
    }
    return novoNodo;
}
//função para destruir um nó
void destruirNodo(nodo_t *nodo){
    if (nodo) {
        //Libera os registros associados às chaves
        for (int i = 0; i < nodo->numChaves; i++) {
            destroirRegistro(nodo->registros[i]);
        }
        free(nodo);
    }
}
//função para criar uma nova árvore B+
BPlusTree_t *criarArvoreBPlus() {
    BPlusTree_t *novaArvore = (BPlusTree_t *)malloc(sizeof(BPlusTree_t));
    if (!novaArvore) {
        perror("Erro ao alocar memória para a árvore B+");
        exit(EXIT_FAILURE);
    }
    novaArvore->raiz = criarNodo(); // cria a raiz como um nó folha
    novaArvore->numNodos = 1; // inicializa o número de nós
    return novaArvore;
}
//função para destruir a árvore B+
void destruirArvoreBPlus(nodo_t *raiz){
    if (raiz) {
        //Se o nó não for folha, destrói os filhos recursivamente
        if (!raiz->folha) {
            for (int i = 0; i <= raiz->numChaves; i++) {
                destruirArvoreBPlus(raiz->filhos[i]);
            }
        }
        //Destrói o próprio nó
        destruirNodo(raiz);
    }
}
//função para inserir um registro na árvore B+
void inserir(BPlusTree_t *arvore, registro_t registro);
//função para buscar um registro na árvore B+
registro_t *buscar(BPlusTree_t *arvore, int chave) {
    if (!arvore || !arvore->raiz) {
        return NULL; // Árvore vazia
    }

    nodo_t *atual = arvore->raiz;

    while (atual != NULL) {
        int i = 0;

        // Encontra a posição correta no nó atual
        while (i < atual->numChaves && atual->chaves[i] < chave) {
            i++;
        }

        // Verifica se a chave foi encontrada no nó atual
        if (i < atual->numChaves && atual->chaves[i] == chave) {
            return atual->registros[i]; // Retorna o registro correspondente
        }

        // Se não for folha, desce para o próximo nó
        if (!atual->folha) {
            atual = atual->filhos[i];
        } else {
            break; // Se for folha e não encontrou, a chave não existe
        }
    }

    return NULL; // Chave não encontrada
}

//imprime o as chaves de um nodo
void imprimeArvore(nodo_t *nodo) {
    if (nodo == NULL) {
        printf("Nó vazio.\n");
        return;
    }
    
    printf("Chaves no nó: ");
    for (int i = 0; i < nodo->numChaves; i++) {
        printf("%d ", nodo->chaves[i]);
    }
    printf("\n");
    
    // Se o nó for folha, imprime os registros associados
    if (nodo->folha) {
        printf("Registros no nó:\n");
        for (int i = 0; i < nodo->numChaves; i++) {
            printf("Chave: %d, Modelo: %s, Ano: %d, Cor: %s\n",
                   nodo->registros[i]->chave,
                   nodo->registros[i]->modelo,
                   nodo->registros[i]->ano,
                   nodo->registros[i]->cor);
        }
    } else {
        // Se não for folha, imprime os filhos
        printf("Filhos do nó:\n");
        for (int i = 0; i <= nodo->numChaves; i++) {
            imprimeArvore(nodo->filhos[i]);
        }
    }
}
