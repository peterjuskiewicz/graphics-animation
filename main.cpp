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


#define RAINSIZE 500

//this defines a constant for the array size
#define SPRAYSIZE 500

int scene = 1;


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




int winWidth = 1000, winHeight = 1000;
int counter = 0;










float waveAngle = 0.0; float waveInc = 5.0;
float rotAngle = 60;
bool goingClockwise = false;


time_t t;




int frame=0;



// the properties of a spray particle are defined in a struct
struct sprayParticle {
    float x = 0; // current position  x
    float y = 0; // current position  y
    float startx = 0; // birth position  x
    float starty = 0; // birth position y
    int startTime; // a birthtime in frames when it will be born
    int startRange = 10000; // the maximum time at which a birth can happen
    bool started = false; // tracks whether the particle has benn born or not
    float speed = 0.1;
    float radius;
    float startxd = 0; // starting direction vector x value
    float startyd = 0; // startingdirection vestor y value
    float xd = 0;  //  current direction vector x value
    float yd = 0;  // current direction vector x value
    float alpha = 1.0; // transparency
};



class wcPt2D{
public:
    float x, y;
};

// Rain functions

struct drop {
    float x = 400;
    float y = 400;
    float inc = 0.01;
    //float r = 1; float g = 1;  float b = 1;
    float radius = 5;
    float scale = 1.0;
    float rotationAngle = 0;
    float rotationInc = 1;
    float r, g, b, a;
};


drop rain[RAINSIZE];

sprayParticle spray[SPRAYSIZE];

void initRain()
{

    //    srand((unsigned)time(&t));
    for (int i = 0; i < RAINSIZE; i++) {
        rain[i].x = rand() % winWidth - 20;
        rain[i].y = rand() % winHeight;
        rain[i].inc = 0.05 + (float)(rand() % 100) / 10.0;
        rain[i].r = (float)(rand() % 1000) / 1000.0;
        rain[i].g = (float)(rand() % 1000) / 1000.0;
        rain[i].b = (float)(rand() % 1000) / 1000.0;
        rain[i].a = 0.5;
        //rain[i].radius = (float)(rand() % 15);
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
    //glColor3f(rain[i].r, rain[i].g, rain[i].b);

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

float elapsedTime = 0, base_time = 0, fps = 0, frames;

void calcFPS()
{
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    if ((elapsedTime - base_time) > 1000.0)
    {
        fps = frames*1000.0 / (elapsedTime - base_time);
        printf("fps: %f", fps);
        base_time = elapsedTime;
        frames = 0;
    }
    frames++;
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
    glVertex2i(x + humanX, y - 150 + humanY);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2i(x - 10 + humanX, y - 150 + humanY);
    glVertex2i(x + 10 + humanX, y - 150 + humanY);
    glVertex2i(x + 10 + humanX, y - 210 + humanY);
    glVertex2i(x - 10 + humanX, y - 210 + humanY);
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

//            glTranslatef(-800 - humanX, -500 - humanY, 0.0);

            glPopMatrix();



    if(rotateDirection == 1) {
        fRotate1--;
        if(fRotate1 < -130.0){
            rotateDirection = 0;
        }
    }

    if(rotateDirection == 0) {
        fRotate1 = fRotate1 + 2;
        if(fRotate1 > -90) {
            rotateDirection = 1;
        }
    }

    if(rotateDirection == -1) {
        fRotate1 = fRotate1 + 1;

        if(fRotate1 > 0.0){
            rotateDirection = -2;
        }
    }


}


void drawWindow() {

    glColor3f(1.0, 0.0, 1.0);

    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 0);
    glVertex2i(0, 700);
    glVertex2i(600, 700);
    glVertex2i(600, 0);
    glVertex2i(0, 0);
    glEnd();

}











wcPt2D tweenPoly[20];
float proportion =0.0;





float  fRotate    = 0.0;       // Speed Of The Rotation
float fScale     = 0.0;     // Speed Of The Scaling






int mode;

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



// Fish var and functions


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

        //            glTranslatef(-800 - humanX, -500 - humanY, 0.0);

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
    calcFPS();
    glFlush(); // force all drawing to finish
    counter++;

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
//
//        glPushMatrix();
//        glTranslatef(fTranslate,0.0f,0.0);
//        drawFish();;
//        glPopMatrix();

        move_fish();



        drawHuman();
        drawHand(800, 500, humanX, humanY);
        drawLamp();

//        glPushMatrix();
//        //glTranslatef(75, 75,-5.0f);
//        glTranslatef(100,200,0.0);
//        glRotatef(fRotate, 1,0,0);    // rotate around Z axis and the centre of the project.
//        drawFish();


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
         renderSpacedBitmapString(50,400,GLUT_BITMAP_HELVETICA_18,"1st Scene");
    }

    fprintf(stdout,"Right number= %i\n", right);




//    if(right == 1) fTranslate += 2.0;
//    if(left == 1) fTranslate -= 2.0;
//    if(fTranslate == 200.0f) {left = 1; right = 0;}
//    if(fTranslate == 0.0f) { left = 0; right = 1;}




    fRotate    += 5.0f;       // Speed Of The Rotation
    fScale     += 0.005f;





    if(fScale < 0.5f)     fScale     = 1.0f;   // Reset Scaling to 1.0f
    //if(fRotate>45.0f) fRotate=0.0f;

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
    glClearColor (1.0, 1.0, 1.0, 1.0);

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



