#include "animation.h"

#include "stdafx.h"
const float borderW = 2;
const float borderS = 1;
const float borderE = 20;
const float borderN = 23;

class entity
{
	protected:
		float x,y;
		float rx,ry;
		float tsx,tsy; //texture radius
		float tx1,ty1;
		float tx2,ty2;
		GLuint texture;
	public:
		void loadTexture(char* filename){texture=loadTextureL(filename);}
		inline void getHitbox(float* xout,float* yout,float* rxout,float* ryout);
		virtual void draw();
		entity(float xin=0.5,float yin=0.5)
		{
			tx1=0;tx2=1;ty1=0;ty2=1;
			x=xin;
			y=yin;
			tsx=0.5;
			tsy=0.5;
			rx=0.5;
			ry=0.5;
		}
};



////-------------------------------------------------------------------
void entity::draw()
{
	drawSprite(&texture,x-tsx,y-tsy,x+tsx,y+tsy,tx1,ty1,tx2,ty2);
}	
//-----------------------------------------------------------------------------
void inline entity::getHitbox(float* xout,float* yout,float* rxout,float* ryout)
{
			*xout=x;
			*yout=y;
			*rxout=rx;
			*ryout=ry;
}
///-------------------------------------------------

//----------------------------------------------------
class brick:public entity
{
	public:
		bool none;//no brick now. Coming soon
		brick(GLuint tex=0,int nx=0,int ny=0);
};
//----------realisation---------------------------

brick::brick(GLuint tex,int nx,int ny)
{
	texture=tex;
	y=19-ny+0.5;
	x=2+nx+0.5;
	rx=0.45;
	ry=0.45;
	none=false;
}
//-------------------------------------------

//----------------------------------------------------
class subject:public entity
{
	protected:
		float dx,dy;
	public:
		virtual void move()
			{x+=dx;y+=dy;};
};
///-------------------------------------------------

class bat:public subject
{
	protected:
		animation standAnim;
		animation leftAnim;
		animation rightAnim;
		animation *curAnim;
	public:
		void move(int dir);
		void draw();
		bat(GLuint tex=0,float xin=4.5,float yin=3);		
};

//--------------implementation-----------------

bat::bat(GLuint tex,float xin,float yin)
{
	x=xin;
	y=yin;
	dx=0.2;
	standAnim=animation(&texture,0.25,0,0,1,2,0,ONCE);
	leftAnim=animation(&texture,0.25,0,1,1,3,5,LOOP);
	rightAnim=animation(&texture,0.25,0,2,1,3,5,LOOP);
	rx=1.5;
	ry=0.5;
	tsx=1.5;
	tsy=1;
	texture=tex;
}

//--------------------------------------------------

void bat::move(int dir)
{
			x+=dx*dir;
			if (dir>0) curAnim=&leftAnim;
		    else if (dir<0)curAnim=&rightAnim;
			else curAnim=&standAnim;
			if(x+rx>borderE){x=borderE-rx;return;}
			if(x-rx<borderW){x=borderW+rx;return;}
}
//-------------------------------------------------
void bat::draw()
{
	curAnim->draw(1,x-tsx,y-tsx+0.5,x+tsx,y+tsx+0.5);
}
//----------------------------------------------------
class ball:public subject
{
	protected:
		bool lying;
		entity* owner;
	public:
		bool hit(bat* thing);
		bool hit(brick* thing);
		bool hit(float x1,float y1,float rx1,float ry1);
		void move(bool *fall);
		ball(float xin=2.5,float yin=1.5);	
		void putOn(entity *thing);
		inline void release(){lying=false;};
};


//------------------Realisation------------------------
ball::ball(float xin,float yin)
{
			lying=false;
			x=xin;
			y=yin;
			rx=0.31;
			ry=0.31;
			dx=0.2;
			dy=0.2;
};

void ball::putOn(entity* thing)
{
	owner=thing;
	lying=true;
	dx=0.2;dy=0.2;
}
///-------------------------------------------------
void ball::move(bool *fall)
		{
			if (lying)
			{
				float rx1,ry1;
				owner->getHitbox(&x,&y,&rx1,&ry1);
				y+=ry+ry1-0.19;x-=rx1*0.5;
				*fall=false;
			}
			x+=dx;
			y+=dy;
			if (x+rx>borderE){x=borderE-rx;dx=-dx;}
			else if (x-rx<borderW){x=borderW+rx;dx=-dx;}
			else if (y+ry>borderN){y=borderN-ry;dy=-dy;}
			else if (y+ry<0){putOn(owner);*fall=true;}
		};
////------------------------------------		

bool ball::hit(float x1,float y1,float rx1,float ry1)
		{
			float rcx=fabs(x-x1);
			float rsumx=rx+rx1;
			if(rsumx<rcx) return false;
			float rcy=fabs(y-y1);
			float rsumy=ry+ry1;
			if(rsumy<rcy) return false;
			float olapX=rsumx-rcx; //overlap value;
			float olapY=rsumy-rcy;
			if (olapX<olapY)
			{
				x=x+olapX;
				if(dx>0){x=x-olapX;}
				dx=-dx;
			}
			else
			{
				y=y+olapY;
				if(dy>0){y=y-olapY;}
				dy=-dy;	
			}
			return true;
		}
		
bool ball::hit(brick* thing)
		{
			if(thing->none) return false;
			float x1,y1,rx1,ry1;
			thing->getHitbox(&x1,&y1,&rx1,&ry1);
			bool answer=hit(x1,y1,rx1,ry1);
			if(answer)thing->none=true;
			return answer;
		}

bool ball::hit(bat* thing)
		{
			float x1,y1,rx1,ry1;
			thing->getHitbox(&x1,&y1,&rx1,&ry1);
			bool answer=hit(x1,y1,rx1,ry1);
			return answer;
		}
//------------------------------------------------












