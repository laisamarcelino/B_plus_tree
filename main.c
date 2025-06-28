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
        registro_t r;

        linha[strcspn(linha, "\n")] = 0;
        sscanf(linha, "%d,%19[^,],%d,%19[^\n]", &r.chave, r.modelo, &r.ano, r.cor);

        inserir(arvore, r);
    }

    fclose(arquivo);
}

int main() {
    BPlusTree_t *arvore = criarArvoreBPlus();

    const char *nomeArquivo = "registros_carros.txt";
    carregarRegistro("registros_carros.txt", arvore);

    // fazer buscas, impressões e testes
    // obs Elen: para os testes vou gerar um script parecido com os dos prof de prog2

    return 0;
}