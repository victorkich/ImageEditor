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
#include "Slider.h"
#include "Checkbox.h"
#include "Textfield.h"
#include "Histogram.h"


Botao *bt_gs = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
Botao *bt_r = NULL;
Botao *bt_b = NULL;
Botao *bt_g = NULL;
Botao *bt_clear = NULL;
Botao *bt_right = NULL;
Botao *bt_left = NULL;

Histogram *hist = NULL;
Checkbox *cb = NULL;
Slider *slid_r = NULL;
Slider *slid_g = NULL;
Slider *slid_b = NULL;

Bmp *img1;
unsigned char *data;

bool moving = false, resizing = false, sliding_r = false, sliding_g = false, sliding_b = false;
int savedx=0;
int savedy=0;
int ang=0;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    //CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    //CV::text(10,320, str);
}

void updateScales()
{
   cb->updateScale((screenWidth>>5)*26, screenHeight>>5, round(float(40*screenHeight)/720), round(float(40*screenWidth)/1280));
   hist->updateScale((screenWidth>>5)*24 - int((1280-screenWidth)*0.2), (screenHeight>>5)*6, round(float(150*screenHeight)/720));
   slid_r->updateScale((screenWidth>>5)*25, (screenHeight>>5)*23, round(float(50*screenHeight)/720), round(float(200*screenWidth)/1280));
   slid_g->updateScale((screenWidth>>5)*25, (screenHeight>>5)*26, round(float(50*screenHeight)/720), round(float(200*screenWidth)/1280));
   slid_b->updateScale((screenWidth>>5)*25, (screenHeight>>5)*29, round(float(50*screenHeight)/720), round(float(200*screenWidth)/1280));
   bt_clear->updateScale(screenWidth>>5, screenHeight>>4, round(float(50*screenHeight)/720));
   bt_r->updateScale(screenWidth>>5, (screenHeight>>4)*3, round(float(50*screenHeight)/720));
   bt_g->updateScale(screenWidth>>5, (screenHeight>>4)*5, round(float(50*screenHeight)/720));
   bt_b->updateScale(screenWidth>>5, (screenHeight>>4)*7, round(float(50*screenHeight)/720));
   bt_gs->updateScale(screenWidth>>5, (screenHeight>>4)*9, round(float(50*screenHeight)/720));
   bt_right->updateScale(screenWidth>>5, (screenHeight>>4)*11, round(float(50*screenHeight)/720));
   bt_left->updateScale(screenWidth>>5, (screenHeight>>4)*13, round(float(50*screenHeight)/720));
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   glClearColor(slid_r->getValue()/100, slid_g->getValue()/100, slid_b->getValue()/100, 0);
   updateScales();
   cb->Render();
   slid_r->Render();
   slid_g->Render();
   slid_b->Render();
   if (cb->getStatus())
   {
       hist->Render();
   }
    
   if (moving)
   {
      img1->setAddx(mouseX-savedx);
      img1->setAddy(mouseY-savedy);
   }
   if (resizing)
   {
      img1->resize(mouseX-savedx, mouseY-savedy);
   }
   if (sliding_r)
   {
       slid_r->setValue(mouseX-savedx);
   }
   if (sliding_g)
   {
       slid_g->setValue(mouseX-savedx);
   }
   if (sliding_b)
   {
       slid_b->setValue(mouseX-savedx);
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
           bt_gs->setColor(0.4, 0.4, 0.4);
           img1->convertRGBtoGRAY();
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( img1->collide(x, y) )
       {
           savedx = mouseX;
           savedy = mouseY;
           moving = true;
       }
       if (img1->windowCollide(x, y))
       {
           savedx = mouseX;
           savedy = mouseY;
           resizing = true;
       }
       if( bt_r->Colidiu(x, y) )
       {
           bt_r->setColor(0.8, 0, 0);
           img1->chooseChannel(1);
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_g->Colidiu(x, y) )
       {
           bt_g->setColor(0, 0.6, 0);
           img1->chooseChannel(2);
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_b->Colidiu(x, y) )
       {
           bt_b->setColor(0, 0, 0.8);
           img1->chooseChannel(3);
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_clear->Colidiu(x, y) )
       {
           bt_clear->setColor(0, 0.4, 0);
           img1->restore(".\/Canvas2D\/resources\/kyoto.bmp");
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_right->Colidiu(x, y) )
       {
           bt_right->setColor(0, 0.2, 0.4);
           ang -= 10;
           img1->rotate(ang);
       }
       if( bt_left->Colidiu(x, y) )
       {
           bt_left->setColor(0.4, 0.2, 0);
           ang += 10;
           img1->rotate(ang);
       }
       if( cb->Colidiu(x, y) )
       {
           cb->updateStatus();
       }
       if (slid_r->Colidiu(x, y))
       {
           savedx = mouseX;
           sliding_r = true;
       }
       if (slid_g->Colidiu(x, y))
       {
           savedx = mouseX;
           sliding_g = true;
       }
       if (slid_b->Colidiu(x, y))
       {
           savedx = mouseX;
           sliding_b = true;
       }
   }
   if(state == 1){
      if( img1->collide(x, y) )
       {
           moving = false;
           img1->updateStartx();
           img1->updateStarty();
       }
       if( resizing ){
           resizing = false;
           img1->updateStartx();
           img1->updateStarty();
           img1->updateResize();
       }
       if( sliding_r )
       {
           sliding_r = false;
           slid_r->updateValue();
       }
       if( sliding_g )
       {
           sliding_g = false;
           slid_g->updateValue();
       }
       if( sliding_b )
       {
           sliding_b = false;
           slid_b->updateValue();
       }
       if( bt_gs->Colidiu(x, y) )
       {
           bt_gs->setColor(0.6, 0.6, 0.6);
       }
       if( bt_r->Colidiu(x, y) )
       {
           bt_r->setColor(1, 0.2, 0.2);
       }
       if( bt_g->Colidiu(x, y) )
       {
           bt_g->setColor(0.2, 0.8, 0.2);
       }
       if( bt_b->Colidiu(x, y) )
       {
           bt_b->setColor(0.2, 0.2, 1);
       }
       if( bt_clear->Colidiu(x, y) )
       {
           bt_clear->setColor(0.2, 0.6, 0.2);
       }
       if( bt_right->Colidiu(x, y) )
       {
           bt_right->setColor(0.2, 0.4, 0.6);
       }
       if( bt_left->Colidiu(x, y) )
       {
           bt_left->setColor(0.6, 0.4, 0.2);
       }
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Image Editor");

   img1 = new Bmp(".\/ImageEditor\/resources\/kyoto.bmp");
   img1->convertBGRtoRGB();
   data = img1->getImage();
   img1->setAddx((screenWidth>>4)*5);
   img1->setAddy((screenHeight>>5)*9);
   img1->updateStartx();
   img1->updateStarty();
   img1->useWindow(true);

   hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());

   bt_gs = new Botao(200, 50, 140, 50, "Grayscale");
   bt_gs->setColor(0.6, 0.6, 0.6);
   bt_r = new Botao(200, 150, 140, 50, "Red Channel");
   bt_r->setColor(1, 0.2, 0.2);
   bt_g = new Botao(200, 250, 140, 50, "Green Channel");
   bt_g->setColor(0.2, 0.8, 0.2);
   bt_b = new Botao(200, 350, 140, 50, "Blue Channel");
   bt_b->setColor(0.2, 0.2, 1);
   bt_clear = new Botao(200, 450, 140, 50, "Clear");
   bt_clear->setColor(0.2, 0.6, 0.2);
   bt_right = new Botao(200, 550, 140, 50, "Right Spin");
   bt_right->setColor(0.2, 0.4, 0.6);
   bt_left = new Botao(200, 650, 140, 50, "Left Spin");
   bt_left->setColor(0.6, 0.4, 0.2);

   cb = new Checkbox(1000, 700, 40, 40, "Histogram", false);
   slid_r = new Slider(1000, 500, 200, 50, 25);
   slid_r->setColor(1,0.3,0.3);
   slid_g = new Slider(1000, 550, 200, 50, 25);
   slid_g->setColor(0.3,1,0.3);
   slid_b = new Slider(1000, 600, 200, 50, 25);
   slid_b->setColor(0.3,0.3,1);

   CV::run();
}
