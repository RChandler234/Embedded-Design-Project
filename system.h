#ifndef SYSTEM_H
#define SYSTEM_H
#include "box.h"
#include "node.h"
class System : public Box
{
private:
	int objects;
	Node * first;
	int check;
public:
        System();
        ~System();
	void SimulationStart(int size);
	void Simulation();
	void append(struct Node ** head_ref, Object new_data);
	void addObject(int size);
	void Collision_Sound();
	int check_keypad();
};

#endif

