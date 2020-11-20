#ifndef __TEXTFIELD_H__
#define __TEXTFIELD_H__

#include "gl_canvas2d.h"
#define MAX_CHARS 162

class Textfield{
  float altura, largura, x, y, cr1=1, cg1=1, cb1=1, cr2=0, cg2=0, cb2=0;
  char label_line[MAX_CHARS], buf[MAX_CHARS];
  int num_chars=0;

public:
  Textfield(float _x, float _y, float _larg, float _alt)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
  }

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