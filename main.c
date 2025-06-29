#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BPlusTree.h" 

#define MAX_LINHA 256

void carregarRegistro(const char *nomeArquivo, BPlusTree_t *arvore) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    // para definir o tamanho do buffer de leitura
    char linha[MAX_LINHA];

    // fgets precisa de um buffer para armazenar temporariamente a linha que tá sendo lida do arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {
        unsigned long long chave;
        int ano;
        char modelo[TAM_MODELO], cor[TAM_COR];

        linha[strcspn(linha, "\n")] = 0;
        if (sscanf(linha, "%llu,%19[^,],%d,%19[^\n]", &chave, modelo, &ano, cor) != 4) {
            fprintf(stderr, "Erro ao ler linha: %s\n", linha);
            continue; // Pula linha mal formatada
        }
        // Criar registro dinamicamente
        registro_t *registro = criarRegistro(chave, modelo, ano, cor);

        // Inserir na árvore
        inserir(arvore, registro);
    }

    fclose(arquivo);
}

int main() {
    BPlusTree_t *arvore = criarArvoreBPlus();

    const char *nomeArquivo = "registros_carros.txt";
    carregarRegistro(nomeArquivo, arvore);
    
    printf("\n==== Árvore B+ após inserções ====\n");
    imprimeArvore(arvore->raiz);

    destruirArvoreBPlus(arvore->raiz);
    free(arvore);

    // fazer buscas, impressões e testes
    // obs Elen: para os testes vou gerar um script parecido com os dos prof de prog2

    return 0;
}
