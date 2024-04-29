#include <iostream>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <string.h>
#include <stdio.h>

#define UP 1
#define DOWN 0
#define MAX 10
#define SIZE_MIS_X 55   
#define SIZE_MIS_y 30   
#define MAX_MISSILES 3 
#define SPEED 1

using namespace std;
int frames=60;

int frameCount = 0;
float fps = 0;
int currentTime = 0, previousTime = 0;
char scene[50];
int ch_scene=1;
int max_dist, max_miss;
int hit_missile=0;
int plane_choice=2;
int setting=0;

void* currentfont;

float x_step=-171.0;    
float y_cre=0;            
float y_pos=0;          
float theta=0;         
bool state;            
int update_mis;

GLfloat x = 0.0f;       

GLfloat fuel=9800;        
GLfloat dist, missiles;
GLfloat missile_x=250,missile_y[MAX_MISSILES]= {0};
int no_of_missiles=3;

int full = 1;
int i_bck,i_mis1,i_mis2,i_mis3,i_plane,i_inst21;
int i_cre22,i_sel31,i_sel32, i_fin4,i_2,i_23,i_0,i_1;
int i_s,i;

GLfloat windowWidth;
GLfloat windowHeight;

GLuint tex_2d, tex_2d_mis[4], tex_2d_plane;
GLuint tex_2d_0, tex_2d_1, tex_2d_2, tex_2d_21, tex_2d_22, tex_2d_31, tex_2d_4, tex_2d_23;

int page=0;

//class to draw the plane
class plane
{
public:
	float x[MAX], y[MAX], i;
	float ymax, ymin;
	int button;
	plane()
	{
		x[1]=-30, x[2]=30, x[3]=30, x[4]=-30;
		y[1]=15, y[2]=15, y[3]=-15, y[4]=-15;
	}
	void draw_plane()
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		if(plane_choice==2)
		{
			if (i_plane == 0)
			{
				tex_2d_plane = SOIL_load_OGL_texture
					       (
						       "res/plane2.png",
						       SOIL_LOAD_AUTO,
						       SOIL_CREATE_NEW_ID,
						       SOIL_FLAG_MULTIPLY_ALPHA
					       );
				i_plane = 1;
			}
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d_plane);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(x[1], y[1]);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(x[2], y[2]);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(x[3], y[3]);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(x[4], y[4]);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
} plane1;

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		//27 is the ASCII value of the ESC key
	case 27:
		x=0.0;
		if(page!=2)
		{
			i_bck=0,i_mis1=0,i_mis2=0,i_mis3=0,i_plane=0,i_sel31=0,i_sel32=0, i_fin4=0;
			i_23=0,i_0=0,i_1=0;
			missiles=0;
			dist=0;
			setting=0;
			hit_missile=0;
			y_pos=0;
			missile_x=250;
			fuel=98;
			y_cre=0;
			x=0.0;
			cout<<full<<endl;
			cout<<x<<endl;
			page=2;
		}
		else
			//exit game
			exit(0);
		break;

	case 'f':       //full screen
		if (full == 0)
		{
			glutFullScreen();
			full = 1;
		}
		else
		{
			glutReshapeWindow(800, 450);
			glutPositionWindow(320,150);
			full = 0;
		}
	}
}

//Special keys have been used to choose scene (in page 32)
void SpecialKeys(int key, int x, int y)
{
	if(page==32)
	{
		switch (key)
		{
		case GLUT_KEY_RIGHT:
			if(ch_scene<5)
			{
				ch_scene++;
				i_s=0;
				cout<<ch_scene<<endl;
			}
			break;

		case GLUT_KEY_LEFT:
			if(ch_scene>1)
			{
				ch_scene--;
				i_s=0;
				cout<<ch_scene<<endl;
			}
			break;
		}
		glutPostRedisplay();
	}
}

//For glutBitmapCharacter (used only in page 1)
void setFont(void* font)
{
	currentfont=font;
}

void drawString(float x,float y,float z,char* string)
{
	char* c;
	glRasterPos3f(x,y,z);

	for(c=string; *c!='\0'; c++)
	{	glColor3f(0.0,0.0,0.0);
		glutBitmapCharacter(currentfont,*c);
	}
}

//Determines the action on mouse click event
void Mouse(int button, int m_state, int m_x, int m_y)
{
	if(page==1)
	{
		if(m_x>620 && m_y>260 && m_state==GLUT_UP)
		{
			cout<<m_x<<" "<<m_y<<endl;
			page=2;
		}
	}
	else if(page==2)
	{
		if(full==0)
		{
			if(m_state==GLUT_UP)
				cout<<m_x<<" "<<m_y<<endl;
			if(m_y>92 && m_y<256 && m_state==GLUT_UP)
			{
				cout<<"play"<<endl;
				page=31;
			}
			if(m_y>260 && m_state==GLUT_UP)
			{
				cout<<"exit"<<endl;
				exit(0);
			}
		}
		else
		{
			if(state==GLUT_UP)
				cout<<m_x<<" "<<m_y<<endl;
			if(m_y>154 && m_y<425 && m_state==GLUT_UP)
			{
				cout<<"play"<<endl;
				page=31;
			}
			if(m_y>430 && m_state==GLUT_UP)
			{
				cout<<"exit"<<endl;
				exit(0);
			}

		}

	}
	else if(page==31)
	{
		if(button==GLUT_LEFT_BUTTON && m_state==GLUT_UP)
		{
			if(full==0)
			{
				cout<<m_x<<" "<<m_y<<endl;
				if(m_y>168 && m_y<205)
				{
					plane_choice=1;
					i_plane=0;
					glutPostRedisplay();
				}
				if(m_y>242 && m_y<287)
				{
					plane_choice=2;
					i_plane=0;
					glutPostRedisplay();
				}
				if(m_y>300)
				{
					cout<<"next"<<endl;
					i_plane=0;
					page=32;
				}
			}
			else
			{
				if(m_y>307 && m_y<347)
				{
					plane_choice=1;
					i_plane=0;
					glutPostRedisplay();
				}
				if(m_y>416 && m_y<463)
				{
					plane_choice=2;
					i_plane=0;
					glutPostRedisplay();
				}
				if(m_y>470)
				{
					cout<<"next"<<endl;
					i_plane=0;
					page=32;
				}
			}
		}
	}
	else if(page==32)
	{
		if(button==GLUT_LEFT_BUTTON && m_state==GLUT_UP)
		{
			if(full==0)
			{
				if(m_y>300)
				{
					cout<<"next"<<endl;
					i_plane=0;
					if(setting==1)
					{
						setting=0;
						page=2;
					}
					else
						page=3;
				}
			}
			else
			{
				if(m_y>470)
				{
					cout<<"next"<<endl;
					i_plane=0;
					if(setting==1)
					{
						setting=0;
						page=2;
					}
					else
						page=3;
				}
			}
		}
	}
	else if(page==3)
	{
		if(button==GLUT_LEFT_BUTTON && m_state==GLUT_DOWN)
		{
			state=UP;
			cout<<"Going Up!"<<endl;
		}
		else if(button==GLUT_LEFT_BUTTON && m_state==GLUT_UP)
		{
			state=DOWN;
			cout<<"Going Down"<<endl;
		}
	}
}

//draw text in finifh screen (page=4)
void draw_fin_text()

{
	char string[6][40];
	int i,lengthOfString;

	strcpy(string[0],"WOW!!");
	sprintf(string[1],"You just travelled %d meters",(int)dist);
	sprintf(string[2],"and dodged %d missiles",(int)missiles);
	sprintf(string[3],"before you ran out of fuel!!");
	sprintf(string[4],"before you were hit by one!!");

	//update high scores;
	if((int)dist>max_dist)
	{
		max_dist=dist;
		cout<<max_dist<<" "<<dist<<endl;
	}
	if(missiles>max_miss)
	{
		max_miss=missiles;
	}

	glLineWidth(4);
	glPushMatrix();
	glTranslatef(-105,55,0);
	glScalef(0.3,0.3,0.3);
	lengthOfString = (int)strlen(string[0]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string[0][i]);
	}
	glPopMatrix();

	glLineWidth(3);
	glPushMatrix();
	glTranslatef(-155,35,0);
	glScalef(0.1,0.1,0.1);
	lengthOfString = (int)strlen(string[1]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string[1][i]);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-155,20,0);
	glScalef(0.1,0.1,0.1);
	lengthOfString = (int)strlen(string[2]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string[2][i]);
	}
	glPopMatrix();

	if(!hit_missile)
	{
		glPushMatrix();
		glTranslatef(-155,5,0);
		glScalef(0.1,0.1,0.1);
		lengthOfString = (int)strlen(string[3]);
		for(i=0; i<lengthOfString; i++)
		{
			glColor3f(1,1,1);
			glutStrokeCharacter(GLUT_STROKE_ROMAN,string[3][i]);
		}
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glTranslatef(-155,5,0);
		glScalef(0.1,0.1,0.1);
		lengthOfString = (int)strlen(string[4]);
		for(i=0; i<lengthOfString; i++)
		{
			glColor3f(1,1,1);
			glutStrokeCharacter(GLUT_STROKE_ROMAN,string[4][i]);
		}
		glPopMatrix();
	}
}


void draw_high_text()
{
	char string_high[10][20];
	int lengthOfString,i;

	strcpy(string_high[0],"High Scores: ");
	sprintf(string_high[1],"Distance: %d",max_dist);
	sprintf(string_high[2],"Missiles: %d",max_miss);

	glLineWidth(3);
	glPushMatrix();
	glTranslatef(0,30,0);
	glScalef(0.2,0.2,0.2);
	lengthOfString = (int)strlen(string_high[0]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string_high[0][i]);
	}
	glPopMatrix();

	glLineWidth(2);
	glPushMatrix();
	glTranslatef(70,0,0);
	glScalef(0.1,0.1,0.1);
	lengthOfString = (int)strlen(string_high[1]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string_high[1][i]);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(70,-15,0);
	glScalef(0.1,0.1,0.1);
	lengthOfString = (int)strlen(string_high[2]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string_high[2][i]);
	}
	glPopMatrix();
}

//draw text in page 2 (menu screen)
void draw_menu_text()
{

	char string_menu[5][5];
	int lengthOfString,i;

	strcpy(string_menu[0],"Play!");

	glLineWidth(1);
	glPushMatrix();
	glTranslatef(-120,50,0);
	glScalef(0.1,0.1,0.1);
	lengthOfString = (int)strlen(string_menu[0]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string_menu[0][i]);
	}
	glPopMatrix();

}


//IMPORTANT
void draw_chScene_text()
{
	char string[15][120];
	int i,lengthOfString;


	strcpy(string[0],"Choose Scene");
	strcpy(string[1],"Use arrow keys");
	strcpy(string[2],"Next");

	glLineWidth(2);
	glPushMatrix();
	glTranslatef(-130,55,0);
	glScalef(0.3,0.3,0.3);
	lengthOfString = (int)strlen(string[0]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string[0][i]);
	}
	glPopMatrix();

	glLineWidth(3);
	glPushMatrix();
	glTranslatef(-75,35,0);
	glScalef(0.15,0.15,0.15);
	lengthOfString = (int)strlen(string[1]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string[1][i]);
	}
	glPopMatrix();

	glLineWidth(2);
	glPushMatrix();
	glTranslatef(-25,-75,0);
	glScalef(0.1,0.1,0.1);
	lengthOfString = (int)strlen(string[2]);
	for(i=0; i<lengthOfString; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,string[2][i]);
	}
	glPopMatrix();

}
void draw_score()
{
	int length;
	char score_text[15];
	strcpy(score_text,"Distance: ");
	glLineWidth(1);
	glPushMatrix();
	glTranslatef(85,82,0);
	glScalef(0.08,0.08,0.08);
	length = (int)strlen(score_text);
	for(i=0; i<length; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,score_text[i]);
	}
	glPopMatrix();

	char dist_text_val[15];
	sprintf(dist_text_val,"%d",(int)dist);
	glPushMatrix();
	glTranslatef(130,82,0);
	glScalef(0.08,0.08,0.08);
	length = (int)strlen(dist_text_val);
	for(i=0; i<length; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,dist_text_val[i]);
	}
	glPopMatrix();

	char missiles_text[15];
	strcpy(missiles_text,"Missiles: ");
	glPushMatrix();
	glTranslatef(85,72,0);
	glScalef(0.08,0.08,0.08);
	length = (int)strlen(missiles_text);
	for(i=0; i<length; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,missiles_text[i]);
	}
	glPopMatrix();

	char mis_text_val[15];
	sprintf(mis_text_val,"%d",(int)missiles);
	glPushMatrix();
	glTranslatef(130,72,0);
	glScalef(0.08,0.08,0.08);
	length = (int)strlen(mis_text_val);
	for(i=0; i<length; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,mis_text_val[i]);
	}
	glPopMatrix();

	char fuel_text[15];
	strcpy(fuel_text,"Fuel:  %");
	glPushMatrix();
	glTranslatef(-149,82,0);
	glScalef(0.08,0.08,0.08);
	length = (int)strlen(fuel_text);
	for(i=0; i<length; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,fuel_text[i]);
	}
	glPopMatrix();

	char fuel_text_val[15];
	sprintf(fuel_text_val,"%d",(int)fuel);
	glPushMatrix();
	glTranslatef(-125,82,0);
	glScalef(0.08,0.08,0.08);
	length = (int)strlen(fuel_text_val);
	for(i=0; i<length; i++)
	{
		glColor3f(1,1,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,fuel_text_val[i]);
	}
	glPopMatrix();
}

//select the scene
void select_scene()
{
	switch(ch_scene)
	{
	case 1:
		sprintf(scene,"res/scene1.jpg");
		break;
	case 2:
		sprintf(scene,"res/scene2.png");
		break;
	}
}

void rocket1(int x_cor, int y_cor)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (i_mis1 == 0)
	{
		tex_2d_mis[1] = SOIL_load_OGL_texture
				(
					"res/rocket2.png",
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MULTIPLY_ALPHA
				);
		i_mis1 = 1;
	}
	glBindTexture(GL_TEXTURE_2D, tex_2d_mis[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(x_cor, y_cor);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor+SIZE_MIS_y-10);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(x_cor, y_cor+SIZE_MIS_y-10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void rocket2(int x_cor, int y_cor)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (i_mis2 == 0)
	{
		tex_2d_mis[2] = SOIL_load_OGL_texture
				(
					"res/rocket3.png",
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MULTIPLY_ALPHA
				);
		i_mis2 = 1;
	}
	glBindTexture(GL_TEXTURE_2D, tex_2d_mis[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(x_cor, y_cor);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor+SIZE_MIS_y-10);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(x_cor, y_cor+SIZE_MIS_y-10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void rocket3(int x_cor, int y_cor)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (i_mis3 == 0)
	{
		tex_2d_mis[3] = SOIL_load_OGL_texture
				(
					"res/rocket4.png",
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MULTIPLY_ALPHA
				);
		i_mis3 = 1;
	}
	glBindTexture(GL_TEXTURE_2D, tex_2d_mis[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(x_cor, y_cor);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor+SIZE_MIS_y-10);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(x_cor, y_cor+SIZE_MIS_y-10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void draw_rockets()
{
	if(missile_x>200)
	{
		no_of_missiles=rand()%MAX_MISSILES+1;
	}
	if(missile_x>=195 && missile_x<=200)
	{
		for(int k=1; k<=no_of_missiles; k++)
			missile_y[k]=-101+rand()%165;
	}
	switch(no_of_missiles)
	{
	case 1:
		rocket1(missile_x,missile_y[1]);
		break;
	case 2:
		rocket1(missile_x,missile_y[1]);
		rocket2(missile_x,missile_y[2]);
		break;
	case 3:
		rocket1(missile_x,missile_y[1]);
		rocket2(missile_x,missile_y[2]);
		rocket3(missile_x,missile_y[3]);
		break;
	case 4:
		rocket1(missile_x,missile_y[1]);
		rocket3(missile_x,missile_y[2]);
		rocket2(missile_x,missile_y[3]);
		rocket3(missile_x,missile_y[4]);
		break;
	default:
	case 5:
		rocket1(missile_x,missile_y[1]);
		rocket3(missile_x,missile_y[2]);
		rocket2(missile_x,missile_y[3]);
		rocket3(missile_x,missile_y[4]);
		rocket1(missile_x,missile_y[5]);
		break;

	}
}

void draw_chosen_plane()
{
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-150,-40);
	glVertex2f(-30,-40);
	glVertex2f(-30,40);
	glVertex2f(-150,40);
	glEnd();

	glPushMatrix();
	glTranslatef(-90,0,0);
	glScalef(1.6,1.6,0);
	plane1.draw_plane();
	glPopMatrix();
}


void drawfps()
{
	int length;
	char d_fps[15];
	sprintf(d_fps,"FPS: %.1f",fps);
	glLineWidth(1);
	glPushMatrix();
	glTranslatef(-160,-85,0);
	glScalef(0.04,0.04,0.04);
	length = (int)strlen(d_fps);
	for(i=0; i<length; i++)
	{
		glColor3f(0,0,1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,d_fps[i]);
	}
	glPopMatrix();
}

void RenderScene()
{
	if(page==2)
	{

		glClear(GL_COLOR_BUFFER_BIT);

		//load .jpg image
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if(i_2==0)
		{
			tex_2d_2 = SOIL_load_OGL_texture
				   (
					   "res/plane_menu.jpg",
					   SOIL_LOAD_RGBA,
					   SOIL_CREATE_NEW_ID,
					   SOIL_FLAG_NTSC_SAFE_RGB
				   );
			i_2=1;
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d_2);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-178.0f, -100.0f);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(178.0f, -100.0f);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(178.0f, 100.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-178.0f, 100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		draw_menu_text();
		drawfps();
		glutSwapBuffers();
	}
	if(page==23)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//load .jpg image
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if(i_23==0)
		{
			tex_2d_23 = SOIL_load_OGL_texture
				    (
					    "res/scene2.png",
					    SOIL_LOAD_RGBA,
					    SOIL_CREATE_NEW_ID,
					    SOIL_FLAG_NTSC_SAFE_RGB
				    );
			i_23=1;
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d_23);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-178.0f, -100.0f);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(178.0f, -100.0f);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(178.0f, 100.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-178.0f, 100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		draw_high_text();
		glutSwapBuffers();
	}
	if(page==32)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		select_scene();
		if (i_s == 0)
		{
			tex_2d = SOIL_load_OGL_texture
				 (
					 scene,
					 SOIL_LOAD_RGBA,
					 SOIL_CREATE_NEW_ID,
					 SOIL_FLAG_NTSC_SAFE_RGB
				 );
			i_s = 1;
		}

		glBindTexture(GL_TEXTURE_2D, tex_2d);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-178.0f, -100.0f);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(178.0f, -100.0f);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(178.0f, 100.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-178.0f, 100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		draw_chScene_text();

		glutSwapBuffers();
	}
	if(page==3)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if (i_bck == 0)
		{
			tex_2d = SOIL_load_OGL_texture
				 (
					 scene,
					 SOIL_LOAD_RGBA,
					 SOIL_CREATE_NEW_ID,
					 SOIL_FLAG_NTSC_SAFE_RGB
				 );
			i_bck = 1;
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-190.0f+x, -100.0f);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(890.0f+x, -100.0f);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(890.0f+x, 100.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-190.0f+x, 100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//fuel indicator outline
		glColor3f(0,0,0);
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glVertex2f(-50-100,80);
		glVertex2f(50-100,80);
		glVertex2f(50-100,70);
		glVertex2f(-50-100,70);
		glEnd();

		//fuel indicator
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2f(-49-100,79);
		glVertex2f(-49+fuel-100,79);
		glVertex2f(-49+fuel-100,71);
		glVertex2f(-49-100,71);
		glEnd();

		//seprator--to seprate score and game screen
		glLineWidth(1);
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		glVertex2f(-200,64);
		glVertex2f(200,64);
		glEnd();

		draw_score();

		draw_rockets();

		glPushMatrix();
		glTranslatef(-130,y_pos,0);
		glRotatef(theta,0,0,1);
		plane1.draw_plane();
		glPopMatrix();
		drawfps();
		glutSwapBuffers();
	}
	if(page==4)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//load .png image
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if(i_fin4==0)
		{
			tex_2d_4 = SOIL_load_OGL_texture
				   (
					   "res/finish.png",
					   SOIL_LOAD_RGBA,
					   SOIL_CREATE_NEW_ID,
					   SOIL_FLAG_NTSC_SAFE_RGB
				   );
			i_fin4=1;
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d_4);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-178.0f, -100.0f);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(178.0f, -100.0f);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(178.0f, 100.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-178.0f, 100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		//drawText();
		draw_fin_text();
		glutSwapBuffers();
	}

}
void calculateFPS()
{
	//  Increase frame count
	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	//  Calculate time passed
	int timeInterval = currentTime - previousTime;

	if(timeInterval > 1000)
	{
		//  calculate the number of frames per second
		fps = frameCount / (timeInterval / 1000.0f);
		//  Set time
		previousTime = currentTime;
		//  Reset frame count
		frameCount = 0;
	}
}
void TimerFunction(int v)
{
	calculateFPS();
	if(page==3)
	{
		if(fuel==98||x<-700)
			x=0;
                x-=0.3*SPEED;
		cout<<x<<endl;
		if(missile_x<-210)
			missile_x=250;
		missile_x-=1*SPEED;
		if(state==UP)
		{
			if(fuel>0)
				fuel-=.1*SPEED;
		}

		dist+=0.1*SPEED;

		//update number of missiles douged
		{
			if(missile_x<-90)
			{
				if(update_mis==0)
				{
					missiles+=no_of_missiles;
					update_mis=1;
				}
			}
			else
				update_mis=0;
		}

		//plane position
		if(state==UP)
		{
			if(fuel>0)
			{
				if(y_pos<=90)
					SPEED*y_pos++;
				if(theta<0)
					theta+=.3*SPEED;
				else
					theta+=.1*SPEED;
			}
			else
				y_pos--*SPEED;
		}
		else
		{
			if(y_pos>=-100)
				SPEED*y_pos--;
			else
			{
				y_pos=0;
				page=4;
			}
			if(theta>0)
				theta-=.3*SPEED;
			else
				theta-=.1*SPEED;

		}

		//check for collision
		if(missile_x<-110)
		{
			cout<<"Possibility of crash"<<endl;
			for(int m=1; m<=no_of_missiles; m++)
			{
				if(missile_y[m]>y_pos-10 && missile_y[m]<y_pos+10)
				{
					cout<<"Crash"<<endl;
					hit_missile=1;
					for(int m1=1; m1<=no_of_missiles; m1++)
						missile_x=200;
					y_pos=0;
					x=0;
					page=4;
					break;
				}
			}

		}
	}

	glutPostRedisplay();
	glutTimerFunc(1000/v,TimerFunction, v);
}

void myinit(void)
{
	glClearColor(0.0f, 0.8f, 0.0f, 1.0f);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}

void Resize(int w, int h)
{
	GLfloat aspectRatio;

	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho (-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho (-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,450);
	glutCreateWindow("SkyRider!");
	//glutPositionWindw(320,150);
	glutFullScreen();
	myinit();
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glutMouseFunc(Mouse);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(100, TimerFunction, frames);
	glutReshapeFunc(Resize);
	glutMainLoop();
	return 0;
}
