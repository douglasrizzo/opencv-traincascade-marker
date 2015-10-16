# opencv-traincascade-marker
Application to create annotations for OpenCV object detection methods

## WARNING
The original annotation application was copied from [here](http://www.technolabsz.com/2012/07/how-to-do-opencv-haar-training-in.html). However, due to limitations in its functionality and extensive use in our laboratory, a few changes were made to the code in order to make the program more robust. All credit goes to its original author.

### Explicação dos arquivos
* Marcar os objetos nas imagens através da ferramenta *obj_marker* gera arquivo `.dat`;
* Passagem do arquivo `.dat` ao `opencv_createsamples` gera arquivo `.vec`;
* Passagem do arquivo `.vec` ao `opencv_traincascade` gera arquivo `.xml`;
* Necessário criar arquivo listando **exemplos negativos** de imagens, denominado abaixo como `negatives.txt`.

### Procedimento
O procedimento abaixo supõe uma estrutura de diretórios e arquivos parecida com a seguinte:

    dir/
        annotations.dat
        annotations.vec
        negatives.txt
        /classifier
            cascade.xml
            stage-0.xml
            ...
            stage-n.xml

Para criar o arquivo texto listando imagens negativas, utilize um comando que lista o caminho absoluto de todas as imagens em um diretório, recursivamente, e os salva em um arquivos `negatives.txt`, como o seguinte:

    find `pwd` -not -path '*/\.*' -regextype posix-extended -regex '.*\.(png|bmp|jpg|jpeg)' -type f > negatives.txt

**Importante: não usar links relativos em nenhum arquivo, apenas absolutos.**

Depois, gerar arquivo `.vec` através do arquivo `.dat`:

    opencv_createsamples -info annotations.dat -vec annotations.vec

* `.dat` contém as imagens positivas e as coordenadas dos retângulos
* `.vec` é o formato utilizado posteriormente no treinamento do classificador

*Opcional:* visualizar conteúdo do `.vec`

    opencv_createsamples -vec annotations.vec

Por último, iniciar treinamento:

    opencv_traincascade -data hog -vec annotations.vec -bg negatives.txt -numStages 12 -minHitRate 0.999 -maxFalseAlarmRate 0.5 -numPos $(($(wc -l < annotations.dat)/100*90)) -numNeg $(($(wc -l < negatives.txt)*10)) -mode ALL

**Com relação aos parâmetros:**

* `-data`: diretório onde seram salvados os XML de treinamento;
* `-numPos`: número de imagens positivas utilizadas para treinamento. Não é o mesmo número de imagens positivas utilizadas na criação do arquivo `.vec`, deve ser menor para que algumas sejam usadas no teste;
* `-numNeg`: número de imagens negativas. Pode ser o total de imagens no arquivo de negativas ou mais (o programa corta cada imagem em menores resoluções).

Exemplo de como passar 95% das imagens positivas no treinamento: `$(($(wc -l < annotations.dat)/100*95))`

Exemplo de como passar 100% das negativas no treinamento: `$(wc -l < negatives.txt)`

Usar parâmetro `[-featureType <{HAAR(default), LBP, HOG}>]` para selecionar qual classificador será treinado.

### Características dos algoritmos
Tempo de treinamento (VILÃO, 2015):

* HAAR: 10 a 15 horas
* HOG: < 1 hora
* LBP: não testado

Algoritmos em ordem de precisão (VILÃO, 2015):

* HOG
* HAAR
* LBP (não testado)