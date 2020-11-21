#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

#include "gl_canvas2d.h"

//Classe de criacao da barra de rolagem vertical
class Scrollbar{
  float altura, largura, x, y, cr1=0, cg1=0, cb1=0, cr2=0, cg2=0, cb2=0;
  int value=0, addv=0;

//Chamada de criacao
public:
  Scrollbar(float _x, float _y, float _larg, float _alt, int val)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     value = val;
  }

  // Funcao para configurar as cores da barra de fundo e da barra de selecao
  void setColor(float r1, float g1, float b1, float r2, float g2, float b2){
    cr1 = r1;
    cg1 = g1;
    cb1 = b1;
    cr2 = r2;
    cg2 = g2;
    cb2 = b2;
  }

  //Funcao para retornar o valor de 0 ate 100 indicado pela barra de rolagem vertical
  int getValue()
  {
    return value + addv;
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

  //Funcao para reescalonar a imagem dentro do canvas
  void updateScale(int w, int h, int a, int l){
    x = w;
    y = h;
    altura = a;
    largura = l;
  }

  //Funcao para renderizar a barra de selecao juntamente com a barra atras dela
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

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima da barra de rolagem vertical
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