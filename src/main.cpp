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


Botao *bt_gs = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
Botao *bt_r = NULL;
Botao *bt_b = NULL;
Botao *bt_g = NULL;
Botao *bt_clear = NULL;
Botao *bt_right = NULL;
Botao *bt_left = NULL;

Bmp *img1;
unsigned char *data;

bool flag = false;
int savedx=0;
int savedy=0;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1280, screenHeight = 800; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
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
   if (flag){
      img1->setAddx(mouseX-savedx);
      img1->setAddy(mouseY-savedy);
   }
   img1->Render();
   bt_gs->Render();
   bt_r->Render();
   bt_g->Render();
   bt_b->Render();
   bt_clear->Render();
   bt_right->Render();
   bt_left->Render();
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
       if( bt_gs->Colidiu(x, y) )
       {
           img1->convertRGBtoGRAY();
       }
       if( img1->collide(x, y) )
       {
           savedx = mouseX;
           savedy = mouseY;
           flag = true;
       }
       if( bt_r->Colidiu(x, y) )
       {
           img1->chooseChannel(1);
       }
       if( bt_g->Colidiu(x, y) )
       {
           img1->chooseChannel(2);
       }
       if( bt_b->Colidiu(x, y) )
       {
           img1->chooseChannel(3);
       }
       if( bt_clear->Colidiu(x, y) )
       {
           img1->restore(".\/Canvas2D\/resources\/kyoto.bmp");
       }
       if( bt_right->Colidiu(x, y) )
       {
          //
       }
       if( bt_left->Colidiu(x, y) )
       {
          //
       }
   }
   if(state == 1){
      if( img1->collide(x, y) )
       {
           flag = false;
           img1->updateStartx();
           img1->updateStarty();
       }
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Image Editor");

   img1 = new Bmp(".\/Canvas2D\/resources\/kyoto.bmp");
   img1->convertBGRtoRGB();
   data = img1->getImage();
   img1->setAddx(550);
   img1->setAddy(200);
   img1->updateStartx();
   img1->updateStarty();
   img1->useWindow(true);

   bt_gs = new Botao(200, 50, 140, 50, "Grayscale");
   bt_r = new Botao(200, 150, 140, 50, "Only Red");
   bt_g = new Botao(200, 250, 140, 50, "Only Green");
   bt_b = new Botao(200, 350, 140, 50, "Only Blue");
   bt_clear = new Botao(200, 450, 140, 50, "Clear");
   bt_right = new Botao(200, 550, 140, 50, "Right Roll");
   bt_left = new Botao(200, 650, 140, 50, "Left Roll");

   CV::run();
}
