#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "gl_canvas2d.h"

class Slider{
  float altura, largura, x, y;

public:
  Slider(float _x, float _y, float _larg, float _alt)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
  }

  void updateLocation(float _x, float _y)
  {
    x = _x;
    y = _y;
  }

  void Render()
  {
    CV::color(0, 1, 0);
    CV::rectFill(x, y, x + largura, y + altura);
    CV::color(0, 0, 0);

  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Colidiu(int mx, int my)
  {
      if( mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura) )
      {
          return true;
      }
      return false;
  }

};

#endif