#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "gl_canvas2d.h"

class Slider{
  float altura, largura, x, y, cr=0, cg=0, cb=0;
  int value=0, addv=0;

public:
  Slider(float _x, float _y, float _larg, float _alt, int val)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     value = val;
  }

  void setColor(float r, float g, float b){
    cr = r;
    cg = g;
    cb = b;
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
      addv = (float(x)*100)/largura;
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

  void updateLocation(float _x, float _y)
  {
    x = _x;
    y = _y;
  }

  void Render()
  {
    CV::color(cr, cg, cb);
    CV::rectFill(x, y+(altura/2)-3, (largura/100)*float(value+addv)+x-6, y+(altura/2)+3);
    CV::rectFill((largura/100)*float(value+addv)+x+6, y+(altura/2)-1, x+largura, y+(altura/2)+1);
    CV::rectFill((largura/100)*float(value+addv)+x-6, y+2, (largura/100)*float(value+addv)+x+6, y+altura-2);
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Colidiu(int mx, int my)
  {
      if( mx >= (largura/100)*float(value+addv)+x-6 && mx <= (largura/100)*float(value+addv)+x+6 && my >= y+2 && my <= y+altura-2 )
      {
          return true;
      }
      return false;
  }

};

#endif