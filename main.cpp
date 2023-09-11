#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#include <gl/glut.h>


#define PI 3.14159
#define GAME_SCREEN 0

#define MENU_SCREEN 10
#define MAX_STONES  100
#define MAX_STONE_TYPES 5
#define stoneRotationSpeed 60
#define SPACESHIP_SPEED 40
int stoneTranslationSpeed = 5;



GLint m_viewport[5];
GLint CI = 0;

bool mouseButtonPressed = false, startGame = false, gameOver = false;
bool startScreen = true , nextScreen = false, previousScreen = false;
bool gameQuit = false, instructionsGame = false, optionsGame = false;

int x, y;
int i;
int randomStoneIndices[100];
int index;
int Score = 0;
int alienLife = 10;
int game_level = 1;
float mouseX, mouseY;
float LaserAngle = 0 ,stoneAngle = 0,lineWidth = 1;
float xOne = 0,yOne = 0;
float xStone[MAX_STONES], yStone[MAX_STONES];
float xHealthBarStart = 1200;
GLint stoneAlive[MAX_STONES];



GLfloat a[][2]={0, -50, 70, -50, 70,70, -70,70};

GLfloat LightColor[][3]= {1,0,0,  0,1,0,   0,0,1};

GLfloat AlienBody[][2]={{-3,9}, {6,-2}, {0,0}, {0.7,9}, {-0.15,12}, {14,18}, {19,10}, {20,0},{-6.5,0}};

GLfloat AlienCollar[][2] = {{-9,10.5}, {-6,11}, {-5,12}, {6,18}, {10,20}, {13,23}, {16,30}, {19,39}, {16,38},
						  {10,37}, {-13,39}, {-18,41}, {-20,43}, {-20.5,42}, {-21,30}, {-19.5,23}, {-19,20},
						  {-14,16}, {-15,17},{-13,13},  {-9,10.5}};

GLfloat ALienFace[][2] = {{-6,11}, {-4.5,18}, {0.5,20}, {0.,20.5}, {0.1,19.5}, {1.8,19}, {5,20}, {7,23}, {9,29},
						{6,29.5}, {5,28}, {7,30}, {10,38},{11,38}, {11,40}, {11.5,48}, {10,50.5},{8.5,51}, {6,52},
						{1,51}, {-3,50},{-1,51}, {-3,52}, {-5,52.5}, {-6,52}, {-9,51}, {-10.5,50}, {-12,49}, {-12.5,47},
						{-12,43}, {-13,40}, {-12,38.5}, {-13.5,33},{-15,38},{-14.5,32},  {-14,28}, {-13.5,33}, {-14,28},
						{-13.8,24}, {-13,20}, {-11,19}, {-10.5,12}, {-6,11} } ;

GLfloat ALienBeak[][2] = {{-6,21.5}, {-6.5,22}, {-9,21}, {-11,20.5}, {-20,20}, {-14,23}, {-9.5,28}, {-7,27}, {-6,26.5},
						{-4.5,23}, {-4,21}, {-6,19.5}, {-8.5,19}, {-10,19.5}, {-11,20.5} };

char highScore[100], ch;

void display();
void StoneGenerate();

void displayText(float x ,float y ,float z ,char *stringToDisplay) {
	int length;

	glRasterPos3f(x, y, z);

		length = strlen(stringToDisplay);

	for(int i=0 ;i<length ;i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]);
	}
}


void SetDisplayMode(int modeToDisplay) {
		switch(modeToDisplay){
		case GAME_SCREEN: glClearColor(0, 0, 0, 1);break;
		case MENU_SCREEN : glClearColor(0, 0 , 0, 1);break;
	}
}
void initializeStoneArray() {
	//random stones index

	for(int i = 0; i < MAX_STONES ;i++) {
		randomStoneIndices[i] = rand()%MAX_STONE_TYPES;
		stoneAlive[i]=true;
	}

	xStone[0] = -(200 * MAX_STONES) - 600;

	for(int i = 0;i<MAX_STONES ;i++) {
		yStone[i]=rand() % 600;
		if(int(yStone[i]) %2 )
			yStone[i] *= -1;
		xStone[i+1] = xStone[i] + 200;
	}
}
void DrawAlienBody()
{
	glColor3f(0,1,0);
	glBegin(GL_POLYGON);
	for(i=0; i<=8; i++)
		glVertex2fv(AlienBody[i]);
	glEnd();

	glColor3f(0,0,0);
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=8;i++)
		glVertex2fv(AlienBody[i]);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(-13,11);
		glVertex2f(-15,9);
	glEnd();
}
void DrawAlienCollar()
{
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);
	for(i=0;i<=20 ;i++)
		glVertex2fv(AlienCollar[i]);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=20 ;i++)
		glVertex2fv(AlienCollar[i]);
	glEnd();
}
void DrawAlienFace()
{

	glColor3f(0,0,1);
	glBegin(GL_POLYGON);
	for(i=0;i<=42 ;i++)
		glVertex2fv(ALienFace[i]);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=42 ;i++)
		glVertex2fv(ALienFace[i]);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f(3.3,22);
		glVertex2f(4.4,23.5);
		glVertex2f(6.3,26);
	glEnd();
}
void DrawAlienBeak()
{
	glColor3f(1,1,0);
	glBegin(GL_POLYGON);
	for(i=0;i<=14 ;i++)
		glVertex2fv(ALienBeak[i]);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=14 ;i++)
		glVertex2fv(ALienBeak[i]);
	glEnd();
}
void DrawAlienEyes()
{

	glColor3f(0,1,1);

	glPushMatrix();
	glRotated(-10,0,0,1);
	glTranslated(-6,32.5,0);
	glScalef(2.5,4,0);
	glutSolidSphere(1,20,30);
	glPopMatrix();

	glPushMatrix();
	glRotated(-1,0,0,1);
	glTranslated(-8,36,0);
	glScalef(2.5,4,0);
	glutSolidSphere(1,100,100);
	glPopMatrix();
}
void DrawAlien()
{
	DrawAlienBody();
	DrawAlienCollar();
	DrawAlienFace();
	DrawAlienBeak();
	DrawAlienEyes();
}
void DrawSpaceshipBody()
{
	glColor3f(0.5,0.5,0.4);

	glPushMatrix();
	glScalef(70,20,1);
	glutSolidSphere(1,50,50);
	glPopMatrix();

	glPushMatrix();
	glScalef(2,2,1);
	glTranslated(29,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,300);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,30);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,30);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,30);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,30);
    glTranslated(-5,0,0);
	glColor3fv(LightColor[(CI+0)%3]);
    glutSolidSphere(1,300,30);




	glPopMatrix();
}
void DrawSteeringWheel()
{
	glPushMatrix();
	glLineWidth(3);
	glColor3f(0.20,0.,0.20);
	glScalef(7,4,1);
	glTranslated(-1.9,5.5,0);
	glutWireSphere(1,8,8);
	glPopMatrix();

}
void DrawSpaceshipDoom()
{
	glColor4f(0.7,1,1,0.0011);
	glPushMatrix();
	glTranslated(0,30,0);
	glScalef(35,50,1);
	glutSolidSphere(1,50,50);
	glPopMatrix();
}
void DrawSpaceShipLazer() {

	glColor3f(1, 1, 1);
	glPushMatrix();
	glBegin(GL_POLYGON);
		glVertex2f(-55 ,10);
		glVertex2f(-55 ,30);
		glVertex2f(-50, 30);
		glVertex2f(-50 ,10);
	glEnd();

	float xMid =0,yMid =0;

	xMid = (55+50)/2.0;
	yMid = (25+35)/2.0;


	glTranslated(-xMid, yMid, 0);
	glRotated(LaserAngle, 0, 0 ,1);
	glTranslated(xMid , -yMid ,0);


	float midPoint = -(55+50)/2.0;

	glBegin(GL_POLYGON);
		glVertex2f(midPoint + 10 ,25);
		glVertex2f(midPoint + 10 ,35);
		glVertex2f(midPoint - 10 ,35);
		glVertex2f(midPoint - 10 ,25);
	glEnd();

	glPopMatrix();
}
void DrawLazerBeam() {

	float xMid = -(55+50)/2.0;
	float yMid = (25+35)/2.0;

	float mouseXEnd = -((- mouseX) + xOne);
	float mouseYEnd = -((- mouseY) + yOne);
	glLineWidth(5);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(xMid ,yMid);
		glVertex2f(mouseXEnd ,mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch(StoneIndex)
	{
	case 0:

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glColor3f(0.4f, 0.0f, 0.0f);
		glScalef(35,35,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,10,1);
		glutSolidSphere(1,5,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,60,1);
		glutSolidSphere(1,5,50);
		break;

	case 1:
		glColor3f(1.0f, 0.8f, 0.8f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(15,20,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(40,5,1);
		glutSolidSphere(1,5,50);
		break;

	case 2:
		glColor3f(0.2f, 0.2f, 0.0f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,25,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,60,1);
		glutSolidSphere(1,9,50);

		break;

	case 3:
		glColor3f(0.8f, 0.8f, 0.1f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(35,10,1);
		glutSolidSphere(1,10,7);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(50,20,1);
		glutSolidSphere(1,5,50);
		break;
	case 4:
		glColor3f(0.26f, 0.26f, 0.26f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,55,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(20,10,1);
		glutSolidSphere(1,9,50);
		glLoadIdentity();

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle+45 ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,10,1);
		glutSolidSphere(1,9,50);
		break;
	}
	glPopMatrix();
}
bool ifSpaceshipIsHit() {
		for(int i =0 ;i<MAX_STONES ;i++) {
		if(stoneAlive[i]&((xOne >= (xStone[i]/2 -70) && xOne <= (xStone[i]/2 + 70) && yOne >= (yStone[i]/2 -18 ) && yOne <= (yStone[i]/2 + 53)) || (yOne <= (yStone[i]/2 - 20) && yOne >= (yStone[i]/2 - 90) && xOne >= (xStone[i]/2 - 40) && xOne <= (xStone[i]/2 + 40))))
		{
			stoneAlive[i]=0;
			return false;
		}
	}
	return true;
}
void SpaceshipCreate(){
	glPushMatrix();
	glTranslated(xOne,yOne,0);
	if(!ifSpaceshipIsHit() && alienLife ){
		alienLife-= 1;
		xHealthBarStart -= 230;
	}
	DrawSpaceshipDoom();
	glPushMatrix();
	glTranslated(4,19,0);
	DrawAlien();
	glPopMatrix();
	DrawSteeringWheel();
	DrawSpaceshipBody();
	DrawSpaceShipLazer();
	if(mouseButtonPressed) {
		DrawLazerBeam();
	}
	glEnd();
	glPopMatrix();
}
void DisplayHealthBar() {

	glColor3f(1 ,0 ,0);
	glBegin(GL_POLYGON);
		glVertex2f(-xHealthBarStart ,700);
		glVertex2f(1200 ,700);
		glVertex2f(1200 ,670);
		glVertex2f(-xHealthBarStart, 670);
	glEnd();
	char temp[40];
	glColor3f(0 ,0 ,1);
	sprintf(temp,"SCORE = %d",Score);
	displayText(-1100 ,600 ,0.4 ,temp);
	sprintf(temp,"  LIFE = %d",alienLife);
	displayText(800 ,600 ,0.4 ,temp);
	sprintf(temp,"  LEVEL : %d",game_level);
	displayText(-100 ,600 ,0.4 ,temp);
	glColor3f(1 ,0 ,0);
}
void startScreenDisplay()
{
	glLineWidth(50);
	SetDisplayMode(MENU_SCREEN);

	glColor3f(0.4,0.5,0.5);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-750 ,-500 ,0.5);
		glVertex3f(-750 ,550 ,0.5);
		glVertex3f(750 ,550 ,0.5);
		glVertex3f(750 ,-500, 0.5);
	glEnd();

	glLineWidth(1);

	glColor3f(1, 1, 0.4);
	glBegin(GL_POLYGON);
		glVertex3f(-200 ,300 ,0.5);
		glVertex3f(-200 ,400 ,0.5);
		glVertex3f(200 ,400 ,0.5);
		glVertex3f(200 ,300, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();

	if(mouseX >=- 100 && mouseX <= 100 && mouseY >= 150 && mouseY <= 200){
		glColor3f(0 ,0 ,1) ;
		if(mouseButtonPressed){
			startGame = true ;
			gameOver = false;
			mouseButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);

	displayText(-100 ,340 ,0.4 ,"Start Game");

	if(mouseX >= -100 && mouseX<=100 && mouseY>=30 && mouseY<=80) {
		glColor3f(0 ,0 ,1);
		if(mouseButtonPressed){
			instructionsGame = true ;
			mouseButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);
	displayText(-120 ,80 ,0.4 ,"Instructions");

	if(mouseX >= -100 && mouseX <= 100 && mouseY >= -90 && mouseY <=- 40){
		glColor3f(0 ,0 ,1);
		if(mouseButtonPressed){
			gameQuit = true ;
			mouseButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);
	displayText(-100 ,-170 ,0.4 ,"    Quit");

}
void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);

	DisplayHealthBar();

	glScalef(2, 2 ,0);

	if(alienLife){
		SpaceshipCreate();
	}
	else {
		gameOver=true;
		instructionsGame = false;
		startScreen = false;
	}

	StoneGenerate();

}
void readFromFile() {

	FILE *fp = fopen("HighScoreFile.txt" ,"r");
	int i=0;
	if(fp!= NULL){
		while(fread(&ch,sizeof(char),1 ,fp)){
			highScore[i++] = ch;
		}
		highScore[i] = '\0';
	}
	fclose(fp);
}
void writeIntoFile() {
	FILE *fp = fopen("HighScoreFile.txt" ,"w");
	int i=0;
	char temp[40];
	if(fp!= NULL){
		int n= Score;
		while(n){
			ch = (n%10)+ '0';
			n/=10;
			temp[i++] = ch;
		}
		temp[i] = '\0';
		strrev(temp);
		puts(temp);
		if(temp[0] == '\0')
			temp[i++] = '0' ,temp[i++] = '\0';
		fwrite(temp ,sizeof(char)*i ,i ,fp);
	}
 fclose(fp);
}
void GameOverScreen()
{
	SetDisplayMode(MENU_SCREEN);
	glColor3f(0.4,0.5,0.5);
	glLineWidth(50);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-650 ,-500 ,0.5);
		glVertex3f(-650 ,520 ,0.5);
		glVertex3f(650 ,520 ,0.5);
		glVertex3f(650 ,-500, 0.5);
	glEnd();

	glLineWidth(1);
	stoneTranslationSpeed=5;
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
		glVertex3f(-550 ,810,0.5);
		glVertex3f(-550 ,610 ,0.5);
		glVertex3f(550 ,610 ,0.5);
		glVertex3f(550 ,810, 0.5);
	glEnd();

	glColor3f(1, 1, 0.4);
	glBegin(GL_POLYGON);
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();


	displayText(-300 ,640 ,0.4 ,"G A M E    O V E R ! ! !");
	glColor3f(0 , 0, 0);
	char temp[40];

	sprintf(temp,"Score : %d",Score);
	displayText(-100 ,340 ,0.4 ,temp);
	readFromFile();
	char temp2[40];
	if(atoi(highScore) < Score){
		writeIntoFile();
		sprintf(temp2 ,"Highest Score :%d" ,Score);
	} else
		sprintf(temp2 ,"Highest Score :%s" ,highScore);

	displayText(-250 ,400 ,0.4 ,temp2);

	if(mouseX >= -100 && mouseX <= 100 && mouseY >= 25 && mouseY <= 75){
		glColor3f(0 ,0 ,1);
		if(mouseButtonPressed){
			startGame = true ;
			gameOver = false;
			mouseButtonPressed = false;
			initializeStoneArray();
			alienLife = 10;
			xHealthBarStart = 1200;
			Score = 0;
			game_level = 1;
			GameScreenDisplay();
		}
	} else
		glColor3f(0 , 0, 0);
	displayText(-70 ,80 ,0.4 ,"Restart");

	if(mouseX >= -100 && mouseX <= 100 && mouseY >= -100 && mouseY <=- 50){
		glColor3f(0 ,0 ,1);
		if(mouseButtonPressed){
			exit(0);
			mouseButtonPressed = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayText(-100 ,-170 ,0.4 ,"    Quit");

}
void StoneGenerate(){

		if(xStone[0]>=1200){
			game_level++;
			stoneTranslationSpeed+=3;
			Score+=50;
			initializeStoneArray();
			GameScreenDisplay();
		}

	for(int i=0; i<MAX_STONES ;i++){
		index = i;

		if(mouseX <= (xStone[i]/2+20) && mouseX >=(xStone[i]/2-20) && mouseY >= (yStone[i]/2-20) && mouseY <= (yStone[i]/2+20) && mouseButtonPressed){
			if(stoneAlive[i]){
				stoneAlive[i] = 0;
				Score++;
				if(Score %1== 0) {
					stoneTranslationSpeed += 1;
				}
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if(stoneAlive[i] )
			DrawStone(randomStoneIndices[i]);
	}
	stoneAngle+=stoneRotationSpeed;
	if(stoneAngle > 360) stoneAngle = 0;
}
void backButton() {
	if(mouseX <= -450 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250){
			glColor3f(1, 1, 1);
			if(mouseButtonPressed){
				mouseButtonPressed = false;
				instructionsGame = false;
				startScreenDisplay();
			}
	}
	else glColor3f(1.0, 1.0, 1.0);
	displayText(-1000 ,-550 ,0, "<-Back");
}
void InstructionsScreenDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDisplayMode(MENU_SCREEN);

	glColor3f(0.3, 0.3, 0.3);
	displayText(-900 ,400 ,0.4 ,"Press 'w' to move up.");
	displayText(-900 ,300 ,0.4 ,"Press 's' to move down.");
	displayText(-900 ,200 ,0.4 ,"Press 'd' to move right.");
	displayText(-900 ,100 ,0.4 ,"Press 'a' to move left.");
	displayText(-900 ,0.0 ,0.4 ,"Left mouse click to shoot laser");
	displayText(-900 ,-200 ,0.4 ,"You will Get 1 point for shooting each object and 50 points for completing each level ");
	displayText(-900, -270,0.4, "Your Objective is to score maximum points");
	displayText(-900, -370,0.4, "HAPPY GAMING!");
	backButton();
    if(previousScreen)
		nextScreen = false ,previousScreen = false;


}
void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,1200,700);

	if(startGame && !gameOver)
		GameScreenDisplay();

	else if(instructionsGame)
		InstructionsScreenDisplay();

	else if(gameOver)
		GameOverScreen();


	else if(startScreen){

			startScreenDisplay();
			if(gameQuit || startGame || optionsGame || instructionsGame){


				if(startGame){
					SetDisplayMode(GAME_SCREEN);
					startScreen = false;

				} else if(gameQuit)
					exit(0);

				} else if(instructionsGame) {
					SetDisplayMode(GAME_SCREEN);
					InstructionsScreenDisplay();
				}
		}


	glScalef(1/2 ,1/2 ,0);
	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}
void RecalculateLaserAngle() {

	float mouseXForTan = (-50 - mouseX) + xOne;
	float mouseYForTan = (35 - mouseY) + yOne;
	float LaserAngleInRadian = atan(mouseYForTan/mouseXForTan);
	LaserAngle = (180/PI) * LaserAngleInRadian;
}
void keys(unsigned char key, int x, int y)
{

	if(key == 'd')
        xOne+=SPACESHIP_SPEED;

	if(key == 'a')
        xOne-=SPACESHIP_SPEED;

	if(key == 'w')
        yOne+=SPACESHIP_SPEED;

	if(key == 's')
        yOne-=SPACESHIP_SPEED;

	if(key == 'd' || key == 'a' || key == 'w' || key == 's')
		RecalculateLaserAngle();

	display();

}
void myinit()
{
	glClearColor(0.0,0.0,0.0,0);
	glColor3f(0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluOrtho2D(-1200, 1200, -700, 700);
	glMatrixMode(GL_MODELVIEW);
}
void passiveMotionFunc(int x,int y) {


	mouseX = float(x)/(m_viewport[2]/1200.0)-600.0;
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);


	RecalculateLaserAngle();
	display();
}
 void mouseClick(int buttonPressed ,int state ,int x, int y) {

	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mouseButtonPressed = true;
	else
		mouseButtonPressed = false;
	display();
}
 void UpdateColorIndexForSpaceshipLights(int value)
{
	 CI=(CI+1)%3;
 	 glutTimerFunc(100,UpdateColorIndexForSpaceshipLights,0);
}
 void idleCallBack() {
	 display();
 }
 int main(int argc, char** argv) {

	 FILE *fp = fopen("HighScoreFile.txt" ,"r") ;
	 if(fp!=NULL)
		fclose(fp);
	 else
		 writeIntoFile();

	glutInit(&argc, argv);
	glutInitWindowSize(1200,700);
	glutInitWindowPosition(90 ,0);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutTimerFunc(50,UpdateColorIndexForSpaceshipLights,0);
	glutCreateWindow("THE SPACE WAR");
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutPassiveMotionFunc(passiveMotionFunc);
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
	glutIdleFunc(idleCallBack);
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT ,m_viewport);
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();
 }
