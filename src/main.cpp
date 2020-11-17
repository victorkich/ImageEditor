/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "gl_canvas2d.h"

#include "Botao.h"
#include "Bmp.h"

#define RECT_SIZE 10


Botao *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
Botao *bimg = NULL;
Bmp *img1;
unsigned char *data;
GLuint texture;

int v = 0;
int h = 0;
float ang=0;

bool flag = false;
int savedx=0;
int savedy=0;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   if (flag){
      img1->setStartx(mouseX-savedx);
      img1->setStarty(mouseY-savedy);
      bimg->updateLocation(mouseX-savedx, mouseY-savedy);
   }
   bimg->Render(true);
   img1->Render();
   bt->Render(false);
   DrawMouseScreenCoords();
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if( key < 200 )
   {
      opcao = key;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( state == 0 ) //clicou
   {
       if( bt->Colidiu(x, y) )
       {
           img1->convertRGBtoGRAY();
           data = img1->getImage();
       }
       if( bimg->Colidiu(x, y) )
       {
           savedx = mouseX;
           savedy = mouseY;
           flag = true;
       }
   }
   if(state == 1){
      if( bimg->Colidiu(x, y) )
       {
           flag = false;
       }
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Trabalho 1");

   img1 = new Bmp(".\/Canvas2D\/resources\/kyoto.bmp");
   img1->convertBGRtoRGB();
   data = img1->getImage();

   bimg = new Botao(img1->getStartx(), img1->getStarty(), img1->getWidth(), img1->getHeight(), "");
   bt = new Botao(100, 100, 140, 50, "Grayscale");

   CV::run();
}
