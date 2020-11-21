#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"

//Classe de criacao do botao
class Botao{
  float altura, largura, x, y, cr=0, cg=0, cb=0;
  char label[100];

//Chamada de criacao
public:
  Botao(float _x, float _y, float _larg, float _alt, char *_label)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     strcpy(label, _label);
  }

  // Funcao para configurar as cores da barra de rolagem horizontal
  void setColor(float r, float g, float b)
  {
    cr = r;
    cg = g;
    cb = b;
  }

  //Funcao para atualizar a localizacao em torno do ponto inicial x e y
  void updateLocation(float _x, float _y)
  {
    x = _x;
    y = _y;
  }

  //Funcao para reescalonar a imagem dentro do canvas
  void updateScale(int w, int h, int a){
    x = w;
    y = h;
    altura = a;
  }

  //Funcao para renderizar o botao
  void Render()
  {
    CV::color(cr, cg, cb);
    CV::rectFill(x, y, x + largura, y + altura);
    CV::color(1, 1, 1);
    CV::text(x+5, y+altura/2, label); //escreve o label do botao mais ou menos ao centro.
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
