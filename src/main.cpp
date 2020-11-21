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
#include "Scrollbar.h"


Botao *bt_gs = NULL;
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
Scrollbar *sb = NULL;
Textfield *tf = NULL;

Bmp *img1;
unsigned char *data;


//variavel global para selecao do que sera exibido na canvas.
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool moving = false, resizing = false, sliding_r = false, sliding_g = false, sliding_b = false, scrolling = false, writing = false;
int savedx=0, savedy=0, ang=0;

//Funcao que atualiza posicoes e tamanhos dos itens contidos no canvas levando em consideracao o tamanho atual do canvas
void updateScales()
{
   printf("\nNivel scroolbar: %d", sb->getValue());
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
   tf->updateScale((screenWidth>>5)*25 - int((1280-screenWidth)*0.2), (screenHeight>>5)*15);
   sb->updateScale(screenWidth-(screenWidth>>6), 0, screenHeight, screenWidth);
}

//Funcao principal de renderizacao dos itens graficos contidos dentro do canvas
void render()
{
   glClearColor(slid_r->getValue()/100, slid_g->getValue()/100, slid_b->getValue()/100, 0);
   updateScales();
   cb->Render();
   if (cb->getStatus()) //checa se o checkbox esta ativado para entao mostrar o histograma
   {
       hist->Render();
   }
   slid_r->Render(sliding_r, mouseX-savedx);
   slid_g->Render(sliding_g, mouseX-savedx);
   slid_b->Render(sliding_b, mouseX-savedx);
   tf->Render();
   sb->Render(scrolling, mouseY-savedy);
   img1->Render(moving, resizing, mouseX-savedx, mouseY-savedy);
   bt_gs->Render();
   bt_r->Render();
   bt_g->Render();
   bt_b->Render();
   bt_clear->Render();
   bt_right->Render();
   bt_left->Render();
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   if( writing ) //Se esta escrevendo, envia o caractere para o textfield
   {
       tf->addChar(key);
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //Espaco para implementacoes futuras
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   if( state == 0 ) //clicou
   {
       if( tf->Colidiu(x, y) )//Troca o status escrevendo/nao escrevendo
       {
           writing = !writing;
           tf->setStatus(writing);
       }
       if( bt_gs->Colidiu(x, y) )//Converte para grayscale e atualiza o histograma
       {
           bt_gs->setColor(0.4, 0.4, 0.4);
           img1->convertRGBtoGRAY();
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( img1->collide(x, y) )//Ativa a movimentacao da imagem com o mouse
       {
           savedx = mouseX;
           savedy = mouseY;
           moving = true;
       }
       if (img1->windowCollide(x, y))//Ativa o reescalamento da imagem atraves do mouse
       {
           savedx = mouseX;
           savedy = mouseY;
           resizing = true;
       }
       if( bt_r->Colidiu(x, y) )//Converte para somente vermelho e atualiza o histograma
       {
           bt_r->setColor(0.8, 0, 0);
           img1->chooseChannel(1);
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_g->Colidiu(x, y) )//Converte para somente verde e atualiza o histograma
       {
           bt_g->setColor(0, 0.6, 0);
           img1->chooseChannel(2);
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_b->Colidiu(x, y) )//Converte para somente azul e atualiza o histograma
       {
           bt_b->setColor(0, 0, 0.8);
           img1->chooseChannel(3);
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_clear->Colidiu(x, y) )//Recarrega a imagem e atualiza o histograma
       {
           bt_clear->setColor(0, 0.4, 0);
           img1->restore(".\/Canvas2D\/resources\/kyoto.bmp");
           data = img1->getImage();
           hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());
       }
       if( bt_right->Colidiu(x, y) )//Gira a imagem para a direita
       {
           bt_right->setColor(0, 0.2, 0.4);
           ang -= 10;
           img1->rotate(ang);
       }
       if( bt_left->Colidiu(x, y) )//Gira a imagem para a esquerda
       {
           bt_left->setColor(0.4, 0.2, 0);
           ang += 10;
           img1->rotate(ang);
       }
       if( cb->Colidiu(x, y) )//Atualiza o status da checkbox
       {
           cb->updateStatus();
       }
       if( slid_r->Colidiu(x, y) )//Movimenta a primeira barra de rolagem horizontal
       {
           savedx = mouseX;
           sliding_r = true;
       }
       if( slid_g->Colidiu(x, y) )//Movimenta a segunda barra de rolagem horizontal
       {
           savedx = mouseX;
           sliding_g = true;
       }
       if( slid_b->Colidiu(x, y) )//Movimenta a terceira barra de rolagem horizontal
       {
           savedx = mouseX;
           sliding_b = true;
       }
       if( sb->Colidiu(x, y) )//Movimenta a barra de rolagem vertical
       {
           savedy = mouseY;
           scrolling = true;
       }
   }
   if( state == 1 ){
      if( img1->collide(x, y) )//Salva a posicao onde a imagem parou
       {
           moving = false;
           img1->updateStartx();
           img1->updateStarty();
       }
       if( resizing ){//Salva a escala que a imagem parou
           resizing = false;
           img1->updateStartx();
           img1->updateStarty();
           img1->updateResize();
       }
       if( sliding_r )//Salva a posicao onde a primeira barra de rolagem horizontal parou
       {
           sliding_r = false;
           slid_r->updateValue();
       }
       if( sliding_g )//Salva a posicao onde a segunda barra de rolagem horizontal parou
       {
           sliding_g = false;
           slid_g->updateValue();
       }
       if( sliding_b )//Salva a posicao onde a terceira barra de rolagem horizontal parou
       {
           sliding_b = false;
           slid_b->updateValue();
       }
       if( scrolling )//Salva a posicao onde a barra de rolagem vertical parou
       {
           scrolling = false;
           sb->updateValue();
       }
       if( bt_gs->Colidiu(x, y) )//Atualiza a cor do botão grayscale
       {
           bt_gs->setColor(0.6, 0.6, 0.6);
       }
       if( bt_r->Colidiu(x, y) )//Atualiza a cor do botão red
       {
           bt_r->setColor(1, 0.2, 0.2);
       }
       if( bt_g->Colidiu(x, y) )//Atualiza a cor do botão green
       {
           bt_g->setColor(0.2, 0.8, 0.2);
       }
       if( bt_b->Colidiu(x, y) )//Atualiza a cor do botão blue
       {
           bt_b->setColor(0.2, 0.2, 1);
       }
       if( bt_clear->Colidiu(x, y) )//Atualiza a cor do botão clear
       {
           bt_clear->setColor(0.2, 0.6, 0.2);
       }
       if( bt_right->Colidiu(x, y) )//Atualiza a cor do botão right
       {
           bt_right->setColor(0.2, 0.4, 0.6);
       }
       if( bt_left->Colidiu(x, y) )//Atualiza a cor do botão left
       {
           bt_left->setColor(0.6, 0.4, 0.2);
       }
   }
   if( button == 3 )//Atualiza valor do scrollbar atraves do scroll do mouse, para cima
   {
       sb->setValue(-(screenHeight>>5));
       sb->updateValue();
   }
   if( button == 4 )//Atualiza valor do scrollbar atraves do scroll do mouse, para baixo
   {
       sb->setValue(screenHeight>>5);
       sb->updateValue();
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Image Editor");

   //Cria e carrega a imagem do tipo BMP
   img1 = new Bmp(".\/ImageEditor\/resources\/kyoto.bmp");
   img1->convertBGRtoRGB();
   data = img1->getImage();
   img1->setAddx((screenWidth>>4)*5);
   img1->setAddy((screenHeight>>5)*9);
   img1->updateStartx();
   img1->updateStarty();
   img1->useWindow(true); //Escolhe utilizar uma janela em volta da imagem

   //Cria e inicializa o histograma
   hist = new Histogram(625, 600, 150, data, img1->getWidth(), img1->getHeight(), img1->getbytesPerLine());

   //Cria a inicializa todos os botoes
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

   //Cria a inicializa a caixa de selecao
   cb = new Checkbox(1000, 700, 40, 40, "Histogram", false);

   //Cria a inicializa as barras de rolamento horizontal
   slid_r = new Slider(1000, 500, 200, 50, 25);
   slid_r->setColor(1,0.3,0.3);
   slid_g = new Slider(1000, 550, 200, 50, 25);
   slid_g->setColor(0.3,1,0.3);
   slid_b = new Slider(1000, 600, 200, 50, 25);
   slid_b->setColor(0.3,0.3,1);

   //Cria e inicializa a barra de rolamento vertical
   sb = new Scrollbar(1260, 0, 20, 720, 0);
   sb->setColor(0.5, 0.3, 0.3, 0.7, 0.2, 0.2);

   //Cria e inicializa o campo de texto
   tf = new Textfield(1000, 325, 200, 150);
   tf->setColor(1,1,1,0,0,0);

   CV::run();
}
