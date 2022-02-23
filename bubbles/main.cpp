
#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<iostream>

using namespace std;




#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double maxlength = 200.0;

double big_radius = 60;
double small_radius = 8;

struct point
{
	double x,y,z;
};

point crc_pos[5];

point velocity[5];

double speed = 0.005;






void makeBorder()
{
    glBegin(GL_LINES);

    glVertex3f( maxlength, maxlength,0);
    glVertex3f( maxlength,0,0);
    glVertex3f( maxlength,0,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,0);
    glVertex3f(0, maxlength,0);
    glVertex3f(0, maxlength,0);
    glVertex3f( maxlength, maxlength,0);

	glEnd();
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 200,0,0);
			glVertex3f(-200,0,0);

			glVertex3f(0,-200,0);
			glVertex3f(0, 200,0);

			glVertex3f(0,0, 200);
			glVertex3f(0,0,-200);
		}
		glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(.6, .6, .6);	//grey
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
		glVertex3f( a, a,4);
		glVertex3f( a,-a,4);
		glVertex3f(-a,-a,4);
		glVertex3f(-a, a,4);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    //glColor3f(0.7,0.7,0.7);
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


void drawSS()
{
    glColor3f(0,1,0);
    makeBorder();

    glColor3f(1,0,0);
    glPushMatrix();
    {
        glTranslatef(maxlength/2,maxlength/2,0);
        drawCircle(big_radius,40);
    }
    glPopMatrix();


    glColor3f(1,0.6,0);

    for(int j=0;j<5;j++){
        glPushMatrix();

        glTranslatef(crc_pos[j].x,crc_pos[j].y,0);
        drawCircle(small_radius,15);
        glPopMatrix();
    }





}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			speed /= 1.1;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			speed *= 1.1;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:

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
	gluLookAt(maxlength/2,maxlength/2,100,	maxlength/2,maxlength/2,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

	//codes for any changes in Models, Camera

	double temp_x,temp_y;
	double temp_distance;

	for(int j=0;j<5;j++){

        if(crc_pos[j].z == 0){
            temp_x = crc_pos[j].x + velocity[j].x * speed;
            temp_y = crc_pos[j].y + velocity[j].y * speed;

            temp_distance = sqrt((maxlength/2 - temp_x)*(maxlength/2 - temp_x) + (maxlength/2 - temp_y)*(maxlength/2 - temp_y));

            if(temp_distance < big_radius-small_radius){
                crc_pos[j].z = 1;
            }

            else{
                if(temp_x >= (maxlength-small_radius) && temp_y < maxlength-small_radius){
                    velocity[j].x = -velocity[j].x;
                }
                else if(temp_y >= (maxlength-small_radius) && temp_x < maxlength-small_radius){
                    velocity[j].y = -velocity[j].y;
                }
                else if(temp_x < small_radius && temp_y >= small_radius){
                    velocity[j].x = -velocity[j].x;
                }
                else if(temp_y < small_radius && temp_x >= small_radius){
                    velocity[j].y = -velocity[j].y;
                }
                crc_pos[j].x += velocity[j].x * speed;
                crc_pos[j].y += velocity[j].y * speed;
            }


        }



	}

	int status[5] = {-1,-1,-1,-1,-1};
	double new_pos_x[5];
	double new_pos_y[5];

	for(int j=0;j<5;j++){
        if(crc_pos[j].z == 1){
            double x_val = crc_pos[j].x + velocity[j].x * speed;
            double y_val = crc_pos[j].y + velocity[j].y * speed;

            double _dist = sqrt((maxlength/2-x_val)*(maxlength/2-x_val) + (maxlength/2-y_val)*(maxlength/2-y_val));
            if(_dist <= big_radius-small_radius){
                //crc_pos[j].x = x_val;
                //crc_pos[j].y = y_val;

                status[j] = 0;
                new_pos_x[j] = x_val;
                new_pos_y[j] = y_val;

            }
            else{
                double n_y = -(x_val-crc_pos[j].x);
                double n_x = y_val-crc_pos[j].y;
                double magnitude = sqrt(n_x * n_x + n_y * n_y);
                double n_x_1 = n_x/magnitude;
                double n_y_1 = n_y/magnitude;

                double val_1 = velocity[j].x * n_x_1 + velocity[j].y * n_y_1;

                n_x_1 = n_x_1 * val_1;
                n_y_1 = n_y_1 * val_1;

                double t1 = velocity[j].x - n_x_1;
                double t2 = velocity[j].y - n_y_1;

                velocity[j].x = velocity[j].x - 2 * t1;
                velocity[j].y = velocity[j].y - 2 * t2;

                crc_pos[j].x = crc_pos[j].x + velocity[j].x * speed;
                crc_pos[j].y = crc_pos[j].y + velocity[j].y * speed;

            }
        }
	}


	for(int j=0;j<4;j++){
        if(status[j] == 0){
            double x_com = new_pos_x[j];
            double y_com = new_pos_y[j];

            int changed = 0;

            for(int k=j+1;k<5;k++){

                if(status[k] == 0){
                    double more_x = new_pos_x[k];
                    double more_y = new_pos_y[k];

                    double diff_x = abs(more_x-x_com);
                    double diff_y = abs(more_y-y_com);

                    if(diff_x <=2 * small_radius  && diff_y <= 2 * small_radius){

                        double tg_x = crc_pos[k].y - crc_pos[j].y;
                        double tg_y = -(crc_pos[k].x - crc_pos[j].x);
                        double mgn = sqrt(tg_x*tg_x + tg_y*tg_y);

                        double tg_1_x = tg_x/mgn;
                        double tg_1_y = tg_y/mgn;

                        double r_vel_x = velocity[j].x - velocity[k].x;
                        double r_vel_y = velocity[j].y - velocity[k].y;

                        double dot_pro = tg_1_x * r_vel_x + tg_1_y * r_vel_y;

                        double vc_x = tg_1_x * dot_pro;
                        double vc_y = tg_1_y * dot_pro;

                        double f_x = r_vel_x - vc_x;
                        double f_y = r_vel_y - vc_y;

                        velocity[j].x -= f_x;
                        velocity[j].y -= f_y;

                        velocity[k].x += f_x;
                        velocity[k].y += f_y;

                        crc_pos[j].x = crc_pos[j].x + velocity[j].x * speed;
                        crc_pos[j].y = crc_pos[j].y + velocity[j].y * speed;

                        crc_pos[k].x = crc_pos[k].x + velocity[k].x * speed;
                        crc_pos[k].y = crc_pos[k].y + velocity[k].y * speed;


                        status[k] = 1;
                        changed = 1;
                        break;
                    }
                }

            }

            if(changed == 0){
                crc_pos[j].x = new_pos_x[j];
                crc_pos[j].y = new_pos_y[j];
            }
        }
	}

	if(status[4] == 0){
        crc_pos[4].x = new_pos_x[4];
        crc_pos[4].y = new_pos_y[4];
	}

	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=350.0;
	cameraAngle=1.0;
	angle=0;

	int min_val = 1;
	for(int j=0;j<5;j++){
        min_val = small_radius + rand() % 3;
        crc_pos[j].x = min_val;
        crc_pos[j].y = min_val;
        crc_pos[j].z = 0;

        velocity[j].x = ((double) rand() / (RAND_MAX));
        velocity[j].y = ((double) rand() / (RAND_MAX));
        velocity[j].z = 0;
	}



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
	gluPerspective(100,	1,	1,	1000.0);
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
