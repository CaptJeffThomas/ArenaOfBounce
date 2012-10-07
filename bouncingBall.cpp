/* 
 * Author: Jeff Thomas
 * CMPUT 370: Assn2
 *  The Arena of Bounce: A Neo-Reconstructionist Dialogue On Wittgenstein's Tractatus Logico-Philosophicus
 */

#include <cstdlib>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <string.h>

void drawBounce();
void drawGrid();
void RenderScene();
void SetupRC();
void ChangeSize(GLsizei w, GLsizei h);
     
const int WINDOW_SIZE = 800; // default size of window
const int WORLD_SIZE = 1000; // the initial and minimum size of our world
const int GRID_SIZE = 50; // the initial and minimum size of our grid lines
const int NUM_BALLS = 10;
const int MINIMUM_RADIUS = 10; //smallest radii used to define ball
const int MAXIMUM_RADIUS = 30; //largest radii used to define ball
const int MINIMUM_SPEED = 1;  //smallest velocity achievable by ball
const int MAXIMUM_SPEED = 10; //largest velocity achievable by ball


typedef struct
{
    GLfloat x, y, velX, velY, radius, color1, color2, color3; 
} BBall;

//creates the properties of our ball objects and sets up window color
void SetupRC(){
    /* Set the background color */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
      /* Clear our model view Matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //create the ball objects
   
}

/*
 * Draw reference grid.
 */
   void drawGrid()
{
    GLfloat shade = 0.2f;
    GLfloat xmax = WORLD_SIZE, ymax = WORLD_SIZE, x, y;

    /* Draw the grid */
    glColor3f(shade, shade, shade);
    for (x=0; x<=xmax; x+=GRID_SIZE) {
    	glBegin(GL_LINES);
            glVertex2f(x, -ymax);
            glVertex2f(x, ymax);
        glEnd();
    }
    for (x=-GRID_SIZE; x>=-xmax; x-=GRID_SIZE) {
        glBegin(GL_LINES);
            glVertex2f(x, -ymax);
            glVertex2f(x, ymax);
	glEnd();
    }
    for (y=0; y<=ymax; y+=GRID_SIZE) {
	glBegin(GL_LINES);
            glVertex2f(-xmax, y);
            glVertex2f(xmax, y);
	glEnd();
    }
    for (y=-GRID_SIZE; y>=-ymax; y-=GRID_SIZE) {
	glBegin(GL_LINES);
            glVertex2f(-xmax, y);
            glVertex2f(xmax, y);
	glEnd();
    }
}

// handles the motion of our balls.
void drawBounce(){
    
}

// our function that is called constantly by glutMain to draw our scene
void RenderScene()
{
      /* Clear the background */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw the supporting entities */
    drawGrid();

    /* Draw the object(s) */
    drawBounce();
    glFlush();
    
    /* Throw everything the above methods draw onto the screen */
    glutSwapBuffers();
}

/*
 * Resizes the windows
 * scales ball and object size.  prevents distortion.
 */
void ChangeSize(GLsizei w, GLsizei h)
{
     GLfloat aspectRatio;
    GLfloat factor = 1.0; //scales the world to a 1-1 relation with the window
    GLfloat dimension = WORLD_SIZE * factor;

    
    /* Prevent a divide by zero, when window is too short you cant make a window of zero width) */
    if (h == 0){
		h = 1;
    }
    
    /* Set the viewport to be the entire window */
    glViewport(0, 0, w, h);

    /* Reset coordinate system */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Calculate the aspect ratio of the window */
    aspectRatio = (GLfloat)w / (GLfloat)h;

    if (w <= h){
		glOrtho(-dimension, dimension,
			    -dimension/aspectRatio, dimension/aspectRatio, 1.0, -1.0);
    }
    else{
		glOrtho(-dimension*aspectRatio, dimension*aspectRatio,
				-dimension, dimension, 1.0, -1.0);
    }
    /* Reset the matrix stack */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// sets up our window and begins GLUT main
int main (int argc, char **argv)
{
   
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("Arena of Bounce");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    SetupRC();
    
    glutMainLoop();

    return EXIT_SUCCESS;
}



