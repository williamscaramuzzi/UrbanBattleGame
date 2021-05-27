#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

/* 
*			Aluno: William Scaramuzzi Teixeira
*			Disciplina: Computação Gráfica - SI - UFGD
*			Professor: Adailton José Alves da Cruz
*		
*
*
*			Aplicativo: Jogo 3D de Batalha Urbana entre canhões.
*
*
 */

GLuint window, subwindow;
char texto[68] = "";
float PI = 3.141592;
static int slices = 16;
static int stacks = 16;
double theta = 0.0;
double beta = 0.0;
float eyeX = 0;
float eyeY = 0;
float eyeZ = 30;
float lookX = 0;
float lookY = 0;
float lookZ = -580;
float azulangle = 0;
float pontaCanhaoAzul_x_pos = 0;
float pontaCanhaoAzul_y_pos = 0;
float pontaCanhaoAzul_z_pos = 0;
float vermelhoangle = 0;
float pontaCanhaoVermelho_x_pos = 0;
float pontaCanhaoVermelho_y_pos = 0;
float pontaCanhaoVermelho_z_pos = 0;
float azul_x = 0, azul_y = 0, azul_z = 0, vermelho_x = 0, vermelho_y=0, vermelho_z=0;
float azulangle_hor = 90;
float vermelhoangle_hor = 90;
bool vezdoazul = true;
float alturaazul = 0;
float alturavermelho = 0;
float mouseXpos = 0;
float mouseYpos = 0;
float canhaoAzulRandomPositionZ = 2;
float canhaoVermelhoRandomPositionZ = 2;
bool azulDisparando = false;
float azulContador = 0;
bool vermelhoDisparando = false;
float vermelhoContador = 0;
float azulVelocity = 5;
float vermelhoVelocity = 5;
float viewAngle;
int placarAzul=0, placarVermelho=0;
bool azulColidiu = false;
bool vermelhoColidiu = false;
bool fullscreen = false;
bool vermelhoExplodindo= false;
bool azulExplodindo= false;
int vermelhoExplosaoContador = 0;
int azulExplosaoContador = 0;


// Vari�veis pra facilitar a passagem de cor
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaGreen[] = {0.18, 0.92, 0.1, 0.1};
	GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat qaBlue[] = {0.0, 0.0, 1.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaOrange[] = {0.9, 0.5, 0.2, 1.0};
	GLfloat qaBrown[] = {0.53, 0.176, 0.196, 1.0};
	GLfloat qaLowAmbient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat qaFullAmbient[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaExplosao[] = {1.0, 0.8, 0, 1.0};


void MoveMouse(int x, int y){
     mouseXpos = x; //variáveis globais que armazenarão a posição do meu mouse
     mouseYpos = y;
     glutPostRedisplay();
}
void desenhaMouse(char* string){
        // Posi��o no universo onde o texto sera colocado
        glRasterPos3f(-8, 2, -19);
        // Exibe caracter a caracter
        while(*string) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++);
}
static void resize(int width, int height){
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 300.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
void desenhaTrajetoVermelho(float angle, float velocity){
    float g = 1.5; //gravidade é interessante não colocar o valor real do planeta terra pra ficar melhor na animação
    float radian = ((angle)*3.1415)/180;
    float radian_hor = ((vermelhoangle_hor)*3.1415926)/180;
    float velVertical = velocity*sin(radian);
    float velHorizontal = velocity*cos(radian);
    float y=0, x=0, z=0;
    glPointSize(2);
    glColor3f(1, 0.1, 0.1);
    glBegin(GL_POINTS);
		for(float t=0; t<=10; t=t+0.05) {
            x = pontaCanhaoVermelho_x_pos + velHorizontal*t*sin(radian_hor);
            y = pontaCanhaoVermelho_y_pos + 0.2 + velVertical*t - g*t*t/2;
            z = pontaCanhaoVermelho_z_pos + velHorizontal*t*cos(radian_hor);
			glVertex3f(x, y, z);
		}
	glEnd();
	glColor3f(0, 0, 0);
}
void desenhaTrajetoAzul(float angle, float velocity){
    float g = 1.5; //gravidade é interessante não colocar o valor real do planeta terra pra ficar melhor na animação
    float radian = ((angle)*3.1415)/180;
    float radian_hor = ((azulangle_hor)*3.1415926)/180;
    float velVertical = velocity*sin(radian);
    float velHorizontal = velocity*cos(radian);
    float y=0, x=0, z=0;
    glPointSize(2);
    glColor3f(0.0, 0.0, 1);
    glBegin(GL_POINTS);
		for(float t=0; t<=10; t=t+0.05) {
            x = pontaCanhaoAzul_x_pos + velHorizontal*t*sin(radian_hor);
            y = pontaCanhaoAzul_y_pos + 0.2 + velVertical*t - g*t*t/2;
            z = pontaCanhaoAzul_z_pos + velHorizontal*t*cos(radian_hor);
			glVertex3f(x, y, z);
		}
	glEnd();
	glColor3f(0, 0, 0);
}
void desenhaPredio(double xLen, double yLen, double zLen){

        float x = xLen;
        float y = yLen;
        float z = zLen;
        //Escolhendo a cor cinza do corpo central
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaLowAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaLowAmbient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaLowAmbient);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
        glPushMatrix();
        glTranslated(0,y,0);
                glBegin(GL_QUADS);// Begin drawing the color cube with 6 quads
                  // Define vertices in counter-clockwise (CCW) order with normal pointing out
                    // Top face (y = 1.0f)
                  glColor3f(0.0f, 0.8f, 0.1f);
                  glNormal3f(0.0, 1.0, 0.0);      // Green
                  glVertex3f( x, y, -z);
                  glVertex3f(-x, y, -z);
                  glVertex3f(-x, y,  z);
                  glVertex3f( x, y,  z);
                  // Bottom face (y = -1.0f)
                 glColor3f(0.0f, 0.5f, 0.1f);     // Green
                  glNormal3f(0.0, -1.0, 0.0);
                  glVertex3f( x, -y,  z);
                  glVertex3f(-x, -y,  z);
                  glVertex3f(-x, -y, -z);
                  glVertex3f( x, -y, -z);
                    // Front face  (z = 1.0f)
                  glColor3f(0.0f, 0.8f, 0.1f);     // Green
                  glNormal3f(0.0, 0.0, 1.0);
                  glVertex3f( x,  y, z);
                  glVertex3f(-x,  y, z);
                  glVertex3f(-x, -y, z);
                  glVertex3f(x, -y, z);
                  // Back face (z = -1.0f)
                  glColor3f(0.0f, 0.5f, 0.1f);     // Green
                  glNormal3f(0.0, 0.0, -1.0);
                  glVertex3f( x, -y, -z);
                  glVertex3f(-x, -y, -z);
                  glVertex3f(-x,  y, -z);
                  glVertex3f( x,  y, -z);
                  // Left face (x = -1.0f)
                  glColor3f(0.0f, 0.5f, 0.1f);     // Green
                  glNormal3f(-1.0, 0.0, 0.0);
                  glVertex3f(-x,  y,  z);
                  glVertex3f(-x,  y, -z);
                  glVertex3f(-x, -y, -z);
                  glVertex3f(-x, -y,  z);
                  // Right face (x = 1.0f)
                  glColor3f(0.0f, 0.6f, 0.1f);     // Green
                  glNormal3f(1.0, 0.0, 0.0);
                  glVertex3f(x,  y, -z);
                  glVertex3f(x,  y,  z);
                  glVertex3f(x, -y,  z);
                  glVertex3f(x, -y, -z);
                glEnd();
        glPopMatrix();
}
void desenhaParalelepipedo(float xLen, float yLen, float zLen){
        float x = xLen/2;
        float y = yLen/2;
        float z = zLen/2;
        glPushMatrix();
        glRotated(0,0,0,1);
                glBegin(GL_QUADS);               // Begin drawing the color cube with 6 quads
                  // Define vertices in counter-clockwise (CCW) order with normal pointing out
                    // Top face (y = 1.0f)
                  glColor3f(0.0f, 0.8f, 0.1f);
                  glNormal3f(0.0, 1.0, 0.0);    // Green
                  glVertex3f( x, y, -z);
                  glVertex3f(-x, y, -z);
                  glVertex3f(-x, y,  z);
                  glVertex3f( x, y,  z);
                  // Bottom face (y = -1.0f)
                 glColor3f(0.0f, 0.5f, 0.1f);
                 glNormal3f(0.0, -1.0, 0.0);     // Green
                  glVertex3f( x, -y,  z);
                  glVertex3f(-x, -y,  z);
                  glVertex3f(-x, -y, -z);
                  glVertex3f( x, -y, -z);
                    // Front face  (z = 1.0f)
                  glColor3f(0.0f, 0.8f, 0.1f);
                  glNormal3f(0.0, 0.0, 1.0);     // Green
                  glVertex3f( x,  y, z);
                  glVertex3f(-x,  y, z);
                  glVertex3f(-x, -y, z);
                  glVertex3f(x, -y, z);
                  // Back face (z = -1.0f)
                  glColor3f(0.0f, 0.5f, 0.1f);
                  glNormal3f(0.0, 0.0, -1.0);      // Green
                  glVertex3f( x, -y, -z);
                  glVertex3f(-x, -y, -z);
                  glVertex3f(-x,  y, -z);
                  glVertex3f( x,  y, -z);
                  // Left face (x = -1.0f)
                  glColor3f(0.0f, 0.5f, 0.1f);
                  glNormal3f(-1.0, 0.0, 0.0);     // Green
                  glVertex3f(-x,  y,  z);
                  glVertex3f(-x,  y, -z);
                  glVertex3f(-x, -y, -z);
                  glVertex3f(-x, -y,  z);
                  // Right face (x = 1.0f)
                  glColor3f(0.0f, 0.6f, 0.1f);
                  glNormal3f(1.0, 0.0, 0.0);     // Green
                  glVertex3f(x,  y, -z);
                  glVertex3f(x,  y,  z);
                  glVertex3f(x, -y,  z);
                  glVertex3f(x, -y, -z);
                glEnd();
        glPopMatrix();
}

static void desenhaCanhaoAzul(){
    //Escolhendo a cor verde do corpo central
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    desenhaParalelepipedo(1.5, 0.3, 0.5);
    //Escolhendo a cor laranja para as rodas
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaOrange);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaOrange);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glPushMatrix();
        glTranslated(-0.4,-0.15,-0.4);
        gluDisk(gluNewQuadric(), 0,0.2,50,50); //cap laranja do cilindro no come�o aonde ele nasce
        gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.8, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,0.4); //cap do cilindro no final
        gluDisk(gluNewQuadric(), 0,0.2,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,-0.4);
        gluDisk(gluNewQuadric(), 0,0.2,50,50);
        gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.8, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,0.4); //cap do cilindro
        gluDisk(gluNewQuadric(), 0,0.2,50,50);
    glPopMatrix();
     //Escolhendo a cor azul pras rodas e bola central
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaBlue);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glPushMatrix();
        glTranslated(0.4,-0.15,0.2);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,-0.38);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,-0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,-0.38);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,-0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,0.2);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.15,0.2,0);
        glutSolidSphere(0.26,20,20);
    glPopMatrix();
    //Escolhendo a cor marrom pro cano
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaBrown);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBrown);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glPushMatrix();
        glTranslated(-0.15,0.2,0);
        glRotated(azulangle_hor, 0, 1, 0);
        glRotated(azulangle, -1, 0, 0);
        gluCylinder(gluNewQuadric(), 0.1, 0.1, 0.9, 50, 50);
    glPopMatrix();
    //chamar desenho do trajeto aqui dentro dessa matrix
        if(vezdoazul){
            float a = azulangle*3.141592/180;
            float b = (azulangle_hor)*3.141592/180;
            pontaCanhaoAzul_x_pos = (1*sin(b))/10;
            pontaCanhaoAzul_y_pos = (-1*sin(a+b)-1*sin(a-b))/20;
            pontaCanhaoAzul_z_pos = (1*cos(a+b)+1*cos(a-b))/20;
            desenhaTrajetoAzul(azulangle, azulVelocity);
        }
}

static void desenhaCanhaoVermelho(){
       //Escolhendo a cor verde do corpo central
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
		glMaterialfv(GL_BACK, GL_DIFFUSE, qaGreen);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    desenhaParalelepipedo(1.5, 0.3, 0.5);
    //Escolhendo a cor laranja para as rodas
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaOrange);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaOrange);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glPushMatrix();
        glTranslated(-0.4,-0.15,-0.4);
        gluDisk(gluNewQuadric(), 0,0.2,50,50); //cap laranja do cilindro no come�o aonde ele nasce
        gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.8, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,0.4); //cap do cilindro no final
        gluDisk(gluNewQuadric(), 0,0.2,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,-0.4);
        gluDisk(gluNewQuadric(), 0,0.2,50,50);
        gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.8, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,0.4); //cap do cilindro
        gluDisk(gluNewQuadric(), 0,0.2,50,50);
    glPopMatrix();
     //Escolhendo a cor vermelha pras rodas e bola central
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glPushMatrix();
        glTranslated(0.4,-0.15,0.2);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,-0.38);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.4,-0.15,-0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,-0.38);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,-0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,0.2);
        gluCylinder(gluNewQuadric(), 0.22, 0.22, 0.18, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.4,-0.15,0.38); //cap azul da roda
        gluDisk(gluNewQuadric(),0,0.22,50,50);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.15,0.2,0);
        glutSolidSphere(0.26,20,20);
    glPopMatrix();
    //Escolhendo a cor marrom pro cano
		glMaterialfv(GL_FRONT, GL_AMBIENT, qaBrown);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBrown);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glPushMatrix();
        glTranslated(-0.15,0.2,0);
        glRotated(vermelhoangle_hor, 0, 1, 0); //rodo no eixo y
        glRotated(vermelhoangle, -1, 0, 0); //rodo no eixo x
        gluCylinder(gluNewQuadric(), 0.1, 0.1, 0.9, 50, 50);
    glPopMatrix();
        //chamar desenho do trajeto aqui dentro dessa matrix
        if(vezdoazul==false){
            float a = vermelhoangle*3.141592/180;
            float b = (vermelhoangle_hor)*3.141592/180;
            pontaCanhaoVermelho_x_pos = (1*sin(b))/10;
            pontaCanhaoVermelho_y_pos = (-1*sin(a+b)-1*sin(a-b))/20;
            pontaCanhaoVermelho_z_pos = (1*cos(a+b)+1*cos(a-b))/20;
            desenhaTrajetoVermelho(vermelhoangle, vermelhoVelocity);
        }

}
void desenhaEixos(){
    glPushMatrix();
        glBegin(GL_LINES);
        glMaterialfv(GL_FRONT, GL_AMBIENT, qaBlue);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaBlue);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
                  glColor3f(0.0f, 0.0f, 0.9f);    //Eixo X
                  glVertex3f( -12, 0, 0);
                  glVertex3f(12, 0, 0);
        glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaGreen);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
                  glColor3f(0.0f, 0.9f, 0.0f);      //Eixo y
                  glVertex3f( 0, -12, 0);
                  glVertex3f(0, 12, 0);
        glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaRed);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
                  glColor3f(0.9f, 0.0f, 0.0f);
                  glVertex3f( 0, 0, -12);
                  glVertex3f(0, 0, 12);
      glEnd();
    glPopMatrix();
}
void ajustaLook(){
    viewAngle = mouseXpos*0.0049;
    lookZ = -cos(viewAngle)*(-40); //lookZ e etc sao variáveis globais que eu armazeno pra onde minha camera está olhando
    lookX = sin(viewAngle)*(-40);
    float yradians = (mouseYpos)*0.005;
    lookY = cos(yradians)*30;
}
void desenhaCidade(){
    glPushMatrix();
    GLfloat qaAzulCeleste[] = {0.56, 0.75, 0.92, 0.9};
        glMaterialfv(GL_FRONT, GL_AMBIENT, qaAzulCeleste);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaAzulCeleste);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaAzulCeleste);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glutSolidSphere(100,20,20);
    glPopMatrix();
    GLfloat qaVerdeChao[] = {0.1, 0.4, 0.1, 0.1};
    glPushMatrix();
        glBegin(GL_QUADS);
        glMaterialfv(GL_FRONT, GL_AMBIENT, qaVerdeChao);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, qaVerdeChao);
		glMaterialfv(GL_FRONT, GL_SPECULAR, qaVerdeChao);
		glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
                  // Define vertices in counter-clockwise (CCW) order with normal pointing out
                    // Top face (y = 1.0f)
                  glColor3f(0.0f, 0.8f, 0.0f);
                  glNormal3f(0.0, 1.0, 0.0);    // Green
                  glVertex3f( 150, 0, -150);
                  glVertex3f(-150, 0, -150);
                  glVertex3f(-150, 0,  150);
                  glVertex3f( 150, 0,  150);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glTranslated(-14, 0, -5);
        desenhaPredio(2,alturaazul+2,2); //atras lado azul
    glPopMatrix();
    glPushMatrix();
        glTranslated(-14, 0, 6);
        desenhaPredio(2,alturaazul-0.9,3); //frente lado azul
    glPopMatrix();
    glPushMatrix();
        glTranslated(14, 0, -7);
        desenhaPredio(5,alturavermelho+1,1); //atras lado vermelho
    glPopMatrix();
    glPushMatrix();
        glTranslated(2, 0, -8);
        desenhaPredio(5,alturaazul,3); //meio atras
    glPopMatrix();
    glPushMatrix();
        glTranslated(0, 0, 0);
        desenhaPredio(2,alturavermelho,3); //meio meio
    glPopMatrix();
    glPushMatrix();
        glTranslated(16, 0, 7);
        desenhaPredio(5,alturaazul-0.9,1); //frente lado vermelho
    glPopMatrix();
}
void vermelhoExplosao(){
            glTranslated(vermelho_x, vermelho_y, vermelho_z);
            glMaterialfv(GL_FRONT, GL_AMBIENT, qaExplosao);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, qaExplosao);
            glMaterialfv(GL_FRONT, GL_SPECULAR, qaExplosao);
            glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
            glutSolidSphere(vermelhoExplosaoContador, 30, 30);
            Sleep(15);
    vermelhoExplosaoContador++;
    if(vermelhoExplosaoContador>10){
        vermelhoColidiu = false;
        vermelhoExplodindo = false;
        vermelhoExplosaoContador = 0;
    }
}
void azulExplosao(){
            glTranslated(azul_x, azul_y, azul_z);
            glMaterialfv(GL_FRONT, GL_AMBIENT, qaExplosao);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, qaExplosao);
            glMaterialfv(GL_FRONT, GL_SPECULAR, qaExplosao);
            glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
            glutSolidSphere(azulExplosaoContador, 30, 30);
            Sleep(15);
    azulExplosaoContador++;
    if(azulExplosaoContador>10){
        azulColidiu = false;
        azulExplodindo = false;
        azulExplosaoContador = 0;
    }
}
void azulDisparou(){
    float g = 1.5; //gravidade é interessante não colocar o valor real do planeta terra pra ficar melhor na animação
    float radian = ((azulangle)*3.1415)/180;
    float radian_hor = ((azulangle_hor)*3.1415)/180;
    float velVertical = azulVelocity*sin(radian);
    float velHorizontal = azulVelocity*cos(radian);
    float y=0, x=0, z=0;
    glPointSize(2);
    glColor3f(0.1, 0.1, 1);
    x = pontaCanhaoAzul_x_pos + velHorizontal*azulContador*sin(radian_hor);
    y = pontaCanhaoAzul_y_pos + 0.2 + velVertical*azulContador - g*azulContador*azulContador/2;
    z = pontaCanhaoAzul_z_pos + velHorizontal*azulContador*cos(radian_hor);
    glPushMatrix();
            glTranslated(x, y, z); //essa translação fudeu tudo, mas consegui arrumar
                glMaterialfv(GL_FRONT, GL_AMBIENT, qaBlue);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
                glMaterialfv(GL_FRONT, GL_SPECULAR, qaBlue);
                glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
			glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
	glColor3f(0, 0, 0);
	azulContador = azulContador+0.125;
	//tratamento de colisão
	//tenho que pegar a posicao do vermelho e descontar da minha, lembrando que eu estou posicionado transladado na linha 520 acima
	if(x+azul_x>(vermelho_x-0.9) && x+azul_x<(vermelho_x+0.9)){
        if(z+azul_z<(vermelho_z+1) && z+azul_z>(vermelho_z-1)){
            if(y+azul_y<(vermelho_y+1) && y+azul_y>(vermelho_y-1)){
                printf("vermelhoColidiu true!\n");
                vermelhoColidiu = true;
                vermelhoExplodindo = true;
            }
        }
	}
	Sleep(15);
    if(vermelhoColidiu){
        placarAzul++;
        vermelhoColidiu = false;
        azulContador=15;
	}
	if(azulContador>12){
        azulDisparando=false;
        azulContador=0;
        vezdoazul = false;
        glutSetWindow(subwindow);
            glutPostRedisplay();
            glutSetWindow(window);
            glutPostRedisplay();
	}
}
void vermelhoDisparou(){
    float g = 1.5; //gravidade é interessante não colocar o valor real do planeta terra pra ficar melhor na animação
    float radian = ((vermelhoangle)*3.1415)/180;
    float radian_hor = ((vermelhoangle_hor)*3.1415)/180;
    float velVertical = vermelhoVelocity*sin(radian);
    float velHorizontal = vermelhoVelocity*cos(radian);
    float y=0, x=0, z=0;
    glPointSize(2);
    glColor3f(0.1, 0.1, 1);
    x = pontaCanhaoVermelho_x_pos + velHorizontal*vermelhoContador*sin(radian_hor);
    y = pontaCanhaoVermelho_y_pos + 0.2 + velVertical*vermelhoContador - g*vermelhoContador*vermelhoContador/2;
    z = pontaCanhaoVermelho_z_pos + velHorizontal*vermelhoContador*cos(radian_hor);
    glPushMatrix();
            glRotated(180, 0, 1, 0);
            glTranslated(x, y, z);
                glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
                glMaterialfv(GL_FRONT, GL_SPECULAR, qaRed);
                glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
			glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
	glColor3f(0, 0, 0);
	vermelhoContador = vermelhoContador+0.125;
	//tratamento de colisão
	//tenho que pegar a posicao do azul e descontar da minha posicao
	//printf("vermelho_x-x: %f vermelho_z-z: %f  y+vermelho_y: %f\n", vermelho_x-x, vermelho_z-z, y+vermelho_y);
	if(vermelho_x-x>(azul_x-0.9) && vermelho_x-x<(azul_x+0.9)){
        if(vermelho_z-z<(azul_z+1) && vermelho_z-z>(azul_z-1)){
            if(y+vermelho_y<(azul_y+1) && y+vermelho_y>(azul_y-1)){
                printf("azulColidiu true!\n");
                azulColidiu = true;
                azulExplodindo = true;
            }
        }
	}
	Sleep(15);
	if(azulColidiu){
        placarVermelho++;
        azulColidiu = false;
        vermelhoContador=13;
	}
	if(vermelhoContador>12){
        vermelhoDisparando=false;
        vermelhoContador=0;
        vezdoazul = true;
        glutSetWindow(subwindow);
            glutPostRedisplay();
            glutSetWindow(window);
            glutPostRedisplay();
	}
}

static void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);
	// Desenhos aqui
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ajustaLook();
    gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 99, 0);
    //desenhaEixos();
    desenhaMouse(texto);
    glPushMatrix();
        if(canhaoAzulRandomPositionZ==1){
            glTranslated(-14, (alturaazul+2)*2+0.29, -5);
            azul_x = -14;
            azul_y = (alturaazul+2)*2+0.29;
            azul_z = -5;
            if(azulDisparando){
                    azulDisparou();
            }
        } else if(canhaoAzulRandomPositionZ==2){
            glTranslated(-14, alturaazul*2+0.29, 0);
            azul_x=-14;
            azul_y =alturaazul*2+0.29;
            azul_z =0;
            if(azulDisparando){
                    azulDisparou();
            }
        } else{
            glTranslated(-14,(alturaazul-0.9)*2+0.29, 6);
            azul_x = -14;
            azul_y = (alturaazul-0.9)*2+0.29;
            azul_z = 6;
            if(azulDisparando){
                    azulDisparou();
                }
            }
        desenhaCanhaoAzul();
    glPopMatrix();
    glPushMatrix();
        if(canhaoVermelhoRandomPositionZ==1){
            glTranslated(14, (alturavermelho+1)*2+0.29, -7);
            vermelho_x = 14;
            vermelho_y = (alturavermelho+1)*2+0.29;
            vermelho_z = -7;
            if(vermelhoDisparando){
                vermelhoDisparou();
            }
        } else if(canhaoVermelhoRandomPositionZ==2){
            glTranslated(14, alturavermelho*2+0.29, 0);
            vermelho_x = 14;
            vermelho_y = alturavermelho*2+0.29;
            vermelho_z = 0;
            if(vermelhoDisparando){
                vermelhoDisparou();
            }
        } else{
            glTranslated(14,(alturaazul-0.9)*2+0.29, 7);
            vermelho_x = 14;
            vermelho_y = (alturaazul-0.9)*2+0.29;
            vermelho_z = 7;
            if(vermelhoDisparando){
                vermelhoDisparou();
            }
            }
            glRotated(180, 0, 1, 0);
        desenhaCanhaoVermelho();
    glPopMatrix();
    glPushMatrix();
    //carrinho azul
        glTranslated(-14, 0, 0);
        desenhaPredio(2,alturaazul,2); //altura � partir do centro
    glPopMatrix();
    glPushMatrix();
    //carrinho vermelho
        glTranslated(14, 0, 0);
        desenhaPredio(3,alturavermelho,3); //altura � partir do centro
    glPopMatrix();
    desenhaCidade();
    if(vermelhoExplodindo) vermelhoExplosao();
    if(azulExplodindo) azulExplosao();
	glFlush();
}

static void key(unsigned char key, int x, int y){
    switch (key)
    {
    case 27:
        exit(0);
        case 'd':
            eyeX -= (float)cos(viewAngle);
            eyeZ -= (float)sin(viewAngle);
            break;

        case 'a':
            eyeX += (float)cos(viewAngle);
            eyeZ += (float)sin(viewAngle);
            break;

        case 'w':
            eyeZ += (float)cos(viewAngle);
            eyeX -= (float)sin(viewAngle);
            break;

        case 's':
            eyeZ -= (float)cos(viewAngle);
            eyeX += (float)sin(viewAngle);
            break;

        case 'q':
            eyeY = eyeY + 1;
            break;

        case 'z':
            eyeY = eyeY -1;
            break;

        case 'i':
            if(vezdoazul){
                if(azulangle<=89) azulangle = azulangle+1;

            } else {
                if(vermelhoangle<=89) vermelhoangle = vermelhoangle+1;
            }
            break;
        case 'k':
            if(vezdoazul){
                if(azulangle>=1) azulangle = azulangle-1;
            } else {
                if(vermelhoangle>=1) vermelhoangle = vermelhoangle-1;
            }
            break;
        case 'j':
            if(vezdoazul){
                azulangle_hor = azulangle_hor+1;
            } else {
                vermelhoangle_hor = vermelhoangle_hor+1;
            }
            break;
        case 'l':
            if(vezdoazul){
                azulangle_hor = azulangle_hor-1;
            } else {
                vermelhoangle_hor = vermelhoangle_hor-1;
            }
            break;

        case 'u':
            if(vezdoazul){
                if(azulVelocity>0) azulVelocity = azulVelocity-0.25;

            } else {
                if(vermelhoVelocity>0) vermelhoVelocity = vermelhoVelocity-0.25;
            }
            glutSetWindow(subwindow);
            glutPostRedisplay();
            glutSetWindow(window);
            glutPostRedisplay();
            break;
        case 'o':
            if(vezdoazul){
                if(azulVelocity<15) azulVelocity = azulVelocity+0.25;

            } else {
                if(vermelhoVelocity<15) vermelhoVelocity = vermelhoVelocity+0.25;
            }
            glutSetWindow(subwindow);
            glutPostRedisplay();
            glutSetWindow(window);
            glutPostRedisplay();
            break;
        case ' ':
            if(vezdoazul){
                azulDisparando = true;
            } else {
                vermelhoDisparando = true;            }

            glutSetWindow(subwindow);
            glutPostRedisplay();
            glutSetWindow(window);
            glutPostRedisplay();
            break;

        case 'f': //toggle screenmode
        if(!fullscreen){
        glutFullScreen();
        fullscreen = true;
        } else if(fullscreen){
        glutReshapeWindow(1920,1080);
        glutPositionWindow(10,10);
        fullscreen = false;
        }
        break;

    }
}

static void idle(void)
{
    glutPostRedisplay();
}
void desenhaBarraAzul(){
    glPushMatrix();
    glColor3f(0, 0, 1);
    char vez[13] = "Vez do Azul!";
    // Exibe caracter a caracter
    glRasterPos2f(0.2, 0.1);
    for(int i=0; i<=13; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, vez[i]);
    }
    glRasterPos2f(0.2, 0.0);
    char comandos[] = "J e L: < >   I e K: ^ v";
    for(int i=0; i<=22; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, comandos[i]);
    }
    glRasterPos2f(0.2, -0.1);
    char velocidade[28];
    sprintf(velocidade, "Velocidade (teclas U e O): %.2f", azulVelocity);
    for(int i=0; i<=30; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, velocidade[i]);
    }
    glRasterPos2f(0.2, -0.2);
    char disparar[] = "Espaco: disparar";
    for(int i=0; i<=15; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, disparar[i]);
    }
    glPopMatrix();

    glColor3f(1, 1, 1);
    glPushMatrix();
        glRasterPos2f(-1.9, 0.05);
        char camera[] = "Camera: WASD e mouse, altura: Q Z";
        for(int i=0; i<=sizeof(camera)-1; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, camera[i]);
        }
    glPopMatrix();
    glPushMatrix();
        glRasterPos2f(-1.9, -0.2);
        char sairtexto[] = "ESC para sair";
        for(int i=0; i<=12; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, sairtexto[i]);
        }
    glPopMatrix();

}

void desenhaBarraVermelha(){
    glPushMatrix();
    glColor3f(1, 0, 0);
    char vez[] = "Vez do Vermelho!";
    // Exibe caracter a caracter
    glRasterPos2f(0.2, 0.1);
    for(int i=0; i<=15; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, vez[i]);
    }
    glRasterPos2f(0.2, 0.0);
    char comandos[] = "J e L: < >   I e K: ^ v";
    for(int i=0; i<=22; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, comandos[i]);
    }
    glRasterPos2f(0.2, -0.1);
    char velocidade[28];
    sprintf(velocidade, "Velocidade (teclas U e O): %.2f", vermelhoVelocity);
    for(int i=0; i<=30; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, velocidade[i]);
    }
    glRasterPos2f(0.2, -0.2);
    char disparar[] = "Espaco: disparar";
    for(int i=0; i<=15; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, disparar[i]);
    }
    glPopMatrix();

    glColor3f(1, 1, 1);
    glPushMatrix();
        glRasterPos2f(-1.9, 0.05);
        char camera[] = "Camera: WASD e mouse, altura: Q Z";
        for(int i=0; i<=sizeof(camera)-1; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, camera[i]);
        }
    glPopMatrix();
    glPushMatrix();
        glRasterPos2f(-1.9, -0.2);
        char sairtexto[] = "ESC para sair";
        for(int i=0; i<=12; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, sairtexto[i]);
        }
    glPopMatrix();
}

void desenhaPlacar(){
    glPushMatrix();
    glColor3f(0, 0, 1);
    char a[2];
    sprintf(a, "%d", placarAzul);
    // Exibe caracter a caracter
    glRasterPos2f(1.5, 0.35);
    for(int i=0; i<=1; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, a[i]);
    }
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 0, 0);
    char v[2];
    sprintf(v, "%d", placarVermelho);
    // Exibe caracter a caracter
    glRasterPos2f(-1.5, 0.35);
    for(int i=0; i<=1; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, v[i]);
    }
    glPopMatrix();

}
/* Program entry point */
void initialize(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 8.0);
	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// Set lighting intensity and color
	GLfloat qaAmbientLight[]	= {0.2, 0.2, 0.2, 1.0};
	GLfloat qaDiffuseLight[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat qaSpecularLight[]	= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);
	// Set the light position
	GLfloat qaLightPosition[]	= {0.5, 80, 30, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
	glutSwapBuffers();
}
void subwindow_display(){
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	// Desenhos aqui
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -0.5, 0.5);
	glColor3f(0.99, 0.9, 0.1);
	desenhaPlacar();
	if(vezdoazul) desenhaBarraAzul(); else desenhaBarraVermelha();

}
void subwindow_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char *argv[])
{
    srand(time(0));//gera uma "semente" pra função random ficar randômica
    alturaazul = ((rand()%12)+1); //me dá uma altura randômica entre 1 e 12
    alturavermelho = ((rand()%12)+1);
    canhaoAzulRandomPositionZ = ((rand()%3)+1); //me dá uma altura randômica entre 1 e 3
    canhaoVermelhoRandomPositionZ = ((rand()%3)+1); //me dá uma altura randômica entre 1 e 3
    if(alturaazul>alturavermelho) eyeY = alturaazul*2+2;
    else eyeY = alturavermelho*2+2;
    glutInit(&argc, argv);
    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    window = glutCreateWindow("Batalha Urbana 3d");
    initialize();
    glutPassiveMotionFunc(MoveMouse);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    //glutFullScreen();
    subwindow = glutCreateSubWindow(window, 420,700,700,200); //largura total no meu pc 1540
    glutReshapeFunc(subwindow_reshape);
    glutDisplayFunc(subwindow_display);
    glutIdleFunc(idle);
    glutMainLoop();
    return EXIT_SUCCESS;
}
