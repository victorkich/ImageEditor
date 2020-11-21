//*********************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
// Autor: Cesar Tadeu Pozzer
//        pozzer@inf.ufsm.br
//  Versao 09/2010
//
//**********************************************************

#include "Bmp.h"
#include <string.h>
#include "gl_canvas2d.h"

double angle = 0;
int new_width = 0, new_height=0, borderType=0, addh=0, addw=0;
int left=0, right=0, top=0, bottom=0;

//Chamada de criacao
Bmp::Bmp(const char *fileName)
{
   width = height = 0;
   data = NULL;
   
   if( fileName != NULL && strlen(fileName) > 0 )
   {
      load(fileName);
   }
   else
   {
      printf("Error: Invalid BMP filename");
   }
}

//Retorna o vetor dos pixels da imagem
uchar* Bmp::getImage()
{
  return data;
}

//Retorna o tamanho horizontal da imagem
int Bmp::getWidth(void)
{
  return width;
}

//Retorna o tamanho vertical da imagem
int Bmp::getHeight(void)
{
  return height;
}

//Atualiza o ponto x inicial para a imagem
void Bmp::updateStartx()
{
   startx += addx;
   addx = 0;
}

//Atualiza o ponto y inicial para a imagem
void Bmp::updateStarty()
{
   starty += addy;
   addy = 0;
}

//Retorna o ponto x inicial da imagem
int Bmp::getStartx(){
   return startx;
}

//Retorna o ponto y inicial da imagem
int Bmp::getStarty(){
   return starty;
}

//Atualiza o valor dinamico de x para modificar a posicao da imagem
void Bmp::setAddx(int x)
{
   addx = x;
}

//Atualiza o valor dinamico de y para modificar a posicao da imagem
void Bmp::setAddy(int y)
{
   addy = y;
}

//Funcao de conversar de radianos para graus
void Bmp::rotate(int a){
   angle = (a * M_PI) / 180;
}

//Funcao para selecionar se a imagem tera uma janela como borda ou nao
void Bmp::useWindow(bool w){
   window = w;
}

//Funcao que verifica qual borda foi clicada, superior, inferior, lateral direita, lateral esquerda
bool Bmp::windowCollide(int mx, int my){
   if( mx >= (startx + addx - 5) && mx <= (addx + startx + right + 5) && my >= (starty + addy - 25) && my <= (starty + bottom + addy + 5) )
   {
      if (mx >= (startx + addx - 5) && mx <= (addx + startx + 5) && my >= (starty + addy) && my <= (starty + bottom + addh + addy)){
         borderType = 1;
         return true;
      }else if (mx >= (startx + addx + right + addw) && mx <= (addx + startx + right + addw + 5) && my >= (starty + addy) && my <= (starty + bottom + addh + addy)){
         borderType = 2;
         return true;
      }else if (mx >= (startx + addx - 5) && mx <= (addx + startx + right + addw + 5) && my >= (starty + addy - 25) && my <= (starty + addy)){
         borderType = 3;
         return true;
      }else if (mx >= (startx + addx - 5) && mx <= (addx + startx + right + addw + 5) && my >= (starty + addy + bottom + addh) && my <= (starty + bottom + addh + addy + 5)){
         borderType = 4;
         return true;
      }
   }
   borderType = 0;
   return false;
}

//Funcao que aplica o reescalonamento da imagem para a respectiva borda clicada
void Bmp::resize(int x, int y){
   switch (borderType){
      case 1:
         addx = x;
         addw = -x;
         break;
      case 2:
         addw = x;
         break;
      case 3:
         addy = y;
         addh = -y;
         break;
      case 4:
         addh = y;
         break;
   }

}

//Funcao que salva os valores modificados no reescalonamento para o novo tamanho de imagem
void Bmp::updateResize(){
   new_width += addw;
   new_height += addh;
   addw = 0;
   addh = 0;
}

//Retorna a quantidade de espaco de cores por linha na imagem
int Bmp::getbytesPerLine(){
   return bytesPerLine;
}

//Funcao para recarregar a imagem novamente
void Bmp::restore(const char *fileName){
   load(fileName);
   convertBGRtoRGB();
}

//Funcao para verificar se a imagem foi clicada para ser movimentada pelo canvas
bool Bmp::collide(int mx, int my)
{
   if( mx >= startx+addx && mx <= (addx + startx + right + addw) && my >= starty+addy && my <= (starty + bottom + addy + addh) )
   {
      return true;
   }
   return false;
}

//Funcao para renderizar a imagem checando as escalas e as rotacoes, bem como a janela a sua volta
void Bmp::Render(bool moving, bool resizing, int val_x, int val_y)
{
   if (moving)//Checa se a imagem esta clicada para atualizar sua posicao
   {
      setAddx(val_x);
      setAddy(val_y);
   }
   if (resizing)//Checa se a imagem esta sendo reescalonada para modificar sua visualizacao
   {
      resize(val_x, val_y);
   }

   if (window){//Checa se a imagem ira mostrar uma janela em sua volta
      if (moving)
      {
         CV::color(0.1,0.8,0.1);
      }else if (resizing)
      {
         CV::color(0.1,0.1,0.8);
      }else
      {
         CV::color(0.8,0.1,0.1);
      }
      CV::rectFill(left+startx+addx-5,top+starty+addy-25,right+startx+addx+5, bottom+starty+addy+5);
      CV::color(1,1,1);
      CV::text(left+startx+addx,top+starty+addy-7, "Kyoto.bmp");
   }
   if( data != NULL)
   {
      //Acha o centro de rotacao da imagem
      int new_centre_height= round(((new_height+addh+1)/2)-1);
      int new_centre_width= round(((new_width+addw+1)/2)-1);

      //Calcula o passo para realizar a rotacao da imagem
      double x_step = 3*(double(width)/double(new_width+addw));
      double y_step = double(height)/double(new_height+addh);

      //Primeira vez rodando
      bool first = true;

      for(int y=0; y<height; y++)//Percorre todos os valores da imagem
      for(int x=0; x<width*3; x+=3){
         int xc = abs(x-width*3);//Desvira a imagem horizontalmente
         int pos = y*bytesPerLine + xc;//Calcula a quantidade de pixels por linha
         //Seleciona a cor de cada pixel que sera desenhado
         CV::color((float)(data[pos])/255, (float)(data[pos+1])/255, (float)(data[pos+2])/255);

         //Calcula os antigos pontos x e y que serao rotacionados
         double old_x = x/x_step-new_centre_width-1;
         double old_y = y/y_step-new_centre_height-1;

         //Calcula os novos pontos pos rotacao da imagem
         int new_y = round(-old_x*sin(angle)+old_y*cos(angle));
         int new_x = round(old_x*cos(angle)+old_y*sin(angle));

         //Recentralia a imagem ja rotacionada
         new_y=new_centre_height-new_y;
         new_x=new_centre_width-new_x;

         //Checa as bordas da imagem para ajustar a janela de fundo e o redimencionamento
         if (first){
            top = y;
            bottom = y;
            left = x;
            right = x;
            first=false;
         }else{
            if (top > new_y)
               top = new_y;
            if (bottom < new_y)
               bottom = new_y;
            if (left > new_x)
               left = new_x;
            if (right < new_x)
               right = new_x;
         }

         CV::point(new_x+startx+addx, new_y+starty+addy);
      }
      
   }

}

//Funcao que escolhe um canal de cor para manter e zera os outros canais de cores da imagem
void Bmp::chooseChannel(int c)
{
  unsigned char tmp;
  if( data != NULL )
  {
     for(int y=0; y<height; y++)
     for(int x=0; x<width*3; x+=3)
     {
        int pos = y*bytesPerLine + x;
        switch(c)
        {
           case 1:
             data[pos+1] = 0;
             data[pos+2] = 0;
             break;
           case 2:
             data[pos] = 0;
             data[pos+2] = 0;
             break;
           case 3:
             data[pos] = 0;
             data[pos+1] = 0;
             break;
        }
     }
  }
}

//Converte a imagem do padrao BGR para o RGB
void Bmp::convertBGRtoRGB()
{
  unsigned char tmp;
  if( data != NULL )
  {
     for(int y=0; y<height; y++)
     for(int x=0; x<width*3; x+=3)
     {
        int pos = y*bytesPerLine + x;
        tmp = data[pos];
        data[pos] = data[pos+2];
        data[pos+2] = tmp;
     }
  }
}

//Converte a imagem do padrao RGB para o padrao grayscale
void Bmp::convertRGBtoGRAY()
{
  unsigned char tmp;
  if( data != NULL )
  {
     for(int y=0; y<height; y++)
     for(int x=0; x<width*3; x+=3)
     {
        int pos = y*bytesPerLine + x;
        tmp = (data[pos] + data[pos+1] + data[pos+2]) / 3;
        data[pos] = tmp;
        data[pos+1] = tmp;
        data[pos+2] = tmp;
     }
  }
}

//Carrega a imagem .BMP
void Bmp::load(const char *fileName)
{
  FILE *fp = fopen(fileName, "rb");
  if( fp == NULL )
  {
     printf("\nErro ao abrir arquivo %s para leitura", fileName);
     return;
  }

  printf("\n\nCarregando arquivo %s", fileName);

  //le o HEADER componente a componente devido ao problema de alinhamento de bytes. Usando
  //o comando fread(header, sizeof(HEADER),1,fp) sao lidos 16 bytes ao inves de 14
  fread(&header.type,      sizeof(unsigned short int), 1, fp);
  fread(&header.size,      sizeof(unsigned int),       1, fp);
  fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
  fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
  fread(&header.offset,    sizeof(unsigned int),       1, fp);

  //le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
  fread(&info.size,        sizeof(unsigned int),       1, fp);
  fread(&info.width,       sizeof(int),                1, fp);
  fread(&info.height,      sizeof(int),                1, fp);
  fread(&info.planes,      sizeof(unsigned short int), 1, fp);
  fread(&info.bits,        sizeof(unsigned short int), 1, fp);
  fread(&info.compression, sizeof(unsigned int),       1, fp);
  fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
  fread(&info.xresolution, sizeof(int),                1, fp);
  fread(&info.yresolution, sizeof(int),                1, fp);
  fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
  fread(&info.impcolours,  sizeof(unsigned int),       1, fp);

  width  = info.width;
  height = info.height;
  bits   = info.bits;
  bytesPerLine =(3 * (width + 1) / 4) * 4;
  imagesize    = bytesPerLine*height;
  int delta    = bytesPerLine - (3 * width);

  printf("\nImagem: %dx%d - Bits: %d", width, height, bits);
  printf("\nbytesPerLine: %d", bytesPerLine);
  printf("\nbytesPerLine: %d", width * 3);
  printf("\ndelta: %d", delta);
  printf("\nimagesize: %d %d", imagesize, info.imagesize);

  if( header.type != 19778 )
  {
     printf("\nError: Arquivo BMP invalido");
     getchar();
     exit(0);
  }

  if( width*height*3 != imagesize )
  {
     printf("\nWarning: Arquivo BMP nao eh potencia de 2");
     getchar();
  }

  if( info.compression != 0 )
  {
     printf("\nError: Formato BMP comprimido nao suportado");
     getchar();
     return;
  }
  if( bits != 24 )
  {
     printf("\nError: Formato BMP com %d bits/pixel nao suportado", bits);
     getchar();
     return;
  }

  if( info.planes != 1 )
  {
     printf("\nError: Numero de Planes nao suportado: %d", info.planes);
     getchar();
     return;
  }

  data = new unsigned char[imagesize];
  fread(data, sizeof(unsigned char), imagesize, fp);
  fclose(fp);

  new_width = width;
  new_height = height;
}
