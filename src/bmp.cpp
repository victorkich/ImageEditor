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

uchar* Bmp::getImage()
{
  return data;
}

int Bmp::getWidth(void)
{
  return width;
}

int Bmp::getHeight(void)
{
  return height;
}

void Bmp::updateStartx()
{
   startx = startx+addx;
   addx = 0;
}

void Bmp::updateStarty()
{
   starty = starty+addy;
   addy = 0;
}

int Bmp::getStartx(){
   return startx;
}

int Bmp::getStarty(){
   return starty;
}

void Bmp::setAddx(int x)
{
   addx = x;
}

void Bmp::setAddy(int y)
{
   addy = y;
}

void Bmp::rotate(int a){
   angle = (a * M_PI) / 180;
}

void Bmp::useWindow(bool w){
   window = w;
}

int Bmp::getbytesPerLine(){
   return bytesPerLine;
}

void Bmp::restore(const char *fileName){
   load(fileName);
   convertBGRtoRGB();
}

bool Bmp::collide(int mx, int my)
{
   if( mx >= startx+addx && mx <= (addx + startx + width) && my >= starty+addy && my <= (starty + height + addy) )
   {
      return true;
   }
   return false;
}

void Bmp::Render()
{
   if (window){
      CV::color(0.8,0.1,0.1);
      CV::rectFill(startx+addx-5,starty+addy-25,startx+addx+width+5,starty+addy+height+5);
      CV::color(1,1,1);
      CV::text(startx+addx,starty+addy-7, "Kyoto.bmp");
   }
   if( data != NULL)
   {
      for(int y=0; y<height; y++)
      for(int x=0; x<width*3; x+=3){
         int pos = y*bytesPerLine + x;
         CV::color((float)(data[pos])/255, (float)(data[pos+1])/255, (float)(data[pos+2])/255);
         double old_x = x/3+startx+addx;
         double old_y = abs(y-height)+starty+addy;
         int new_x = (int)(old_x * cos(angle) - old_y * sin(angle));
         int new_y = (int)(old_x * sin(angle) + old_y * cos(angle));
         CV::point(new_x, new_y);
      }
   }

}

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
}
