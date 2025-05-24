# Trabalho prático 2 de Projeto e Analise de Algoritmos

## Problema

Zorc precisa recrutar soldados de diferentes povos do Mundo de Zambis para enfrentar uma grande batalha contra o mundo de Xulambs. Cada povo oferece soldados com peso e habilidade fixos, em quantidade ilimitada. Zorc pode se mover entre povos conectados, respeitando um limite total de distância percorrida e o peso máximo que sua nave suporta. A cada visita, ele pode recrutar soldados do povo atual e seguir para um vizinho. O objetivo é encontrar o caminho que maximize a habilidade total dos soldados recrutados.

A solução foi implementada de duas formas: uma usando programação dinâmica e outra com uma estratégia gulosa como heurística.

## 💻 Pré-requisitos

Antes de começar, verifique se você atendeu aos seguintes requisitos:
Você instalou um compilador C (como gcc) e a versão mais recente do make.

Você leu este README até o final para entender como compilar e executar o programa.

## 🚀 Compilando o código

Para compilar o código, siga estas etapas:

- Estando na pasta do projeto, rode o seguinte comando:

```bash
make
```

- Limpar binários (Caso seja necessário)

```bash
make clean
```

---

## ☕ Rodando o programa

Para rodar o programa, utilize o seguinte formato:

```bash
./tp1 {estratégia} -i entrada.txt -o saida.txt
```

- `{estratégia}` deve ser `1` para usar a abordagem de programação dinâmica ou `2` para usar a abordagem gulosa.
- Os parâmetros de estratégia (`1` ou `2`) e o arquivo de entrada (`-i`) **são obrigatórios**.
- O parâmetro de saída (`-o`) é **opcional**. Se for omitido, o arquivo de saída será criado automaticamente no diretório `output/` com o nome `output.txt`.
- Sempre que um nome de arquivo de saída for fornecido, ele será utilizado.

Exemplos:

```bash
./tp1 1 -i entrada.txt
# Gera o arquivo output/output.txt

./tp1 2 -i entrada.txt -o resultado.txt
# Gera o arquivo resultado.txt
```

## 📚 Documentação

🔗 [Acesse este repositório no GitHub](https://github.com/ArthurDetomi/PAA_TP2_Zambis_VS_Xulambs)

- A documentação segue-se no diretorio /docs

[Instruções](/docs/tp2.pdf)

🚧 Em construção. Será adicionada até a data de entrega.

[Documentação](#)

## 🤝 Dupla

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/arthurdetomi" title="Perfil do GitHub de Geraldo Arthur">
        <img src="https://avatars.githubusercontent.com/u/99772832?v=4" width="100px;" alt="Foto do perfil do Geraldo Arthur Detomi no GitHub"/><br>
        <sub>
          <b>Geraldo Arthur Detomi</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/HomoGodzilla" title="Perfil do GitHub de Oscar Alves">
        <img src="https://avatars.githubusercontent.com/u/89045220?v=4" width="100px;" alt="Foto do perfil do Oscar Alves no GitHub"/><br>
        <sub>
          <b>Oscar Alves</b>
        </sub>
      </a>
    </td>
  </tr>
</table>
