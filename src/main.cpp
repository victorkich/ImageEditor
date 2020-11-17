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

void buildTextures();

void buildTextures()
{
   //geracao automatica de coordenadas
   glGenTextures( 1, &texture);
   glBindTexture( GL_TEXTURE_2D, texture);

   float escala = 1.0 / 10;
   float p1[4] = { escala, 0,      0, 0 };
   float p2[4] = { 0,      escala, 0, 0 };
   glTexGenfv(GL_S, GL_OBJECT_PLANE, p1);
   glTexGenfv(GL_T, GL_OBJECT_PLANE, p2);

   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D,
                     GL_RGB,
                     img1->getWidth(),
                     img1->getHeight(),
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     data);
   glutSwapBuffers();
}

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
   float abertura = 35;
   float znear  = 1;
   float zfar   = 200;
   glEnable(GL_TEXTURE);
   glEnable(GL_TEXTURE_2D);
   CV::plot(abertura, znear, zfar, h, v, RECT_SIZE);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_TEXTURE);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   bt->Render();
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
           buildTextures();
       }
       if( bimg->Colidiu(x, y) )
       {
           img1->convertRGBtoGRAY();
           data = img1->getImage();
           buildTextures();
       }
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Trabalho 1");

   bt = new Botao(100, 100, 140, 50, "Grayscale");
   
   img1 = new Bmp(".\/Canvas2D\/resources\/kyoto.bmp");
   img1->convertBGRtoRGB();
   data = img1->getImage();
   if( data != NULL )
   {
      glEnable(GL_TEXTURE);
      glEnable(GL_TEXTURE_2D);
      buildTextures();
   }
   bimg = new Botao(img1->getWidth(), img1->getHeight(), 140, 50, "");

   CV::run();
}
