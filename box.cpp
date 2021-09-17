#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include<math.h>
#include "box.h"
#include "object.h"
#include "node.h"
using namespace std;

//class that inherits from the shapes class
//handles drawing the shapes,moving one object at a time, and tracking collision with the walls of the screen

const unsigned int BUFFER_OFFSET      = 0x3F203020;  //offset for the pixel_buffer
const unsigned int PIXEL_BUFFER_OFFSET     = 0x8000000;  //offset for where the pixel_buffer info is storeed
const unsigned int BACK_BUFFER_OFFSET = 0x3F203024; //offset for the pixel_back_buffer
const unsigned int PIXEL_BACK_BUFFER_OFFSET = 0x0; //offset for where the pixel_back_buffer info is stored
const double PI = 2*acos(0.0); //PI constant
const int X_WIDTH = 320;  // Width of the display
const int Y_HEIGHT = 240; // Height of the display
const short OBJECT_COLOR = 20; //color of all the shapes
const unsigned int PIXEL_STATUS_OFFSET = 0x3F20302C;//offset for the status bit of the pixel buffer controller

   //Constructor
   Box::Box(){
   };
   //Deconstructor
   Box::~Box(){
   };
	//Draws a shape of the specified type s, within the bounds
        //x1: lower x bound of bounding box, y1: lower y bound of bounding box
        //x2: upper x bound of bounding box, y2: upper y bound of bounding box
	void Box::Draw_Shape(int x1, int y1, int x2, int y2, char s){
		if(s=='b')
			Draw_Box(x1,y1,x2,y2,OBJECT_COLOR); //draw a box
		else if (s=='t')
			Draw_T(x1,y1,x2,y2,OBJECT_COLOR); //draw a T
		else if (s=='l')
			Draw_L(x1,y1,x2,y2,OBJECT_COLOR); //draw a L
		else if (s=='v')
			Draw_V(x1,y1,x2,y2,OBJECT_COLOR); //draw a V
		else if (s=='x')
			Draw_X(x1,y1,x2,y2,OBJECT_COLOR); //draw an X
		else if (s=='s')
			Draw_Star(x1,y1,x2,y2,OBJECT_COLOR); //draw a star
		else if (s=='r')
			Draw_Triangle(x1,y1,x2,y2,OBJECT_COLOR); //draw a triangle
		else if (s=='d')
			Draw_Diamond(x1,y1,x2,y2,OBJECT_COLOR); //draw a diamond
		else if (s=='p')
			Draw_Plus(x1,y1,x2,y2,OBJECT_COLOR); //draw a plus
		else if (s=='a')
			Draw_Trapezoid(x1,y1,x2,y2,OBJECT_COLOR); //draw a trapezoid
	}


	//moves bounding box, handles collision with bondaries of screen
        //x1: lower x bound of bounding box, y1: lower y bound of bounding box
        //x2: upper x bound of bounding box, y2: upper y bound of bounding box
	//d: number of pixels travelled by the shape during the translation
	//a: angle the shape moves in
	//n: pointer to a node in the linked list in the system class
	void Box::Translate(int x1, int y1, int x2, int y2, int d, int a, Node * n){
                double theta = (a * PI) / 180; //converts the inputted angle to radians
		int x = d*cos(theta); //calculate the x displacement
		int y = d*sin(theta); //calculate the y displacement
		Object obj = n->data; //save a copy of the Object in the node that the pointer points to
		//if the shape hits the left wall
		if(x1+x<=0){
			//bottom left corner
			if(y1+y<=0){
				Draw_Shape(0,0,x2-x1,y2-y1,obj.shape); //draw the shape actually colliding with the wall
				n->data.collision_corner = true; //set the corner collision variable in the object in the node to true
				n->data.UpdateObject(0,0,x2-x1,y2-y1,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			//top left corner
			else if(y1+y>= Y_HEIGHT){
				Draw_Shape(0,Y_HEIGHT-(y2-y1), x2-x1,Y_HEIGHT,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
				n->data.UpdateObject(0,Y_HEIGHT-(y2-y1), x2-x1,Y_HEIGHT,obj.distance, a,obj.shape);//update the variables in the object in the node
			}
			else if (y<0){
				Draw_Shape(0,y1,x2-x1,y2,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_counterclockwise = true;//set the counterclockwise collision variable in the object in the node to true
				n->data.UpdateObject(0,y1,x2-x1,y2,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			else if(y>0){
				Draw_Shape(0,y1,x2-x1,y2,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_clockwise = true;//set the clockwise collision variable in the object in the node to true
				n->data.UpdateObject(0,y1,x2-x1,y2,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			// if y = 0, the shape has hit the wall head on, so it flips 180 degrees (same effect as the corner collision)
			else{
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
			}
		}
		// if it hits top wall
		else if (y1+y<=0){
			// top left corner
			if(x1+x<=0){
                                Draw_Shape(0,0,x2-x1,y2-y1,obj.shape);//draw the shape actually colliding with the wall
                                n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
				n->data.UpdateObject(0,0,x2-x1,y2-y1,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			//top right corner
			else if(x1+x>=X_WIDTH){
				Draw_Shape(X_WIDTH-(x2-x1),0,X_WIDTH,y2-y1,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
				n->data.UpdateObject(X_WIDTH-(x2-x1),0,X_WIDTH,y2-y1,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			else if (x>0){
				Draw_Shape(x1,0,x2,y2-y1,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_counterclockwise = true;//set the counterclockwise collision variable in the object in the node to true
				n->data.UpdateObject(x1,0,x2,y2-y1,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			else if (x<0){
				Draw_Shape(x1,0,x2,y2-y1,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_clockwise = true;//set the clockwise collision variable in the object in the node to true
				n->data.UpdateObject(x1,0,x2,y2-y1,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			// if x = 0, the shape hit the wall head on, so it flips 180 degrees(which has the same effect as the corner collision)
			else{
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
			}
		}
		//if it hits the right wall
		else if (x2+x >= X_WIDTH){
			// bottom right corner
			if(y1+y<=0){
				Draw_Shape(X_WIDTH-(x2-x1),0,X_WIDTH,y2-y1,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
				n->data.UpdateObject(X_WIDTH-(x2-x1),0,X_WIDTH,y2-y1,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			// top right corner
			else if(y1+y>=Y_HEIGHT){
				Draw_Shape(X_WIDTH-(x2-x1),Y_HEIGHT-(y2-y1),X_WIDTH,Y_HEIGHT,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
				n->data.UpdateObject(X_WIDTH-(x2-x1),Y_HEIGHT-(y2-y1),X_WIDTH,Y_HEIGHT,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			else if(y>0){
				Draw_Shape(X_WIDTH-(x2-x1),y1,X_WIDTH,y2,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_counterclockwise = true;//set the counterclockwise collision variable in the object in the node to true
				n->data.UpdateObject(X_WIDTH-(x2-x1),y1,X_WIDTH,y2,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			else if(y<0){
				Draw_Shape(X_WIDTH-(x2-x1),y1,X_WIDTH,y2,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_clockwise = true;//set the clockwise collision variable in the object in the node to true
				n->data.UpdateObject(X_WIDTH-(x2-x1),y1,X_WIDTH,y2,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			// if y = 0, the shape has hit the wall head on, so it flips 180 degrees (same effect as the corner collision)
			else{
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
			}
		}
		//if it hits the bottom wall
		else if (y2+y >= Y_HEIGHT){
			//bottom left corner
			if(x1+x<=0){
                                Draw_Shape(0,Y_HEIGHT-(y2-y1), x2-x1,Y_HEIGHT,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
				n->data.UpdateObject(0,Y_HEIGHT-(y2-y1), x2-x1,Y_HEIGHT,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			//bottom right corner
			else if(x1+x>=X_WIDTH){
                                Draw_Shape(X_WIDTH-(x2-x1),Y_HEIGHT-(y2-y1),X_WIDTH,Y_HEIGHT,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
				n->data.UpdateObject(X_WIDTH-(x2-x1),Y_HEIGHT-(y2-y1),X_WIDTH,Y_HEIGHT,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			else if(x<0){
				Draw_Shape(x1,Y_HEIGHT-(y2-y1),x2,Y_HEIGHT,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_counterclockwise = true;//set the counterclockwise collision variable in the object in the node to true
				n->data.UpdateObject(x1,Y_HEIGHT-(y2-y1),x2,Y_HEIGHT,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			else if(x>0){
				Draw_Shape(x1,Y_HEIGHT-(y2-y1),x2,Y_HEIGHT,obj.shape);//draw the shape actually colliding with the wall
				n->data.collision_clockwise = true;//set the clockwise collision variable in the object in the node to true
				n->data.UpdateObject(x1,Y_HEIGHT-(y2-y1),x2,Y_HEIGHT,obj.distance,a,obj.shape);//update the variables in the object in the node
			}
			// if x = 0, the shape hit the wall head on, so it flips 180 degrees(which has the same effect as the corner collision)
			else{
				n->data.collision_corner = true;//set the corner collision variable in the object in the node to true
			}
		}
		//the shape has not collided with a wall
		else{
			Draw_Shape(x1+x,y1+y,x2+x,y2+y,obj.shape); //Draw moved shape
			n->data.UpdateObject(x1+x,y1+y,x2+x,y2+y,obj.distance,a,obj.shape);//update the variables in the object in the node
		}
	}

