#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include "box.h"
#include "system.h"
using namespace std;
const unsigned int MPCORE_PRIV_TIMER_LOAD_OFFSET = 0x3FFEC600; //offset for the timer load
const unsigned int MPCORE_PRIV_TIMER_COUNTER_OFFSET = 0x3FFEC604; //offset for the timer counter
const unsigned int MPCORE_PRIV_TIMER_CONTROL_OFFSET = 0x3FFEC608; //offset for the timer control
const unsigned int MPCORE_PRIV_TIMER_INTERRUPT_OFFSET = 0x3FFEC60C; //offset for the timer interrupt
const unsigned int ITERATION_TIME = 100000000/8; //time between iterations: quarter of a second
const unsigned int JP1_DIR_OFFSET = 0x3F200064; //Points to DATA Register for JP1
const unsigned int BUTTON_OFFSET =0x3F200050; //offset for the buttons on the board
const unsigned int JP1_DATA_OFFSET = 0x3F200060;

int main(void)
{
	cout << "Welcome to my this object collision simulation program!!"<< endl;
	cout << "Press button 3 on the board to begin the simulation" << endl;
	cout << "Press button 0 on the board to add another shape to the screen" << endl;
	cout << "Press button 1 on the board to decrease the size of the next added shape" << endl;
	cout << "Press button 2 on the board to increase the size of the next added shape" << endl;
	System *system =  new System; // create pointer to new system object

// code for registering the input from the keypad
//works for buttons 1,2,3,A; could be adapted to code below
// NOT USED BECAUSE KEYPAD IS LIKELY BROKEN(4 button would register when nothing is pressed down)
/*
	int duplicate = 0; //checks when switch has been flipped
	int repeat = 0; //only allows a new number after the switch has been flipped, used to avoid duplicate button reading
	while(true){
		int check = system->check_keypad();
		if(system->RegisterRead(0x3F200040)!= duplicate){
			duplicate  = system->RegisterRead(0x3F200040) ;
			repeat = 1;
		}
        	if(check != 0 && repeat == 1){
			cout<<check << endl;
			repeat = 0;
		}
	}
*/


	int checkbutton = 0; //used to make sure a button press is only registered once
	int bit0; //used for input from button 0
	int bit1; //used for input from button 1
	int bit2; //used for input from button 2
	int bit3; //used for input from button 3
        int size=2; //set default size of the added shape to medium
	//wait for a button to be pressed to start the simulation
	while(true){
		//makes sure it isn't a duplicate button press
                if(system->RegisterRead(BUTTON_OFFSET) != checkbutton){
                        bit1 = ((system->RegisterRead(BUTTON_OFFSET) & (1 << 1)) >> 1); //get input from button 1
                        bit2 = ((system->RegisterRead(BUTTON_OFFSET) & (1 << 2)) >> 2); //get input from button 2
                        bit3 = ((system->RegisterRead(BUTTON_OFFSET) & (1 << 3)) >> 3); //get input from button 3
			//if button 3 is pressed, end the while loop, starting the simulation
                        if(bit3 == 1){
                                break;
                        }
			//if button 1 is pressed, decrease the size of the added shape with a lower bound of 1(small)
                        if(bit1 == 1 && size > 1){
                                size = size - 1;
                        }
			//if button 2 is pressed, increase the size of the added shape with an upper bound of 3(large)
                        if(bit2 == 1 && size < 3){
                                size = size+1;
                        }
			//uesd for avoiding duplicate button presses
                        checkbutton = system->RegisterRead(BUTTON_OFFSET);
                }
	}


	int s = 400; //number of iterations the simulation runs
	system->RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, ITERATION_TIME); //initialize timer load to time between iterations
	system->RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3); //initialize timer control to 3
	int i=0; //tracks number of iterations the simulation has been thru
	system->SimulationStart(size); //displays the first frame of the simulation
	//while the simulation has not been iterated the given number of times
	while(i<s){
		//check for button presses
		if(system->RegisterRead(BUTTON_OFFSET) != checkbutton){
			bit0 = ((system->RegisterRead(BUTTON_OFFSET) & (1 << 0)) >> 0); //get input from button 0
			bit1 = ((system->RegisterRead(BUTTON_OFFSET) & (1 << 1)) >> 1); //get input from button 1
			bit2 = ((system->RegisterRead(BUTTON_OFFSET) & (1 << 2)) >> 2); //get input from button 2
			//if button 0 is pressed, add an object
			if(bit0 == 1){
				system->addObject(size);
			}
			//if button 1 is pressed, decrease size with a lower bound of 1 (small)
			if(bit1 == 1 && size > 1){
				size = size - 1;
			}
			//if button 2 is pressed, increase size with an upper bound of 3 (large)
			if(bit2 == 1 && size < 3){
				size = size+1;
			}
			checkbutton = system->RegisterRead(BUTTON_OFFSET);
		}
		if(system->RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0)
		{
			system->RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1);
			//reset timer flag
			//display the next iteration of the simulation
			system->Simulation();
			i++; //increment the variable counting the number of iterations
		}
	}


	delete system; //delete the system pointer
	cout << "Ending program..." << endl;
	return 0;

}
