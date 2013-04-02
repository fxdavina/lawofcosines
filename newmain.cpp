#include <allegro.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include "ScreenPolygon.h"
#include "Triangle.h"
#include "Outline.h"
#include "Proof.h"

#define DEBUG
#ifdef DEBUG
#include <fstream>
#include <string>
#include <sstream>
void debugOut(std::string message) {
     std::ofstream out;
     out.open("debug.txt",std::ios::app);
     out << message << std::endl;
     out.close();
}

#endif


void drawInterface(BITMAP* buffer, int screen_width, int screen_height, int size, int color);
void drawTriangleInfo(BITMAP* buffer, const Outline& o);
void drawProof(BITMAP* buffer, const Proof& p);
int mouseOver(Outline& o);
Point centerObject(int minX, int minY, int maxX, int maxY, int width, int height);
double scaleObject(double maxW, double maxH, double width, double height);
void centerProof(Proof& p, Outline& constrain);
void moveOutline(Outline& o, Proof& p);
void labelIndices(Polygon* p, BITMAP* buffer);

int main() {
    
    #ifdef DEBUG
    std::ofstream out;
    out.open("debug.txt");
    out.close();
    debugOut("init");
    #endif
    
    int screen_height, screen_width;
    allegro_init();
    install_keyboard();
    install_mouse();
    set_mouse_speed(5,5);
    get_desktop_resolution(&screen_width,&screen_height);
    set_color_depth(desktop_color_depth());
    set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, screen_width, screen_height, 0, 0);
    BITMAP* buffer = create_bitmap(screen_width,screen_height);    
    show_mouse(screen);
    int screen_midX = screen_width / 2;
    int border = 100;
    int bgColor = makecol(255,255,255);
    int fgColor = makecol(0,0,0);
    bool redraw = true, rescale = true;
    int wasClicked = -1;
    Outline points(3), reflection(3), leftBox(2),rightBox(2);    
    Triangle *tri, *refl;
    Proof *leftProof, *rightProof;

    #ifdef DEBUG
    debugOut("init points");
    #endif

    points.Element(2).X = 10;
    points.Element(2).Y = 150;
    points.Element(1).X = 100;
    points.Element(1).Y = 200;
    points.Element(0).X = 200;
    points.Element(0).Y = 300;
    tri = new Triangle(points.Element(0),points.Element(1),points.Element(2));
    refl = new Triangle(*tri);
    refl->Translate(screen_midX,0);
    #ifdef DEBUG
    debugOut("init proofs");
    #endif
    leftBox.Element(0).X = 0;
    leftBox.Element(0).Y = border;
    leftBox.Element(1).X = screen_midX;
    leftBox.Element(1).Y = screen_height;
    
    rightBox.Element(0).X = screen_midX;
    rightBox.Element(0).Y = border;
    rightBox.Element(1).X = screen_width;
    rightBox.Element(1).Y = screen_height; 
    
    if(tri->Angle(0) > 90) {
            leftProof = new Proof(LEFT_OBTUSE,*tri);
            rightProof = new Proof(RIGHT_OBTUSE,*refl);
       }
       else {
            leftProof = new Proof(LEFT_ACUTE,*tri);
            rightProof = new Proof(RIGHT_ACUTE,*refl);
       }
    #ifdef DEBUG
    debugOut("enter loop");
    #endif

    while(!key[KEY_ESC]) {
         if(mouse_b & 1) {
              wasClicked = (wasClicked == -1) ? mouseOver(points) : wasClicked;
              if(wasClicked > -1) {
                    #ifdef DEBUG
                    debugOut("Point clicked");
                    #endif

                   points.Element(wasClicked).X = mouse_x;
                   points.Element(wasClicked).Y = mouse_y;
                   delete leftProof;
                   delete rightProof;
                   delete tri;
                   delete refl;
                   tri = new Triangle(points.Element(0),points.Element(1),points.Element(2));
                   refl = new Triangle(*tri);
                   refl->Translate(screen_midX,0);
                   if(tri->Angle(0) > 90) {
                        leftProof = new Proof(LEFT_OBTUSE,*tri);
                        rightProof = new Proof(RIGHT_OBTUSE,*refl);
                   }
                   else {
                        leftProof = new Proof(LEFT_ACUTE,*tri);
                        rightProof = new Proof(RIGHT_ACUTE,*refl);
                   }
                   redraw = true;
              }
         }
         else {
                    #ifdef DEBUG
                    debugOut("Mouse 1 not clicked");
                    #endif
              if(wasClicked > -1) {
                                   #ifdef DEBUG
                    debugOut("set rescale");
                    #endif
                   wasClicked = -1;
                   rescale = true;
              }
         }
         if(mouse_b & 2) {
                    #ifdef DEBUG
                    debugOut("Mouse 2 clicked");
                    #endif
              redraw = true;
         }
         if(rescale) {
              rescale = false;
              centerProof(*leftProof,leftBox);
              centerProof(*rightProof,rightBox);
              moveOutline(points,*leftProof);
              redraw = true;
         }
         if(redraw) {
             clear_to_color(buffer,bgColor);
             drawInterface(buffer,screen_width,screen_height,border,fgColor);
             drawTriangleInfo(buffer,points);
             drawProof(buffer,*leftProof);
             drawProof(buffer,*rightProof);
             labelIndices(&points,buffer);
             redraw = false;
         }
         blit(buffer,screen,0,0,0,0,screen_width,screen_height);
         show_mouse(screen);
         rest(50);
    }
    
    return 0;
}

END_OF_MAIN()

void drawInterface(BITMAP* buffer, int screen_width, int screen_height, int size, int color) {
     line(buffer,0,size,screen_width,size,color);
     line(buffer,screen_width /2,size,screen_width /2,screen_height,color);
}

void drawTriangleInfo(BITMAP* buffer, const Outline& o) {
     //Print angle/side info
}

void drawProof(BITMAP* buffer, const Proof& p) {
     for(unsigned int i = 1; i < p.Count(); i++)
          p.Shape(i).Draw(buffer);
     p.Shape(0).Draw(buffer);
}

int mouseOver(Outline& o) {
    for(unsigned int i = 0; i < o.Size(); i++)
        if(mouse_x > o.Element(i).X - 10 && mouse_x < o.Element(i).X + 10)
             if(mouse_y > o.Element(i).Y - 10 && mouse_y < o.Element(i).Y + 10)
                 return i;
    return -1;
}

Point centerObject(int minX, int minY, int maxX, int maxY, int width, int height) {
      Point result;
      result.X = ((maxX - minX) - width) / 2 + minX;
      result.Y = ((maxY - minY) - height) / 2 + minY;
    #ifdef DEBUG
    std::stringstream ss;
    ss << "Center:" << "\tminX: " << minX << ", minY: " << minY << ", maxX: " << maxX << ", maxY: " << maxY << ", width: " << width << ", height: " << height << std::endl;
    ss << "offsetX: " << result.X << ", offsetY: " << result.Y << std::endl;
    debugOut(ss.str());
    #endif
    return result;
}

double scaleObject(double maxW, double maxH, double width, double height) {
       double result, scaleW, scaleH;
       scaleW = maxW / width;
       scaleH = maxH / height;
       result = (scaleH < scaleW) ? scaleH : scaleW;
       if(result > 1)
            result = 1;
#ifdef DEBUG
    std::stringstream ss;
    ss << "\tScale:" << result;
    debugOut(ss.str());
#endif

       return result;
}

void centerProof(Proof& p, Outline& constrain) {
double width, height, scale;

width = constrain.Element(1).X - constrain.Element(0).X;
height = constrain.Element(1).Y - constrain.Element(0).Y;
scale = scaleObject(width,height,p.Width(), p.Height());

p.Scale(scale);
Point offset = centerObject((int)constrain.Element(0).X,(int)constrain.Element(0).Y,(int)constrain.Element(1).X,(int)constrain.Element(1).Y,int(p.Width()),int(p.Height()));
p.Translate(offset.X-p.MinX(),offset.Y-p.MinY());
     
}


void moveOutline(Outline& o, Proof& p) {
     for(int i = 0; i < 3; i++) {
         o.Element(i).X = p.Shape(0).Shape().Element(i).X;
         o.Element(i).Y = p.Shape(0).Shape().Element(i).Y;
     }
}

void labelIndices(Polygon* p, BITMAP* buffer) {
     std::string labels[3] = {"C","B","A"};
     for(unsigned int i = 0; i < p->Size(); i++) {
        circlefill( buffer, int(p->Element(i).X), int(p->Element(i).Y), 10, makecol( 0,0, 0));
        textout_ex( buffer, font, labels[i].c_str(), int(p->Element(i).X) - 3, int(p->Element(i).Y) - 3,
                    makecol( 0, 255, 0), makecol( 0, 0, 0) );
      }
}
