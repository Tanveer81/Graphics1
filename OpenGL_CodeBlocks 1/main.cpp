#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double len = 50;
double rad = 20;

double c = cos(.01);
double s = sin(.01);

struct point
{
	double x,y,z;
};

struct point pos;
struct point u;
struct point r;
struct point l;

double rx;
double ry;
double rz;
double n1;
double n2;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawUpperSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawLowerSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				//glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}



void drawCylinder(double height,double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=height*(i/stacks);
		r=radius;//*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}



void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
		    // u  er sapekkhe r ar l ke ghurano

//		    r.x = r.x*c + l.x*s;
//		    r.y = r.y*c + l.y*s;
//		    r.z = r.z*c + l.z*s;
//
//		    l.x = l.x*c - r.x*s;
//		    l.y = l.y*c - r.y*s;
//		    l.z = l.z*c - r.z*s;

            rx = r.x*c + l.x*s;
            ry = r.y*c + l.y*s;
            rz = r.z*c + l.z*s;
            n1 = sqrt(rx*rx + ry*ry + rz*rz);

		    l.x = l.x*c - rx*s;
		    l.y = l.y*c - ry*s;
		    l.z = l.z*c - rz*s;
            n2 = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

            r.x = rx/n1;
		    r.y = ry/n1;
		    r.z = rz/n1;

            l.x = l.x/n2;
		    l.y = l.y/n2;
		    l.z = l.z/n2;
			break;

		case '2':
//            r.x = r.x*c - l.x*s;
//		    r.y = r.y*c - l.y*s;
//		    r.z = r.z*c - l.z*s;
//
//		    l.x = l.x*c + r.x*s;
//		    l.y = l.y*c + r.y*s;
//		    l.z = l.z*c + r.z*s;

            rx = r.x*c - l.x*s;
            ry = r.y*c - l.y*s;
            rz = r.z*c - l.z*s;
            n1 = sqrt(rx*rx + ry*ry + rz*rz);

		    l.x = l.x*c + rx*s;
		    l.y = l.y*c +  ry*s;
		    l.z = l.z*c + rz*s;
            n2 = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

            r.x = rx/n1;
		    r.y = ry/n1;
		    r.z = rz/n1;

            l.x = l.x/n2;
		    l.y = l.y/n2;
		    l.z = l.z/n2;
			break;

		case '3':
//		    u.x = u.x*c - l.x*s;
//		    u.y = u.y*c - l.y*s;
//		    u.z = u.z*c - l.z*s;
//
//		    l.x = l.x*c + u.x*s;
//		    l.y = l.y*c + u.y*s;
//		    l.z = l.z*c + u.z*s;

            rx = u.x*c - l.x*s;
		    ry = u.y*c - l.y*s;
		    rz = u.z*c - l.z*s;
            n1 = sqrt(rx*rx + ry*ry + rz*rz);

		    l.x = l.x*c + rx*s;
		    l.y = l.y*c + ry*s;
		    l.z = l.z*c + rz*s;
            n2 = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

            u.x = rx/n1;
		    u.y = ry/n1;
		    u.z = rz/n1;

            l.x = l.x/n2;
		    l.y = l.y/n2;
		    l.z = l.z/n2;
			break;

		case '4':
//            u.x = u.x*c + l.x*s;
//		    u.y = u.y*c + l.y*s;
//		    u.z = u.z*c + l.z*s;
//
//		    l.x = l.x*c - u.x*s;
//		    l.y = l.y*c - u.y*s;
//		    l.z = l.z*c - u.z*s;
            rx = u.x*c + l.x*s;
		    ry = u.y*c + l.y*s;
		    rz = u.z*c + l.z*s;
            n1 = sqrt(rx*rx + ry*ry + rz*rz);


		    l.x = l.x*c - rx*s;
		    l.y = l.y*c - ry*s;
		    l.z = l.z*c - rz*s;
            n2 = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);

            u.x = rx/n1;
		    u.y = ry/n1;
		    u.z = rz/n1;

            l.x = l.x/n2;
		    l.y = l.y/n2;
		    l.z = l.z/n2;
			break;

		case '5':
//            u.x = u.x*c + r.x*s;
//		    u.y = u.y*c + r.y*s;
//		    u.z = u.z*c + r.z*s;
//
//		    r.x = r.x*c - u.x*s;
//		    r.y = r.y*c - u.y*s;
//		    r.z = r.z*c - u.z*s;

            rx = u.x*c + r.x*s;
		    ry = u.y*c + r.y*s;
		    rz = u.z*c + r.z*s;
            n1 = sqrt(rx*rx + ry*ry + rz*rz);


		    r.x = r.x*c - rx*s;
		    r.y = r.y*c - ry*s;
		    r.z = r.z*c - rz*s;
            n2 = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);

            u.x = rx/n1;
		    u.y = ry/n1;
		    u.z = rz/n1;

            r.x = r.x/n2;
		    r.y = r.y/n2;
		    r.z = r.z/n2;
			break;

		case '6':
//            u.x = u.x*c - r.x*s;
//		    u.y = u.y*c - r.y*s;
//		    u.z = u.z*c - r.z*s;
//
//		    r.x = r.x*c + u.x*s;
//		    r.y = r.y*c + u.y*s;
//		    r.z = r.z*c + u.z*s;

            rx = u.x*c - r.x*s;
		    ry = u.y*c - r.y*s;
		    rz = u.z*c - r.z*s;
            n1 = sqrt(rx*rx + ry*ry + rz*rz);

		    r.x = r.x*c + rx*s;
		    r.y = r.y*c + ry*s;
		    r.z = r.z*c + rz*s;
            n2 = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);

            u.x = rx/n1;
		    u.y = ry/n1;
		    u.z = rz/n1;

            r.x = r.x/n2;
		    r.y = r.y/n2;
		    r.z = r.z/n2;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		//up arrow key
			pos.x += 2*l.x;
			pos.y += 2*l.y;
			pos.z += 2*l.z;
			break;
		case GLUT_KEY_DOWN:		// down arrow key
			pos.x -= 2*l.x;
			pos.y -= 2*l.y;
			pos.z -= 2*l.z;
			break;

		case GLUT_KEY_RIGHT:
            pos.x += 2*r.x;
			pos.y += 2*r.y;
			pos.z += 2*r.z;
			break;
		case GLUT_KEY_LEFT:
			pos.x -= 2*r.x;
			pos.y -= 2*r.y;
			pos.z -= 2*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x += 2*u.x;
			pos.y += 2*u.y;
			pos.z += 2*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x -= 2*u.x;
			pos.y -= 2*u.y;
			pos.z -= 2*u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
            if(rad<len/2)rad++;
			break;
		case GLUT_KEY_END:
            if(rad>0)rad--;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);

	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	//glScaled(1,1,5);
    //glutWireSphere(50,10,12);
    //glTranslatef(110,0,0);
    //glRotatef(90,0,0,1);
    glColor3f(1,0,0);

    //drawSquare(50);


    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);


    double c = (len-2*rad)/2;
    //drawing the 4 one eighth upper spheres
    glPushMatrix();{
    glTranslated(c,c,c);
	drawUpperSphere(rad,24,20);
	}glPopMatrix();


    glPushMatrix();{
    glTranslated(-c,c,c);
	glRotated(90,0,0,1);
	drawUpperSphere(rad,24,20);
    }glPopMatrix();

    glPushMatrix();{
    glTranslated(-c,-c,c);
	glRotated(180,0,0,1);
	drawUpperSphere(rad,24,20);
	}glPopMatrix();

    glPushMatrix();{
    glTranslated(c,-c,c);
	glRotated(270,0,0,1);
	drawUpperSphere(rad,24,20);
	}glPopMatrix();

	//drawing the 4 one eighth lower spheres

	glPushMatrix();{
    glTranslated(c,c,-c);
	drawLowerSphere(rad,24,20);
	}glPopMatrix();


    glPushMatrix();{
    glTranslated(-c,c,-c);
	glRotated(90,0,0,1);
	drawLowerSphere(rad,24,20);
    }glPopMatrix();

    glPushMatrix();{
    glTranslated(-c,-c,-c);
	glRotated(180,0,0,1);
	drawLowerSphere(rad,24,20);
	}glPopMatrix();

    glPushMatrix();{
    glTranslated(c,-c,-c);
	glRotated(270,0,0,1);
	drawLowerSphere(rad,24,20);
	}glPopMatrix();


	double d = (len-2*rad)/2;
	//drawing 6 squares
    glColor3f(1,1,1);

	glPushMatrix();{
    glTranslated(0,0,len/2-2);
	drawSquare(d);
	}glPopMatrix();

	glPushMatrix();{
    glTranslated(0,0,-len/2-2);
	drawSquare(d);
	}glPopMatrix();

	glPushMatrix();{
    glTranslated(len/2-2,0,0);
    glRotated(90,0,1,0);
	drawSquare(d);
	}glPopMatrix();

	glPushMatrix();{
    glTranslated(-len/2-2,0,0);
    glRotated(90,0,1,0);
	drawSquare(d);
	}glPopMatrix();

	glPushMatrix();{
    glTranslated(0,len/2+2,0);
    glRotated(90,1,0,0);
	drawSquare(d);
	}glPopMatrix();

	glPushMatrix();{
    glTranslated(0,-len/2+2,0);
    glRotated(90,1,0,0);
	drawSquare(d);
	}glPopMatrix();



    //draw cylinder

    glColor3f(0,1,0);

    //middle

    glPushMatrix();{
    glTranslated(d,d,0);
    drawCylinder(d,rad,5,6);

    glTranslated(-2*d,0,0);
    glRotated(90,0,0,1);
    drawCylinder(d,rad,5,6);
	}glPopMatrix();

    glPushMatrix();{
    glTranslated(-d,-d,0);
    glRotated(180,0,0,1);
    drawCylinder(d,rad,5,6);

    glTranslated(-2*d,0,0);
    glRotated(90,0,0,1);
    drawCylinder(d,rad,5,6);
	}glPopMatrix();

    //upper
    glPushMatrix();{
    glTranslated(d,0,d);
    glRotated(90,1,0,0);
    drawCylinder(d,rad,5,6);

    glTranslated(-2*d,0,0);
    glRotated(90,0,0,1);
    drawCylinder(d,rad,5,6);
	}glPopMatrix();

    glPushMatrix();{
    glTranslated(0,d,d);
    glRotated(-90,0,1,0);
    drawCylinder(d,rad,5,6);

    glTranslated(0,-2*d,0);
    glRotated(-90,0,0,1);
    drawCylinder(d,rad,5,6);
	}glPopMatrix();

	//lower

    glPushMatrix();{
    glTranslated(0,d,-d);
    glRotated(90,0,1,0);
    drawCylinder(d,rad,5,6);

    glTranslated(0,-2*d,0);
    glRotated(-90,0,0,1);
    drawCylinder(d,rad,5,6);
	}glPopMatrix();

    glPushMatrix();{
    glTranslated(d,0,-d);
    glRotated(-90,1,0,0);
    drawCylinder(d,rad,5,6);

    glTranslated(-2*d,0,0);
    glRotated(90,0,0,1);
    drawCylinder(d,rad,5,6);
	}glPopMatrix();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	pos.x = 100; pos.y=100; pos.z=0;
    u.x = 0; u.y=0; u.z=1;
    r.x = -1/sqrt(2); r.y=1/sqrt(2); r.z=0;
    l.x = -1/sqrt(2); l.y=-1/sqrt(2); l.z=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
