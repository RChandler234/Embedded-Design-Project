#ifndef DE1SOCFPGA_H
#define DE1SOCFPGA_H

class DE1SoCfpga
{
public:
	char *pBase; //variable for initializing memory
	int fd; //variable for initializing memory
	DE1SoCfpga();
	~DE1SoCfpga();
	void RegisterWrite(unsigned int offset, int value);
	int RegisterRead(unsigned int offset);
};

#endif

