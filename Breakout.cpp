////////////////////////////////////////////////////////////////      
// MainDriverClass.cpp (Probably the only file)
//
// This program simulates the old arcade game "Breakout."
//
// Interaction:
// Click to begin the game. Move the mouse to move the paddle to keep the ball from falling.
//
// Eric Palace
////////////////////////////////////////////////////////////////   
#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;
#define PI 3.14159265358979324
// Globals.
static GLsizei height;
static GLsizei width;
static int isAnimate = 0; // Animated?
static int Lost=0; //Has the player lost?
static int Won=0; //Has the player won?
static int animationPeriod = 2.5; // Time interval between frames.
//coordinates and translation of the paddle
float paddleTransBy=0;
float translationPaddle=0;
//translating the ball accordingly
float xTranslate=0;
float yTranslate=0;
//x and y speed of the ball
float xspeed=0;
float yspeed=0;
//used for the rotation of the ball
float xroll=1;
float yroll=1;
float alpha=0;
float mousex=0;
float speedScalar=1;
float totalmove=1;
//The hit detectors/draw cues for the bricks.
//First Row
int hit0=0;
int hit1=0;
int hit2=0;
int hit3=0;
int hit4=0;
int hit5=0;
//Second Row
int hit6=0;
int hit7=0;
int hit9=0;
int hit8=0;
int hit10=0;
int hit11=0;
//Third Row
int hit12=0;
int hit13=0;
int hit14=0;
int hit15=0;
int hit16=0;
int hit17=0;
//Fourth Row
int hit18=0;
int hit19=0;
int hit20=0;
int hit21=0;
int hit22=0;
int hit23=0;

bool clicked=false;
double rotationMatrix[16];


// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //Begin Writing Routine
   if(Lost)
   {
   glPushMatrix();
   glTranslatef(-5.0, 0.0, 0.0);
   glScalef(0.025, 0.025, 0.025);
   writeStrokeString(GLUT_STROKE_ROMAN, "You Lose");
   glPopMatrix();
   glFlush();
   }
   else if(Won)
   {
	   glPushMatrix();
	   glTranslatef(-5.0, 0.0, 0.0);
	   glScalef(0.025, 0.025, 0.025);
	   writeStrokeString(GLUT_STROKE_ROMAN, "You Win!");
	   glPopMatrix();
	   glFlush();
   }
   //End Writing Routine
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -25.0);
   glColor3f(0.0,1.0,0.0);
   //Bricks (order doesn't necesarily make sense)
   glPushMatrix();
   //First Row
   if(hit0==0)
	   glRectf(3,11,8,9.5);
   if(hit1==0)
	   glRectf(-5,11,0,9.5);
   if(hit2==0)
	   glRectf(11,11,16,9.5);
   if(hit3==0)
	   glRectf(19,11,24,9.5);
   if(hit4==0)
	   glRectf(-13,11,-8,9.5);
   if(hit5==0)
	   glRectf(-21,11,-16,9.5);
   glColor3f(0.0,0.0,1.0);
   //Second Row
   if(hit6==0)
	   glRectf(3,14.5,8,13);
   if(hit7==0)
	   glRectf(-5,14.5,0,13);
   if(hit8==0)
	   glRectf(11,14.5,16,13);
   if(hit9==0)
	   glRectf(19,14.5,24,13);
   if(hit10==0)
	   glRectf(-13,14.5,-8,13);
   if(hit11==0)
	   glRectf(-21,14.5,-16,13);
   //Third Row
   if(hit12==0)
	   glRectf(3,18,8,16.5);
   if(hit13==0)
	   glRectf(-5,18,0,16.5);
   if(hit14==0)
	   glRectf(11,18,16,16.5);
   if(hit15==0)
	   glRectf(19,18,24,16.5);
   if(hit16==0)
	   glRectf(-13,18,-8,16.5);
   if(hit17==0)
	   glRectf(-21,18,-16,16.5);
   glColor3f(1.0,0.0,0.0);
   //Fourth row
   if(hit18==0)
	   glRectf(3,21.5,8,20);
   if(hit19==0)
	   glRectf(-5,21.5,0,20);
   if(hit20==0)
	   glRectf(11,21.5,16,20);
   if(hit21==0)
	   glRectf(19,21.5,24,20);
   if(hit22==0)
	   glRectf(-13,21.5,-8,20);
   if(hit23==0)
	   glRectf(-21,21.5,-16,20);
   glPopMatrix();
   //Paddle
   glColor3f(0.0,1.0,0.0);
   glPushMatrix();
   glTranslatef(translationPaddle,0,0);
   glRectf(-5,-20,5,-20.5);
   glPopMatrix();
   //Ball.
   glPushMatrix();
   glTranslatef(xTranslate, yTranslate, 0.0);
   glMultMatrixd(rotationMatrix);
   glColor3f(0.0, 0.0, 1.0);
   glutWireSphere(1.0, 10, 10);
   glPopMatrix();
   // End revolving ball.
   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
	//Binding the paddle to the mouse
	if(mousex<paddleTransBy)
	{
		translationPaddle-=.1;
		paddleTransBy--;
	}
	if(mousex>paddleTransBy)
	{
		translationPaddle+=.1;
		paddleTransBy++;
	}
	
   if (isAnimate) 
   {
	   //Keeping the ball inside the screen.
	   if(xTranslate>23.5 || xTranslate<-23.5)
		   xspeed*=-1;
	   if(yTranslate>23.5)
		   yspeed*=-1;
	   //If the ball hits the bottom, the player loses.
	   if(yTranslate<-23.5)
	   {
		   isAnimate=0;
		   Lost=1;
	   }
	   //If the player hits every brick, they have won.
	   if(hit0&&hit1&&hit2&&hit3&&hit4&&hit5&&hit6&&hit7&&hit8&&hit9&&hit10&&hit11&&
		   hit12&&hit13&&hit14&&hit15&&hit16&&hit17&&hit18&&hit19&&hit20&&hit21&&hit22&&hit23)
	   {
		   isAnimate=0;
		   Won=1;
	   }

	   //Bouncing off of the paddle
	   if(yTranslate<-19 && yTranslate>-21.5 && xTranslate>(translationPaddle-6) && xTranslate<(translationPaddle+6))
		   yspeed*=-1;
	   //Colliding with the bricks
	   //****************************************************************************
	   //First Row of Bricks
	   //****************************************************************************
	   if(yTranslate>8)
	   {
		   //First Brick--First Row
		   if(hit0==0)
		   {
			   if(yTranslate<12 && yTranslate>8.5 && xTranslate>2.25 && xTranslate<8.75)
			   {
				   yspeed*=-1;
				   hit0=1;
			   }
		   }
		   if(hit0==0)
		   {
			   if(xTranslate<9 && xTranslate>2 && yTranslate<11.75 && yTranslate>8.75)
			   {
				   xspeed*=-1;
				   hit0=1;
			   }
		   }
		   //Second Brick--First Row
		   if(hit1==0)
		   {
			   if(yTranslate<12 && yTranslate>8.5 && xTranslate>-5.75 && xTranslate<0.75)
			   {
				   yspeed*=-1;
				   hit1=1;
			   }
		   }
		   if(hit1==0)
		   {
			   if(xTranslate<1 && xTranslate>-6 && yTranslate<11.75 && yTranslate>8.75)
			   {
				   xspeed*=-1;
				   hit1=1;
			   }
		   }
		   //Third Brick--First Row
		   if(hit2==0)
		   {
			   if(yTranslate<12 && yTranslate>8.5 && xTranslate>10.25 && xTranslate<15.75)
			   {
				   yspeed*=-1;
				   hit2=1;
			   }
		   }
		   if(hit2==0)
		   {
			   if(xTranslate<17 && xTranslate>10 && yTranslate<11.75 && yTranslate>8.75)
			   {
				   xspeed*=-1;
				   hit2=1;
			   }
		   }
		   //Fourth Brick--First Row
		   if(hit3==0)
		   {
			   if(yTranslate<12 && yTranslate>8.5 && xTranslate>18.25 && xTranslate<23.75)
			   {
				   yspeed*=-1;
				   hit3=1;
			   }
		   }
		   if(hit3==0)
		   {
			   if(xTranslate<25 && xTranslate>18 && yTranslate<11.5 && yTranslate>9)
			   {
				   xspeed*=-1;
				   hit3=1;
			   }
		   }
		   //Fifth Brick--First Row
		   if(hit4==0)
		   {
			   if(yTranslate<12 && yTranslate>8.5 && xTranslate>-12.75 && xTranslate<-8.25)
			   {
				   yspeed*=-1;
				   hit4=1;
			   }
		   }
		   if(hit4==0)
		   {
			   if(xTranslate<-7 && xTranslate>-14 && yTranslate<11.75 && yTranslate>8.75)
			   {
				   xspeed*=-1;
				   hit4=1;
			   }
		   }
		   //Sixth Brick--First Row
		   if(hit5==0)
		   {
			   if(yTranslate<12 && yTranslate>8.5 && xTranslate>-20.75 && xTranslate<-16.25)
			   {
				   yspeed*=-1;
				   hit5=1;
			   }
		   }
		   if(hit5==0)
		   {
			   if(xTranslate<-15 && xTranslate>-22 && yTranslate<11.75 && yTranslate>8.75)
			   {
				   xspeed*=-1;
				   hit5=1;
			   }
		   }
	   }
	   //**********************************************************************
	   //Second Row of Bricks
	   //**********************************************************************
	   if(yTranslate>11)
	   {
		   //First Brick--Second Row
		   if(hit6==0)
		   {
			   if(yTranslate<15.5 && yTranslate>12.25 && xTranslate>2.25 && xTranslate<8.75)
			   {
				   yspeed*=-1;
				   hit6=1;
			   }
		   }
		   if(hit6==0)
		   {
			   if(xTranslate<9 && xTranslate>2 && yTranslate<15.25 && yTranslate>12.25)
			   {
				   xspeed*=-1;
				   hit6=1;
			   }
		   }
		   //Second Brick--Second Row
		   if(hit7==0)
		   {
			   if(yTranslate<15.5 && yTranslate>12.25 && xTranslate>-5.75 && xTranslate<0.75)
			   {
				   yspeed*=-1;
				   hit7=1;
			   }
		   }
		   if(hit7==0)
		   {
			   if(xTranslate<1 && xTranslate>-6 && yTranslate<15.25 && yTranslate>12.25)
			   {
				   xspeed*=-1;
				   hit7=1;
			   }
		   }
		   //Third Brick--Second Row
		   if(hit8==0)
		   {
			   if(yTranslate<15.5 && yTranslate>12.25 && xTranslate>10.25 && xTranslate<15.75)
			   {
				   yspeed*=-1;
				   hit8=1;
			   }
		   }
		   if(hit8==0)
		   {
			   if(xTranslate<17 && xTranslate>10 && yTranslate<15.25 && yTranslate>12.25)
			   {
				   xspeed*=-1;
				   hit8=1;
			   }
		   }
		   //Fourth Brick--Second Row
		   if(hit9==0)
		   {
			   if(yTranslate<15.5 && yTranslate>12.25 && xTranslate>18.25 && xTranslate<23.75)
			   {
				   yspeed*=-1;
				   hit9=1;
			   }
		   }
		   if(hit9==0)
		   {
			   if(xTranslate<25 && xTranslate>18 && yTranslate<15.25 && yTranslate>12.25)
			   {
				   xspeed*=-1;
				   hit9=1;
			   }
		   }
		   //Fifth Brick--Second Row
		   if(hit10==0)
		   {
			   if(yTranslate<15.5 && yTranslate>12.25 && xTranslate>-12.75 && xTranslate<-8.25)
			   {
				   yspeed*=-1;
				   hit10=1;
			   }
		   }
		   if(hit10==0)
		   {
			   if(xTranslate<-7 && xTranslate>-14 && yTranslate<15.25 && yTranslate>12.25)
			   {
				   xspeed*=-1;
				   hit10=1;
			   }
		   }
		   //Sixth Brick--Second Row
		   if(hit11==0)
		   {
			   if(yTranslate<15.5 && yTranslate>12.25 && xTranslate>-20.75 && xTranslate<-16.25)
			   {
				   yspeed*=-1;
				   hit11=1;
			   }
		   }
		   if(hit11==0)
		   {
			   if(xTranslate<-15 && xTranslate>-22 && yTranslate<15.25 && yTranslate>12.25)
			   {
				   xspeed*=-1;
				   hit11=1;
			   }
		   }
	   }
	   //**********************************************************************
	   //Third Row of Bricks
	   //**********************************************************************
	   if(yTranslate>11)
	   {
		   //First Brick--Third Row
		   if(hit12==0)
		   {
			   if(yTranslate<19 && yTranslate>15.75 && xTranslate>2.25 && xTranslate<8.75)
			   {
				   yspeed*=-1;
				   hit12=1;
			   }
		   }
		   if(hit12==0)
		   {
			   if(xTranslate<9 && xTranslate>2 && yTranslate<18.75 && yTranslate>15.75)
			   {
				   xspeed*=-1;
				   hit12=1;
			   }
		   }
		   //Second Brick--Third Row
		   if(hit13==0)
		   {
			   if(yTranslate<19 && yTranslate>15.75 && xTranslate>-5.75 && xTranslate<0.75)
			   {
				   yspeed*=-1;
				   hit13=1;
			   }
		   }
		   if(hit13==0)
		   {
			   if(xTranslate<1 && xTranslate>-6 && yTranslate<18.75 && yTranslate>15.75)
			   {
				   xspeed*=-1;
				   hit13=1;
			   }
		   }
		   //Third Brick--Third Row
		   if(hit14==0)
		   {
			   if(yTranslate<19 && yTranslate>15.75 && xTranslate>10.25 && xTranslate<15.75)
			   {
				   yspeed*=-1;
				   hit14=1;
			   }
		   }
		   if(hit14==0)
		   {
			   if(xTranslate<17 && xTranslate>10 && yTranslate<18.75 && yTranslate>15.75)
			   {
				   xspeed*=-1;
				   hit14=1;
			   }
		   }
		   //Fourth Brick--Third Row
		   if(hit15==0)
		   {
			   if(yTranslate<19 && yTranslate>15.75 && xTranslate>18.25 && xTranslate<23.75)
			   {
				   yspeed*=-1;
				   hit15=1;
			   }
		   }
		   if(hit15==0)
		   {
			   if(xTranslate<25 && xTranslate>18 && yTranslate<18.75 && yTranslate>15.75)
			   {
				   xspeed*=-1;
				   hit15=1;
			   }
		   }
		   //Fifth Brick--Third Row
		   if(hit16==0)
		   {
			   if(yTranslate<19 && yTranslate>15.75 && xTranslate>-12.75 && xTranslate<-8.25)
			   {
				   yspeed*=-1;
				   hit16=1;
			   }
		   }
		   if(hit16==0)
		   {
			   if(xTranslate<-7 && xTranslate>-14 && yTranslate<18.75 && yTranslate>15.75)
			   {
				   xspeed*=-1;
				   hit16=1;
			   }
		   }
		   //Sixth Brick--Third Row
		   if(hit17==0)
		   {
			   if(yTranslate<19 && yTranslate>15.75 && xTranslate>-20.75 && xTranslate<-16.25)
			   {
				   yspeed*=-1;
				   hit17=1;
			   }
		   }
		   if(hit17==0)
		   {
			   if(xTranslate<-15 && xTranslate>-22 && yTranslate<18.75 && yTranslate>15.75)
			   {
				   xspeed*=-1;
				   hit17=1;
			   }
		   }
	   }
	   //**********************************************************************
	   //Fourth Row of Bricks
	   //**********************************************************************
	   if(yTranslate>11)
	   {
		   //First Brick--Fourth Row
		   if(hit18==0)
		   {
			   if(yTranslate<22.5 && yTranslate>19.25 && xTranslate>2.25 && xTranslate<8.75)
			   {
				   yspeed*=-1;
				   hit18=1;
			   }
		   }
		   if(hit18==0)
		   {
			   if(xTranslate<9 && xTranslate>2 && yTranslate<22.25 && yTranslate>19.25)
			   {
				   xspeed*=-1;
				   hit18=1;
			   }
		   }
		   //Second Brick--Fourth Row
		   if(hit19==0)
		   {
			   if(yTranslate<22.5 && yTranslate>19.25 && xTranslate>-5.75 && xTranslate<0.75)
			   {
				   yspeed*=-1;
				   hit19=1;
			   }
		   }
		   if(hit19==0)
		   {
			   if(xTranslate<1 && xTranslate>-6 && yTranslate<22.25 && yTranslate>19.25)
			   {
				   xspeed*=-1;
				   hit19=1;
			   }
		   }
		   //Third Brick--Fourth Row
		   if(hit20==0)
		   {
			   if(yTranslate<22.5 && yTranslate>19.25 && xTranslate>10.25 && xTranslate<15.75)
			   {
				   yspeed*=-1;
				   hit20=1;
			   }
		   }
		   if(hit20==0)
		   {
			   if(xTranslate<17 && xTranslate>10 && yTranslate<22.25 && yTranslate>19.25)
			   {
				   xspeed*=-1;
				   hit20=1;
			   }
		   }
		   //Fourth Brick--Fourth Row
		   if(hit21==0)
		   {
			   if(yTranslate<22.5 && yTranslate>19.25 && xTranslate>18.25 && xTranslate<23.75)
			   {
				   yspeed*=-1;
				   hit21=1;
			   }
		   }
		   if(hit21==0)
		   {
			   if(xTranslate<25 && xTranslate>18 && yTranslate<22.25 && yTranslate>19.25)
			   {
				   xspeed*=-1;
				   hit21=1;
			   }
		   }
		   //Fifth Brick--Fourth Row
		   if(hit22==0)
		   {
			   if(yTranslate<22.5 && yTranslate>19.25 && xTranslate>-12.75 && xTranslate<-8.25)
			   {
				   yspeed*=-1;
				   hit22=1;
			   }
		   }
		   if(hit22==0)
		   {
			   if(xTranslate<-7 && xTranslate>-14 && yTranslate<22.25 && yTranslate>19.25)
			   {
				   xspeed*=-1;
				   hit22=1;
			   }
		   }
		   //Sixth Brick--Fourth Row
		   if(hit23==0)
		   {
			   if(yTranslate<22.5 && yTranslate>19.25 && xTranslate>-20.75 && xTranslate<-16.25)
			   {
				   yspeed*=-1;
				   hit23=1;
			   }
		   }
		   if(hit23==0)
		   {
			   if(xTranslate<-15 && xTranslate>-22 && yTranslate<22.25 && yTranslate>19.25)
			   {
				   xspeed*=-1;
				   hit23=1;
			   }
		   }
	   }
	   //for rotating the ball
	   glPushMatrix();
	   glLoadIdentity();
	   alpha=((sqrt(pow(xspeed,2)+pow(yspeed,2)))/(PI*4))*360;
	   glRotated(alpha, -yspeed, xspeed, 0);
	   glMultMatrixd(rotationMatrix);
	   glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);
	   glPopMatrix();
	   xTranslate+=xspeed;
	   yTranslate+=yspeed;
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing.
   	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);
	glPopMatrix();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   height=h;
   width=w;//Need this for keeping the paddle bound to the mouse
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN)
	   {
		   animationPeriod += 5;
   }
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}

void mouseReader(int x, int y)
{
	//keeping tabs of where the mouse is to move the paddle
	mousex=x-(width-(width/2));
}
void mouseControl(int button, int state, int x, int y)
{
	
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
	   if(clicked)
	   {
			   //nothing
	   }

	  else
		  {
	  clicked=true;//we don't want to be able to click twice.
	  mousex=x-(width-(width/2));//keeping track of the mouse, or you could hold down the button and not have the paddle move
	  speedScalar=1;//just in case I want the ball to go faster/slower
	  int direction=rand() % 100 + 1;
	  if(direction<50)//randomly decides if the ball starts by going right or left
		  xspeed=.05;//(due to the psuedocode above, this result is much more likely
	  else
		  xspeed=(-.05);
	  xspeed*=speedScalar;
	  yspeed=-.05;
	  yspeed*=speedScalar;
	  isAnimate = 1;
	   }
   }

   glutPostRedisplay();
}
// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout <<"The paddle will automatically follow the mouse.\nTry to break all the blocks without letting the ball touch the bottom.\nClick anywhere to begin."<<endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Play Breakout!"); 
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMouseFunc(mouseControl);
   glutPassiveMotionFunc(mouseReader);
   glutMotionFunc(mouseReader);
   glutTimerFunc(5, animate, 1);
   glutMainLoop(); 

   return 0;  
}