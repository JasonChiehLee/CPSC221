#include <iostream>
#include <fstream>
#include "Stack.h"
#include "Maze.h"

bool findPath(Stack& s, MazeLocation cur, Maze& m) {
	 
	 bool ans=false;
	 //base case, when it gets the finish location, return true
	 if(m.isFinished(cur)){
	 	s.push(cur);
	 	m.setChar(cur,'*');
	 	cout<<m<<endl;
	 	ans =true;
	 	return ans;
	 }else{
	 //search all the possible way out
	 	if(!m.invalidLocation(cur)){
		 	if(m.isOpen(cur)){
		 		m.setChar(cur,'*');
		 		cout<<m<<endl;
		 		s.push(cur);
		 		ans = findPath(s,cur.upOne(),m);
		 		if(ans==true)
		 			return ans;
		 		ans = findPath(s,cur.rightOne(),m);
		 		if(ans==true)
		 			return ans;
		 		ans = findPath(s,cur.leftOne(),m);
				if(ans==true)
		 			return ans;
				ans = findPath(s,cur.downOne(),m);
				if(ans==true)
		 			return ans;
	 	}
	 	//start backtrack
	 	else if(m.visited(s.getTop())&& !m.isOpen(s.getTop().upOne()) && !m.isOpen(s.getTop().downOne()) && !m.isOpen(s.getTop().leftOne()) && !m.isOpen(s.getTop().rightOne())){
		 		cur = s.getTop();
		 		m.setChar(cur,':');
		 		cout<<m<<endl;
		 		s.pop();
		 		findPath(s,cur,m);
		 		if(ans==true)
		 			return ans;
		}
		}
	}

	return ans;

}

/******************************************************************************************************************************************/


int main(int argc, char* argv[]) {

	// Main maze program

	// Consumes the maze data file as a command line argument.
	if (argc < 2) {
		cerr << "ERROR" << endl << "usage: ./pa2 maze.txt" << endl;
		return 1;
	}

	ifstream inputFile(argv[1]);

	// Calls maze constructor linked to input file.
	Maze m(inputFile);
	// Creates a stack with enough space to hold every location in m
	Stack s(m.getSize());

	if (findPath(s, m.getStart(), m)) {
		cout << "Maze solved" << endl;
	} else {
		cout << "Ahhh, couldn't get out!" << endl;
	}

	// Stack should contain only locations on path to finish (not dead-ends)
	// or be empty if no solution was found (as everything was popped off).
	//ofstream outputFile("solution1.txt");
	while(!s.isEmpty()) {
		s.printTop();
		s.pop();
	}

}
