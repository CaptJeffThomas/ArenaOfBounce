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
#include <time.h>

void drawBounce();
void drawGrid();
void RenderScene();
void SetupRC();
void ChangeSize(GLsizei w, GLsizei h);
void drawBall();
bool collides(int i, int j); 
void bounce(int i, int j);
void wallBounce(int i);
void newPosition(int i);
     
const int WINDOW_SIZE = 800; // default size of window
const int WORLD_SIZE = 1000; // the initial and minimum size of our world
const int GRID_SIZE = 50; // the initial and minimum size of our grid lines
const int NUM_BALLS = 10;
const int MINIMUM_RADIUS = 10; //smallest radii used to define ball
const int MAXIMUM_RADIUS = 30; //largest radii used to define ball
const int MINIMUM_SPEED = 1;  //smallest velocity achievable by ball
const int MAXIMUM_SPEED = 10; //largest velocity achievable by ball
const int ANIM_SPEED = 33;  //the # of milliseconds between new frames  33 ~ 30FPS

typedef struct {
    GLfloat x, y, velX, velY, radius, color1, color2, color3; 
} BBall;

BBall balls[NUM_BALLS];

//creates the properties of our ball objects and clears our canvas for painting
void SetupRC(){
    
    /* Set the background color */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST)
    
      /* Clear our model view Matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    srand(time(NULL)); //seeds our PRG
    
    //create the ball objects and provides their random properties
    for(int i = 0; i < NUM_BALLS ; i++){
        
        balls[i].color1 = (rand()%101)/100;  //creates random color values between 0.00 - 1.00
        balls[i].color2 = (rand()%101)/100;
        balls[i].color3 = (rand()%101)/100;
        balls[i].radius = (rand()%(MAXIMUM_RADIUS - MINIMUM_RADIUS + 1) + MINIMUM_RADIUS); //sets a random radius within interval
        balls[i].velX = (rand()%(MAXIMUM_SPEED - MINIMUM_SPEED + 1) + MINIMUM_SPEED); //sets a random speed within interval
        balls[i].velY = (rand()%(MAXIMUM_SPEED - MINIMUM_SPEED +1) + MINIMUM_SPEED);
        newPosition(i);
        
        //this loop handles ball overlap during creation
        for(int j = 0; j < i; j++){
            //if we have a collision with any ball, randomize a new position and check against limits  
            if(collides(i, j)){              
                newPosition(i);
                j=-1; //after this loop j is reset to 0 because the above new position could clash with previous balls
            }
        } 
    }   
}

// creates a random centre for our Ball between  -WORLD_SIZE and +WORLD_SIZE 
void newPosition(int i){
    int posX = rand()% (2 * WORLD_SIZE + 1) - WORLD_SIZE;
    int posY = rand()% (2 * WORLD_SIZE + 1) - WORLD_SIZE;
    
    // if our randomized position is too close to the right wall
    if (posX > (WORLD_SIZE - balls[i].radius)){
        balls[i].x = posX - (posX - (WORLD_SIZE - balls[i].radius));
    }
    
    //if randomized position is too close to the left wall
    if (posX < (-1 * WORLD_SIZE + balls[i].radius)){
        balls[i].x = posX + (posX + (WORLD_SIZE - balls[i].radius));
    }
    
     //if randomized position is too close to the top wall
    if (posY > (WORLD_SIZE - balls[i].radius)){
        balls[i].y = posY - (posY - (WORLD_SIZE - balls[i].radius));
    }
    
     //if randomized position is too close to the bottom wall
    if (posY < (-1 * WORLD_SIZE + balls[i].radius)){
        balls[i].y = posY + (posY + (WORLD_SIZE - balls[i].radius));
    }
    
}

/*
 * Draw reference grid.
 */
 void drawGrid(){
    GLfloat shade = 0.2f;
    GLfloat ymax = WORLD_SIZE, xmax = WORLD_SIZE, x, y;

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
    
    //first we translate all of our balls by their velocity
    for(int i = 0; i < NUM_BALLS; i++){
        balls[i].x += balls[i].velX;
        balls[i].y += balls[i].velY;
    }
    
    //then we check for collisions and resolve them
    for (int i = 0; i < NUM_BALLS; i++){ 
        for (int j = i + 1; j < NUM_BALLS; j++){  
                if (collides(balls[i], balls[j])){
                        bounce(i, j);
                }
         }
        wallBounce(i);
    } 
    
    //all collisions are settled let's draw our balls
    for (int i = 0; i < NUM_BALLS; i++){
        drawBall(balls[i]);
    }
}

void drawBall(BBall ball){
    
    float angle;
    glColor3f(ball.color1, ball.color2, ball.color3); // draws ball with random color
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 100; i++) { 
        angle = i*2*M_PI/100; 
        glVertex2f(ball.x + (cos(angle) * ball.radius), ball.y + (sin(angle) * ball.radius)); 
    } 
    glEnd();
}

//Returns true if the two balls collide
bool collides(int i, int j){
    float xd = balls[i].x - balls[j].x;
    float yd = balls[i].y - balls[j].y;
    float sqrRadius = (balls[i].radius + balls[j].radius) * (balls[i].radius + balls[j].radius);
    float distSqr = (xd * xd) + (yd * yd);
    
    // if the centres lie closer together than two radii, they have collided
    if (distSqr <= sqrRadius){
        return true;
    }
    return false;
}

// Handles balls bouncing off the margins of our world
void wallBounce(int i){
    //reflecting off left wall
    if((balls[i].x - balls[i].radius) <= -WORLD_SIZE){
        balls[i].velX = -1 * balls[i].velX;
    }
    
    //reflecting off right wall
    if((balls[i].x + balls[i].radius) >= WORLD_SIZE){
        balls[i].velX = -1 * balls[i].velX;
    }
    
    //reflecting off bottom wall
    if((balls[i].y - balls[i].radius) <= -WORLD_SIZE){
        balls[i].velY = -1 * balls[i].velY;
    }
    
    //reflecting off top wall
    if((balls[i].y + balls[i].radius) >= WORLD_SIZE){
        balls[i].velY = -1 * balls[i].velY;
    }
}

void bounce(BBall one, BBall two){
    
}

// our function that is called constantly by glutMain to draw our scene
void RenderScene(){
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

void TimerFunction(int value)
{
	/* Redraw the scene with new coordinates */
	glutPostRedisplay(); 
	glutTimerFunc(ANIM_SPEED,TimerFunction, 1);
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
    glutTimerFunc(ANIM_SPEED, TimerFunction, 1);
    SetupRC();
    
    glutMainLoop();

    return EXIT_SUCCESS;
}



