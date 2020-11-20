#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

#include "gl_canvas2d.h"

class Scrollbar{
  float altura, largura, x, y, cr1=0, cg1=0, cb1=0, cr2=0, cg2=0, cb2=0;
  int value=0, addv=0;

public:
  Scrollbar(float _x, float _y, float _larg, float _alt, int val)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     value = val;
  }

  void setColor(float r1, float g1, float b1, float r2, float g2, float b2){
    cr1 = r1;
    cg1 = g1;
    cb1 = b1;
    cr2 = r2;
    cg2 = g2;
    cb2 = b2;
  }

  float getValue()
  {
    return float(value + addv);
  }

  void updateValue(){
    value += addv;
    addv = 0;
  }

  void setValue(int x){
    if (100 >= (addv + value) && (addv + value) >= 0)
    {
      addv = (float(x)*100)/altura;
    }

    if ((addv + value) > 100)
    {
      addv = addv - (value + addv - 100);
    }else if((addv + value) < 0)
    {
      addv = addv + abs(value + addv);
    }
    
  }

  void updateScale(int w, int h, int a, int l){
    x = w;
    y = h;
    altura = a;
    largura = l;
  }

  void Render(bool scrolling, int val)
  {
    if (scrolling)
    {
       setValue(val);
    }
    CV::color(cr1, cg1, cb1);
    CV::rectFill(x, y, x+largura, y+altura);
    CV::color(cr2, cg2, cb2);
    CV::rectFill(x+1, (altura/100)*float(value+addv)+y-6, x+largura-1, (altura/100)*float(value+addv)+y+6);
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Colidiu(int mx, int my)
  {
      if( mx >= x+1 && mx <= x+largura-1 && my >= (altura/100)*float(value+addv)+y-6 && my <= (altura/100)*float(value+addv)+y+6 )
      {
          return true;
      }
      return false;
  }

};

#endif