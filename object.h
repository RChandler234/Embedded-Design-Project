#ifndef OBJECT_H
#define OBJECT_H
using namespace std;

class Object {
	public:
		int x_min;
		int y_min;
		int x_max;
		int y_max;
		bool collision_corner;
		bool collision_clockwise;
		bool collision_counterclockwise;
		bool collision_otherobject;
		char shape; //variable for tracking what type of shape the object is
		int distance; //variable for tracking what number of pixels the object moves each iteration
		int angle; //variable for tracking what angle the object moves each iteration
		Object();
		~Object();
		void UpdateObject(int x1, int y1, int x2, int y2, int d, int a, char s);
};

#endif

