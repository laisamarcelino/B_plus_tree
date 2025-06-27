import random

modelos = ["Gol", "Onix", "Corolla", "Civic", "HB20", "Fiesta", "Ka", "Sandero", "Compass", "Polo"]
cores = ["Preto", "Branco", "Prata", "Vermelho", "Azul", "Cinza", "Verde"]
anos = list(range(1995, 2025))

def gerar_renavams_unicos(qtd):
    renavams = set()
    while len(renavams) < qtd:
        renavam = random.randint(10000000000, 99999999999)
        renavams.add(renavam)
    return list(renavams)

def gerar_registros(qtd):
    renavams = gerar_renavams_unicos(qtd)
    registros = []

    for i in range(qtd):
        modelo = random.choice(modelos)
        cor = random.choice(cores)
        ano = random.choice(anos)
        registro = f"{renavams[i]},{modelo},{ano},{cor}"
        registros.append(registro)

    return registros

def salvar_em_arquivo(registros, nome_arquivo="registros_carros.txt"):
    with open(nome_arquivo, "w", encoding="utf-8") as f:
        for r in registros:
            f.write(r + "\n")

if __name__ == "__main__":
    total = 10000
    registros = gerar_registros(total)
    salvar_em_arquivo(registros)
    print(f"{total} registros gerados e salvos em 'registros_carros.txt'")