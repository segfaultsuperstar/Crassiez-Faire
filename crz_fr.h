#ifndef CRZ_FR
#define CRZ_FR

void loadtextures();
void init(void);
void display();
void unitTriangle();
void unitSquare();
void unitCircle(float r);
void drawCube();
void drawwheel();
void drawWheelStructure();
void drawSeat();
void dayscene();
void nightscene();
void togglecycle();
void draw_ride();
void drawScene();
void drawsupport();
void drawtree();
void drawlamp();
void drawsurroundings();
void drawminigame();
void menfunc(int choice);
void controlspeed();
void idle();
void movespot();
void lightfunc(int l_choice);
void scene_func(int subchoice);
void spinWheel();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

#endif