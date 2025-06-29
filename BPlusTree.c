#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BPlusTree.h"


//função para criar um novo registro
registro_t *criarRegistro(unsigned long long chave, const char *modelo, int ano, const char *cor){
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
void destruirRegistro(registro_t *registro){
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
            destruirRegistro(nodo->registros[i]);
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

// Função que divide o filho do nó 'pai' no índice 'indice'.
// Usada quando o filho está cheio e precisa ser dividido em dois nós.
void dividirFilho(nodo_t *pai, int indice) {
    nodo_t *filho = pai->filhos[indice];
    nodo_t *novoFilho = criarNodo();
    novoFilho->folha = filho->folha;

    int meio = (ORDEM - 1) / 2;

    if (filho->folha) {
        // Copia a segunda metade dos registros e chaves
        for (int j = meio; j < ORDEM - 1; j++) {
            novoFilho->chaves[j - meio] = filho->chaves[j];
            novoFilho->registros[j - meio] = filho->registros[j];
        }

        novoFilho->numChaves = ORDEM - 1 - meio;
        filho->numChaves = meio;

        // Encadeamento de folhas
        novoFilho->proximo = filho->proximo;
        filho->proximo = novoFilho;

        // Promove a menor chave do novo nó folha
        for (int j = pai->numChaves; j > indice; j--) {
            pai->chaves[j] = pai->chaves[j - 1];
            pai->filhos[j + 1] = pai->filhos[j];
        }

        pai->chaves[indice] = novoFilho->chaves[0];
        pai->filhos[indice + 1] = novoFilho;
        pai->numChaves++;
    } else {
        // Nó interno: copia chaves e ponteiros
        for (int j = meio + 1; j < ORDEM - 1; j++) {
            novoFilho->chaves[j - (meio + 1)] = filho->chaves[j];
        }
        for (int j = meio + 1; j < ORDEM; j++) {
            novoFilho->filhos[j - (meio + 1)] = filho->filhos[j];
        }

        novoFilho->numChaves = filho->numChaves - meio - 1;
        filho->numChaves = meio;

        // Promove chave do meio
        for (int j = pai->numChaves; j > indice; j--) {
            pai->chaves[j] = pai->chaves[j - 1];
            pai->filhos[j + 1] = pai->filhos[j];
        }

        pai->chaves[indice] = filho->chaves[meio];
        pai->filhos[indice + 1] = novoFilho;
        pai->numChaves++;
    }
}

// Função para inserir um registro em um nó que ainda não está cheio
void inserirNaoCheio(nodo_t *nodo, registro_t *registro) {
    int i = nodo->numChaves - 1;

    if (nodo->folha) {
        // Inserção ordenada em folha
        while (i >= 0 && registro->chave < nodo->chaves[i]) {
            nodo->chaves[i + 1] = nodo->chaves[i];
            nodo->registros[i + 1] = nodo->registros[i];
            i--;
        }

        nodo->chaves[i + 1] = registro->chave;
        nodo->registros[i + 1] = registro;
        nodo->numChaves++;
    } else {
        // Busca posição para descer
        while (i >= 0 && registro->chave < nodo->chaves[i]) {
            i--;
        }
        i++;

        // Se o filho estiver cheio, precisa dividir
        if (nodo->filhos[i]->numChaves == ORDEM - 1) {
            dividirFilho(nodo, i);

            // Decide em qual metade continuar
            if (registro->chave > nodo->chaves[i]) {
                i++;
            }
        }

        inserirNaoCheio(nodo->filhos[i], registro);
    }
}

//função para inserir um registro na árvore B+
void inserir(BPlusTree_t *arvore, registro_t *registro) {
    if (!arvore || !registro) return;

    nodo_t *raiz = arvore->raiz;

    // Caso a raiz esteja cheia
    if (raiz->numChaves == ORDEM - 1) {
        nodo_t *novaRaiz = criarNodo();
        novaRaiz->folha = 0;
        novaRaiz->filhos[0] = raiz;
        dividirFilho(novaRaiz, 0);
        arvore->raiz = novaRaiz;
        arvore->numNodos++;
        inserirNaoCheio(novaRaiz, registro);
    } else {
        inserirNaoCheio(raiz, registro);
    }
}

//função para buscar um registro na árvore B+
registro_t *buscar(BPlusTree_t *arvore, unsigned long long chave) {
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
        printf("%llu ", nodo->chaves[i]);
    }
    printf("\n");
    
    // Se o nó for folha, imprime os registros associados
    if (nodo->folha) {
        printf("Registros no nó:\n");
        for (int i = 0; i < nodo->numChaves; i++) {
            printf("Chave: %llu, Modelo: %s, Ano: %d, Cor: %s\n",
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

