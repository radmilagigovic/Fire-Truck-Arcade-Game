#include <math.h>		
#include <stdio.h>
#include <stdlib.h>		
#include "Glut.h"
#include<iostream>
#include<string>

int score = 0;                //for score counting
int start = 0;
bool game_over = false;
static int highest_score = 0;
bool collide = false;   //check if truck collides to make game over
char buffer[10];

int truckX = 400, truckY = 70;
int barrierX[4], barrierY[4];            //barriers for level one
int barrier_secX[4], barrier_secY[4];    //barriers for level two
int y_divider = 4, divider;
int x_divider[2] = { 180,620 };          //dimensions of white arrows

const int font1 = (int)GLUT_BITMAP_TIMES_ROMAN_24;   
const int font2 = (int)GLUT_BITMAP_HELVETICA_18;
const int font3 = (int)GLUT_BITMAP_8_BY_13;

bool left_check = 0, right_check = 0, up_check = 0, down_check = 0;

int max_count = 1, offset = 15;  //limit for truck not to go outside of the road
int count_check = max_count;
int fuel = 5;                    

int i, q;
void move() {
	if (left_check == 1 && count_check != 0) {
		truckX -= offset;
		count_check--;
		if (count_check == 0) {
			left_check = 0;
			count_check = max_count;
		}
	}
    if (right_check == 1 && count_check != 0) {
		truckX += offset;
		count_check--;
		if (count_check == 0) {
			right_check = 0;
			count_check = max_count;
		}
	}
	if (up_check == 1 && count_check != 0) {
		truckY -= offset;
		count_check--;
		if (count_check == 0) {
			up_check = 0;
			count_check = max_count;
		}
	}
	else if (down_check == 1 && count_check != 0) {
		truckY += offset;
		count_check--;
		if (count_check == 0) {
			down_check = 0;
			count_check = max_count;
		}
	}
}



void Text(char ch[], int xpos, int ypos)  
{
	int numofchar = strlen(ch);
	glLoadIdentity();
	glRasterPos2f(xpos, ypos);
	for (i = 0; i <= numofchar - 1; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ch[i]);
	}
}


void Num(char ch[], int numtext, int xpos, int ypos)     //counting the score
{
	int len;
	int k;
	k = 0;
	len = numtext - strlen(ch);
	glLoadIdentity();
	glRasterPos2f(xpos, ypos);
	for (i = 0; i <= numtext - 1; i++)
	{
		if (i < len)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ch[k]);
			k++;
		}

	}
}

//Function for initializating barriers positions using random numbers to decide where 
// the barriers should be on the road. Values are stored in array 

void barriers_positions()    
{
	//glClearColor(0, 0, 1, 0);
	for (i = 0; i < 4; i++)
	{
		if (rand() % 3 == 0)
		{
			barrierX[i] = 250;   
		}
		else if (rand() % 3 == 1)
		{
			barrierX[i] = 300;  
		}
		else
		{
			barrierX[i] = 450;   
		}
		barrierY[i] = 1000 - i * 400;
	}
}

void barriers_positions_sec()
{
	//glClearColor(0, 0, 1, 0);
	for (i = 0; i < 4; i++)
	{
		if (rand() % 3 == 0)
		{
			barrier_secX[i] = 250;   
		}
		else if (rand() % 3 == 1)
		{
			barrier_secX[i] = 300;  
		}
		else
		{
			barrier_secX[i] = 450;   
		}
		barrier_secY[i] = 1000 - i * 400;
	}
}
 
void initialize(int life, int s) {   //Function for reseting the game after the collision
	truckX = 400;                   //fuel is decreasing by 1, and figures are back on original places
	truckY = 70;
	left_check = 0;
	right_check = 0;
	up_check = 0;
	down_check = 0;
	count_check = max_count;
	fuel = life;
	score = s;
	barriers_positions();
	barriers_positions_sec();
}

void drawBul(int x, int y)
{
	int newx = x;
	int newy = y;
	//Bottom
	glColor3f(0.169, 0.169, 0.169);
	glBegin(GL_POLYGON);
	glVertex2f(newx - 50, newy);
	glVertex2f(newx - 50, newy + 100);
	glVertex2f(newx + 50, newy + 100);
	glVertex2f(newx + 50, newy);
	glEnd();
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_POLYGON);
	glVertex2f(newx, newy);
	glVertex2f(newx + 50, newy);
	glVertex2f(newx + 50, newy + 80);
	glVertex2f(newx, newy + 80);
	glEnd();

}
void positionBul() {
	//Left side
	drawBul(95, 15);
	drawBul(135, 35);
	drawBul(60, 390);
	drawBul(65, 140);
	drawBul(100, 160);
	drawBul(135, 300);
	drawBul(50, 340);
	drawBul(100, 380);

	//Right side
	drawBul(705, 15);
	drawBul(665, 35);
	drawBul(725, 200);
	drawBul(735, 140);
	drawBul(700, 160);
	drawBul(665, 300);
	drawBul(750, 340);
	drawBul(700, 380);
}

// road drawing
void drawRoad()
{
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(400 - 225, 500);
	glVertex2f(400 - 225, 0);
	glVertex2f(400 + 225, 0);
	glVertex2f(400 + 225, 500);
	glEnd();
}

void drawDivider() //drawing white patch which divides road and city
{
	glLoadIdentity();
	glTranslatef(0, divider, 0); //moving effect 
	for (int k = 0; k < 2; k++) {
		for (i = 1; i <= 10; i++)
		{
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2f(x_divider[k] - 5, y_divider * 15 * i + 18);
			glVertex2f(x_divider[k] - 5, y_divider * 15 * i - 18);
			glVertex2f(x_divider[k] + 5, y_divider * 15 * i - 18);
			glVertex2f(x_divider[k] + 5, y_divider * 15 * i + 18);
			glEnd();
		}
	}

	glLoadIdentity();
}


void drawWindow(int x, int y) {
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + 20, y);
	glVertex2f(x + 20, y + 20);
	glVertex2f(x, y + 20);
	glEnd();
}

void drawBuilding(int x, int y) {
	glColor3f(0.211, 0.211, 0.211);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + 50, y);
	glVertex2f(x + 50, y + 150);
	glVertex2f(x, y + 150);
	glEnd();

	drawWindow(x, y + 30);
	drawWindow(x + 20, y + 70);
	drawWindow(x, y + 110);
}

void drawTruck()   //truck
{
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glColor3f(0.211, 0.211, 0.211);
	glVertex2f(truckX - 25, truckY + 35);  
	glVertex2f(truckX - 25, truckY + 25);
	
	glVertex2f(truckX - 25, truckY - 25);  
	glVertex2f(truckX - 25, truckY - 35);

	glVertex2f(truckX + 25, truckY + 35);  
	glVertex2f(truckX + 25, truckY + 25);
  
	glVertex2f(truckX + 25, truckY - 25);
    glVertex2f(truckX + 25, truckY - 35);

	glVertex2f(truckX + 0, truckY + 55);
	

	glVertex2f(truckX - 22, truckY + 70);
	glVertex2f(truckX + 22, truckY + 70);  
	glVertex2f(truckX - 22, truckY + 80);
	glVertex2f(truckX + 22, truckY + 80);  
	glEnd();

	
	glPointSize(30.0);
	glBegin(GL_POINTS);  
	glColor3f(1, 1, 1);
	glVertex2f(truckX - 7, truckY + 75);  
	glVertex2f(truckX + 7, truckY + 75);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(truckX - 25, truckY + 55);
	glVertex2f(truckX - 25, truckY - 55);
	glVertex2f(truckX + 25, truckY - 55);
	glVertex2f(truckX + 25, truckY + 55);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.211, 0.211, 0.211);
	glVertex2f(truckX - 17, truckY + 45);
	glVertex2f(truckX - 17, truckY - 45);
	glVertex2f(truckX + 17, truckY - 45);
	glVertex2f(truckX + 17, truckY + 45);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(truckX - 21, truckY + 90);
	glVertex2f(truckX - 15, truckY + 98);
	glVertex2f(truckX + 15, truckY + 98);
	glVertex2f(truckX + 21, truckY + 90);
	glEnd();

	
	glColor3f(1 , 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(truckX - 20, truckY + 110);
	glVertex2f(truckX - 15, truckY + 110 - 11);
	glVertex2f(truckX - 10, truckY + 110);
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(truckX + 20, truckY + 110);
	glVertex2f(truckX + 15, truckY + 110 - 11);
	glVertex2f(truckX + 10, truckY + 110);
	glEnd();

}

void drawBarriers()
{

	for (i = 0; i < 4; i++)
	{
		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);
		glVertex2f(barrierX[i] - 35, barrierY[i] + 40 );
		glVertex2f(barrierX[i], barrierY[i] +40 + 35);
		glVertex2f(barrierX[i] + 35, barrierY[i] + 40 );
		glEnd();

		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);
		glVertex2f(barrierX[i] - 45, barrierY[i] + 35);
		glVertex2f(barrierX[i] - 30, barrierY[i] + 35 + 20);
		glVertex2f(barrierX[i] + 45, barrierY[i] + 35);
		glEnd();


		barrierY[i] = barrierY[i] - 8;   //moving barriers

		if (abs(barrierY[i] - truckY) < 90)
		{
			if (abs(barrierX[i] - truckX) < 50)   //
				collide = true;
		}

		if (barrierY[i] < -25)
		{
			if (rand() % 3 == 0)
			{
				barrierX[i] = 250;
			}
			else if (rand() % 3 == 1)
			{
				barrierX[i] = 400;
			}
			else
			{
				barrierX[i] = 550;
			}
			barrierY[i] = 700;
		}
	}
}



void drawBarriers_sec()
{

	for (i = 0; i < 4; i++)
	{
		

		glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex2f(barrier_secX[i] - 50, barrier_secY[i] + 15);
		glVertex2f(barrier_secX[i] - 50, barrier_secY[i] - 15);
		glVertex2f(barrier_secX[i] + 50, barrier_secY[i] + 20);
		glVertex2f(barrier_secX[i] + 50, barrier_secY[i] + 20);
		glEnd();


		barrier_secY[i] = barrier_secY[i] - 12;

		if (abs(barrier_secY[i] - truckY) < 90)
		{
			if (abs(barrier_secX[i] - truckX) < 70)
				collide = true;
		}

		if (barrier_secY[i] < -25)
		{
			if (rand() % 3 == 0)
			{
				barrier_secX[i] = 250;
			}
			else if (rand() % 3 == 1)
			{
				barrier_secX[i] = 400;
			}
			else
			{
				barrier_secX[i] = 550;
			}
			barrier_secY[i] = 700;
		}
	}
}

void mySpecialKeyFunc(int key, int x, int y)  //keys for changing coordinates for truck
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if (truckX != 250 && left_check == 0 && right_check == 0) {
			truckX -= offset;
			left_check = true;
		}

		break;
	case GLUT_KEY_RIGHT:
		if (truckX != 550 && left_check == 0 && right_check == 0) {
			truckX += offset;
			right_check = true;
		}
		break;
	case GLUT_KEY_DOWN:
		if (truckY != 100 && up_check == 0 && down_check == 0) {
			truckY -= offset;
			up_check = true;
		}
		break;
	case GLUT_KEY_UP:
		if (truckY != 900 && up_check == 0 && down_check == 0) {
			truckY += offset;
			down_check = true;
		}
		break;
	}
	glutPostRedisplay();
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':  //for space button
		if (game_over == 1) {
			game_over = 0;
			barriers_positions();
			barriers_positions_sec();
		}
		else if (start == 0) {
			start = 1;
		}
		break;
	case 27:exit(0);
	}
}

void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 500);
	glMatrixMode(GL_MODELVIEW);
}


void welcome_screen() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1);
	drawRoad();
	positionBul();
	drawDivider();
	drawTruck();

	//Left side
	drawBuilding(15, 50);
	drawBuilding(55, 280);
	drawBuilding(110, 250);

	//Right side
	drawBuilding(750, 50);
	drawBuilding(695, 280);
	drawBuilding(640, 250);

	glColor3f(1.000, 1.000, 0.000);     
	char heading[] = "FIRE TRUCK GAME ";
	Text(heading, 300, 450);

	glColor3f(1.000, 1.000, 0.000);
	char Rule[] = "RULES:";
	Text(Rule, 350, 350);

	glColor3f(1.000, 1.000, 0.000);
	char st[] = "To start the game press SPACE";
	Text(st, 230, 280);

	glColor3f(1.000, 1.000, 0.000);
	char Rule2[] = "To move left - press left arrow key";
	char Rule3[] = "To move rigth - press rigth arrow key";
	char Rule4[] = "To move up - press up arrow key";
	char Rule5[] = "To move down - press down arrow key";
	Text(Rule2, 230, 260);
	Text(Rule3, 230, 240);
	Text(Rule4, 230, 220);
	Text(Rule5, 230, 200);
	glutSwapBuffers();
}

void game_over_screen() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1);
	truckX = 400, truckY = 70;
	glColor3f(1.000, 1.000, 0.000);
	if (score > highest_score)
		highest_score = score;
	char over[] = " GAME OVER ";
	Text(over, 300, 400);
	char st[] = "Press SPACE to RESTART game";
	Text(st, 250, 250);
	char hs[] = "Your highest score is: ";
	Text(hs, 250, 350);
	_itoa_s(highest_score, buffer, 10);
	Num(buffer, 6, 480, 350);
	char sc[] = "Your score is: ";
	Text(sc, 250, 300);
	_itoa_s(score, buffer, 10);
	Num(buffer, 6, 420, 300);
	glutSwapBuffers();
}

void level_two() {
	move();
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1);
	//Road BackGround
	glColor3f(0.4314, 0.1725, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(800, 0);
	glColor3f(0.9216, 0.5961, 0.3059);
	glVertex2f(800, 300);
	glVertex2f(0, 300);
	glEnd();

	
	//Left side
	drawBuilding(0, 250);
	drawBuilding(55, 280);
	drawBuilding(110, 250);

	//Right side
	drawBuilding(750, 250);
	drawBuilding(695, 280);
	drawBuilding(640, 250);

	drawRoad();
	drawDivider();
	drawTruck();
	//drawBarriers(0.5, 0, 0.5, 0, 0, 0);
	drawBarriers();
	drawBarriers_sec();
	divider = divider - 16;
	if (divider < -60)
	{
		divider = 0;
	}
	score = score + 1;
	glColor3f(1, 1, 1);
	char score_text[] = "Score: ";
	Text(score_text, 660, 455);
	_itoa_s(score, buffer, 10);
	Num(buffer, 6, 720, 455);

	char fuel_ch[] = { fuel + '0','\n' };
	char live_text[] = "F U E L: ";
	Text(live_text, 340, 460);
	Num(fuel_ch, 6, 450, 460);
	//Text(live_text, 300, 50);       
	//Num(fuel_ch, 6, 720, 485);
	glutSwapBuffers();
	for (q = 0; q <= 10000000; q++) { ; }
	if (collide == true)
	{
		fuel--;
		if (fuel == 0) {    //no fuel - game over
			start = 0;
			game_over = true;
			glutSwapBuffers();
		}
		initialize(fuel, score);
		collide = 0;
	}
}



void level_one()
{
	move();
	glClearColor(0.47, 0.79, 0.79, 0);  
	glClear(GL_COLOR_BUFFER_BIT);
	//Road BackGround
	
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(800, 0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(800, 500);
	glVertex2f(0, 500);
	glEnd();
	positionBul();
	drawRoad();
	drawDivider();
	drawTruck();

	//Left side
	drawBuilding(15, 50);
	drawBuilding(55, 280);
	drawBuilding(110, 250);

	//Right side
	drawBuilding(750, 50);
	drawBuilding(695, 280);
	drawBuilding(640, 250);
	drawBarriers();
	//drawBarriers_sec();
	divider = divider - 16;
	if (divider < -60)
	{
		divider = 0;
	}
	score = score + 1;
	glColor3f(1, 1, 1);
	char score_text[] = "Score: ";
	Text(score_text, 660, 455);
	_itoa_s(score, buffer, 10);
	Num(buffer, 6, 720, 455);

	char live_text[] = "F U E L : ";
	char fuel_ch[] = { fuel + '0','\n' };
	Text(live_text, 340, 460);
	Num(fuel_ch, 6, 450, 460);
	//Text(live_text, 660, 485);
	//Num(fuel_ch, 6, 720, 485);

	glutSwapBuffers();
	for (q = 0; q <= 10000000; q++) { ; }
	if (collide == true)
	{
		fuel--;
		if (fuel == 0) {
			start = 0;
			game_over = true;
			glutSwapBuffers();
		}
		initialize(fuel, score);
		collide = 0;

	}
}

// function for changing levels depending on the score

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (start == 1 && score <= 500) {
		glClearColor(0.000, 0.392, 0.000, 1);
		level_one();
	}
else if (start == 1 && score > 500) {
	level_two();
	}
	else if (start == 0 && game_over == 1) {
		game_over_screen();
	}
	else {
		initialize(5, 0);   //fuel
		welcome_screen();
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("Fire Truck game");
	barriers_positions();
	barriers_positions_sec();
	init();
	glutDisplayFunc(display);
	glutSpecialFunc(mySpecialKeyFunc);
	glutKeyboardFunc(myKeyboardFunc);
	glutIdleFunc(display);
	glutMainLoop();
}



