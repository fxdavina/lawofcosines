//http://www.cppgameprogramming.com/cgi/nav.cgi?page=allegprimitive

#include <allegro.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ScreenPolygon.h"
#include "Triangle.h"
#include "Proof.h"

/** 
    All these drawing functions draw to the "screen" directly. 
    This gets pretty slow if you do it too many times.a
    drawing to a buffer and then writing the buffer to the screen is
       more efficient.
*/

//prototype

using namespace std;

void getMouseInfo();
void getRightMouse();
void getMouseDrag(int& x, int& y);
void Show();
int pointclick(int x1, int y1, int x2, int y2, int x3, int y3);
int rpointclick(int x1, int y1, int x2, int y2, int x3, int y3);
void Scale();
void UpdateTriangle(Polygon* poly);
void reflector(const Polygon* object);
void Animation();
void Center();
void Background();

//global
BITMAP *buffer = NULL;
Triangle* base;
Triangle* reflection;
int cursor_x = 0;
int cursor_y = 0;
int cursor_r_x = 0;
int cursor_r_y = 0;

int x1(400);
int y_1(450);
int x2(240);
int y2(400);
int x3(335);
int y3(310);
int CenterX(0);
int CenterY(0);
const int MiddleX(320);
const int MiddleY(370);

int main()
{
 	double baseangle;
	allegro_init();
    install_keyboard();
    install_mouse();
    set_mouse_speed(10,10);
    set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1280, 640, 0, 0);
    
    base = new Triangle(Point(x1,y_1),Point(x2,y2),Point(x3,y3));
    reflector(base);
    
    buffer = create_bitmap(1280,640);
    Center();
    Show();
    show_mouse(screen);
	
	while(!key[KEY_ESC])
	{	
        getMouseInfo();
		if(pointclick(x1, y_1, x2, y2, x3, y3)==1)
		{
	      getMouseDrag(x1,y_1);	
		}
		if(pointclick(x1, y_1, x2, y2, x3, y3)==2)
		{
	      getMouseDrag(x2,y2);	
		}
		if(pointclick(x1, y_1, x2, y2, x3, y3)==3)
		{    
			getMouseDrag(x3,y3);	
		}	  
		
		if(key[KEY_A])
		{

              Animation();
          	  cursor_x = 0;
          	  cursor_y = 0;
          	  rest(400);
      	}
      	
      	getRightMouse();
        if(rpointclick(x1, y_1, x2, y2, x3, y3)==1)
        {
			if(y_1 >= y2 && y_1 >= y3) //top
			{
				if(x2 < x3)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x2,y2),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(x3 < x2)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x3,y3),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
			else if(y_1 <= y2 && y_1 <= y3) //bottom
			{
				if(x2 < x3)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x3,y3),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(x3 < x2)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x2,y2),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}			
			else if(x1 >= x2 && x1 >= x3)  //right
			{
				if(y2 < y3)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x3,y3),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y3 < y2)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x2,y2),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
			else if(x1 <= x2 && x1 <= x3)  //left
			{
				cout <<"left";
                if(y2 < y3)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x2,y2),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y3 < y2)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x3,y3),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
			else   //middle Y
			{
				if(y_1 < y3)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x2,y2),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y_1 < y2)
				{
					Triangle* temp = new Triangle(Point(x1,y_1),Point(x3,y3),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
        }
      	else if(rpointclick(x1, y_1, x2, y2, x3, y3)==2)
        {
			if(y2 >= y_1 && y2 >= y3) //top
			{
				if(x1 < x3)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x1,y_1),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(x3 < x1)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x3,y3),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
			else if(y2 <= y_1 && y2 <= y3) //bottom
			{
				if(x1 < x3)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x3,y3),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(x3 < x1)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x1,y_1),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}			
			else if(x2 >= x1 && x2 >= x3)  //right
			{
				if(y_1 < y3)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x3,y3),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y3 < y_1)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x1,y_1),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}			
			else if(x2 <= x1 && x2 <= x3)  //left
			{
				if(y_1 < y3)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x1,y_1),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y3 < y_1)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x3,y3),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
			else   //middle
			{
				if(y2 < y3)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x1,y_1),Point(x3,y3));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y2 < y_1)
				{
					Triangle* temp = new Triangle(Point(x2,y2),Point(x3,y3),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
        }
        if(rpointclick(x1, y_1, x2, y2, x3, y3)==3)
        {
			if(y3 >= y_1 && y3 >= y2) //top
			{
				if(x1 < x2)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x1,y_1),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(x2 < x1)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x2,y2),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
			else if(y3 <= y_1 && y3 <= y2) //bottom
			{
				if(x1 < x2)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x2,y2),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(x2 < x1)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x1,y_1),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}
			else if(x3 >= x1 && x3 >= x2)  //right
			{
				if(y_1 < y2)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x2,y2),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y2 < y_1)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x1,y_1),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			}			
			else if(x3 <= x1 && x3 <= x2)  //left
			{
				if(y_1 < y2)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x1,y_1),Point(x2,y2));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
				else if(y2 < y_1)
				{
					Triangle* temp = new Triangle(Point(x3,y3),Point(x2,y2),Point(x1,y_1));
					delete base;
					base = temp;
					baseangle = temp->Angle(0);
				}
			} 
        }
      	
        x1=int(base->Element(0).X);
	    y_1=int(base->Element(0).Y);
	    x2=int(base->Element(1).X);
	    y2=int(base->Element(1).Y);
	    x3=int(base->Element(2).X);
	    y3=int(base->Element(2).Y);
    	          if(x1 == x2 && x2 == x3 && x3 == x1)
          {
                
          }
          else if(y_1 == y2 && x2 == y3 && y3 == y_1)
          {
          }
          else
          {
		Center();
        Scale();
		UpdateTriangle(base);
		reflector(base);
        Show();
        }
	}
	
	    

    destroy_bitmap(buffer);
    return 0;
    
}

END_OF_MAIN();

void getMouseInfo()
{
  if(mouse_b & 1)
  {
   cursor_x = mouse_x;
   cursor_y = mouse_y;
  }
}

void getRightMouse()
{
  if(mouse_b & 2)
  {
    cursor_r_x = mouse_x;
    cursor_r_y = mouse_y;
  }
}


void getMouseDrag(int& x, int& y)
{
  while(mouse_b & 1)
  {
  	x = mouse_x;
	  y = mouse_y;
    if (x > 640) x = 640;
    if (y < 75) y = 75;
    UpdateTriangle(base);
    reflector(base);
    Show();	
  }
}

void UpdateTriangle(Polygon* poly)
{
    delete poly;
    poly = new Triangle(Point(x1,y_1),Point(x2,y2),Point(x3,y3));
} 

void Show()
{
    double baseangle;
    int centerTriX(0);
    int centerTriY(0);
    clear(buffer);
    int pMinY ;
    int rMinY;
    int diff;
   	clear_to_color( buffer, makecol( 255, 255, 255));
   	line(buffer,0, 75, 1280, 75, makecol(0,0, 0));
   	
    line(buffer,640, 75, 640, 640, makecol(0,0, 0));
    
    Triangle* temp = (Triangle*)base;
    baseangle = temp->Angle(0); 
    if(baseangle < 90)
    {
      Proof P(LEFT_ACUTE, *temp);
      Proof R(RIGHT_ACUTE,*reflection); //RIGHT_ACUTE,*reflection);
      pMinY = P.MinY();
      rMinY = R.MinY();
      R.Translate(0, pMinY - rMinY);
      
      P.Shape(1).Draw(buffer);
      P.Shape(2).Draw(buffer);
      P.Shape(3).Draw(buffer);
      P.Shape(4).Draw(buffer);
      P.Shape(0).Draw(buffer);
      textout_ex( buffer, font, "Base", CenterX - 20, CenterY,
                  makecol( 255, 255, 255), makecol( 0, 0, 255));
      textout_ex( buffer, font, "A^2", int((P.Shape(1).Shape().Element(0).X +
                  P.Shape(1).Shape().Element(2).X)/2)- 10,
                  int((P.Shape(1).Shape().Element(0).Y +
                  P.Shape(1).Shape().Element(2).Y)/2),
                  makecol( 0, 0, 0), P.Shape(1).Color);
      textout_ex( buffer, font, "B^2", int((P.Shape(2).Shape().Element(0).X +
                  P.Shape(2).Shape().Element(2).X)/2) - 10,
                  int((P.Shape(2).Shape().Element(0).Y +
                  P.Shape(2).Shape().Element(2).Y)/2),
                  makecol( 0, 0, 0), P.Shape(2).Color);
      centerTriX = int((P.Shape(3).Shape().Element(1).X + P.Shape(3).Shape().Element(0).X)/2);
      centerTriY = int((P.Shape(3).Shape().Element(1).Y + P.Shape(3).Shape().Element(2).Y)/2);
      textout_ex( buffer, font, "Base", centerTriX, centerTriY,
                  makecol( 0, 0, 0), P.Shape(3).Color);
      
      
      centerTriX = int((P.Shape(4).Shape().Element(1).X + P.Shape(3).Shape().Element(0).X)/2);
      centerTriY = int((P.Shape(4).Shape().Element(1).Y + P.Shape(3).Shape().Element(2).Y)/2);
      textout_ex( buffer, font, "Base", centerTriX, centerTriY,
                  makecol( 0, 0, 0), P.Shape(4).Color);
                  
      R.Shape(1).Draw(buffer);
      R.Shape(2).Draw(buffer);
      R.Shape(3).Draw(buffer);
      R.Shape(4).Draw(buffer);
      R.Shape(5).Draw(buffer);
      R.Shape(0).Draw(buffer);
    }
    if(baseangle > 90)
    {
      Proof P(LEFT_OBTUSE, *base);
      Proof R(RIGHT_OBTUSE,*reflection);
      
      P.Shape(1).Draw(buffer);
      P.Shape(2).Draw(buffer);
      P.Shape(3).Draw(buffer);
      P.Shape(4).Draw(buffer);
      P.Shape(5).Draw(buffer);
      P.Shape(0).Draw(buffer);
      R.Shape(0).Draw(buffer);
      R.Shape(1).Draw(buffer);
      R.Shape(2).Draw(buffer);
      
    }

    
    //triangle(buffer, x1, y_1 , x2, y2, x3, y3, makecol(0, 0, 255));
	circlefill( buffer, x1, y_1, 5, makecol( 0,0, 0));
   	circlefill( buffer, x2, y2, 5, makecol( 0,0, 0));
   	circlefill( buffer, x3, y3, 5, makecol( 0,0, 0));
   	//Center circlefill( buffer, CenterX, CenterY, 2, makecol( 0,0, 0));
   	//Triangle Window   rect( buffer, 220, 270, 420, 470, makecol( 0, 0, 255));	
        textout_ex( buffer, font, "C", x1 - 3, y_1 - 3,
                makecol( 0, 255, 0), makecol( 0, 0, 0) );
      textout_ex( buffer, font, "B", x2 - 3, y2 -3 ,
                makecol( 0, 255, 0), makecol( 0, 0, 0) );
      textout_ex( buffer, font, "A", x3 - 3, y3 - 3,
                makecol( 0, 255, 0), makecol( 0, 0, 0) );


	
	//Reflection
	circlefill( buffer, int(reflection->Element(0).X), int(reflection->Element(0).Y), 5, makecol( 0,0, 0));
   	circlefill( buffer, int(reflection->Element(1).X), int(reflection->Element(1).Y), 5, makecol( 0,0, 0));
  	circlefill( buffer, int(reflection->Element(2).X), int(reflection->Element(2).Y), 5, makecol( 0,0, 0));
  	textout_ex( buffer, font, "C", int(reflection->Element(0).X), int(reflection->Element(0).Y),
                makecol( 0, 255, 0), makecol( 0, 0, 0) );
      textout_ex( buffer, font, "B", int(reflection->Element(1).X), int(reflection->Element(1).Y),
                makecol( 0, 255, 0), makecol( 0, 0, 0) );
      textout_ex( buffer, font, "A", int(reflection->Element(2).X), int(reflection->Element(2).Y),
                makecol( 0, 255, 0), makecol( 0, 0, 0) );
 //   triangle(buffer, x1+640, y_1 , x2+640, y2, x3+640, y3, makecol(0, 0, 255));
	
	//Text Box
	line(buffer,0, 75, 1280, 75, makecol(0,0, 0));
    textout_ex( buffer, font, "Triangle: ", 0, 0,
                makecol( 0, 0, 0), makecol( 255, 255, 255) );
                
    rect( buffer, 1150, 40, 1250, 70, makecol( 0, 0, 0));
    textout_centre_ex( buffer, font, "ANIMATE", 1200, 52, makecol(0,0,0), makecol(255,255,255) );
    
		//A
    textprintf_ex( buffer, font, 80,0, makecol(0, 0, 0),
                   makecol(255, 255, 255),"A: (%-3d,", x1);
    textprintf_ex( buffer, font, 150,0, makecol(0, 0, 0),
                   makecol(255, 255, 255), "%-3d)", y_1);
    baseangle = temp->Angle(0);
    textprintf_ex( buffer, font, 190,0, makecol(0, 0, 0),
                   makecol(255, 255, 255), "Angle: %d", int(baseangle));
    textprintf_ex( buffer, font, 280,0, makecol(0, 0, 0),
                   makecol(255, 255, 255), "A->B: %-3d", 
                   int(distance(x1,y_1,x2,y2)));
	//B
    textprintf_ex( buffer, font, 80,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "B: (%-3d,", x2);
    textprintf_ex( buffer, font, 150,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "%-3d)", y2);
    textprintf_ex( buffer, font, 190,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "Angle: %-3d", int(base->Angle(1)));
    textprintf_ex( buffer, font, 280,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "B->C: %-3d", 
                   int(distance(x2,y2,x3,y3)));
	//C
    textprintf_ex( buffer, font, 80,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "C: (%-3d,", x3);
    textprintf_ex( buffer, font, 150,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "%-3d)", y3);
    textprintf_ex( buffer, font, 190,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "Angle: %-3d", int(base->Angle(2)));
    textprintf_ex( buffer, font, 280,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "C->A: %-3d", 
                   int(distance(x1,y_1,x3,y3)));
	
	draw_sprite( screen, buffer, 0, 0);
}

int pointclick(int x1, int y_1, int x2, int y2, int x3, int y3)
{
  if(cursor_x <= x1+10 && cursor_x >= x1-10 && cursor_y <= y_1+10 && cursor_y >= y_1-10)
    return 1;
  if(cursor_x <= x2+10 && cursor_x >= x2-10 && cursor_y <= y2+10 && cursor_y >= y2-10)
    return 2;
  if(cursor_x <= x3+10 && cursor_x >= x3-10 && cursor_y <= y3+10 && cursor_y >= y3-10)
    return 3;
    
  return 0;
}

int rpointclick(int x1, int y_1, int x2, int y2, int x3, int y3)
{
  if(cursor_r_x <= x1+10 && cursor_r_x >= x1-10 && cursor_r_y <= y_1+10 && cursor_r_y >= y_1-10)
    return 1;
  if(cursor_r_x <= x2+10 && cursor_r_x >= x2-10 && cursor_r_y <= y2+10 && cursor_r_y >= y2-10)
    return 2;
  if(cursor_r_x <= x3+10 && cursor_r_x >= x3-10 && cursor_r_y <= y3+10 && cursor_r_y >= y3-10)
    return 3;
    
  return 0;
}

void Animation()
{
   double baseangle = base->Angle(0);
	 PROOF_TYPE Ptype;
	 PROOF_TYPE Rtype;
   if(baseangle < 90)
	 {
    Ptype = LEFT_ACUTE;
    Rtype = RIGHT_ACUTE;
  }
	if(baseangle > 90)
	{
    Ptype = LEFT_OBTUSE;
    Rtype = RIGHT_OBTUSE;
  }
	Proof P(Ptype, *base);
  Proof R(Rtype,*reflection);
  int pMinY = P.MinY();
  int rMinY = R.MinY();
  R.Translate(0, pMinY - rMinY);
	
	if(baseangle < 90)
    {
      P.Shape(1).Draw(buffer);
      P.Shape(2).Draw(buffer);
      P.Shape(3).Draw(buffer);
      P.Shape(4).Draw(buffer);
      P.Shape(0).Draw(buffer);
      R.Shape(1).Draw(buffer);
      R.Shape(2).Draw(buffer);
      R.Shape(3).Draw(buffer);
      R.Shape(4).Draw(buffer);
      R.Shape(5).Draw(buffer);
      R.Shape(0).Draw(buffer);
    }
    if(baseangle > 90)
    {
      P.Shape(1).Draw(buffer);
      P.Shape(2).Draw(buffer);
      P.Shape(3).Draw(buffer);
      P.Shape(4).Draw(buffer);
      P.Shape(5).Draw(buffer);
      P.Shape(0).Draw(buffer);
      R.Shape(0).Draw(buffer);
      R.Shape(1).Draw(buffer);
      R.Shape(2).Draw(buffer);
      
    }
    
   	double PminX = P.MinX();
	  double PminY = P.MinY();
	  double RminX = R.MinX();
	  double RminY = R.MinY();
	  
   	while(PminX<=RminX)
   	{
  	  clear(buffer);
   	  clear_to_color( buffer, makecol( 255, 255, 255));
   	  line(buffer,0, 75, 1280, 75, makecol(0,0, 0));
      
    if(PminX<=RminX)
	  {
	    P.Translate(1,0);
		  R.Translate(-1,0);
	  }
    if(baseangle < 90)
	  {
      P.Shape(1).Draw(buffer);
      P.Shape(2).Draw(buffer);
      P.Shape(3).Draw(buffer);
      P.Shape(4).Draw(buffer);
      P.Shape(0).Draw(buffer);
      R.Shape(1).Draw(buffer);
      R.Shape(2).Draw(buffer);
      R.Shape(3).Draw(buffer);
      R.Shape(4).Draw(buffer);
      R.Shape(5).Draw(buffer);
      R.Shape(0).Draw(buffer);	
   }
	
	if(baseangle > 90)
	{
   	  P.Shape(1).Draw(buffer);
      P.Shape(2).Draw(buffer);
      P.Shape(3).Draw(buffer);
      P.Shape(4).Draw(buffer);
      P.Shape(5).Draw(buffer);
      P.Shape(0).Draw(buffer);
      R.Shape(0).Draw(buffer);
      R.Shape(1).Draw(buffer);
      R.Shape(2).Draw(buffer);	
	}
	
	PminX++;
	PminY = P.MinY();
	RminX--;
	RminY = R.MinY();
	//Text Box
	line(buffer,0, 75, 1280, 75, makecol(0,0, 0));
    textout_ex( buffer, font, "Triangle: ", 0, 0,
                makecol( 0, 0, 0), makecol( 255, 255, 255) );
		//A
    textprintf_ex( buffer, font, 80,0, makecol(0, 0, 0),
                   makecol(255, 255, 255),"A: (%-3d,", x1);
    textprintf_ex( buffer, font, 150,0, makecol(0, 0, 0),
                   makecol(255, 255, 255), "%-3d)", y_1);
    baseangle = base->Angle(0);
    textprintf_ex( buffer, font, 190,0, makecol(0, 0, 0),
                   makecol(255, 255, 255), "Angle: %d", int(baseangle));
    textprintf_ex( buffer, font, 280,0, makecol(0, 0, 0),
                   makecol(255, 255, 255), "A->B: %-3d", 
                   int(distance(x1,y_1,x2,y2)));
	//B
    textprintf_ex( buffer, font, 80,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "B: (%-3d,", x2);
    textprintf_ex( buffer, font, 150,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "%-3d)", y2);
    textprintf_ex( buffer, font, 190,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "Angle: %-3d", int(base->Angle(1)));
    textprintf_ex( buffer, font, 280,10, makecol(0, 0, 0),
                   makecol(255, 255, 255), "B->C: %-3d", 
                   int(distance(x2,y2,x3,y3)));
	//C
    textprintf_ex( buffer, font, 80,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "C: (%-3d,", x3);
    textprintf_ex( buffer, font, 150,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "%-3d)", y3);
    textprintf_ex( buffer, font, 190,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "Angle: %-3d", int(base->Angle(2)));
    textprintf_ex( buffer, font, 280,20, makecol(0, 0, 0),
                   makecol(255, 255, 255), "C->A: %-3d", 
                   int(distance(x1,y_1,x3,y3)));	
    draw_sprite( screen, buffer, 0, 0);
    rest(5);
    }
}

void Scale()
{
    int DistX,
        DistY;
        
        
    double scale(1);
    //270, 270, 470, 470
    while((x1 < 220 || x1 > 420) || (y_1 < 270 || y_1 > 470) || 
          (x2 < 220 || x2 > 420) || (y2 < 270 || y2 > 470) || 
          (x3 < 220 || x3 > 420) || (y3 < 270 || y3 > 470) )
    {      
        DistX = CenterX - MiddleX;
        DistY = CenterY - MiddleY;
        CenterX = MiddleX;
        CenterY = MiddleY;
        x1 = x1 - DistX;
        y_1 = y_1 - DistY;
        x2 = x2 - DistX;
        y2 = y2- DistY;
        x3 = x3 - DistX;
        y3 = y3 - DistY;
        scale = scale * .99; 
        x1 = int(MiddleX + (x1 - MiddleX) * scale) ;
        y_1 = int(MiddleY + (y_1 - MiddleY) * scale);
        x2 = int(MiddleX + (x2 - MiddleX) * scale);
        y2 = int(MiddleY + (y2 - MiddleY) * scale);
        x3 = int(MiddleX + (x3 - MiddleX) * scale);
        y3 = int(MiddleY + (y3 - MiddleY) * scale);
    }
}

void Center()
{
    int TempX,
        TempY,
        MidpointX,
        MidpointY;
    
    CenterX = (x1 + x2 + x3) / 3;
    CenterY = (y_1 + y2 + y3) / 3;
        
    
}

void reflector(const Polygon* object)
{
  delete reflection;
  reflection = new Triangle(Point(object->Element(0).X+640,object->Element(0).Y+110),
                            Point(object->Element(1).X+640,object->Element(1).Y+110),
                            Point(object->Element(2).X+640,object->Element(2).Y + 110));
}
/*
    putpixel( buffer, 5, 5, makecol( 128, 200, 23));

    
    //draw the outer circle first, then a filled circle.  
    circle( buffer, 20, 20, 10, makecol( 255, 0, 0));
    circlefill( buffer, 50, 50, 25, makecol( 0,255, 0));

    rect( buffer, 70, 70, 90, 90, makecol( 0, 0, 255));
    rectfill( buffer, 100, 100, 120, 120, makecol( 12, 34, 200));

    line( buffer, 130, 130, 150, 150, makecol( 255, 0, 0));
    line( buffer, 130, 130, 170, 130, makecol( 255, 0, 0));
    line( buffer, 170, 130, 150, 150, makecol( 255, 0, 0));
    
    //paint bucket! fill area starting at 140,135
    floodfill( buffer, 140, 135, makecol( 255, 255, 0));


    
    triangle( buffer, 200, 200, 200, 220, 220, 210, makecol( 213, 79, 40)); 
    
    draw_sprite( screen, buffer, 0, 0);
*/
