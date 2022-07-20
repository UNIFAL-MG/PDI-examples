# Processamento de Imagens

> Nesse repositório estão os códigos fontes dos exemplos desenvolvidos no curso de Processamento de Imagens da UNIFAL-MG.

Os diretórios contém os seguintes códigos
- **Fourier**: estão duas implementações do algoritmo para transformada de
Fourie, um implementação de acordo com as equações e um implementação rápida, baseada
na reoodenação do termos da transformada. Existe um código que exemplifica os cálculos para
um sinal monodimensional (uma função discreta).
- **Histograma**: contém uma implementação do algoritmo de equalização de histograma
- **Intensidade**: contém alguns exemplos de transformação de intensidade em imagens em tons
de cinza.
- **Morfologia Binária**: estão as implementações da dilatação e erosão para imagens binárias.
Contém um simulador das operações para imagens pequenas.
- **Morfologia Numérica**: contem a implementação das operações de dilatação e erosão para
imagens em tons de cinza.
- **Negativo**: código básico que contém a transformação negativo para uma imagem em tons
de cinza.
- **Rotulação**: está a implementação de um algoritmo de rotulação de regiões gráficas.
- **utils**: contém as bibliotecas para leitura e escrita de imagens em formato pnm (pbm,
pgm o ppm). Contém também o visualizador de imagens para windows (irfanview)

Para compilar, basta digitar na pasta do código exemplo:

```
make
```

Para limpar o projeto:
```
make clean
```
