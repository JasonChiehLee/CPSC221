
#include "Stack.h"

/**
*
*stack constructor
*
**/
Stack::Stack(int size) {
  top = -1;
  MAX_SIZE = size;
  data = new MazeLocation[size];
}

/**
*
*check if the stack is empty
*
**/
bool Stack::isEmpty() const {
    if(top==-1){
    	return true;
    }else{
    	return false;
    }
	
}


/**
*
*check if the stack is full
*
**/
bool Stack::isFull() const {
	if(top==MAX_SIZE-1){
		return true;
	}
	return false;
}

/**
*
*stack deconstructor
*
**/
Stack::~Stack() {
	delete[] data;
	data = NULL;
	MAX_SIZE=0;
	top=-1;
}

/**
*
*push mazelocation to the stack
*
**/
void Stack::push(MazeLocation m) {
	if(top == MAX_SIZE-1){
		return;
	}
	if(top<MAX_SIZE-1){
		data[++top] = m;
		
	}
}

/**
*
*set pointer to point to the top element in the stack 
*
**/
void Stack::pop() {
	if(isEmpty()==false){
		top--;
	}else{
		return;
	}

	
}

/**
*
*print out the top element of the stack
*
**/
void Stack::printTop() {
	cout<<data[top];
}

/**
*
*Helper function added to be used in main 
*
**/
MazeLocation Stack::getTop(){
	return data[top];
}

