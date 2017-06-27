/**************************
 * Includes
 *
 **************************///

#include "stdafx.h"
#include "entitys.h"
unsigned int key;

bool keyJump=false;
char dir=1;

/**************************.
 * Function Declarations
 *
 **************************/
void drawNumber(GLuint* tex, int n, float x,float y)
{
	float shi=0;
	do{
		short t=n%10;
		n=n/10;
		short i=t/4;
		short j=t%4;
		drawSprite(tex,x-shi,y,x+0.5-shi,y+0.5,j*0.25,i*0.25,j*0.25+0.25,i*0.25+0.25);
		shi+=0.5;
	}while(n);
}



LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);//
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

 
int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{//
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    
    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "Арканойд", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 1024, 768,
      NULL, NULL, hInstance, NULL);



    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
	//testdraw();
	//DrawTile(0,1,0,0,Area3.get_texture());r//
	
	GLuint texBrick=loadTextureL("brick.tga");
	const char wallH=8;
	const char wallW=18;
	brick wall[wallH][wallW];
	for(char i=0;i<wallH;i++)for(char j=0;j<wallW;j++)
	{
		wall[i][j]=brick(texBrick,j,i);
	}
	
	ball theBall(5.5,5.5);
	theBall.loadTexture("ball.tga");
	
	GLuint texBat=loadTexture("bat.tga");
	bat theBat(texBat);
	theBall.putOn(&theBat);
	dir=0;
	int score=0;
	int lives=3;
	GLuint texBack=loadTextureL("BackGround.bmp");
	
	GLuint texFont=loadTextureL("font.tga");
    /* program main loop */
    while (!bQuit)//
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        
        {
            /* OpenGL animation code goes here */
            drawSprite(&texBack,0,0,32,24,0,0,1,0.75);
            
        	if (keyJump) theBall.release();
        	bool empty=true;
        	
            for(char i=0;i<wallH;i++)for(char j=0;j<wallW;j++)
            {
            	if(!wall[i][j].none) {wall[i][j].draw();empty=false;};
            }
            if(lives<=0)
            {
            	 score=0;
            	 lives=3;
            	 empty=true;
        	}
            //restore bricks if there are non
            if(empty) for(char i=0;i<wallH;i++)for(char j=0;j<wallW;j++)
            {
            	wall[i][j].none=false;
            }
            
            
            //checking collisions with near bricks
            int N,S,W,E;//N and S - low and high Y, W and E - low and high X
			{
				float x,y,rx,ry;
            	theBall.getHitbox(&x,&y,&rx,&ry);
            	E=floor(x+rx);
            	W=floor(x-rx);
            	N=floor(y+ry);
            	S=floor(y-ry);
        	}
        	bool hit=false;
        	if((N<=19)&&(N>=11))
        	{
				hit=theBall.hit(&wall[19-N][E-2]);
				if(!hit)hit=theBall.hit(&wall[19-N][W-2]);//if not hit, try with another
        				
			}
			if((S<=19)&&(S>=11))
			{
				if(!hit)hit=theBall.hit(&wall[19-S][E-2]); //if not hit, try with another
        		if(!hit)hit=theBall.hit(&wall[19-S][W-2]);
			}
			if(hit)score+=100;
			theBall.hit(&theBat);
            ;
            bool fall=false;
            theBall.move(&fall);
            if(fall)lives--;
            theBat.move(dir);
            
            theBall.draw();
            drawNumber(&texFont,score,30,20.3);
            drawNumber(&texFont,lives,30,16.3);
            theBat.draw();
            SwapBuffers (hDC);
            Sleep (10);
        }
    }


    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/
///
LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;
	
	case WM_KEYUP: 
		switch (wParam)
    	{
            		//case 0x41
        case VK_LEFT:
				dir=0;
        		return 0;
          		  	//case 0x44:
        case VK_RIGHT://
         		dir=0;
        		return 0;
        case VK_UP:
            	keyJump=false;
       			return 0;
		};
    case WM_KEYDOWN:
        switch (wParam)//
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        case VK_LEFT:
			dir=-1;
            return 0;
            		//case 0x44:..
        case VK_RIGHT://
            dir=1;
        	return 0;
    	case VK_UP:
			keyJump=true;
            return 0;
        }	
        return 0;//

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}
//

/*******************
 * Enable OpenGL
 
 *******************///

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC *///
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);
    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
	glLoadIdentity(); 
	gluOrtho2D(0,32,0,24);
	glMatrixMode(GL_PROJECTION); 
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

};
 
/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}
