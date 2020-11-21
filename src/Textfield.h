#ifndef __TEXTFIELD_H__
#define __TEXTFIELD_H__

#include "gl_canvas2d.h"
#define MAX_CHARS 162

//Classe de criacao do campo de texto
class Textfield{
  float altura, largura, x, y, cr1=1, cg1=1, cb1=1, cr2=0, cg2=0, cb2=0;
  char label_line[MAX_CHARS], buf[MAX_CHARS];
  int num_chars=0;
  bool status = false;

//Chamada de criacao
public:
  Textfield(float _x, float _y, float _larg, float _alt)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
  }

  //Funcao para adicionar ou remover letras do buffer
  void addChar(int _label)
  {
    if( _label != 8 )
    {
      if (num_chars < MAX_CHARS)
      {
        char ch = static_cast<char>(_label);
        buf[num_chars] = ch;
        num_chars++;
      }
    }else
    {
      if (num_chars>0)
      {
        num_chars--;
      }else
      {
        num_chars=0;
      }
    }
  }

  // Funcao para configurar as cores do fundo e das letras
  void setColor(float r1, float g1, float b1, float r2, float g2, float b2){
    cr1 = r1;
    cg1 = g1;
    cb1 = b1;
    cr2 = r2;
    cg2 = g2;
    cb2 = b2;
  }

  //Funcao para trocar o estado atual digitando/nao digitando
  void setStatus(bool new_status)
  {
    status = new_status;
  }

  //Funcao para reescalonar a imagem dentro do canvas
  void updateScale(int w, int h){
    x = w;
    y = h;
  }

  //Funcao para renderizar todos os conjuntos de caracteres de buffer passando para agrupamentos na variavel label_line
  void Render()
  {
    if( status )
    {
      CV::color(0.1, 0.8, 0.1);
      
    }else
    {
      CV::color(0.8, 0.1, 0.1);
    }
    CV::rectFill(x-5, y-5, x + largura+5, y + altura+5);
    CV::color(cr1, cg1, cb1);
    CV::rectFill(x, y, x + largura, y + altura);
    CV::color(cr2, cg2, cb2);
    int break_line = 1, count = -1;
    for(int c=0; c<MAX_CHARS; c++)
    {
      count++;
      if (c < num_chars)
      {
        label_line[count] = buf[c];
      }else
      {
        char word = static_cast<char>(32);
        label_line[c] = word;
      }
      if (count*3.5 > (largura/3.5))
      {
        CV::text(x+10, y+(altura/10)*break_line+2, label_line);
        for(int clear=0; clear<count+1; clear++)
        {
          char word = static_cast<char>(32);
          label_line[clear] = word;
        }
        count = -1;
        break_line++;
      }
    }    
  }

  //Recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima da caixa de texto
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