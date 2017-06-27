#include "stdafx.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

enum playType{ONCE=0x00,LOOP=0x01,REVERSE=0x02};

void drawSprite(GLuint *texture,float x1,float y1,float x2,float y2,float tx1,float ty1,float tx2,float ty2)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,*texture);
	glBegin(GL_QUADS);
		glTexCoord2f(tx1,ty2);	glVertex2f(x1,y1);
		glTexCoord2f(tx1,ty1);	glVertex2f(x1,y2);
		glTexCoord2f(tx2,ty1);	glVertex2f(x2,y2);
		glTexCoord2f(tx2,ty2);	glVertex2f(x2,y1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	
}

GLuint loadTexture(char* filename)
{
	GLuint texture;
	int sX;
	int sY;
	int ChN;
	unsigned char *img = stbi_load(filename,&sX,&sY,&ChN,4);
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,sX,sY,0,GL_RGBA,GL_UNSIGNED_BYTE,img); 
	return texture;
}

GLuint loadTextureL(char* filename)
{
	GLuint texture;
	int sX;
	int sY;
	int ChN;
	unsigned char *img = stbi_load(filename,&sX,&sY,&ChN,4);
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,sX,sY,0,GL_RGBA,GL_UNSIGNED_BYTE,img); 
	return texture;
}

class animation
{
	private:
		playType play;
		GLuint *texture;
		unsigned timer;
		unsigned delay;
		unsigned char x;
		unsigned char y;
		unsigned char rows;
		unsigned char colums;
		unsigned char x0;
		unsigned char y0;
		float sizeX;
		float sizeY;
		bool stop;
	public:
		animation(
			GLuint *iTexture=0, 
			float iSizeX=1, 
			float iSizeY=1, 
			unsigned char  iX0=0, 
			unsigned char  iY0=0, 
			unsigned char  iRows=1, 
			unsigned char  iColums=1, 
			unsigned iDelay=0,
			playType iPlay=ONCE
		)
		{
			play=iPlay;
			texture=iTexture;
			timer=iDelay;
			delay=iDelay;
			x0=iX0;
			y0=iY0;
			x=0;
			y=0;
			sizeX=iSizeX;
			sizeY=iSizeY;
			rows=iRows;
			colums=iColums;
			if (delay!=0) {stop=false;}
			else {stop=true;}
		}
		
		animation(
			GLuint *iTexture, 
			float iSize, 
			unsigned char  iX0, 
			unsigned char  iY0, 
			unsigned char  iRows, 
			unsigned char  iColums, 
			unsigned iDelay,
			playType iPlay
		)
		{
			play=iPlay;
			texture=iTexture;
			delay=iDelay;
			timer=delay;
			x0=iX0;
			y0=iY0;
			x=0;
			y=0;
			sizeX=iSize;
			sizeY=iSize;
			rows=iRows;
			colums=iColums;
			if (delay!=0) {stop=false;}
			else {stop=true;}
		}
		
		unsigned draw(char dir,float x1,float y1, float x2, float y2)
		{
				float fx,fx1,fy,fy1;
				if (dir>0) {fx=(x+x0)*sizeX;fx1=fx+sizeX;}
				else {fx1=(x+x0)*sizeX,fx=fx1+sizeX;}
				fy=(y+y0)*sizeY;
				fy1=fy+sizeY;
				const float by=0.003;
				float bx=by*dir;
				drawSprite(texture,x1,y1,x2,y2,fx,fy+by,fx1-bx,fy1);
				if (!stop)
				{
					if (timer==0)
					{
						timer=delay;
						x++;y++;
						x=x%colums;
						y=y%rows;
						if((play!=LOOP)&&(x==0)&&(y==0)) {stop=true;}
					}
					else {timer--;};
					
				}
				glColor3f(1,1,1);
				return x;
				
		}
		
		inline void setCol(unsigned col)
		{
			x=col;
		}
		
		inline void setRow(unsigned rou)
		{
			y=rou;
		}
		
		inline void freeze()
		{
			stop=true;
		}
		
		inline void unfreeze()
		{
			stop=false;
		}
		
		inline bool is_end()
		{
			return stop;
		}
		
};
