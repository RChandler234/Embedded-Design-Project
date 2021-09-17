#ifndef BOX_H
#define BOX_H
#include "shapes.h"
#include "object.h"
#include "node.h"
class Box : public Shapes
{
public:
	Box();
	~Box();
	void Draw_Shape(int x1, int y1, int x2, int y2, char s);
	void Translate(int x1, int y1, int x2, int y2, int d, int a, Node * n);
};

#endif

