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
Criar arquivo texto listando imagens negativas. Um exemplo de comando que lista o caminho absoluto de todas as imagens em um diretório, recursivamente, e os salva em um arquivos `negatives.txt` é o seguinte:

    find `pwd` -not -path '*/\.*' -regextype posix-extended -regex '.*\.(png|bmp|jpg|jpeg)' -type f > negatives.txt

**Importante: não usar links relativos, apenas absolutos.**

Gerar arquivo `.vec` através do arquivo `.dat`

    opencv_createsamples -info laranja.dat -vec laranja.vec

* `.dat` contém as imagens positivas e as coordenadas dos retângulos
* `.vec` é o formato utilizado posteriormente no treinamento do classificador

*Opcional:* visualizar conteúdo do `.vec`

    opencv_createsamples -vec laranja.vec

Iniciar treinamento:

    opencv_traincascade -data classifier -vec laranja.vec -bg negatives.txt -numStages 20 -minHitRate 0.999 -maxFalseAlarmRate 0.5 -numPos 424  -numNeg 405 -mode ALL -precalcValBufSize 256 -precalcIdxBufSize 256

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