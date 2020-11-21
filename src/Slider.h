#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "gl_canvas2d.h"

//Classe de criacao da barra de rolagem horizontal
class Slider{
  float altura, largura, x, y, cr=0, cg=0, cb=0;
  int value=0, addv=0;

//Chamada de criacao
public:
  Slider(float _x, float _y, float _larg, float _alt, int val)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     value = val;
  }

  // Funcao para configurar as cores da barra de rolagem horizontal
  void setColor(float r, float g, float b){
    cr = r;
    cg = g;
    cb = b;
  }

  //Funcao para retornar o valor de 0 ate 100 indicado pela barra de rolagem horizontal
  float getValue()
  {
    return float(value + addv);
  }

  //Funcao para atualizar o atual estado da barra
  void updateValue(){
    value += addv;
    addv = 0;
  }

  //Funcao que atualiza o valor a ser acrescentado a variavel value levando em consideração a quantidade de pixels que foram somados ou subtraidos
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

  //Funcao para reescalonar a imagem dentro do canvas
  void updateScale(int w, int h, int a, int l){
    x = w;
    y = h;
    altura = a;
    largura = l;
  }

  //Funcao para renderizar a barra de selecao juntamente com a barra atras dela
  void Render(bool sliding, int val)
  {
    if (sliding)
    {
       setValue(val);
    }
    CV::color(cr, cg, cb);
    CV::rectFill(x, y+(altura/2)-3, (largura/100)*float(value+addv)+x-6, y+(altura/2)+3);
    CV::rectFill((largura/100)*float(value+addv)+x+6, y+(altura/2)-1, x+largura, y+(altura/2)+1);
    CV::rectFill((largura/100)*float(value+addv)+x-6, y+2, (largura/100)*float(value+addv)+x+6, y+altura-2);
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima da barra de rolagem horizontal
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