#include <allegro.h>
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
Proof* createProof(const Triangle& tri, PROOF_TYPE type, int width, int height);

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

    #ifdef DEBUG
    debugOut("set allegro options");
    #endif
    set_mouse_speed(10,10);
    get_desktop_resolution(&screen_width,&screen_height);
    set_color_depth(desktop_color_depth());
    set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, screen_width, screen_height, 0, 0);
    BITMAP* buffer = create_bitmap(screen_width,screen_height);    
    int screen_midX = screen_width / 2;
    int border = 100;
    int bgColor = makecol(255,255,255);
    int fgColor = makecol(0,0,0);
    bool redraw = true;
    Outline points(3);    
    Triangle* tri;
    Proof *leftProof, *rightProof;

    #ifdef DEBUG
    debugOut("init points");
    #endif

    points.Element(0).X = 10;
    points.Element(0).Y = 150;
    points.Element(1).X = 100;
    points.Element(1).Y = 200;
    points.Element(2).X = 200;
    points.Element(2).Y = 300;
    tri = new Triangle(points.Element(0),points.Element(1),points.Element(2));
    #ifdef DEBUG
    debugOut("init proofs");
    #endif

    if(tri->Angle(0) > 90) {
                        leftProof = createProof(*tri,LEFT_OBTUSE,screen_midX,screen_height);
                        rightProof = createProof(*tri,RIGHT_OBTUSE,screen_midX,screen_height);
                   }
                   else {
                        leftProof = createProof(*tri,LEFT_ACUTE,screen_midX,screen_height);
                        rightProof = createProof(*tri,RIGHT_ACUTE,screen_midX,screen_height);
                   }
                   rightProof->Translate(screen_midX,0);
    #ifdef DEBUG
    debugOut("enter loop");
    #endif

    while(!key[KEY_ESC]) {
        #ifdef DEBUG
        debugOut("Check mouse click");
        #endif
         if(mouse_b & 1) {
              int i = mouseOver(points);
              if(i > -1) {
                   points.Element(i).X = mouse_x;
                   points.Element(i).Y = mouse_y;
                   delete leftProof;
                   delete rightProof;
                   delete tri;
                   tri = new Triangle(points.Element(0),points.Element(1),points.Element(2));
                   if(tri->Angle(0) > 90) {
                        leftProof = createProof(*tri,LEFT_OBTUSE,screen_midX,screen_height-border);
                        rightProof = createProof(*tri,RIGHT_OBTUSE,screen_midX,screen_height-border);
                   }
                   else {
                        leftProof = createProof(*tri,LEFT_ACUTE,screen_midX,screen_height);
                        rightProof = createProof(*tri,RIGHT_ACUTE,screen_midX,screen_height);
                   }
                   leftProof->Translate(0,border);
                   rightProof->Translate(screen_midX,border);
                   redraw = true;
              }
         }
         else if(mouse_b & 2) {
              //Switch main angle
              redraw = true;
         }
         if(redraw) {
            #ifdef DEBUG
            debugOut("redraw");
            #endif

             clear_to_color(buffer,bgColor);
             #ifdef DEBUG
             debugOut("\tinterface");
             #endif
             drawInterface(buffer,screen_width,screen_height,border,fgColor);
             #ifdef DEBUG
             debugOut("\ttriangle");
             #endif
             drawTriangleInfo(buffer,points);
             #ifdef DEBUG
             debugOut("\tleftProof");
             #endif
             drawProof(buffer,*leftProof);
             #ifdef DEBUG
             debugOut("\trightProof");
             #endif
             drawProof(buffer,*rightProof);
             redraw = false;
         }
         blit(buffer,screen,0,0,0,0,screen_width,screen_height);
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
     for(unsigned int i = 0; i < p.Count(); i++)
          p.Shape(i).Draw(buffer);
}

int mouseOver(Outline& o) {
    for(unsigned int i = 0; i < o.Size(); i++)
        if(mouse_x > o.Element(i).X - 5 && mouse_x < o.Element(i).X + 5)
             if(mouse_y > o.Element(i).Y - 5 && mouse_y < o.Element(i).Y + 5)
                 return i;
    return -1;
}

Point centerObject(int minX, int minY, int maxX, int maxY, int width, int height) {
      Point result;
      result.X = ((maxX - minX) - width) / 2;
      result.Y = ((maxY - minY) - height) / 2;
}

double scaleObject(double maxW, double maxH, double width, double height) {
       double result, scaleW, scaleH;
       scaleW = maxW / width;
       scaleH = maxH / height;
       result = (scaleH < scaleW) ? scaleH : scaleW;
       if(result > 1)
            result = 1;
       return result;
}

Proof* createProof(const Triangle& tri, PROOF_TYPE type, int width, int height) {
       
Proof* temp = new Proof(type,tri);
temp->Scale(scaleObject((double)width,height,temp->Width(),temp->Height()));
Point offset = centerObject(0,0,width,height,int(temp->Width()),int(temp->Height()));
#ifdef DEBUG
std::stringstream ss;
ss << "\tProof MinX: " << temp->MinX() << ", MinY:" << temp->MinY() << ", Scale:";
debugOut(ss.str());
#endif
temp->Translate(temp->MinX() + (offset.X - temp->MinX()),temp->MinY() + (offset.Y - temp->MinY()));
#ifdef DEBUG
ss.str("");
ss << "\tTranslate Proof MinX: " << temp->MinX() << ", MinY:" << temp->MinY();
debugOut(ss.str());
#endif
return temp;
}
