#include "chain.h"
#include "chain_given.cpp"

// PA1 functions you need to implement:

Chain::~Chain(){ 
	clear();
}

void Chain::insertFront(const Block & ndata){
	//constrcut a new node
	Node *newBlock = new Node(ndata);
	//if no element
	if(size()==0){		
		newBlock->next = NULL;
		newBlock->prev = NULL;
		head_ = newBlock;
		tail_ = newBlock;
		length_++;
	}else{
		head_->prev = newBlock;
		newBlock->prev = NULL;
		newBlock->next = head_;
		head_ = newBlock;
		length_++;

	}

}

void Chain::insertBack(const Block & ndata){
   	Node *newBlock = new Node(ndata);
   	if(size()==0){
   		newBlock->next = NULL;
   		newBlock->prev = NULL;
   		head_ = newBlock;
		tail_ = newBlock;
		length_++;
   	}else{
   		newBlock->next = NULL;
   		tail_->next = newBlock;
   		newBlock->prev = tail_;
   		tail_ = newBlock;
   		length_++;
   	}
}

const Block* Chain::getBlock(int index) {
	if(size()==0){
		return NULL;
	}
	int i = 0;
	Node *pointer;
	pointer = head_;
	while(i<index && i < size()-1){
		pointer = pointer->next;
		i++;
	}
	const Block *blockPointer;
	blockPointer = &(pointer->data);
	return blockPointer;
}

void Chain::removeFront() {
	if(size()==0){
		return;
	}else if(size()==1){
		head_ = NULL;
		tail_ = NULL;
		length_ = 0;
	}else{
		Node *temp;
		temp = head_;
		head_ = head_->next;
		head_->prev = NULL;
		delete temp;
		temp = NULL;
		length_--;
	}
}

void Chain::removeBack() {
	if(size()==0){
		return;
	}else if(size()==1){
		head_= NULL;
		tail_= NULL;
		length_ = 0;
	}else{
		Node *temp;
		temp = tail_;
		tail_ = tail_->prev; 
		tail_->next =NULL;
		temp->prev = NULL;
		delete temp;
		temp = NULL;
		length_--;
	}
}

void Chain::replaceBlock(int index, const Block& newB) {
	if(size()==0){
		std::cout<<"can't get from empty chain"<<std::endl;
		return;
	}else if(index > size()-1){
		return;
	}else if(index==0 && size()!=0){
		removeFront();
		insertFront(newB);
	}else if(index==(size()-1)){
		removeBack();
		insertBack(newB);
	}else{
		int i = 0;
		Node *pointer = head_;
		while(i<index){
			pointer = pointer->next;
			i++;
		}
		Node *color = new Node(newB);		
		color->prev = pointer->prev;
		color->next = pointer->next;
		pointer->prev->next = color;
		pointer->next->prev = color;
		delete pointer;
		pointer = NULL;
	}
}



void Chain::removeAt(int index){
	if(size()==0){
		return;
	}else if(index==0){
		removeFront();
		return;
	}else if(index==size()-1){
		removeBack();
		return;
	}else{
		int i = 0;
		Node *pointer = head_;
		while(i<index){
			pointer = pointer->next;
			i++;
		}
		pointer->prev->next = pointer->next;
		pointer->next->prev = pointer->prev;
	}

	
}

void Chain::insertAt(int pos,Node *node){
	if(pos > size()-1){
		return;
	}else if(pos==0 && size()!=0){
		removeFront();
		//Node *newBlock = new Node(ndata);
		head_->prev = node;
		node->prev = NULL;
		node->next = head_;
		head_ = node;
		length_++;
	}else if(pos==(size()-1)){
		removeBack();
		node->next = NULL;
   		tail_->next = node;
   		node->prev = tail_;
   		tail_ = node;
   		length_++;
	}else{
		int i = 0;
		Node *pointer = head_;
		while(i<pos && pos<size()){
			pointer = pointer->next;
			i++;
		}
		//Node *color = new Node(newB);		
		node->prev = pointer->prev;
		node->next = pointer->next;
		pointer->prev->next = node;
		pointer->next->prev = node;
		delete pointer;
		pointer = NULL;
	}
		
}

void Chain::swap(int pos1, int pos2){
	if(pos1==pos2){
		return;
	}else if(pos1<size() && pos2<size()){
		const Block *block1 = getBlock(pos1);
		Node *nodePos1 = new Node(*block1);
		const Block *block2 = getBlock(pos2);
		Node *nodePos2 = new Node(*block2);
		insertAt(pos1,nodePos2);
		insertAt(pos2,nodePos1);
	}else{
		return;
	}
	

}

void Chain::checkeredSwap(Chain &other) {
	//if two chains size not equals
	if(size()!=other.size()){
	 	std::cout << "Block sizes differ." << std::endl;
	 	//ends program
	 	return;
	}
	//if both chains have only one element
	if(size()==1){
		//end program
		return;
	}
	int i = 1;
	Chain colorPic = other;
	while(i<size()){
		const Block *block1 = getBlock(i);
		other.replaceBlock(i,*block1);
		const Block *block2 = colorPic.getBlock(i);
		replaceBlock(i,*block2);
		i= i + 2;
	}

}

void Chain::moveToBack(int startPos, int len){
	int i = 1;
	if(size()==0){
		return;
	}else if(startPos+len == size()){
		return;
	}else if(startPos+len > size()){
		std::cout<<"out of bound"<<std::endl;
		return;
	}else if(len!=0){
		while(i<len+1){
			const Block *saveBlockData = getBlock(startPos);
			Node *replaceNode = new Node(*saveBlockData);
			removeAt(startPos);
   			replaceNode->next = NULL;
   			tail_->next = replaceNode;
   			replaceNode->prev = tail_;
   			tail_ = replaceNode;
   			length_++;

			i++;
		}
	}	
}

void Chain::rotate(int k){
	moveToBack(0,k);
}

