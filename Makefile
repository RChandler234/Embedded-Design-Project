display: DE1SoCfpga.o shapes.o box.o node.o system.o object.o main.o
	g++ DE1SoCfpga.o shapes.o box.o node.o system.o object.o main.o -o runproject 

DE1SoCfpga.o: DE1SoCfpga.cpp DE1SoCfpga.h
	g++ -g -Wall -c DE1SoCfpga.cpp 

shapes.o: shapes.cpp shapes.h
	g++ -g -Wall -c shapes.cpp

box.o: box.cpp box.h
	g++ -g -Wall -c box.cpp 

node.o: node.cpp node.h
	g++ -g -Wall -c node.cpp

system.o: system.cpp system.h
	g++ -g -Wall -c system.cpp 

object.o: object.cpp object.h
	g++ -g -Wall -c object.cpp

main.o: main.cpp
	g++ -g -Wall -c main.cpp
	
clean:
	rm DE1SoCfpga.o shapes.o box.o node.o system.o object.o main.o runproject
