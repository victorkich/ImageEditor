#ifndef __TEXTFIELD_H__
#define __TEXTFIELD_H__

#include "gl_canvas2d.h"

class Textfield{
  float altura, largura, x, y, cr1=1, cg1=1, cb1=1, cr2=0, cg2=0, cb2=0;
  char label[100];

public:
  Textfield(float _x, float _y, float _larg, float _alt)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     //strcpy(label, _label);
  }

  void addWord(char *_label)
  {
    //
  }

  void setColor(float r1, float g1, float b1, float r2, float g2, float b2){
    cr1 = r1;
    cg1 = g1;
    cb1 = b1;
    cr2 = r2;
    cg2 = g2;
    cb2 = b2;
  }

  void updateScale(int w, int h){
    x = w;
    y = h;
  }

  void Render()
  {
    CV::color(cr1, cg1, cb1);
    CV::rectFill(x, y, x + largura, y + altura);
    CV::color(cr2, cg2, cb2);
    //CV::text(x+5, y+altura/2, label);
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