#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "gl_canvas2d.h"

class Checkbox{
  float altura, largura, x, y;
  char label[100];
  bool active;

public:
  Checkbox(float _x, float _y, float _larg, float _alt, char *_label, bool act)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     strcpy(label, _label);
     active = act;
  }

  void updateLocation(float _x, float _y)
  {
    x = _x;
    y = _y;
  }

  void updateScale(int w, int h){
    x = w;
    y = h;
  }

  void updateStatus()
  {
    active = !active;
  }

  bool getStatus()
  {
    return active;
  }

  void Render()
  {
    CV::color(1, 1, 1);
    CV::rect(x, y, x + largura, y + altura);
    CV::color(1, 1, 1);
    CV::text(x+largura+5, y+altura/2, label); //escreve o label do botao mais ou menos ao centro.

    if (active)
    {
      CV::color(1, 1, 1);
      CV::line(x, y, x+largura, y+altura);
      CV::line(x, y+altura, x+largura, y);
    }
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