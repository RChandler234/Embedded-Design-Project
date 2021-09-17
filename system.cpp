#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include<math.h>
#include "system.h"
#include "node.h"
using namespace std;
const unsigned int MPCORE_PRIV_TIMER_LOAD_OFFSET = 0x3FFEC600; //offset for the timer load
const unsigned int MPCORE_PRIV_TIMER_COUNTER_OFFSET = 0x3FFEC604; //offset for the timer counter
const unsigned int MPCORE_PRIV_TIMER_CONTROL_OFFSET = 0x3FFEC608; //offset for the timer  control
const unsigned int MPCORE_PRIV_TIMER_INTERRUPT_OFFSET = 0x3FFEC60C; //offset for the timer interrupt
const unsigned int BUFFER_OFFSET      = 0x3F203020;  //offset for the pixel_buffer
const unsigned int PIXEL_BUFFER_OFFSET     = 0x8000000;  //offset for the initial pixel_buffer
const unsigned int BACK_BUFFER_OFFSET = 0x3F203024; //offset for the pixel_back_buffer
const unsigned int PIXEL_BACK_BUFFER_OFFSET = 0x0; //offset for where the pixel_back_buffer info is stored
const double PI = 2*acos(0.0);// PI constant
const int X_WIDTH = 320;  // Width of the display
const int Y_HEIGHT = 240; // Height of the display
const short OBJECT_COLOR = 20; //color of the shapes displayed on screen
const unsigned int PIXEL_STATUS_OFFSET = 0x3F20302C; //offset for the status bit of the pixel buffer controller
const int DEFAULT_DISTANCE = 2; //default number of pixels the shapes move each iteration
const int DEFAULT_ANGLE = 135; //default angle that the shapes move
const unsigned int JP2_PIN_DATA = 0x3F200070; //offset for the JP2 Pins data (audio)
const unsigned int JP2_PIN_DIR = 0x3F200074;  //direction offset for teh JP2 pins
const char shape_list[10] = {'b','t','l','v','x','s','r','d','p','a'}; //list of chars that represent the shape types
const unsigned int ITERATION_TIME = 100000000/8; //time between iterations: quarter of a second
const unsigned int JP1_DATA_OFFSET = 0x3F200060; //Points to DATA Register for JP1
const unsigned int DIRECTION_JP1_OFFSET = 0x3F200064;//Points to DIRECTION Register for JP1


//handles tracking and adding the objects on screen, drawing each stage of the simulation
//tracking object collision, playing the collision sound, and getting an input from the keypad
   System::System(){
      //initialize the private variables
	objects = 1;	//tracks the number of objects in the system(starts at 1)
	first = NULL;
	first = new Node(); //initialize 1st node of the linked list
	first->data.UpdateObject(100,100,120,120,DEFAULT_DISTANCE,DEFAULT_ANGLE,'b'); //update the data of the object in the first node
	RegisterWrite(JP2_PIN_DIR,1); //set JP2 to output for the audio output
        RegisterWrite(DIRECTION_JP1_OFFSET,27136); //set configure JP1 for the keypad input
	RegisterWrite(JP1_DATA_OFFSET,0);
   };
   //Deconstructor
   System::~System(){
	objects = 0; //resets the number of objects in the system to 0
	RegisterWrite(JP2_PIN_DIR,0); //reset the direction of the JP2 pins
	RegisterWrite(DIRECTION_JP1_OFFSET,0); //reset the directin of the JP1 pins
	RegisterWrite(JP1_DATA_OFFSET,0);
   };

	//function that displays the first iteration of the simulation, takes the size (1,2,or 3)
	//of the first box as an input
	void System::SimulationStart(int size){
		//make sure the contents of back buffer and buffer have been completely switched before
		//starting the next iteration of the simulation
                while(true){
                        if((RegisterRead(PIXEL_STATUS_OFFSET) & (1 << 0))){
                        }
                        else{
                                break;
                        }
                }
                Clear(0,0,X_WIDTH,Y_HEIGHT);//clear back buffer
		int x1 = first->data.x_min; //save the x_min in a variable
		int y1 = first->data.y_min; //save the y_min in a variable
		//if size is 1 make the box the small size
		if(size == 1){
			first->data.UpdateObject(x1,y1,x1+10,y1+10,DEFAULT_DISTANCE,DEFAULT_ANGLE,'b');
		}
		//if size is 2 make the box the medium size
		else if(size == 2){
			first->data.UpdateObject(x1,y1,x1+20,y1+20,DEFAULT_DISTANCE,DEFAULT_ANGLE,'b');
		}
		//if size is 3 make the box the large size
		else if(size == 3){
			first->data.UpdateObject(x1,y1,x1+30,y1+30,DEFAULT_DISTANCE,DEFAULT_ANGLE,'b');
		}
		Object obj = first->data; //get object out of first node
		Draw_Shape(obj.x_min,obj.y_min,obj.x_max,obj.y_max,obj.shape);//Draw the shape
		RegisterWrite(BUFFER_OFFSET,1);//display the iteration on the screen
	}

	//function that handles one iteration of the simulation based on all the information from the previous
	//iteration
	void System::Simulation(){
                //make sure the contents of back buffer and buffer have been completely switched before
                //starting the next iteration of the simulation
		while(true){
			if((RegisterRead(PIXEL_STATUS_OFFSET) & (1 << 0))){
			}
			else{
				break;
			}
		}
		Clear(0,0,X_WIDTH,Y_HEIGHT); //clear the back buffer
		int x1; //used to store x_min of different objects
		int y1;	//used to store y_min of different objects
		int x2; //used to store x_max of different objects
		int y2; //used to store y_max of different objects
		int d;	//used to store distance different objects move in each iteration
		int a;	//used to store angle of different objects
		bool sound = false; //boolean used for tracking whether to play a sound
		double theta; //used to store the angle of different objects in radians
		int angle1; //used to store the angle of object1
		int angle2; //used to store the angle of obejcts2
				//checks eveyr possible combination of objects in the linked list
				//with no repeats
				Node * n1 = first; //initialize n1 to the first node
				Node * n2; //node pointer used for cycling through all the combinations
				//loop from the first node in the list to the end of the list
				while(n1 != NULL){
					n2 = n1->next;
					//loop from the node after the current node in the above loop to the end of the loop
					while(n2 != NULL){
						Object obj1 = n1->data; //make a copy of the first object from the top loop
						Object obj2 = n2->data; //make a copy of the second object from the bottom loop 
						//Object collision algorithm here:
						//calculate where objects 1's values would be in the next simulation cycle
						theta = (obj1.angle * PI) / 180; //convert angle to radians
						x1 = obj1.x_min + (obj1.distance * cos(theta));
						y1 = obj1.y_min + (obj1.distance * sin(theta));
						x2 = obj1.x_max + (obj1.distance * cos(theta));
						y2 = obj1.y_max + (obj1.distance * sin(theta));

						double obj1_xc = (((double) x1)+x2)/2; //x-coordinate of center of object 1
						double obj1_yc = (((double) y1)+y2)/2; //y-coordinate of center of object 1
                                                double obj1_xw = x2 - x1;  //width of object 1
                                                double obj1_yh = y2 - y1;  //height of object 1

						//do the same for object 2
                                                theta = (obj2.angle * PI) / 180;
                                                x1 = obj2.x_min + (obj2.distance * cos(theta));
                                                y1 = obj2.y_min + (obj2.distance * sin(theta));
                                                x2 = obj2.x_max + (obj2.distance * cos(theta));
                                                y2 = obj2.y_max + (obj2.distance * sin(theta));

						double obj2_xc = (((double) x1)+x2)/2; //x-coordinate of center of object 2
						double obj2_yc = (((double) y1)+y2)/2; //y-coordinate of center of object 2
						double obj2_xw = x2 - x1;  //width of object 2
						double obj2_yh = y2 - y1;  //height of object 2

						double len1 = abs(obj1_xc - obj2_xc); //horizontal difference between centers
						double len2 = abs(obj1_yc - obj2_yc); //vertical difference between centers

						//core of the algorithm(if both are true, a collision has occurred:
						if((len1 <= ((obj1_xw+obj2_xw)/2)) && (len2 <= ((obj1_yh+obj2_yh)/2))){
							//set object collision to true for both objects
							n1->data.collision_otherobject = true;
							n2->data.collision_otherobject = true;
							//set new angles after collision
							//because velocity and mass are equal in each case, you only need to swap the angles
							angle1 = obj1.angle;
							angle2 = obj2.angle;
							n1->data.angle = angle2; //update object 1 to the new angle
							n2->data.angle = angle1; //update object 2 to the new angle
						}
						n2=n2->next; //increment the bottom loop
					}
					n1=n1->next; //increment the top loop
				}
				//moves all objects for this iteration of the simulation,
				//adjusting for detected collisions
		                Node * n = first;
				// loop through all the nodes in the linked list
		                 while (n != NULL) {
					Object obj = n->data;	//pull the object data out of the current node
					//update variables for current object that don't change based on collision
					d = obj.distance;
					x1 = obj.x_min;
					y1 = obj.y_min;
					x2 = obj.x_max;
					y2 = obj.y_max;
					if(obj.collision_corner){
						a = obj.angle + 180;	//set new angle based on type of collision
						Translate(x1,y1,x2,y2,d,a,n);	//move bounding box according to new angle
						n->data.collision_corner = false;	//reset corner collision tracker
						sound = true;
					}
					else if(obj.collision_clockwise){
						a = obj.angle + 270; //set new angle based on type of collision
						Translate(x1,y1,x2,y2,d,a,n); //move bounding box according to new angle
						n->data.collision_clockwise = false; //reset clockwise collision tracker
						sound = true;
					}
					else if(obj.collision_counterclockwise){
						a = obj.angle + 90; //set new angle based on type of collision
						Translate(x1,y1,x2,y2,d,a,n); //move bounding box according to new angle
						n->data.collision_counterclockwise = false; //reset counterclockwise collision tracker
						sound = true;
					}
					else if(obj.collision_otherobject){
						a = obj.angle; //new angle already calculated above
						Translate(x1,y1,x2,y2,d,a,n); //move bounding box according to new angle
						n->data.collision_otherobject = false; //reset other object collision tracker
						sound = true;
					}
					//if no collision has occurred for the given object
					else{
						a = obj.angle;
						Translate(x1,y1,x2,y2,d,a,n); //move bounding box according to existing data
					}
		                        n = n->next; //increment the loop
				}
		if(sound){
			Collision_Sound();
		}
		sound = false;
		RegisterWrite(BUFFER_OFFSET,1); //display the updated simulation on screen
	}

	//add node to the end of the linked list (adpated for the Object type)
	void System::append(struct Node ** head_ref, Object new_data) {
		struct Node * new_node = (struct Node * ) malloc(sizeof(struct Node)); //make the new node to be added to the end of the linked list
		struct Node * last = * head_ref; //pointer used to go throught the linked list, starts at the first node
		new_node->data = new_data; //set the data variable of the new node to the inputted data
		new_node->next = NULL; //set the pointer varianle of the new node to NULL
		while (last->next != NULL)
        		last = last->next;
   		last->next = new_node; //add the new node to the end of the linked list
	}

	//adds a shape in a random valid position on the screen; adds the object to the linked list of objects
	//called before simulation in clock cycle, takes the size of the object as an input
	void System::addObject(int size){
		//if the last shape in the list has been drawn, start over from the beginning
		if(objects == 10){
			objects = 0;
		}
		char shape = shape_list[objects]; //choose the next shape in the list to draw
		objects++; //increment through shape type list
		bool placed = true; //boolean to track if the shape has been successfully placed; changed to false when it's been placed
		bool valid; //boolean to track if randomly generated position is valid so far; switched to false, when all existing objects have been checked
		int x1; //variable to hold lower x bound of object
		int y1; //variable to hold lower y bound of object
		int x2; //variable to hold upper x bound of object
		int y2; //variable to hold upper y bound of object
		int angle; // variable to hold angle of objects movement;
		while(placed){
			valid = true; //set initial value of placed to true for new randomly generated shape
			x1 = (((rand() % 27) + 1) * 10); //randomly generates a multiple of 10 up til 270
			y1 = (((rand() % 19) + 1) * 10); //randomly generates a multiple of 10 up til 190
			// y = 2x
			//set the proportions for the diamond shape (2x:1y)
			//changes the size of the shape depending on the input
			if(shape == 'd'){
				if(size == 1){
					x2 = x1 + 10;
					y2 = y1 + 20;
				}
				else if(size == 2){
					x2 = x1 + 16;
					y2 = y1 + 32;
				}
				else if(size == 3){
					x2 = x1 + 20;
					y2 = y1 + 40;
				}
			}
			//sets the proportions for the trapezoid shape (1x:2y)
			//changes the size of the shape depending on the input
			else if(shape == 'a'){
				if(size == 1){
					x2 = x1 + 20;
					y2 = y1 + 10;
				}
				else if(size == 2){
					x2 = x1 + 32;
					y2 = y1 + 16;
				}
				else if(size == 3){
					x2 = x1 + 40;
					y2 = y1 + 20;
				}
			}
			//sets the proportions for the star and X shapes (1x:1.2y)
			//changes the size of the shape depending on the input
			else if(shape == 's' || shape== 'x'){
				if(size == 1){
					x2 = x1 + 12;
					y2 = y1 + 10;
				}
				else if(size == 2){
					x2 = x1 + 24;
					y2 = y1 + 20;
				}
				else if(size == 3){
					x2 = x1 + 36;
					y2 = y1 + 30;
				}
			}
			//sets the proportions for the rest of the shapes (1x:1y)
			//changes the size of the shape depending on the input
			else{
				if(size == 1){
					x2 = x1 + 10;
					y2 = y1 + 10;
				}
				else if(size == 2){
					x2 = x1 + 20;
					y2 = y1 + 20;
				}
				else if(size ==3){
					x2 = x1 + 30;
					y2 = y1 + 30;
				}
			}
			Node * n = first; //node for looping through all the objects current in the linked list, start at the first one
			//loop through the objects currently in the linked list and check if the new object would collide with any of their current positions
			while(n != NULL && valid){
				//object 1 is the new shape
				Object obj2 = n->data; //the pre-existing shape we're checking against
				//Insert object collision algorithm here
				double obj1_xc = (((double) x1)+x2)/2; //x-coordinate of center of object 1
				double obj1_yc = (((double) y1)+y2)/2; //y-coordinate of center of object 1
                                double obj1_xw = x2 - x1;  //width of object 1
                                double obj1_yh = y2 - y1;  //height of object 1

				double obj2_xc = (((double) obj2.x_min)+obj2.x_max)/2; //x-coordinate of center of object 2
				double obj2_yc = (((double) obj2.y_min)+obj2.y_max)/2; //y-coordinate of center of object 2
				double obj2_xw = obj2.x_max - obj2.x_min;  //width of object 2
				double obj2_yh = obj2.y_max - obj2.y_min;  //height of object 2

				double len1 = abs(obj1_xc - obj2_xc); //horizontal difference between centers
				double len2 = abs(obj1_yc - obj2_yc); //vertical difference between centers
				//if the objects collide, set the valid variable to false, ending the loop early
				//because we know the randomly generated variable is invalid
				if((len1 <= ((obj1_xw+obj2_xw)/2)) && (len2 <= ((obj1_yh+obj2_yh)/2))){
					valid = false;
				}
				n = n->next; //increment through the linked list
			}
			//if valid is still true after being checked against all the objects
			if(valid){
				placed = false; //end the while loop generating random positions for the new objects, because a valid position has been found
			}
		}
		angle = (rand() % 8) * 45; //randomly generate an angle
		Object obj; //new object to be added
		obj.UpdateObject(x1,y1,x2,y2,DEFAULT_DISTANCE,angle,shape); //update the object to the generated parameters
		append(&first,obj); //add the newly generated object to a node at the end of the linked list
	}

	//plays a sound for when a collision occurs; hooked up to D0 of JP2
	void System::Collision_Sound(){
	    int increment = 0; //variable for incrementing
	    int frequency = 191113;    //frequency of note played
	    int flip = 0; //variable for flipping the value output to the speaker
	    RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, frequency); //set the load to the frequency of the note to be played
	    RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
	    while(true)
	    {
	        if (RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0)
	        {
	            RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1);
	            if (increment < ((200000000*.05) / frequency))      //play the note for .05 of a second
	            {
	                if (flip == 0)
	                {
	                    RegisterWrite(JP2_PIN_DATA , 0);   //output 0 to the speaker
	                    flip = 1;          //flip
	                    increment++;   //increment by 1
	                }
	                else
	                {
	                    RegisterWrite(JP2_PIN_DATA , 1);   //output 1 to the speaker
	                    flip = 0;          //flip
	                    increment++;   //increment by 1
	                }
	            }
	            else
	                break;       //stop playing the note
	        }
	    }
		RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, ITERATION_TIME); //reset the timer load for the timer in the main
	}

//REPLACED WITH BUTTONS ON BOARD IN MAIN IMPLEMENTATION BECAUSE KEYPAD INCONSISTENT
        // returns: 1 - 1
        //              2-2
        //              3-3
        //              0-ignore
        //              4 - A
	//hooked up to JP1: D9-D21
        int System::check_keypad(){
                check = RegisterRead(JP1_DATA_OFFSET); //stores the current value of the JP1 pins
		if(check == -59905){
			return 1;
		}

                else if(check == -158209){
			return 2;
                }

                else if(check == -551425){
			return 3;
                }

                else if(check == -2124289){
			return 4; //A has been pushed
                }
		else{
			return 0;
		}
        }

