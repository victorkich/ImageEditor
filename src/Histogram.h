#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "gl_canvas2d.h"

class Histogram{
  float altura, x, y;
  char label[100];
  unsigned char *data;
  int width, height, bytesPerLine, max;
  int r[255], g[255], b[255];

public:
  Histogram(float _x, float _y, float _alt, unsigned char *image, int w, int h, int bpl)
  {
     altura  = _alt - 10;
     x = _x;
     y = _y;
     data = image;
     width = w;
     height = h;
     bytesPerLine = bpl;
     max = 0;
     getRGB();
  }

  void getRGB(){
    for(int p = 0 ; p < 255 ; p++){
        r[p] = 0;
        g[p] = 0;
        b[p] = 0;
    }
    if( data != NULL )
    {
        for(int y=0; y<height; y++)
        for(int x=0; x<width*3; x+=3)
        {
            int pos = y*bytesPerLine + x;
            r[(int)(data[pos])] = r[(int)(data[pos])]+1;
            g[(int)(data[pos+1])] = g[(int)(data[pos+1])]+1;
            b[(int)(data[pos+2])] = b[(int)(data[pos+2])]+1;
        }
    }
    for(int p = 1 ; p < 255 ; p++){
        if (r[p] > max)
            max = r[p];
        if (g[p] > max)
            max = g[p];
        if (b[p] > max)
            max = b[p];
    }
  }

  void updateLocation(float _x, float _y)
  {
    x = _x;
    y = _y;
  }

  void updateScale(int w, int h, int a){
    x = w;
    y = h;
    altura = a;
  }

  void Render()
  {
    CV::color(1, 1, 1);
    CV::text(x+80,y-10,"Histogram");
    CV::text(x-10,y+altura+25,"0");
    CV::text(x+252,y+altura+25,"255");
    CV::rectFill(x,y,x+265,y+altura+10);
    for(int p = 2 ; p < 255 ; p++){
       CV::color(1, 0, 0);
       CV::line(p-1+x+5, (altura-(altura/max)*r[p-1])+y+5, p+x+5, (altura-(altura/max)*r[p])+y+5);
       CV::color(0, 1, 0);
       CV::line(p-1+x+5, (altura-(altura/max)*g[p-1])+y+5, p+x+5, (altura-(altura/max)*g[p])+y+5);
       CV::color(0, 0, 1);
       CV::line(p-1+x+5, (altura-(altura/max)*b[p-1])+y+5, p+x+5, (altura-(altura/max)*b[p])+y+5);
    }
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Colidiu(int mx, int my)
  {
      if( mx >= x && mx <= (x + 265) && my >= y && my <= (y + altura) )
      {
          return true;
      }
      return false;
  }

};

#endif
