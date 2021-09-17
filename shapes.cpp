#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include<math.h>
#include "shapes.h"
using namespace std;

//A class with all the functions for drawing the 10 shapes, given the bounds of the bounding box of the shape
// Inherits directly from DE1SoCfpga class

const unsigned int BUFFER_OFFSET      = 0x3F203020;  //offset for the pixel_buffer
const unsigned int PIXEL_BUFFER_OFFSET     = 0x8000000;  //offset for where the info on the buffer is stored
const unsigned int BACK_BUFFER_OFFSET = 0x3F203024; //offset for the back buffer
const unsigned int PIXEL_BACK_BUFFER_OFFSET = 0x0; //offset for where the pixel info for the back buffer is stored
const int X_WIDTH = 320;  // Width of the display
const int Y_HEIGHT = 240; // Height of the display

   //Constructor
   Shapes::Shapes(){
	//Write the new address for the pixel_back_buffer to the back buffer
        RegisterWrite(BACK_BUFFER_OFFSET, 0xC0000000);
	//Make sure the screen is clear before starting
	Clear(0,0,X_WIDTH,Y_HEIGHT);
   };
   //Deconstructor
   Shapes::~Shapes(){
      //Turn off all the pixels when the process is done
        Clear(0,0,X_WIDTH,Y_HEIGHT);
   };
	 //Turns off the pixels in the given section in the back buffer
	//x1: lower x bound, y1: lower y bound
	//x2: upper x bound, y2: upper y bound
        void Shapes::Clear(int x1, int y1, int x2, int y2){
                int pixel_ptr; //variable for holding address of each pixel
                int row; //variable for holding all the y values
                int col; //variable for holding all the x values
		//Nested for loop to go thru every pixel in the bounds (bc it's clearing all the pixels in the box)
                for (row = y1; row <= y2; row++)
                        for (col = x1; col <= x2; ++col) {
                                pixel_ptr = (row << (10)) + (col << 1);//convert the x and y position to an address of a pixel
                                RegisterWrite(pixel_ptr, 0); // write a color to the pixel address
                        }
        }

        //Draws a box in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
	//pixel_color: color being assigned to the drawing
        void Shapes::Draw_Box(int x1, int y1, int x2, int y2, short pixel_color) {
                int pixel_ptr; //variable for holding address of each pixel
                int row; //variable for holding all the y values
                int col; //variable for holding all the x values
		//Nested for loop to go thru every pixel in the bounds (bc it's a box)
                for (row = y1; row <= y2; row++)
                        for (col = x1; col <= x2; ++col) {
                                pixel_ptr = (row << (10)) + (col << 1); //convert the x and y position to an address of a pixel
                	        RegisterWrite(pixel_ptr, pixel_color); // write a color to the pixel address
                        }
        }

        //Draws a T in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	void Shapes::Draw_T(int x1, int y1, int x2, int y2, short pixel_color)
	{
		int pixel_ptr; //variable for holding address of each pixel
		int row;//variable for holding all the y values
		int col;//variable for holding all the x values
		int l; // width of top piece of T
		int  w; //width of vertical piece of the T
                int base = (x1 + x2)/2;  //mid point of base of T
		int increment = 0; // incrementing variable
		l = (int)(((y2-y1)*0.2)+0.5);   //.2 signifies the percent of total height
		w = (int)(((x2-x1)*0.18)+0.5); // .18 signifies the percent of total width
		for (row = y1; row <= y2; row++)
		{
			if (increment < l)   //checks when to switch from the horizontal to verical part
			{
				for(col = x1; col <= x2; col++)   // draws top part of T
				{
					pixel_ptr = (row << (10)) + (col << 1); //converts x/y position to a pixel address
					RegisterWrite(pixel_ptr, pixel_color); //Writes the color to the pixel address
				}
				increment++; //increment the incrementing variable
			}
			else
			{
				for(col = base - (w/2); col <= base + (w/2); col++)   // Draws vertical part of T
				{
					pixel_ptr = (row << (10)) + (col << 1); //converts the x/y position to a pixel address
					RegisterWrite(pixel_ptr, pixel_color); //Writes the color to the pixel address
				}
			}
		}
	}

        //Draws a L in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	void Shapes::Draw_L(int x1, int y1, int x2, int y2, short pixel_color)
	{
		int pixel_ptr; //variable for holding address of each pixel
		int row;//variable for holding all the y values
		int col; //variable for holding all the x values
		int l;//length of vertical part of L
		int  w; //width of vertical part of L
		l = (int)((y2-y1)*0.9);  //.9 is the percent of the total height
		w = (int)((x2-x1)*0.2);   //.2 is the percent of the total width
		int increment = 0; //incrementing variable
		for (row = y1; row <= y2; row++)
		{
			if (increment < l)   //checks for when vertical part of L is done being drawn
			{
				for(col = x1; col <= x1+w; col++)  //vertical part of L
				{
					pixel_ptr = (row << (10)) + (col << 1); //converts the x/y position to a pixel address
					RegisterWrite(pixel_ptr, pixel_color); //Writes the color to the pixel address
				}
				increment++; //increment incrementing variable
			}
			else
			{
				for(col = x1; col <= x2; col++) //horizontal part of the L
				{
					pixel_ptr = (row << (10)) + (col << 1);//converts the x/y position to a pixel address
					RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
				}
			}
		}
	}

        //Draws a V in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	void Shapes::Draw_V(int x1, int y1, int x2, int y2, short pixel_color)
	{
		int pixel_ptr;//variable for holding address of each pixel
		int row; //variable for holding all the y values
		int col; //variable for holding all the x values
		int thick = (int) ((x2-x1)*.2);   //thickness of the block V
		int increment = 0;	//incrementing variable
		for (row = y1; row <= y2; row++)    //left diagonal
		{
			for (col= x1+increment/2; col <= x1+increment/2+thick; ++col)
			{
				pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
				RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
			}
			increment++;//increment incrementing variable
		}
		increment = 0; //reset incrementing variable for use in the right diagonal
		for (row = y1; row <= y2; row++)    //right diagonal
		{
			for (col= x2-(increment/2)-thick; (col <= x2-(increment/2)); ++col)
			{
				pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
				//doesn't double write a value to a pixel
				if(RegisterRead(pixel_ptr) != pixel_color){
					RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
				}
			}
			increment++; //increment the incrementing variable
		}
	}

        //Draws a X in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	//difference in x's = 1.2(difference in y's)
	void Shapes::Draw_X(int x1, int y1, int x2, int y2, short pixel_color)
	{
		int pixel_ptr;//variable for holding address of each pixel
		int row;//variable for holding all the y values
		int col;//variable for holding all the x values
		int thick = (int) ((x2-x1)*.2); //thickness of the x
		int increment = 0; //incrementing variable
		for (row = y1; row <= y2; row++)    //top left to bottom right diagonal
		{
			for (col= x1+increment; col <= x1+increment+thick; ++col)
			{
				if(col<=x2 && col>=x1 && row<=y2 && row>=y1){
					pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
					RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
				}
			}
			increment++;//increment the incrementing variable
		}
		increment = 0; //reset incrementing the incrementing variable
		for (row = y2; row >= y1; row--)    //bottom left to top right diagonal
		{
			for (col= x1+increment; col <= x1+increment+thick; ++col)
			{
				//double checks that the drawing coordinates are within the bounds
                                if(col<=x2 && col>=x1 && row<=y2 && row>=y1){
					pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
					RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
				}
			}
			increment++;//increment the incrementing variable
		}
	}

        //Draws a Star in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
       //difference in x's = 1.2(difference in y's)
        void Shapes::Draw_Star(int x1, int y1, int x2, int y2, short pixel_color)
        {
                int pixel_ptr;//variable for holding address of each pixel
                int row;//variable for holding all the y values
                int col;//variable for holding all the x values
                int thick = (int) ((x2-x1)*.2); //thickness of the diagonals and line through the middle
		int mid = (int) (x2+x1)/2; //midpoint of the x's
                int increment = 0; // incrementing variable
                for (row = y1; row <= y2; row++)    //top left to bottom right diagonal
                {
                        for (col= x1+increment; col <= x1+increment+thick; ++col)
                        {
                                if(col<=x2 && col>=x1 && row<=y2 && row>=y1)
				{
                                        pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
                                        RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
                                }
                        }
                increment++; //increment incrementing variable
                }
                increment = 0;//reset incrementing variable for use in the next diagonal
                for (row = y2; row >= y1; row--)    //top right to bottom left diagonal
                {
                        for (col= x1+increment; col <= x1+increment+thick; ++col)
                        {
                                if(col<=x2 && col>=x1 && row<=y2 && row>=y1)
				{
                                        pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
                                        RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
                                }
                        }
                increment++;//increment incrementing variable
                }
		//draw middle line of the star/asterisk
                for (row = y1; row <= y2; row++)
		{
                        for (col = mid-(thick/2); col <= mid+(thick/2); ++col) {
                               pixel_ptr = (row << (10)) + (col << 1);//converts the x/y position to a pixel address
                 	       RegisterWrite(pixel_ptr, pixel_color); //Writes the color to the pixel address
                        }
		}
        }

        //Draws a Triangle in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	// difference between y's = difference between x's for proportions
	void Shapes::Draw_Triangle(int x1, int y1, int x2, int y2, short pixel_color){
		int pixel_ptr;//variable for holding address of each pixel
		int row;//variable for holding all the y values
		int col;//variable for holding all the x values
		int increment = 0;//reset incrementing variable for use in the next diagonal
		for(row = y2; row>=y1;row--){
			for(col = x1+(increment/2);col<=x2-(increment/2);col++){
                                if(col<=x2 && col>=x1 && row<=y2 && row>=y1){
                                        pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
                                        RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
				}
			}
		increment++; //increment incrementing variable
		}
	}

        //Draws a Diamond in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	// difference between y's = 2*(difference between x's)
	void Shapes::Draw_Diamond(int x1, int y1, int x2, int y2, short pixel_color){
		int pixel_ptr;//variable for holding address of each pixel
		int row;//variable for holding all the y values
		int col;//variable for holding all the x values
		int increment = 0; //incrementing variable
		int halfway = (y1+y2)/2;//halfway point in height
		Draw_Triangle(x1,y1,x2,halfway,pixel_color); //draw a triangle on the top half of the diamond
		//draw an upside triangle on the bottom half of the diamond
		for(row = (y1+y2)/2;row<=y2;row++){
                        for(col = x1+(increment/2);col<=x2-(increment/2);col++)
			{
                                if(col<=x2 && col>=x1 && row<=y2 && row>=y1){
                                        pixel_ptr =(row << (10)) + (col << 1);//converts the x/y position to a pixel address
                                        RegisterWrite(pixel_ptr, pixel_color);//Writes the color to the pixel address
                                }
                        }
		increment++; //increment incrementing variable
		}

	}

        //Draws a Plus in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	//difference between x's = difference between y's
	void Shapes::Draw_Plus(int x1,int y1, int x2, int y2, short pixel_color){
		int thick =(x2-x1)*0.2; //thickness of the plus
		int midx = (x2+x1)/2; //midpoint of the x's
		int midy = (y2+y1)/2; //midpoint of the y's
		Draw_Box(x1,midy-(thick/2),x2,midy+(thick/2),pixel_color); //draws middle horizontal part of plus
		Draw_Box(midx-(thick/2),y1,midx+(thick/2),midy-(thick/2)-1,pixel_color); //draws top part of the vertical part of the plus
		Draw_Box(midx-(thick/2),midy+(thick/2)+1,midx+(thick/2),y2,pixel_color); //draw bottom part of the vertical part of the plus
	}

        //Draws a Trapezoid in the given section of the back buffer in the specified color
        //x1: lower x bound, y1: lower y bound
        //x2: upper x bound, y2: upper y bound
        //pixel_color: color being assigned to the drawing
	// difference between x's = 2*difference between y's
	void Shapes::Draw_Trapezoid(int x1, int y1, int x2, int y2, short pixel_color){
		Draw_Triangle(x1,y1,x2,y2,pixel_color); //draws a triangle, but cuts off the top portion
	}
