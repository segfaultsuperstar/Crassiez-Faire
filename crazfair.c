/********************************************************
					*
 *				 Program that creates an		 		*
 * 				interactive town fair					*
 *                                                      *
 ********************************************************/

#include <GL/glut.h>
#include <stdio.h>
#include "crz_fr.h"
#include "SOIL.h"
static float ppw = 500, pph = 350; 

int main(int argc, char** argv) {
	printf("**DYNAMIC CAMERA** Controls:\n");
	printf("w: move up\ns: move down\na:move left\nd:move right\n");
	printf("z: move back\nc: move foward\n");
	printf("i: look up\nk: look down\nj:look left\nl:look right\n");
	printf("n:turn camera further left AFTER looking left\nm: turn camera further right AFTER looking right\n");
	printf("**Arrow keys control cardinal direction of spotlight**\n");
	printf("HOME key moves spotlight back, END forward\n");
	printf(" < or , rotates spotlight down, > or . rotates it up\n");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(ppw, pph);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Amusement Park Ride");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(movespot);
	int scene_menu = glutCreateMenu(scene_func);
	glutAddMenuEntry("Day", 1);
	glutAddMenuEntry("Night", 2);

	int lightmenu = glutCreateMenu(lightfunc);
	glutAddMenuEntry("Lamps", 4);
	glutAddMenuEntry("Spotlight", 5);
	
	glutCreateMenu (menfunc);
	glutAddMenuEntry ("Start Animation", 1);
	glutAddMenuEntry ("Stop Animation", 2);
	glutAddSubMenu ("Time of Day", scene_menu);
	glutAddSubMenu ("Light Toggles", lightmenu);
	glutAddMenuEntry ("Exit", 3);
	glutAttachMenu (GLUT_RIGHT_BUTTON);
	

	glutTimerFunc(100, controlspeed,0);
	glutMainLoop();
	return 0;
}

