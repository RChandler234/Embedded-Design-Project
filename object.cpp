#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include "object.h"
using namespace std;
const int DEFAULT_DISTANCE = 2; //default value for the number of pixels the shape moves per iteration
const int DEFAULT_ANGLE = 135; //default angle for the shape to move in

//class to hold information about the bounding box, shape type, distance moved per iteration and angle of movement

        Object::Object(){
		x_min = 0; //variable for holding the lower x limit of the bounding box for the shape, initialize it to 0
		y_min = 0; //variable for holding the lower y limit of the bounding box for the shape, initialize it to 0
		x_max = 0; //variable for holding the upper x limit of the bounding box for the shape, initialize it to 0
		y_max = 0; //variable for holding the upper y limit of the bounding box for the shape, initialize it to 0
		collision_corner=false; //tracks whether the bounding box has hit a corner of the screen
		collision_clockwise = false; //tracks whether the bounding box has hit the edge of the screen in a clockwise motion
		collision_counterclockwise = false; //tracks whether the bounding box has hit the edge of the screen in a counter-clockwise motion
		collision_otherobject = false; //tracks whether the bounding box has hit another object
        }

        Object::~Object(){

        }

	//function for updating multiple parameters of the object at once
	void Object::UpdateObject(int x1, int y1, int x2, int y2, int d, int a, char s){
		x_min = x1; //update lower x bound of the bounding box
		y_min = y1; //update lower y bound of the bounding box
		x_max = x2; //update upper x bound of the bounding box
		y_max = y2; //update upper y bound of the bounding box
		distance = d; //update distance moved each iteration
		angle = a; //update angle moved each iteration
		shape = s; //update type of shape
	}
