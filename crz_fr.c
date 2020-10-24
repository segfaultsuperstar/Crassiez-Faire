#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "SOIL.h"
#include "crz_fr.h"
#include <stdio.h>

static float ppw = 500, pph = 350; 
double rwheel;//rotates wheel
double rspeed = 0;//changes rate of rotation of rwheel
int revs = 0, mapx = 0, mapy = 0;//revs - times it has spun
//mapx and mapy for picking
bool startride = false;
long Time, ResetTime;
float interval = 0.25;
static float TWOPI = 2 * 3.1417;
float r_red = 0.25, r_green = 0.0, r_blue = 0.10;//changing orbcolor 
float xeye,yeye,zeye, xat,yat,zat,lx,ly,lz,lr= 0.0;//keyboard function controlling glulookat params and spotlight
bool lamps, spot, day, night = false;//toggles for lights and time of day
GLenum sun = GL_LIGHT0, lspot = GL_LIGHT1;

GLfloat global_ambient[] = { 0.25f,0.25f,0.25f,1.0f };
GLfloat glow[] = {0.75f,0.75f,0.0f};//for emission of lamps
GLfloat resetemission[]= {0.0,0.0,0.0};

//properties for point light
GLfloat sunposition[]={-250.f,500.f,-300.f, 0};
GLfloat sunAmb[4] = { 0.6f,0.6f,0.6f,1.0f };
GLfloat sunDif[4] = { 0.8f,0.8f,0.8f,1.0f };
GLfloat sunSpc[4] = { 1.0f,1.0f,1.0f,1.0f };

//properties for spotlight
GLfloat spotAmb[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat spotdif[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat spotspc[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat spotdir[] = { 0.0, 0.0, -1.0 };

GLuint texobj[10];//scene textures
GLuint skybox;//cube map texture

GLUquadricObj *qdr;

/******************************************************
 *                     loadtextures                 *
 ********************************************************/
 /**Purpose: uses soil for loading and binding textures that make up the scene and skybox*/
void loadtextures()
{
	texobj[0] = SOIL_load_OGL_texture("p4textures/grass64.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (texobj[0]==0)
	{
		printf("***NO BITMAP RETRIEVED for grass***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	
	texobj[1] = SOIL_load_OGL_texture("p4textures/Gravel.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (texobj[1]==0)
	{
		printf("***NO BITMAP RETRIEVED for gravel***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	texobj[2] = SOIL_load_OGL_texture("p4textures/LeavesOP.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (texobj[2]==0)
	{
		printf("***NO BITMAP RETRIEVED for leaves***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	
	texobj[3] = SOIL_load_OGL_texture("p4textures/Bark.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (texobj[3]==0)
	{
		printf("***NO BITMAP RETRIEVED for bark***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	
	texobj[4] = SOIL_load_OGL_texture("p4textures/Iron.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texobj[4]==0)
	{
		printf("***NO BITMAP RETRIEVED for iron***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	texobj[5] = SOIL_load_OGL_texture("p4textures/Planks.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texobj[5]==0)
	{
		printf("***NO BITMAP RETRIEVED for planks***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	texobj[6] = SOIL_load_OGL_texture("p4textures/LavaF1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texobj[6]==0)
	{
		printf("***NO BITMAP RETRIEVED for lava***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	texobj[7] = SOIL_load_OGL_texture("p4textures/GemOv1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texobj[7]==0)
	{
		printf("***NO BITMAP RETRIEVED for gem***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	texobj[8] = SOIL_load_OGL_texture("p4textures/Gold0.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texobj[8]==0)
	{
		printf("***NO BITMAP RETRIEVED for gold***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	texobj[9] = SOIL_load_OGL_texture("p4textures/Gold1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texobj[9]==0)
	{
		printf("***NO BITMAP RETRIEVED for gold***\n");  //Check to see if successfully loaded
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texobj[9]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	skybox = SOIL_load_OGL_cubemap("p4textures/pos_x.jpg",
					"p4textures/neg_x.jpg",
					"p4textures/pos_y.jpg",
					"p4textures/neg_y.jpg",
					"p4textures/pos_z.jpg",
					"p4textures/neg_z.jpg",SOIL_LOAD_AUTO,0,0);
if(skybox == 0)
	{
		puts("loading didnt work");
		const char *errm = SOIL_last_result();
		printf("%s\n", errm);
		exit(1);
	}
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void init(void)
{
	loadtextures();
	
	qdr = gluNewQuadric();
	gluQuadricTexture(qdr, GL_TRUE);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	
	glLightfv(sun, GL_AMBIENT, sunAmb);
	glLightfv(sun, GL_DIFFUSE, sunDif);
	glLightfv(sun, GL_SPECULAR, sunSpc);
	
	glLightfv(lspot, GL_AMBIENT, spotAmb);
	glLightfv(lspot, GL_DIFFUSE, spotdif);
	glLightfv(lspot, GL_SPECULAR, spotspc);
	glLightf(lspot, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(lspot, GL_SPOT_CUTOFF, 10.0);
	glLightfv(lspot, GL_SPOT_DIRECTION, spotdir);
	glLightf(lspot, GL_SPOT_EXPONENT, 0.0);
	
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_COLOR_MATERIAL);//use existing colors in place of glMaterial
	glMateriali(GL_FRONT, GL_SHININESS, 50);
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, ppw/pph, 1.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display(void) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		gluLookAt((240+xeye),(45+yeye),(150+zeye),(250+xat),(70+yat),(-1+zat),0,1,0);//initial position of camera
		glLightfv(sun, GL_POSITION, sunposition);
		float spotpos[] = { 340.0, 0.0, 510.0, 1.0 };//spotlight initial position
		glPushMatrix();
			glRotatef(lr,1,0,0);//rotate spotlight up and down
			glTranslatef(lx,ly,lz);//move spotlight
				glLightfv(lspot, GL_POSITION, spotpos);
		glPopMatrix();
		
		//enabling & disabling lights
		if(night && spot)
		{
			glEnable(lspot);
		}
		else{
			glDisable(lspot);
		}
		if(day){
			glEnable(sun);
		}
		else
		{
			glDisable(sun);
		}
		
		//generation of the skybox
		glPushMatrix();
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
			glTranslated(0,0,0);
			glScaled(30,25,10);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glutSolidCube(100);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_CUBE_MAP);
		glPopMatrix();
		
		//drawing entire scene
			drawScene();
	glPopMatrix();
	togglecycle();
	
	glutSwapBuffers();
	glFlush();
}


/********************************************************
 *                     unitTriangle.c                   *
 ********************************************************/
void unitTriangle ()
{
  glBegin (GL_TRIANGLES);
      glVertex2f (0.0, 0.0);
      glVertex2f (1.0, 0.0);
      glVertex2f (0.5, 1.0);
  glEnd();
}

/********************************************************
 *                      unitSquare                    *
 ********************************************************/
void unitSquare()
{
  glBegin (GL_POLYGON);
      glVertex2f (0.0, 0.0);
      glVertex2f (1.0, 0.0);
      glVertex2f (1.0, 1.0);
      glVertex2f (0.0, 1.0);
  glEnd();
}

/********************************************************
 *                      unitCircle                    *
 ********************************************************/
void unitCircle(float r)
{
  int i;
  float xc, yc;
  float Increment = TWOPI / 100.0;
  glBegin (GL_POLYGON);
    for (i = 0; i < 100; i++) {
      xc = r * cos(i * Increment);
      yc = r * sin(i * Increment);
      glVertex3f (xc, yc, 1);
    }
  glEnd();
}

/******************************************************
 *                     drawCube                 *
 ********************************************************/
 /**Purpose: Code comes from glutSolidCube. Only added texture coordinates*/
void drawCube()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLfloat size = 1.0;
	static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
	glTexCoord2f(0.0f,0.0f);
    glVertex3fv(&v[faces[i][0]][0]);
	glTexCoord2f(3.0f, 0.0f);
    glVertex3fv(&v[faces[i][1]][0]);
	glTexCoord2f(3.0f, 3.0f);
    glVertex3fv(&v[faces[i][2]][0]);
	glTexCoord2f(0.0f, 3.0f);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

/********************************************************
 *                      drawWheel                   	*
 ********************************************************/
 /**Purpose: When startride is true, the ferris wheel(FW) spins
  * for a predetermined amount of time*/
void drawWheel ()
{
	double r=25, sides=10, angle = 0;
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(1.0, 0, 0);
	for(int i=0;i<sides+1;i++){
		angle=i*TWOPI/sides;
		glVertex2f(r*cos(angle),r*sin(angle));
	}
	glEnd();
}

/********************************************************
 *                      drawSeat                   	*
 ********************************************************/
 /**Purpose: Uses solid cubes to add seats to each bar on wheel*/
void drawSeat()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(.57,.43,.85, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//used in place of glMaterial
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glBindTexture(GL_TEXTURE_2D, texobj[9]);
	glPushMatrix();
		glScalef(10,5,1);
			drawCube();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,-3,2);
		glRotatef(90,1,0,0);
		glScalef(10,5,1);
			drawCube();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,-6,4);
		glRotatef(180, 1,0,0);
		glScalef(10,5,1);
				drawCube();
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/******************************************************
 *                     drawWheelStructure             *
 ********************************************************/
 /**Purpose: draws everything(wheel, bars, seats) a typical consists of, minus
  * the support*/
void drawWheelStructure()
{
	glColor4f(.72,.45,.20, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glDisable(GL_TEXTURE_2D);
	drawWheel();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(1,0,0);
		glScaled(1,1,-5);
		glBindTexture(GL_TEXTURE_2D, texobj[8]);
			gluCylinder(qdr, 5, 5, 10, 32, 32);//big bar in middle
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushMatrix();
			glTranslated(1, 0,0);
				unitCircle(5);
			glTranslated(0,0,-50);
				unitCircle(5);
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glTranslatef(0,0,-50);
			drawWheel();
	glPopMatrix();
	glColor4f(0.3, 0.3,0.3,1);
	unitCircle(25);
	glPushMatrix();
		glTranslatef(0,0,-50);
			unitCircle(25);
	glPopMatrix();
	
	//draw bars and seats
	glColor4f(.67,.91,.91, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	double r = 25, sides = 10, angle = 0; 
	for(int i=0;i<sides+1;i++){
		angle=i*TWOPI/sides;
		glPushMatrix();
			glTranslated(r*cos(angle),r*sin(angle), -50);
			glBindTexture(GL_TEXTURE_2D, texobj[8]);
				gluCylinder(qdr, 1, 1, 50, 32, 32);//bars
				glPushMatrix();
					glTranslated(0,-1,25);
					glRotated(90, 0,1,0);
					glScaled(2,.5,1);
						drawSeat();//seats
				glPopMatrix();
		glPopMatrix();
	}
}

/********************************************************
 *                      dayscene                    *
 ********************************************************/
 /**Purpose: Draw the scene with clear blue skies, sun,
  * and brighter colors*/
void dayscene()
{
	glClearColor(0.2, 0.6, 0.8, 0.0);
}

/********************************************************
 *                      nightscene                    *
 ********************************************************/
 /**Purpose: Draw the scene with artificial lights, a moon,
  * and duller colors as well as a subdued background*/
void nightscene()
{
	glClearColor(0.18, 0.18, 0.3, 0.0);
}


/******************************************************
 *                     togglecycle                     *
 ********************************************************/
 /**Purpose: driver behing the toggling function
  * rendering the appropriate scene*/
void togglecycle()
{
	if (day)
	{
		dayscene();
	}
	else if(night)
	{
		nightscene();
	}
}


/******************************************************
 *                     drawSupport                    *
 ********************************************************/
 /**Purpose: adds legs to bottom of ferris wheel using solid cubes*/
void drawSupport()
{
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
		glRotated(15,-1,0,-1);
		glTranslatef(5,-100,5);
		glScalef(5,225,5);
			drawCube();
	glPopMatrix();
	glPushMatrix();
		glRotated(15,-1,0,1);
		glTranslatef(10,-100,5);
		glScalef(5,225,5);
			drawCube();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(10,-170,37.5);
		glRotated(90,0,0,-1);
		glScalef(5,70,5);
			drawCube();
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(0,0,-60);
			glPushMatrix();
				glRotated(15,1,0,-1);
				glTranslatef(5,-100,5);
				glScalef(5,225,5);
					drawCube();
			glPopMatrix();
			glPushMatrix();
				glRotated(15,1,0,1);
				glTranslatef(10,-100,5);
				glScalef(5,225,5);
					drawCube();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(10,-170,-27.5);
				glRotated(90,0,0,-1);
				glScalef(5,70,5);
					drawCube();
			glPopMatrix();
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

/******************************************************
 *                     draw_ride                     *
 ********************************************************/
 /**Purpose: draws spinnable wheelstructure and support for wheel*/
void draw_ride() {
	glLineWidth(5.0);
	glPushMatrix();
		glTranslatef(0,25,0);
		glPushMatrix();
			glRotated(rwheel, 0, 0, -1);
			glScaled(5,5,1);
				drawWheelStructure();
		glPopMatrix();
	glPopMatrix();
		glColor4f(.82,.57,.46, 1);
	drawSupport();
}

/*****************************************************
 *                     drawtree                     *
 ********************************************************/
 /**Purpose: draws a tree to fit in the surroundings using cones
  * and cylinder for the base*/
void drawtree(){
	glColor4f(0.64, 0.16, 0.16, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
		glTranslated(0,0,0);
		glRotated(-90,1,0,0);
		glScaled(10, 5.0,25);
		glBindTexture(GL_TEXTURE_2D, texobj[3]);
			gluCylinder(qdr, 1,1,1,32,32);
	glPopMatrix();
	glPushMatrix();
		glTranslated(0,25,0);
		glRotated(-90,1,0,0);
		glScaled(25,25,25);
		if(day)
			glColor4f(0.0, 1.0, 0.0, 1.0);
		else if(night){
			glColor4f(0.0, 0.1, 0.0, 1);
		}
		else{
			glColor4f(0.0, 1.0, 0.0, 1);
		}
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glColorMaterial (GL_FRONT, GL_SPECULAR);
			glBindTexture(GL_TEXTURE_2D, texobj[2]);
			gluCylinder(qdr,.65f, 0.0f, .65f, 32, 32);
			glPushMatrix();
				glTranslated(0,0,0.35);
					gluCylinder(qdr,.5f, 0.0f, .5f, 32, 32);
					glTranslated(0,0,0.35);
						gluCylinder(qdr,.35f, 0.0f, .35f, 32, 32);
			glPopMatrix();
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_TEXTURE_2D);
}

/******************************************************
 *                     drawminigame                     *
 ********************************************************/
 /**Purpose: Draws a minigame stand to the surroundings*/
void drawminigame()
{
	glEnable(GL_TEXTURE_2D);
	glColor4f(0.62, 0.62, 0.37, 1.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texobj[5]);
		glTranslatef(0,20,0);
		glScaled(65,40,5);
			drawCube();
			glPushMatrix();
				glTranslatef(0,0,8);
					drawCube();
			glPopMatrix();
	glPopMatrix();
			glPushMatrix();
				glRotatef(90,0,1,0);
				glTranslatef(-20,20,30);
				glScalef(45,40,5);
					drawCube();
			glPopMatrix();
	
	glColor4f(0.5, 0.0, 0.0, 1.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glBindTexture(GL_TEXTURE_2D, texobj[6]);
	glPushMatrix();
		glRotatef(90,0,1,0);
		glTranslatef(-40,62.5,30);
		glScaled(5, 45,5);
			drawCube();
		glTranslated(8, 0, 0);
			drawCube();
	glPopMatrix();
	
	glColor4f(0.0, 0.0, 0.25, 1.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glBindTexture(GL_TEXTURE_2D, texobj[7]);
	glPushMatrix();
		glRotatef(-90,0,1,0);
		glTranslated(20, 75, -33.5);
		glScaled(50,30,3.5);
			drawCube();
	glPopMatrix();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_TEXTURE_2D);
}

/******************************************************
 *                     drawlamp                  *
 ********************************************************/
 /**Purpose: adds street lamps to the scene that are 
  * off during the day and emit at night, controllable by toggles*/
void drawlamp()
{
	glEnable(GL_TEXTURE_2D);
	glColor4f(0.1,0.1,0.1,1.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPushMatrix();
		glTranslatef(0,60,0);
		glScaled(10,125,10);
		glBindTexture(GL_TEXTURE_2D, texobj[4]);
			drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	glColor4f(0.25, 0.25, 0,1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	if(night && lamps)glMaterialfv(GL_FRONT, GL_EMISSION, glow);
	glPushMatrix();
		glTranslated(0, 125,0);
			glutSolidSphere(12.5,50,50);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, resetemission);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/******************************************************
 *                     draworb                  *
 ********************************************************/
 /**Purpose: adds orbs to the scene that are 
  * off during the day and emit at night*/
void draworb()
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	float orbcolor[] = {r_red, r_green, r_blue, 1.0};
	glColor4fv(orbcolor);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	if(night) glMaterialfv(GL_FRONT, GL_EMISSION, orbcolor);
	glutSolidSphere(5, 32,32);
	glMaterialfv(GL_FRONT, GL_EMISSION, resetemission);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

/******************************************************
 *                     drawsurroundings                    *
 ********************************************************/
 /**Purpose: Displays the typical environment surrounding 
  * the ferris wheel*/
void drawsurroundings()
{
	//trees
	glPushMatrix();
		glTranslated(470,0,-170);
		glScaled(1.5,2.5,1);
			drawtree();
	glPopMatrix();
	glPushMatrix();
		glTranslated(500,0,100);
		glScaled(1.5,2.5,1);
			drawtree();
	glPopMatrix();
	glPushMatrix();
	glTranslated(475, 0,200);
		glScaled(1.5,2.5,1);
			drawtree();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-70, 0,-50);
		glScaled(1.5,2.5,1);
			drawtree();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-50, 0,-170);
		glScaled(1.5,2.5,1);
			drawtree();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-10, 0,200);
		glScaled(1.5,2.5,1);
			drawtree();
	glPopMatrix();
	
	//lamps
	glPushMatrix();
		glTranslated(425,0,0);
			drawlamp();
	glPopMatrix();
		glPushMatrix();
		glTranslated(25,0,50);
			drawlamp();
	glPopMatrix();
	glPushMatrix();
		glTranslated(500,0,-150);
			drawlamp();
	glPopMatrix();
	glPushMatrix();
		glTranslated(0,0,225);
			drawlamp();
	glPopMatrix();
	
	//games
	glPushMatrix();
		glTranslated(75,0,125);
		glScaled(0.75,0.75,1);
			drawminigame();
	glPopMatrix();
	glPushMatrix();
		glTranslated(400,0,150);
		glRotatef(180,0,1,0);
		glScaled(0.75,0.75,1);
			drawminigame();
	glPopMatrix();
	glPushMatrix();
		glTranslated(75,0,-150);
		glScaled(0.75,0.75,1);
			drawminigame();
	glPopMatrix();
	
	//orbs
	for(double numorbs = 0, inc = 0; numorbs < 5; numorbs++)
	{
		glPushMatrix();
			glTranslated(150,0,200-inc);
				draworb();
		glPopMatrix();
		inc += 100;
	}
	for(double numorbs = 0, inc = 0; numorbs < 5; numorbs++)
	{
		glPushMatrix();
			glTranslated(350,0,200-inc);
				draworb();
		glPopMatrix();
		inc += 100;
	}
	
}

/******************************************************
 *                     drawScene                  *
 ********************************************************/
 /**Purpose: draws the ground, surroundings and the ride*/
void drawScene()
{
	glEnable(GL_TEXTURE_2D);
	glColor4f(0,0.25,0,1.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial (GL_FRONT, GL_SPECULAR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
		glScaled(250,1,500);
		glBindTexture(GL_TEXTURE_2D, texobj[0]);
			drawCube();
			
				glTranslated(1, 0,0);
					glColor4f(0.5,0.5,0.5,1.0);
					glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
					glColorMaterial (GL_FRONT, GL_SPECULAR);
					glBindTexture(GL_TEXTURE_2D, texobj[1]);
						drawCube();
					
					glTranslated(1,0,0);
					glColor4f(0,0.25,0, 1.0);
					glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
					glColorMaterial (GL_FRONT, GL_SPECULAR);
					glBindTexture(GL_TEXTURE_2D, texobj[0]);
						drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	//ride & surroundings
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glTranslated(250, 175, -150);
			draw_ride();
	glPopMatrix();
	
	drawsurroundings();
	
}

/******************************************************
 *                     spinWheel                     *
 ********************************************************/
 /**Purpose: main code for spinning, speeding up, and slowing down
  * the ferris wheel.*/
void spinWheel(){
	rwheel += rspeed;
	if((fmod(rwheel, 360.0))==0.0 && revs < 5)//speedup
	{
		revs++;
		rspeed += 2;
	}
	if(revs >= 5)//gradually slow down till stopped
	{
		rspeed -= 0.1;
		if (rspeed < 0)
		{
			rspeed = 0;
			startride = false;
			revs = 0;
			rwheel = 0;
		}
	}
}

/******************************************************
 *                     controlspeed                    *
 ********************************************************/
 /**Purpose: the main variable in controlling the speed
  * of the ferris wheel is updated every 100 milliseconds
  * while the ride has started*/
void controlspeed()
{
	if(startride){spinWheel();}
	glutTimerFunc(100, controlspeed, 0);
	glutPostRedisplay();
}

/******************************************************
 *                     idle                    *
 ********************************************************/
 /**Purpose: used for changing the color of the orbs at night*/
void idle()
{
	Time = clock();
	if (Time > ResetTime) {
    ResetTime = ResetTime + interval * CLOCKS_PER_SEC;
	if(night)
		{
			r_red = (float)rand()/(float)RAND_MAX;
			r_green = (float)rand()/(float)RAND_MAX;
			r_blue = (float)rand()/(float)RAND_MAX;
		}
	else{
			r_red = 0.25;
			r_green = 0.0;
			r_blue = 0.10;
	}
	}
	
}

/******************************************************
 *                     scenefunc                 *
 ********************************************************/
 /**Purpose: renders a day(1) or night(2) scene based on user choice*/
void scene_func(int subchoice)
{
	switch(subchoice)
	{
		case 1:
			night = false;
			day = true;
			break;
		case 2:
			night = true;
			day = false;
			break;
	}
	glutPostRedisplay();
}

/******************************************************
 *                     lightfunc                  *
 ********************************************************/
 /**Purpose: toggles lamps and spotlight*/
void lightfunc(int l_choice)
{
	switch(l_choice)
	{
		case 4:
			lamps=!lamps;
			break;
		case 5:
			spot=!spot;
			break;
	}
	glutPostRedisplay();
}
/******************************************************
 *                     menfunc                    *
 ********************************************************/
 /**Purpose: controls starting ride, stopping ride, and 
  * exiting program all together */
void menfunc(int choice) {
	switch (choice) {
	case 1:
		startride = true;
		break;
	case 2:
		startride = false;
		break;
	case 3:
		exit(0);
	}
	glutPostRedisplay();
}

/******************************************************
 *                     keyboard                  *
 ********************************************************/
 /**Purpose: controls camera and spotlight rotation*/
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'W':
		yeye += 10.0;
		yat +=10.0;
		break;
	case 's':
	case 'S':
		yeye -= 10.0;
		yat-=10.0;
		break;
	case 'a':
	case 'A':
		xeye -= 10.0;
		xat -=10.0;
		break;
	case 'd':
	case 'D':
		xeye += 10.0;
		xat +=10.0;
		break;
	case 'z':
	case 'Z':
		zeye -= 10.0;
		zat -=10.0;
		break;
	case 'c':
	case 'C':
		zeye += 10.0;
		zat +=10.0;
		break;
	case 'i':
	case 'I':
		yat += 25.0;
		break;
	case 'k':
	case 'K':
		yat -= 25.0;
		break;
	case 'j':
	case 'J':
		xat -= 25.0;
		break;
	case 'l':
	case 'L':
		xat += 25.0;
		break;
	case 'n':
	case 'N':
		zat -= 25.0;
		break;
	case 'm':
	case 'M':
		zat += 25.0;
		break;
	//spotlight rotation
	case '.':
	case '>':
		lr+=5.0;
		break;
	case ',':
	case '<':
		lr-=5.0;
		break;
	}
	glutPostRedisplay();
}

/********************************************************
 *                      movespot                   	*
 ********************************************************/
 /**Purpose: A special keyboard function that controls the spotlight direction*/
void movespot(int mkey)
{
	if(spot){
		switch(mkey)
		{
			case GLUT_KEY_LEFT:
				lx -= 10;
				break;
			case GLUT_KEY_RIGHT:
				lx += 10;
				break;
			case GLUT_KEY_UP:
				ly += 10;
				break;
			case GLUT_KEY_DOWN:
				ly -= 10;
				break;
			case GLUT_KEY_HOME:
				lz += 10;
				break;
			case GLUT_KEY_END:
				lz -= 10;
				break;
		}
	}
}

