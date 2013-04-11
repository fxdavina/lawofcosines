#include <allegro.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include "ScreenPolygon.h"
#include "Triangle.h"
#include "Outline.h"
#include "Proof.h"

//#define DEBUG
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

const int STATIC = 0;
const int DRAG = 1;
const int RESCALE = 2;
const int REDRAW = 4;





void drawInterface(BITMAP* buffer, int screen_width, int screen_height, int size, int color);
void drawTriangleInfo(BITMAP* buffer, const Outline& o);
void drawProof(BITMAP* buffer, const Proof& p);
void drawStats(BITMAP* buffer, Triangle* t, int border, int mid);
int mouseOver(Outline& o);
Point centerObject(int minX, int minY, int maxX, int maxY, int width, int height);
double scaleProof(Proof& p, Outline& constrain);
void centerProof(Proof& p, Outline& constrain);
void moveOutline(Outline& o, Proof& p);
void labelIndices(Polygon* p, BITMAP* buffer);
void swapPoints(Point& p1, Point& p2);
void labelShape(BITMAP* buffer, Polygon* p, std::string label);
void labelProof(BITMAP* buffer, Proof* p, PROOF_TYPE t);

int main() {
    
    #ifdef DEBUG
    std::ofstream out;
    out.open("debug.txt");
    out.close();
    debugOut("init");
    #endif
    
    int screen_height, screen_width;
    int padding = 10;
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
    bool redraw = true, rescale = true, animate = false, obtuse = false;
    int wasClicked = -1;
    double meet, step = 10;
    int metCount = 0;
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
    leftBox.Element(0).X = 0 + padding;
    leftBox.Element(0).Y = border + padding;
    leftBox.Element(1).X = screen_midX - padding;
    leftBox.Element(1).Y = screen_height - padding;
    
    rightBox.Element(0).X = screen_midX + padding;
    rightBox.Element(0).Y = border + padding;
    rightBox.Element(1).X = screen_width - padding;
    rightBox.Element(1).Y = screen_height - padding; 
    obtuse = tri->Angle(0) > 90;
    if(obtuse) {
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
    double scaleLeft,scaleRight,scale = 1;
    while(!key[KEY_ESC]) {
         if(mouse_b & 1 && !animate) {
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
                   obtuse = tri->Angle(0) > 90;
                   if(obtuse) {
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
         else if(!animate) {
              if(wasClicked > -1) {
                                   #ifdef DEBUG
                    debugOut("set rescale");
                    #endif
                   wasClicked = -1;
                   rescale = true;
              }
         }
         if(mouse_b & 2 && !animate) {
                    #ifdef DEBUG
                    debugOut("Mouse 2 clicked");
                    #endif
              int newAngle;
              if( (newAngle = mouseOver(points)) > 0 ) {
                  swapPoints(points.Element(newAngle),points.Element(0));
                                     delete leftProof;
                   delete rightProof;
                   delete tri;
                   delete refl;
                   tri = new Triangle(points.Element(0),points.Element(1),points.Element(2));
                   refl = new Triangle(*tri);
                   refl->Translate(screen_midX,0);
                   obtuse = tri->Angle(0) > 90;
                   if(obtuse) {
                        leftProof = new Proof(LEFT_OBTUSE,*tri);
                        rightProof = new Proof(RIGHT_OBTUSE,*refl);
                   }
                   else {
                        leftProof = new Proof(LEFT_ACUTE,*tri);
                        rightProof = new Proof(RIGHT_ACUTE,*refl);
                   }

                  rescale = true;
              }
         }
         if(rescale) {
              rescale = false;
              scaleLeft = scaleProof(*leftProof,leftBox);
              scaleRight = scaleProof(*rightProof,rightBox);
              scale = (scaleLeft < scaleRight) ? scaleLeft : scaleRight;
              if(scale < 1) {
                   delete leftProof;
                   delete rightProof;
                   delete tri;
                   delete refl;
                   tri = new Triangle(Point(points.Element(0).X*scale,points.Element(0).Y*scale),
                                      Point(points.Element(1).X*scale,points.Element(1).Y*scale),
                                      Point(points.Element(2).X*scale,points.Element(2).Y*scale));
                   refl = new Triangle(*tri);
                   refl->Translate(screen_midX,0);
                   if(obtuse) {
                        leftProof = new Proof(LEFT_OBTUSE,*tri);
                        rightProof = new Proof(RIGHT_OBTUSE,*refl);
                   }
                   else {
                        leftProof = new Proof(LEFT_ACUTE,*tri);
                        rightProof = new Proof(RIGHT_ACUTE,*refl);
                   }
              }
              centerProof(*leftProof,leftBox);
              centerProof(*rightProof,rightBox);
              moveOutline(points,*leftProof);
              delete tri;
              delete refl;
              tri = new Triangle(points.Element(0),points.Element(1),points.Element(2));
              refl = new Triangle(*tri);
              refl->Translate(screen_midX,0);
              redraw = true;
         }
         if(key[KEY_SPACE] && !animate && wasClicked == -1)  {
              animate = true;
              meet = floor(screen_midX - (leftProof->Width() / 2));
         }
         if(animate) {
             #ifdef DEBUG
                std::stringstream ss;
                ss << "Left: " << leftProof->MinX() << ", Right: " << rightProof->MinX() << ", Meet: " << meet;
                debugOut(ss.str());
            #endif
             if( int(leftProof->MinX()) ==  (int)meet ) {
                  metCount++;
             }
             else {
             #ifdef DEBUG
             debugOut("Not met");
             #endif
                 leftProof->Translate(step,0);
                 rightProof->Translate(-step,0);
                 if(leftProof->MinX() > meet || rightProof->MinX() < meet) {
             #ifdef DEBUG
             debugOut("Passed meet");
             #endif                                      
                      leftProof->Translate(leftProof->MinX()-meet,0);
                      rightProof->Translate(meet-rightProof->MinX(),0);
                 }
                      
             }
             if(metCount == 10) {
                  animate = false;
                  rescale = true;
                  metCount = 0;
             }
             else {
                 clear_to_color(buffer,bgColor);
                 drawProof(buffer,*leftProof);
                 drawProof(buffer,*rightProof);
             }
         }
         if(redraw) {
             clear_to_color(buffer,bgColor);
             drawInterface(buffer,screen_width,screen_height,border,fgColor);
             drawTriangleInfo(buffer,points);
             drawProof(buffer,*leftProof);
             drawProof(buffer,*rightProof);
             labelProof(buffer,leftProof, (obtuse) ? LEFT_OBTUSE : LEFT_ACUTE);
             labelProof(buffer,rightProof, (obtuse) ? RIGHT_OBTUSE : RIGHT_ACUTE);
             labelIndices(&points,buffer);
             drawStats(buffer,tri,border,screen_midX);
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

double scaleProof(Proof& p, Outline& constrain) {
       double width, height, result, scaleW, scaleH;
       width = constrain.Element(1).X - constrain.Element(0).X;
       height = constrain.Element(1).Y - constrain.Element(0).Y;
       scaleW = width / p.Width();
       scaleH = height / p.Height();
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
double width, height;

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

void drawStats(BITMAP* buffer, Triangle* t, int border, int mid) {
    int padding = 5, divs = (border - 2*padding)/4;
    std::string points[3] = {"C","B","A"};
    std::string sides[3] =  {"CA","BC","AB"};
    std::string data = ": %-3d";
    textout_ex( buffer, font, "Triangle: ", padding, padding,
                makecol( 0, 0, 0), makecol( 255, 255, 255) );

    for(int i = 0; i < 3; i++) {
         int top = padding + (divs * (3-i));   
         points[i] += data;
         sides[i] += data;
         int other  = (i == 0) ? 2 : i - 1;
         double baseangle = t->Angle(i);
         textprintf_ex( buffer, font, padding,top, makecol(0, 0, 0),
                   makecol(255, 255, 255), points[i].c_str(), int(baseangle));
         textprintf_ex( buffer, font, mid + padding,top, makecol(0, 0, 0),
                   makecol(255, 255, 255), sides[i].c_str(), 
                   int(t->Element(i).Distance(t->Element(other))));
     }
}

void swapPoints(Point& p1, Point& p2) {
     double temp;
     temp = p1.X;
     p1.X = p2.X;
     p2.X = temp;
     temp = p2.Y;
     p2.Y = p1.Y;
     p1.Y = temp;
}

void labelShape(BITMAP* buffer, ScreenPolygon& p, const char* label) {
     double minX, maxX;
     minX = maxX = p.Shape().Element(0).X;
     double minY, maxY;
     minY = maxY = p.Shape().Element(0).Y;
     for(int i = 1; i < p.Shape().Size(); i++) {
          if(p.Shape().Element(i).X < minX)
               minX = p.Shape().Element(i).X;
          else if(p.Shape().Element(i).X > maxX)
               maxX = p.Shape().Element(i).X;
          if(p.Shape().Element(i).Y < minY)
               minY = p.Shape().Element(i).Y;
          else if(p.Shape().Element(i).Y > maxY)
               maxY = p.Shape().Element(i).Y;
     }
     textout_centre_ex(buffer, font, label, int((maxX + minX) / 2), int((maxY + minY) / 2), makecol(0,0,0), p.Color);
}
void labelProof(BITMAP* buffer, Proof* p, PROOF_TYPE t) {
     labelShape(buffer,p->Shape(0),"Tri");
     switch(t) {
     case LEFT_ACUTE:
          labelShape(buffer,p->Shape(1),"A^2");
          labelShape(buffer,p->Shape(2),"B^2");
          labelShape(buffer,p->Shape(3),"Tri");
          labelShape(buffer,p->Shape(4),"Tri");
          break;
     case RIGHT_ACUTE:
          labelShape(buffer,p->Shape(1),"C^2");
          labelShape(buffer,p->Shape(2),"Tri");
          labelShape(buffer,p->Shape(3),"a*b*cos(C)");
          labelShape(buffer,p->Shape(4),"a*b*cos(C)");
          labelShape(buffer,p->Shape(5),"Tri");
          break;
     case LEFT_OBTUSE:
          labelShape(buffer,p->Shape(1),"B^2");
          labelShape(buffer,p->Shape(2),"a*b*cos(C)");
          labelShape(buffer,p->Shape(3),"Tri");
          labelShape(buffer,p->Shape(4),"A^2");
          labelShape(buffer,p->Shape(5),"a*b*cos(C)");
          break;
     case RIGHT_OBTUSE:
          labelShape(buffer,p->Shape(1),"C^2");
          labelShape(buffer,p->Shape(2),"Tri");
          break;
     }
}

