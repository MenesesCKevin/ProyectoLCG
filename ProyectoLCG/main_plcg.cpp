//Semestre 2018 - 1
//Proyecto Laboratorio de computacion grafica, FI, UNAM, Grupo 1 
//************************************************************//
//************************************************************//
//************** Alumno (s): *********************************//
//*************	 Hernandez Gomez Mauricio Alejandro		******//
//*************	 Meneses Cornejo Kevin Osmar			******//
//************************************************************//


#include "texture.h"
#include "figuras.h"
#include "Camera.h"

#include "cmodel/CModel.h"
//Solo para Visual Studio 2015
#if (_MSC_VER == 1900)
#   pragma comment( lib, "legacy_stdio_definitions.lib" )
#endif

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
static GLuint ciudad_display_list;	//Display List for the Monito


//NEW// Keyframes
float posX =0, posY = 2.5, posZ =-3.5, rotRodIzq = 0;
float giroMonito = 0;
float movBrazoDer = 0.0;

#define MAX_FRAMES 9
int i_max_steps = 90;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotInc;
	float giroMonito;
	float giroMonitoInc;
	float movBrazoDer;
	float movBrazoDerInc;

	
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex=5;			//introducir datos
bool play=false;
int playIndex=0;


//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

int w = 500, h = 500;
int frame=0,time,timebase=0;
char s[30];

CCamera objCamera;	//Create objet Camera

GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW) 

int font=(int)GLUT_BITMAP_HELVETICA_18;

//Otras opciones son:
/*GLUT_BITMAP_8_BY_13
GLUT_BITMAP_9_BY_15
GLUT_BITMAP_TIMES_ROMAN_10
GLUT_BITMAP_TIMES_ROMAN_24
GLUT_BITMAP_HELVETICA_10
GLUT_BITMAP_HELVETICA_12
GLUT_BITMAP_HELVETICA_18*/



//GLfloat Diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat Diffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };				// Diffuse Light Values
GLfloat Specular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values
GLfloat Position[]= { 0.0f, 7.0f, -5.0f, 0.0f };			// Light Position
GLfloat Position2[]= { 0.0f, 0.0f, -5.0f, 1.0f };			// Light Position

GLfloat m_dif1[] = { 0.0f, 0.2f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec1[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb1[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s1[] = {18};

GLfloat m_dif2[] = { 0.8f, 0.2f, 0.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec2[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb2[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s2[] = {22};

CTexture nx;	//Cielo
CTexture pz;	//Cielo
CTexture px;	//Cielo
CTexture nz;	//Cielo
CTexture py;	//Cielo
CTexture ny;	//piso
CTexture fachada;
CTexture fachadaDI;	//fachada de los laterales
CTexture techo;	//techo
CTexture text5;	//Pasto01
CTexture text6;	//Casa01

CTexture tree;

CFiguras fig1;
CFiguras fig2;
CFiguras fig3;
CFiguras fig4;	//Pasto011
CFiguras fig5;	//Casa01
CFiguras fig6;
CFiguras fig7;	//Para crear Monito

CFiguras cubo;

//Figuras de 3D Studio
CModel kit;
CModel llanta;
CModel casita;
CModel oldhouse;

//Animación del coche
float angRot = 0.0;
float movKitX = 0.0;
float movKitY = 4.0;
float movKitZ = 0.0;
float rotKit = 0.0;
float rotTires = 0.0;
bool g_fanimacion = false;
bool g_avanza = false;
float girallanta;
bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
bool recorrido6 = false;
bool recorrido7 = false;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	movBrazoDer = KeyFrame[0].movBrazoDer;

}

void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	KeyFrame[playIndex].giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	KeyFrame[playIndex].movBrazoDerInc = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;

}





GLuint createDL() 
{
	GLuint ciudadDL;
	//GLuint cieloDL;

	// Create the id for the list
	ciudadDL = glGenLists(1);
	// start list
	glNewList(ciudadDL,GL_COMPILE);
	// call the function that contains 
	// the rendering commands
		//monito();
	// endList
	glEndList();

	return(ciudadDL);
}
			
void InitGL ( GLvoid )     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);

	glShadeModel (GL_SMOOTH);
	//Para construir la figura comentar esto
	glLightfv(GL_LIGHT1, GL_POSITION, Position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Position2);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	//glEnable ( GL_COLOR_MATERIAL );
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_BACK, GL_LINE);

	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	/* setup blending */
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE);			// Set The Blending Function For Translucency
    //glColor4f(1.0f, 1.0f, 1.0f, 0.5); 
    
    nx.LoadTGA("Texturas/nx.tga");
	nx.BuildGLTexture();
	nx.ReleaseImage();

	pz.LoadTGA("Texturas/pz.tga");
	pz.BuildGLTexture();
	pz.ReleaseImage();

	px.LoadTGA("Texturas/px.tga");
	px.BuildGLTexture();
	px.ReleaseImage();

	nz.LoadTGA("Texturas/nz.tga");
	nz.BuildGLTexture();
	nz.ReleaseImage();

	py.LoadTGA("Texturas/py.tga");
	py.BuildGLTexture();
	py.ReleaseImage();

	ny.LoadTGA("Texturas/ny.tga");
	ny.BuildGLTexture();
	ny.ReleaseImage();

	fachada.LoadTGA("Texturas/fachada.tga");
	fachada.BuildGLTexture();
	fachada.ReleaseImage();

	techo.LoadTGA("Texturas/techo.tga");
	techo.BuildGLTexture();
	techo.ReleaseImage();

	fachadaDI.LoadTGA("Texturas/fachadaDI.tga");
	fachadaDI.BuildGLTexture();
	fachadaDI.ReleaseImage();
	

	casita._3dsLoad("Dollshouse.3ds");

	oldhouse._3dsLoad("oldhouse/oldhouse.3ds");
	oldhouse.LoadTextureImages();
	oldhouse.GLIniTextures();
	oldhouse.ReleaseTextureImages();
	
	objCamera.Position_Camera(0,14.0f,13.0f, 0,2.5f,0, 0, 1, 0);

	//NEW Crear una lista de dibujo
	ciudad_display_list = createDL();


}

void pintaTexto(float x, float y, float z, void *font,char *string)
{
  
  char *c;     //Almacena los caracteres a escribir
  glRasterPos3f(x, y, z);	//Posicion apartir del centro de la ventana
  //glWindowPos2i(150,100);
  for (c=string; *c != '\0'; c++) //Condicion de fin de cadena
  {
    glutBitmapCharacter(font, *c); //imprime
  }
}

void EstructuraCasa()
{
	/////////////////////////////////////////////////////////Planta 1 parte A baño, lavabo, cuarto de maquinas
	glPushMatrix();	//Pared planta 1 de 5.75 m lado izquierdo
		glTranslatef(0.1, 1.5, -2.875);
		glScalef(0.2, 2.3, 5.75);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	
	glPushMatrix();	//Pared planta 1 de 4.05 m frontal
		glTranslatef(2.025, 1.15, -0.1);
		glScalef(4.05, 2.3, 0.2);
		cubo.prisma2(0.0,0.0);
	glPopMatrix();

	glPushMatrix();	//Fachada izquierda
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		glTranslatef(-0.13, 4.5, -12.0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		fig2.plano(9, 24.5, 0.2, fachadaDI.GLindex,2);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	glPopMatrix();
		glPushMatrix();	//Fachada derecha
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		glTranslatef(12.9, 4.5, -12.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		fig2.plano(9, 24.5, 0.2, fachadaDI.GLindex,2);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	glPushMatrix();	//techo izquierda
		glTranslatef(0.2, 10, -12.0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		glRotatef(-15, 1.0, 0.0, 0.0);
		fig2.plano(3, 24.5, 0.2, techo.GLindex, 5);
	glPopMatrix();

	glPushMatrix();	//techo derecha
		glTranslatef(12.7, 10, -12.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glRotatef(-15, 1.0, 0.0, 0.0);
		fig2.plano(3, 24.3, 0.2, techo.GLindex, 5);
	glPopMatrix();

	glPushMatrix();	//techo superior izquierdo 1
		glTranslatef(1.1, 11.5, -12.1);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		fig2.plano(0.8, 24.4, 0.2, techo.GLindex, 5);
	glPopMatrix();

	glPushMatrix();	//techo superior izquierdo 2
		glTranslatef(1.5, 11, -12.1);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		glRotatef(-135, 1.0, 0.0, 0.0);
		fig2.plano(0.8, 24.4, 0.2, techo.GLindex, 5);
	glPopMatrix();

	glPushMatrix();	//techo superior derecha 2
		glTranslatef(11.2, 11, -12.1);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glRotatef(-135, 1.0, 0.0, 0.0);
		fig2.plano(0.8, 24.4, 0.2, techo.GLindex, 5);
	glPopMatrix();

	glPushMatrix();	//techo superior izquierdo 3
	glTranslatef(6, 10.8, -12.1);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	fig2.plano(11, 24.4, 0.2, techo.GLindex, 20);
	glPopMatrix();

	glPushMatrix();	//techo superior izquierda 4
	glTranslatef(5.6, 11.2, -12.1);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-170, 1.0, 0.0, 0.0);
	fig2.plano(2.6, 24.4, 0.2, techo.GLindex, 6);
	glPopMatrix();

	glPushMatrix();	//techo superior derecha  4
	glTranslatef(6.5, 12.5, -12.0);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	fig2.plano(1.2, 24.4, 0.2, techo.GLindex, 6);
	glPopMatrix();

	glPushMatrix();	//techo superior derecha 5
	glTranslatef(7.2, 11.2, -12.1);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-10, 1.0, 0.0, 0.0);
	fig2.plano(2.6, 24.4, 0.2, techo.GLindex, 6);
	glPopMatrix();

	glPushMatrix();	//techo superior derecha 1
	glTranslatef(11.8, 11.5, -12.1);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	fig2.plano(0.8, 24.4, 0.2, techo.GLindex, 5);
	glPopMatrix();

	
	

	glPushMatrix();	//fachada frontal
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		glTranslatef(6.4, 6.2,0.22);
		fig2.plano(12.5,13.3,0.2,fachada.GLindex,1);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	glPushMatrix();	//fachada trasera
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		glTranslatef(6.4, 6.2, -24.3);
		glRotatef(180, 0.0, 1.0, 0.0);
		fig2.plano(12.5, 13.3, 0.2, fachada.GLindex,1);
		glDisable(GL_ALPHA_TEST);
	glPopMatrix();


	glPushMatrix();	//Pared planta 1 de 5.75 m lado derecho
		glTranslatef(4.15, 1.15, -2.875);
		glScalef(0.2, 2.3, 5.75);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//Pared planta 1 de 1.52 m separacion entre baño y cuarto de servicio
		glTranslatef(2.0, 1.15, -0.76);
		glScalef(0.2, 2.3, 1.52);
		cubo.prisma2(0.0, 0.0);	
	glPopMatrix();

	glPushMatrix();	//Pared planta 1 de 1.52 m separacion entre baño y cuarto lavado
		glTranslatef(1.1, 1.15, -2.5);
		glScalef(2.0, 2.3, 0.2);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//Pared planta 1 de 1.5 m separacion rara del cuarto de lavado
		glTranslatef(1.1, 1.15, -3.25);
		glScalef(0.2, 2.3, 1.5);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//Pared planta 1 de 1.5 m separacion rara del cuarto de lavado
		glTranslatef(3.25, 1.15, -3.9);
		glScalef(2.0, 2.3, 0.2);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//Pared planta 1 de 1.85 m separacion cuarto de maquinas y lavado
		glTranslatef(2.75, 1.15, -4.825);
		glScalef(0.2, 2.3, 1.85);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//Pared planta 1 de 0.8 m de lado de la entrada al cuarto de maquinas
		glTranslatef(3.7, 1.15, -5.65);
		glScalef(0.8, 2.3, 0.2);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	//Paredes del jardin y pared del estacinamiento

	glPushMatrix();	//estacionamiento parte derecha
		glTranslatef(12.6, 1.15, -4.4);
		glScalef(0.2, 2.3, 9.0);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//pared jardin izquierda
		glTranslatef(0.1, 1.15, -20.5);
		glScalef(0.2, 2.3, 7.0);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//pared jardin superior
		glTranslatef(6.35, 1.15, -24);
		glScalef(12.7, 2.3, 0.2);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();

	glPushMatrix();	//pared jardin derecha
		glTranslatef(12.6, 1.15, -20.5);
		glScalef(0.2, 2.3, 7.0);
		cubo.prisma2(0.0, 0.0);
	glPopMatrix();


	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(200.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 200.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 200.0f);
	glEnd();


	//////////////////////////////////////////////////////////////Fin de parte 1 de la plana A

	/////////////////////////////////////////////////////////Planta 1 parte B patioservicio  bodega, alacena

	glPushMatrix();

		glTranslatef(0.0, 0.0, -5.75); //Cambio mi centro momentaneamente

		glPushMatrix();	//Pared planta 1 de 4.4 m del patio servicio
			glTranslatef(0.1, 1.15, -2.2);
			glScalef(0.2, 2.3, 4.4);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();	

		glTranslatef(0.0, 0.0, -5.75); //Cambio mi centro momentaneamente

		glPushMatrix();	//Pared planta 1  de 1.35 m de la bodega lado derecho
			glTranslatef(0.1, 1.15, -5.075);
			glScalef(0.2, 2.3, 1.35);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1  de 1.35 m de la bodega lado izquierdo
			glTranslatef(1.1, 1.15, -5.075);
			glScalef(0.2, 2.3, 1.35);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1  de 1 m de la bodega lado superior
			glTranslatef(0.6, 1.15, -5.75);
			glScalef(1.2, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1  de 1.35 m de la alacena lado derecho
			glTranslatef(2.2, 1.15, -5.075);
			glScalef(0.2, 2.3, 1.35);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1  de 1 m de la alacena parte superior
			glTranslatef(2.7, 1.15, -5.75);
			glScalef(1.2, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1  de 2 m de la alacena parte inferior
			glTranslatef(3.15, 1.15, -4.5);
			glScalef(2.1, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1 de 5.75 m lado derecho
			glTranslatef(4.15, 1.15, -4.5);
			glScalef(0.2, 2.3, 2.7);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//continuacion de pared
		glTranslatef(4.15, 1.15, -6.45);
		glScalef(0.2, 2.3, 1.2);
		cubo.prisma2(0.0, 0.0);
		glPopMatrix();

	glPopMatrix();

		//////////////////////////////////////////////////////////////Fin de parte 1 de la plana B

		/////////////////////////////////////////////////////////Planta 1 parte C patioservicio  bodega, alacena, estancia ,escaleras
	glPushMatrix();

		glTranslatef(0.1, 0.0, -11.5); //Cambio mi centro momentaneamente

		glPushMatrix();	//Pared planta 1 de 4.4 m cocina lado izquierdo
			glTranslatef(0.0, 1.15, -2.3);
			glScalef(0.2, 2.3, 4.4);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1 de 1m continiacion del cocina lado izquierdo
			glTranslatef(0.0, 1.15, -5.0);
			glScalef(0.2, 2.3, 1.0);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Ventana puerta deslizante cocina 1
			glTranslatef(1.0, 1.15, -4.4);
			glScalef(1.9, 2.3, 0.05);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pilar entre ventanas de la cocina
			glTranslatef(2.05, 1.15, -4.4);
			glScalef(0.2, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Ventana puerta deslizante cocina 2
			glTranslatef(3.1, 1.15, -4.4);
			glScalef(1.9, 2.3, 0.05);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pilar entre ventanas de la cocina y comedor
			glTranslatef(4.15, 1.15, -4.4);
			glScalef(0.2, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Ventana puerta deslizante comedor 1
			glTranslatef(5.2, 1.15, -4.4);
			glScalef(1.9, 2.3, 0.05);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pilar entre ventanas del comedor
			glTranslatef(6.25, 1.15, -4.4);
			glScalef(0.2, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Ventana puerta deslizante comedor 2
			glTranslatef(7.3, 1.15, -4.4);
			glScalef(1.9, 2.3, 0.05);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pilar en la ultima ventana del comedor
			glTranslatef(8.35, 1.15, -4.4);
			glScalef(0.2, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1 de 1m continuacion area de juegos
			glTranslatef(8.35, 1.15, -5.0);
			glScalef(0.2, 2.3, 1.0);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pilar entre la estancia comedor al inicio de las escaleras
			glTranslatef(8.35, 1.15, 0.0);
			glScalef(0.2, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1 de 4.1m area de juegos parte superior
			glTranslatef(10.45, 1.15, -5.4);
			glScalef(4.1, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1 de 4.1m area de juegos parte derecha
			glTranslatef(12.5, 1.15, -2.7);
			glScalef(0.2, 2.3, 5.6);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

	glPopMatrix();


	//////////////////////////////////////////////////////////////Fin de parte 1 de la plana B

	/////////////////////////////////////////////////////////Planta 1 parte C patioservicio  bodega, alacena, estancia ,escaleras

	glPushMatrix();

		glTranslatef(8.45, 0.0, -11.5); //Cambio mi centro momentaneamente


		glPushMatrix();	//Pared planta 1 de 2.7m escaleras y vestibulo parte derecha
			glTranslatef(4.15, 1.15, 1.35);
			glScalef(0.2, 2.3, 2.5);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

		glPushMatrix();	//Pared planta 1 de 5.1m estancia y vestibulo
			glTranslatef(0.43, 1.15, 1.2);
			glScalef(5.1, 2.3, 0.2);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();


		glPushMatrix();	//Pared planta 1 de 1.5m entre baño y escaleras
			glTranslatef(0.8, 1.15, 1.85);
			glScalef(0.2, 2.3, 1.5);
			cubo.prisma2(0.0, 0.0);
		glPopMatrix();

	glPopMatrix();



}

void display ( void )   // Creamos la funcion donde se dibuja
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	
	glPushMatrix();

	glRotatef(g_lookupdown,1.0f,0,0);

		gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,	
					objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,	
					objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);

		glPushMatrix();		
			glPushMatrix(); //Creamos cielo
				glDisable(GL_LIGHTING);
				glTranslatef(0,50,0);
				fig1.skybox(100.0, 100.0, 100.0,ny.GLindex,nx.GLindex,pz.GLindex,px.GLindex,nz.GLindex,py.GLindex);
				glEnable(GL_LIGHTING);
			glPopMatrix();
			glTranslatef(0.0, 0.0, 10.0);
			glPushMatrix();
				EstructuraCasa();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
			glColor3f(1.0,0.0,0.0);
			pintaTexto(-11,12.0,-14.0,(void *)font,"Proyecto CG");
			pintaTexto(-11,8.5,-14,(void *)font,s);
			glColor3f(1.0,1.0,1.0);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glutSwapBuffers ( );

}

void animacion()
{
	fig3.text_izq-= 0.01;
	fig3.text_der-= 0.01;
	if(fig3.text_izq<-1)
		fig3.text_izq=0;
	if(fig3.text_der<0)
		fig3.text_der=1;

	//Movimiento del coche
	if(g_fanimacion)
	{
		if(g_avanza)
		{
			movKitZ +=1.0;
			rotTires -= 10;
			if(movKitZ>130)
				g_avanza = false;
		}
		else
		{
			movKitZ -=1.0;
			rotTires += 10;
			if(movKitZ<0)
				g_avanza = true;
		}
	}

	if(circuito)
	{
		if(recorrido1)
		{
			printf("recorrido 1 %f,%f,%f\n", movKitX, movKitY, movKitZ);
			movKitZ ++;
			if(movKitZ>155)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if(recorrido2)
		{
			printf("recorrido 2 %f,%f,%f\n", movKitX, movKitY, movKitZ);
			rotKit = 270;
			movKitX--;
			if(movKitX < -125)
			{
				recorrido2 = false;
				recorrido4 = true;
				
			}
		}
		/*if(recorrido3)
		{
			rotKit = 180;
			movKitZ --;
			if(movKitZ<-155)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}*/
		if(recorrido4)
		{
			girallanta = 0;
			printf("recorrido 4 %f,%f,%f\n", movKitX, movKitY, movKitZ);
			rotKit = 150;
			movKitX ++;
			movKitZ-=2.5;
			if (movKitY < 60);
				movKitY++;
			if(movKitZ<30)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
			
		}
		if (recorrido5)
		{
			printf("recorrido 5 %f,%f,%f\n", movKitX, movKitY, movKitZ);
			movKitX++;
			movKitZ -= 2.5;
			
			if (movKitZ<-40)
			{
				
				movKitY--;
				if (movKitY == 4) {
					recorrido5 = false;
					recorrido6 = true;
				}
			}
			
		}
		if (recorrido6)
		{
			printf("recorrido 6 %f,%f,%f\n", movKitX, movKitY, movKitZ);
			movKitZ--;
			girallanta = 90;
			if (movKitZ<-155)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}
		if(recorrido7)
		{
			printf("recorrido 7 %f,%f,%f,\n", movKitX, movKitY, movKitZ);
			rotKit = 0;
			movKitZ ++;
			if(movKitZ>0)
			{
				recorrido7 = false;
				recorrido1 = true;
			}
		}
	}


	//Movimiento del monito
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex>FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotRodIzq += KeyFrame[playIndex].rotInc;
			giroMonito += KeyFrame[playIndex].giroMonitoInc;
			movBrazoDer += KeyFrame[playIndex].movBrazoDerInc;

			i_curr_steps++;
		}

	}

	/*frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time-timebase));
		timebase = time;		
		frame = 0;
	}*/

	glutPostRedisplay();
}

void reshape ( int width , int height )   // Creamos funcion Reshape
{
  if (height==0)										// Prevenir division entre cero
	{
		height=1;
	}

	glViewport(0,0,width,height);	

	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista
	
	glFrustum (-0.1, 0.1,-0.1, 0.1, 0.1, 170.0);

	glMatrixMode(GL_MODELVIEW);							// Seleccionamos Modelview Matrix
	glLoadIdentity();
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {

		case 'w':   //Movimientos de camara
		case 'W':
			objCamera.Move_Camera( CAMERASPEED+0.1 );
			break;

		case 's':
		case 'S':
			objCamera.Move_Camera(-(CAMERASPEED+0.1));
			break;

		case 'a':
		case 'A':
			objCamera.Strafe_Camera(-(CAMERASPEED+0.1));
			break;

		case 'd':
		case 'D':
			objCamera.Strafe_Camera( CAMERASPEED+0.1);
			break;

		case 'O':		//  
		case 'o':
				g_fanimacion^= true; //Activamos/desactivamos la animacíon
				circuito = false;
			break;

		case 'i':		//  
		case 'I':
				circuito^= true; //Activamos/desactivamos la animacíon
				g_fanimacion = false;
			break;

		case 'k':		//
		case 'K':
			if (FrameIndex<MAX_FRAMES)
			{
				saveFrame();
			}

			break;

		case 'l':
		case 'L':
			if (play == false && (FrameIndex>1))
			{

				resetElements();
				//First Interpolation				
				interpolation();

				play = true;
				playIndex = 0;
				i_curr_steps = 0;
			}
			else
			{
				play = false;
			}
			break;

		case 'y':						
		case 'Y':
			posZ++;
			//printf("%f \n", posZ);
			break;

		case 'g':						
		case 'G':
			posX--;
			//printf("%f \n", posX);
			break;

		case 'h':						
		case 'H':
			posZ--;
			//printf("%f \n", posZ);
			break;

		case 'j':						
		case 'J':
			posX++;
			//printf("%f \n", posX);
			break;

		case 'b':						
			rotRodIzq++;
			//printf("%f \n", rotRodIzq);
			break;

		case 'B':						
			rotRodIzq--;
			//printf("%f \n", rotRodIzq);
			break;

		case 'p':						
			giroMonito++;
			//printf("%f \n", giroMonito);
			break;

		case 'P':						
			giroMonito--;
			//printf("%f \n", giroMonito);
			break;

		case 27:        // Cuando Esc es presionado...
			exit ( 0 );   // Salimos del programa
			break;        
		default:        // Cualquier otra
			break;
  }

  glutPostRedisplay();
}

void arrow_keys ( int a_keys, int x, int y )  // Funcion para manejo de teclas especiales (arrow keys)
{
  switch ( a_keys ) {
	case GLUT_KEY_PAGE_UP:
		//pos_camY -= 0.5f;
		//pos_cam.y += 0.5f;
		//eye_cam.y += 0.5f;
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:
		//pos_camY += 0.5f;
		//pos_cam.y -= 0.5f;
		//eye_cam.y -= 0.5f;
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

    case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
		g_lookupdown -= 1.0f;
		break;

    case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:
		objCamera.Rotate_View( CAMERASPEED);
		break;

    default:
		break;
  }
  glutPostRedisplay();
}


int main ( int argc, char** argv )   // Main Function
{

  glutInit            (&argc, argv); // Inicializamos OpenGL
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Doble )
  glutInitWindowSize  (500, 500);	// Tamaño de la Ventana
  glutInitWindowPosition (0, 0);	//Posicion de la Ventana
  glutCreateWindow    ("Proyecto LCG"); // Nombre de la Ventana
  //glutFullScreen     ( );         // Full Screen
  InitGL ();						// Parametros iniciales de la aplicacion
  glutDisplayFunc     ( display );  //Indicamos a Glut función de dibujo
  glutReshapeFunc     ( reshape );	//Indicamos a Glut función en caso de cambio de tamano
  glutKeyboardFunc    ( keyboard );	//Indicamos a Glut función de manejo de teclado
  glutSpecialFunc     ( arrow_keys );	//Otras
  glutIdleFunc		  ( animacion );
  glutMainLoop();          // 

  return 0;
}
