#ifndef SHAPES_H
#define SHAPES_H
#include "DE1SoCfpga.h"
class Shapes : public DE1SoCfpga
{
private:
public:
        Shapes();
        ~Shapes();
        void Clear(int x1, int y1, int x2, int y2);
	void Draw_Box(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_T(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_L(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_V(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_X(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_Star(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_Triangle(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_Diamond(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_Plus(int x1, int y1, int x2, int y2, short pixel_color);
	void Draw_Trapezoid(int x1, int y1, int x2, int y2, short pixel_color);
};

#endif

