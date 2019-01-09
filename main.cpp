//
//  main.cpp
//  ProjectJuskiewicz
//
//  Created by Peter Juskiewicz on 9 Tevet 5779.
//  Copyright © 5779 Peter Juskiewicz. All rights reserved.
//


#include <stdio.h>
#include <math.h>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

// Global variables

int winWidth = 1000, winHeight = 1000;
int scene = 1;
int counter = 0;


//SPRAY

#define SPRAYSIZE 1500

// Struct used to define spray particle
struct sprayParticle {
    float x = 0; // current position  x
    float y = 0; // current position  y
    float startx = 0; // birth position  x
    float starty = 0; // birth position y
    int startTime; // a birthtime in frames when it will be born
    int startRange = 10000; // the maximum time at which a birth can happen
    bool started = false; // tracks whether the particle has benn born or not
    float speed = 0.8;
    float radius;
    float startxd = 0; // starting direction vector x value
    float startyd = 0; // startingdirection vestor y value
    float xd = 0;  //  current direction vector x value
    float yd = 0;  // current direction vector x value
    float alpha = 1.0; // transparency
    float fr = 0.5+(float)(rand() % 500) / 1000.0;
    float fg = 0.5+(float)(rand() % 500) / 1000.0;
    float fb = 0.5+(float)(rand() % 500) / 1000.0;
};




sprayParticle spray[SPRAYSIZE];

float angle = 360; // the angle of the spray: 0 degrees is to the left,
// 90 degrees straight up, 180 to the right etc
float sprayWidth = 4;// the width of the spray in degrees
float sprayCenterX, sprayCenterY;


// the gravity vector
float gx = 0;
float gy = -0.11025;

// the position of thepartcle ystem emitter, wher the rocket should be drawn
float spraystartx =  250, spraystarty = 500;;

void circle(double radius, double xc, double yc)
{
    int i;
    double angle = 2 * 3.1415 / 20;    // circle is drawn using 20 line.
    double circle_xy[40][2];

    circle_xy[0][0] = radius + xc;
    circle_xy[0][1] = yc;
    //glBegin(GL_LINE_LOOP);
    glBegin(GL_POLYGON);
    for (i = 1; i<20; i++)
    {
        circle_xy[i][0] = radius * cos(i *angle) + xc;
        circle_xy[i][1] = radius * sin(i * angle) + yc;

        glVertex2f(circle_xy[i - 1][0], circle_xy[i - 1][1]);
        glVertex2f(circle_xy[i][0], circle_xy[i][1]);
    }
    glEnd();
}
void circlePolygon(double radius, double xc, double yc)
{
    int i;
    double angle = 2 * 3.1415 / 20;    // circle is drawn using 20 line.
    double circle_xy[40][2];

    circle_xy[0][0] = radius + xc;
    circle_xy[0][1] = yc;
    glBegin(GL_POLYGON);
    for (i = 1; i<20; i++)
    {
        circle_xy[i][0] = radius * cos(i *angle) + xc;
        circle_xy[i][1] = radius * sin(i * angle) + yc;

        glVertex2f(circle_xy[i - 1][0], circle_xy[i - 1][1]);
        glVertex2f(circle_xy[i][0], circle_xy[i][1]);
    }
    glEnd();
}

// we calculate the direction vector of the current particle from the global variable angle and spread
void setDirectionVector(int i)
{
    float minAngle, maxAngle, range, newangle;
    double newAngleInRadians;
    int rangeInt;
    minAngle = angle - (sprayWidth / 2.0); // calc the minimum angle the particle could move along
    maxAngle = angle + (sprayWidth / 2.0); // calc the maximum angle
    range = maxAngle - minAngle;
    rangeInt = (int)(range*100.0);
    newangle = minAngle + ((float)(rand() % rangeInt) / 100.0); // generate a random angle between mi and max angles
    newAngleInRadians = (double)(newangle / 360.0)*(2 * 3.1415); // convert it to radians

    spray[i].xd = (float)cos(newAngleInRadians);// calc the diection vector x value
    spray[i].yd = (float)sin(newAngleInRadians);// calc the diection vector y value

}

void initspray()
{
    for (int i = 0; i < SPRAYSIZE; i++) {
        spray[i].x = spraystartx; // set current start x position
        spray[i].y = spraystarty; // set current start y position
        spray[i].startx = spray[i].x; spray[i].starty = spray[i].y;// set start x and y position
        spray[i].speed = 0.1 + (float)(rand() % 150) / 1000.0;// speed is 0.1 to 0.25
        spray[i].startTime = rand() % spray[i].startRange;// set birth time
        spray[i].radius = (float)(rand() % 5); // random radius
        setDirectionVector(i);// set the current direction vector
        spray[i].startxd = spray[i].xd; spray[i].startyd = spray[i].yd; // set start direction vector to current
    }
}


void drawsprayParticle(int i)
{
    glLineWidth(2);
    if (!spray[i].started) {
        if (counter == spray[i].startTime) {
            spray[i].started = true;
            spray[i].x = spraystartx;

        }
    }
    if (spray[i].started)
    {
        glColor4f(spray[i].fr, spray[i].fg, spray[i].fb, spray[i].alpha);

        circle(spray[i].radius, spray[i].x, spray[i].y);
        // update particile movement based on its speed (speed) and its direction vector
        spray[i].x = spray[i].x + (spray[i].xd*spray[i].speed);
        spray[i].y = spray[i].y + (spray[i].yd*spray[i].speed);
        // add gravity
        // this then  produces a direction vector that is a little longer than 1
        spray[i].yd = spray[i].yd + gy;
        // so the normalise the vector to make length 1
//        normalise(i);
        // reduce transparency
        spray[i].alpha -= 0.01015;

        if(spray[i].y < 200) {
            spray[i].started = false;
        }
    }

}


void drawspray()
{
    // draw each spray particle
    for (int i = 0; i < SPRAYSIZE; i++)
    {
        drawsprayParticle(i);
    }

    // increment spray position
    spraystarty -= 10.2;

    // if the particle
    if (spraystarty > 100) { spraystarty = 0; }

    counter++;
}


// EXPLOSION

#define FIREWORKSIZE 500

float fireworkCenterX, fireworkCenterY;
//variables for firework colour, set once per firework
float fr = 1; float fg = 1;  float fb = 1;

// the properties of a fire particle are defined in a struct
struct fireParticle {
    float x = 0; // position in x
    float y = 0; // position in y
    float inc = 0.1;
    float radius;
    float xd = 0; //direction in x
    float yd = 0; //direction in y
};

time_t t;

fireParticle firework[FIREWORKSIZE];

void setFireworkDirectionVector(int i)
{
    firework[i].xd = (float)((rand() % 1000) + 1) / 1000.0;
    firework[i].yd = sqrt(1.0 - (firework[i].xd*firework[i].xd));
    if (rand() % 1000 < 500) firework[i].xd = -firework[i].xd;
    if (rand() % 1000 < 500) firework[i].yd = -firework[i].yd;
}

void initFirework()
{

    for (int i = 0; i < FIREWORKSIZE; i++) {

        firework[i].x = 250;
        firework[i].y = 240;
        firework[i].inc = 3.01 + (float)(rand() % 150) / 1000.0;
        firework[i].radius = (float)(rand() % 15);
        setFireworkDirectionVector(i);
    }
    fr = (float)(rand() % 1000) / 1000.0;
    fg = (float)(rand() % 1000) / 1000.0;
    fb = (float)(rand() % 1000) / 1000.0;
}
void drawFireworkParticleShape(int i)
{
    circle(firework[i].radius, firework[i].x, firework[i].y);
    glBegin(GL_LINES);
    glVertex2d(firework[i].x, firework[i].y);
    glVertex2d(firework[i].x + firework[i].xd * 1, firework[i].y + firework[i].yd * 1);
    glEnd();
}

void drawFireParticle(int i)
{
    glColor3f(fr, fg, fb);
    glLineWidth(2);

    drawFireworkParticleShape(i);
    // update partcile movement based on its speed (inc) and its direction vector
    firework[i].x = firework[i].x + (firework[i].xd*firework[i].inc);
    firework[i].y = firework[i].y + (firework[i].yd*firework[i].inc);

}

void drawFirework()
{
    // draw each fire particle
    for (int i = 0; i < FIREWORKSIZE; i++)
    {
        drawFireParticle(i);
    }
    //start a firework in a new position every so often randomly
    if (rand() % 2000 < 1) {
        fireworkCenterX = rand() % (int) winWidth;
        fireworkCenterY = rand() % (int) winHeight;
        initFirework();
    }
}


//RAIN

// define size of rain array
#define RAINSIZE 500


float waveAngle = 0.0; float waveInc = 5.0;
float rotAngle = 60;
bool goingClockwise = false;

int frame=0;


// Rain functions

struct drop {
    float x = 400;
    float y = 400;
    float inc = 0.01;
    float radius = 5;
    float scale = 1.0;
    float rotationAngle = 0;
    float rotationInc = 1;
    float r, g, b, a;
};


drop rain[RAINSIZE];


void initRain()
{
    for (int i = 0; i < RAINSIZE; i++) {
        rain[i].x = rand() % winWidth - 20;
        rain[i].y = rand() % winHeight;
        rain[i].inc = 0.05 + (float)(rand() % 100) / 10.0;
        rain[i].r = (float)(rand() % 1000) / 1000.0;
        rain[i].g = (float)(rand() % 1000) / 1000.0;
        rain[i].b = (float)(rand() % 1000) / 1000.0;
        rain[i].a = 0.5;
        rain[i].scale = (float)(rand() % 20000) / 1000.0;
        rain[i].rotationAngle = (float)(rand() % 3000) / 1000.0;
        rain[i].rotationInc = (float)(rand() % 100) / 1000.0;
        if ((rand() % 100) > 50) {
            rain[i].rotationInc = -rain[i].rotationInc;
        }
    }
}

void drawParticleShape(int i)
{

    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
        glVertex2d(rain[i].x, rain[i].y);
        glVertex2d(rain[i].x, rain[i].y+ rain[i].radius*2);
    glEnd();

}

void drawDrop(int i)
{
    glLineWidth(2);

    drawParticleShape(i);
    rain[i].y -= rain[i].inc;
    if (rain[i].y < 0) {
        rain[i].y = winHeight;
    }
}

void drawRain()
{
    for (int i = 0; i < RAINSIZE; i++)
    {
        drawDrop(i);
    }
}

void drawAquarium() {

    glColor3f (0.0, 0.0, 1.0);

    glBegin(GL_LINE_STRIP);
    glVertex2i(25, 50);
    glVertex2i(25, 150);
    glVertex2i(375, 150);
    glVertex2i(375, 50);

    glEnd();

    glColor3f (0.0, 0.0, 1.0);

    glBegin(GL_POLYGON);


    glVertex2i (50, 150);
    glVertex2i (350, 150);
    glVertex2i (350, 350);
    glVertex2i (50, 350);

    glEnd();

}

// colours for the lamp

float coloursArr[10][3] = {
    {0.1, 0, 0},
    {0.2, 0, 0},
    {0.3, 0, 0},
    {0.4, 0, 0},
    {0.5, 0, 0},
    {0.6, 0, 0},
    {0.7, 0, 0},
    {0.8, 0, 0},
    {0.9, 0, 0},
    {1.0, 0, 0}
};

void drawLamp() {

    int v1 = rand() % 10;

    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_LINE_STRIP);
    glVertex2i(225, 1000);
    glVertex2i(225, 900);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2i(175, 800);
    glVertex2i(225, 900);
    glVertex2i(275, 800);
    glEnd();

    glColor3f(coloursArr[v1][0], coloursArr[v1][1], coloursArr[v1][2]);

    glBegin(GL_TRIANGLES);
    glVertex2i(200, 800);
    glVertex2i(250, 800);
    glVertex2i(225, 750);
    glEnd();

}


// Human variables and methods

// human movement

float humanX, humanY;


//hand variables

float fRotate1 = 1;

int rotateDirection = 1;



void drawHand(int x, int y, float humanX, float humanY) {

    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINES);
    glVertex2i(x + humanX, y + humanY);
    glVertex2i(x + humanX, y - 180 + humanY);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2i(x - 10 + humanX, y - 180 + humanY);
    glVertex2i(x + 10 + humanX, y - 180 + humanY);
    glVertex2i(x + 10 + humanX, y - 250 + humanY);
    glVertex2i(x - 10 + humanX, y - 250 + humanY);
    glVertex2i(x - 10 + humanX, y - 110 + humanY);
    glEnd();
}


void drawHuman() {

    //human body

    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex2i(700 + humanX, 50 + humanY);
    glVertex2i(900 + humanX, 50 + humanY);
    glVertex2i(800 + humanX, 600 + humanY);
    glEnd();

    // human head

    glBegin(GL_TRIANGLES);
    glVertex2i(800 + humanX, 600 + humanY);
    glVertex2i(850 + humanX, 700 + humanY);
    glVertex2i(750 + humanX, 700 + humanY);
    glEnd();

    // human eyes

    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_LINE_STRIP);
    glVertex2i(775 + humanX, 670 + humanY);
    glVertex2i(785 + humanX, 680 + humanY);
    glVertex2i(795 + humanX, 670 + humanY);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2i(825 + humanX, 670 + humanY);
    glVertex2i(815 + humanX, 680 + humanY);
    glVertex2i(805 + humanX, 670 + humanY);
    glEnd();
}


void rotateHand() {

    glPushMatrix();
    glTranslatef(800 + humanX, 500 + humanY,0.0);
    glRotatef(fRotate1, 0,0,1);
    drawHand(0, 0, 0, 0);
    glPopMatrix();

    if(rotateDirection == 1) {
        fRotate1 = fRotate1 -5;
        if(fRotate1 < -100.0){
            rotateDirection = 0;
        }
    }

    if(rotateDirection == 0) {
        fRotate1 = fRotate1 + 5;
        if(fRotate1 > -90) {
            rotateDirection = 1;
        }
    }

    if(rotateDirection == -1) {
        fRotate1 = fRotate1 + 5;
        if(fRotate1 > 0.0){
            rotateDirection = -2;
        }
    }
}


void drawWindow() {

    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(15.0);

    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 0);
    glVertex2i(0, 700);
    glVertex2i(600, 700);
    glVertex2i(600, 0);
    glVertex2i(0, 0);
    glEnd();

}


// Morphing

class wcPt2D{
public:
    float x, y;
};

wcPt2D tweenPoly[20];
float proportion =0.0;

void tween(wcPt2D source[20], wcPt2D destination[20], int numPoints, double proportion, wcPt2D tweenPoly[20])
{

    for( int i = 0; i < numPoints; i++)
    {
        // get the source point
        double sourceX = source[i].x;
        double sourceY = source[i].y;

        // get the destination point
        double destinationX = destination[i].x;
        double destinationY = destination[i].y;

        // get the difference between source and destination
        double differenceX = (destinationX - sourceX);
        double differenceY = (destinationY - sourceY);

        // tween point is source position + proportion
        // of distance between source and destination
        double tweenX = sourceX + ( differenceX * proportion );
        double tweenY = sourceY + ( differenceY * proportion );

        // create point with tween co-ordinates in tween array
        tweenPoly[i].x = tweenX;
        tweenPoly[i].y = tweenY;
    }


}



// FISH


// fish and mutated fish arrays

wcPt2D fish[6]={{110,240},{110,250},{130,230},{150,245},{130,260},{110,240}};

wcPt2D fishTest[6]={{-20,-5},{-20,5},{0,-15},{20,0},{0,15},{-20,-5}};


wcPt2D mutatedFish[6] = {{0,-5},{0,5},{0,-15},{0,0},{0,15},{0,-5}};


// rotation

float fishRotate = 0.0;
bool rotateFish = false;


// fish movement

float fTranslate = 0.0;     // Speed Of The  Translation

bool left = false;
bool right = true;

float fTranslateY = 0.0;

int up = 1;
int down = 0;


void drawFish() {

    int i;

    glColor3f (0.0, 1.0, 0.0);

    glBegin(GL_LINE_STRIP);
    for(i=0;i<6;i++)
    {
        glVertex2i (fishTest[i].x,fishTest[i].y);
    }
    glEnd();

}

void move_fish() {

    if(!rotateFish) {

        if(right) {
            fTranslate += 10.0;

            glPushMatrix();
            glTranslatef(fTranslate + 110 ,0.0f + 240 ,0.0);
            drawFish();;
            glPopMatrix();

        }

        if(left) {
            fTranslate -= 10.0;

            glPushMatrix();
            glTranslatef(fTranslate + 110 ,0.0f + 240 ,0.0);
            glRotatef(180, 0,0,1);
            drawFish();;
            glPopMatrix();

        }


        if(fTranslate == 140) {
            rotateFish = true;
            left = true; right = false;
        }

        if(fTranslate == 0) {
            rotateFish = true;
            left = false; right = true;
        }
    }

    if(rotateFish){

        fishRotate = fishRotate + 10;

        glPushMatrix();
        glTranslatef(fTranslate + fish[4].x, 240, 0.0);
        glRotatef(fishRotate, 0,0,1);
        drawFish();
        glPopMatrix();

        if(fishRotate == 180.0 || fishRotate == 360) {

            rotateFish = false;

            if(fishRotate == 360) {
                fishRotate = 0;
            }

        }


    }

}


void morph_Fish()
{
    int i;
    int red = rand()%255;
    int green = rand()%255;
    int blue = rand()%255;
    glColor3f (float(red)/255.0, float(green)/255.0, float(blue)/255.0); //flashing colours

    tween(fishTest,mutatedFish,6,proportion,tweenPoly);

    glBegin(GL_LINE_LOOP);
    for(i=0;i<6;i++)
    {
        glVertex2i (tweenPoly[i].x,tweenPoly[i].y);
    }
    glEnd();

    proportion +=0.01;

    if(proportion >1.0) scene = 4;
}

// draw text

void renderSpacedBitmapString(float x, float y, void *font, char *string) {
    char *c;
    int x1=x;
    for (c=string; *c != '\0'; c++) {
        glRasterPos2f(x1,y);
        glutBitmapCharacter(font, *c);
        x1 = x1 + glutBitmapWidth(font,*c);
    }
}


void display1() {
    glClear (GL_COLOR_BUFFER_BIT);

    glViewport(1000,600,1200,800);
    glColor3f(1.0, 1.0, 1.0); // set colour to white
    drawRain();
    drawWindow();
    glFlush(); // force all drawing to finish

}

void display(void)
{
    /* clear all pixels  */
    glClear (GL_COLOR_BUFFER_BIT);

    glLineWidth(10.0);

    if(scene == 1)    // Scene 1.
    {


        display1();

        glViewport(0,0,1300,1000);

        drawAquarium();
        move_fish();
        drawHuman();
        drawHand(800, 500, humanX, humanY);
        drawLamp();
        glPopMatrix();
        humanX -= 1.0;

        if(humanX == -300) {
            scene = 2;
        }


    }

    if(scene == 2) {

        display1();

        glViewport(0,0,1300,1000);

        drawAquarium();
        drawHuman();
        drawLamp();

        if(fTranslate < 140.0){

             move_fish();

            drawHuman();
            drawHand(800, 500, humanX, humanY);
            drawLamp();

            if(right == 1) fTranslate += 2.0;
            if(left == 1) fTranslate -= 2.0;
            if(fTranslate == 200.0f) {left = 1; right = 0;}
            if(fTranslate == 0.0f) { left = 0; right = 1;}
        } else {

            glPushMatrix();
            glTranslatef(fTranslate + 110, fTranslateY + 240.0, 0.0);
            drawFish();
            glPopMatrix();
            rotateHand();
            drawspray();
            if(up == 1) fTranslateY += 2.0;
            if(down == 1) fTranslateY -= 2.0;
            if(fTranslateY == 70.0f) {down = 1; up = 0;}
            if(fTranslateY == 0.0f) { down = 0; up = 1;}

            frame++;

            if(frame == 400) {
                scene = 3;
                rotateDirection = -1;
            }


        }


    }

    if(scene == 3) {

        display1();

        glViewport(0,0,1300,1000);

        drawAquarium();
        drawHuman();
        drawLamp();

        glPushMatrix();
        glTranslatef(250, 240.0 ,0.0);
        morph_Fish();
        glPopMatrix();

        rotateHand();


    }

    if(scene == 4) {

        display1();

        glViewport(0,0,1300,1000);

        drawAquarium();
        drawHuman();
        drawHand(800, 500, humanX, humanY);
        drawLamp();
        drawFirework();

         renderSpacedBitmapString(600,800,GLUT_BITMAP_HELVETICA_18,"THE BEST FISH FOOD IN THE WORLD");
    }

    fprintf(stdout,"Right number= %i\n", right);

    glutSwapBuffers();
    glFlush ();
}



void Timer( int value )
{
    if( value ) glutPostRedisplay();
    glutTimerFunc(40,Timer,value);
}

void visibility(int state)
{
    switch (state)
    {
        case GLUT_VISIBLE:
            Timer(1);
            break;
        case GLUT_NOT_VISIBLE:
            Timer(0);
            break;
        default:
            break;
    }
}

void init (void)
{
    /* select clearing color     */
    glClearColor (0.9, 0.9, 0.9, 0.9);

    /* initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1300, 0, 1000);


}

/*
 * Declare initial window size, position, and display mode
 * (single buffer and RGBA).  Open window with "hello"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
    srand(1);
    initRain();
    initspray();
    initFirework();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (1300, 1000);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Merry Christmas!");

    init ();
    glutDisplayFunc(display);
    glutVisibilityFunc(visibility);

    glutMainLoop();
    return 0;   /* ANSI C requires main to return int. */
}



