
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ 
	height = imIn.height();
	width  = imIn.width();
	stats s(imIn);
	pair<int, int> ul = make_pair(0, 0);
	pair<int, int> lr = make_pair(width - 1, height - 1);
	root = buildTree(s,ul,lr);

}


twoDtree::Node * twoDtree::buildTree(stats & s, pair<int, int> ul, pair<int, int> lr) {
	RGBAPixel a = s.getAvg(ul, lr);
	Node* node = new Node(ul, lr, a);

	int x1 = ul.first;
	int x2 = lr.first;
	int y1 = ul.second;
	int y2 = lr.second;

	if (x1 == x2 && y1 == y2)return node;

	long lineBreaker=LONG_MAX;
	pair<int, int> newLr;
	pair<int, int> newUl;

	for (int i = x1; i < x2; i++) {
		pair<int, int> new_lr = make_pair(i, y2);
		pair<int, int> new_ul = make_pair(i + 1, y1);

		long double score1 = s.getScore(ul, new_lr);
		long double score2 = s.getScore(new_ul, lr);

		if (i == x1 || score1 + score2 <lineBreaker ) {
			lineBreaker = score1 + score2;
			newLr = new_lr;
			newUl = new_ul;
		}
	}

	for (int i = y1; i < y2; i++) {
		pair<int, int> new_lr = make_pair(x2, i);
		pair<int, int> new_ul = make_pair(x1, i + 1);

		long score1 = s.getScore(ul, new_lr);
		long score2 = s.getScore(new_ul, lr);

		if ((i == y1 && x1 ==x2) || score1 + score2 <=lineBreaker) {
			lineBreaker = score1 + score2;
			newLr = new_lr;
			newUl = new_ul;
		}
	}

	node->left = buildTree(s, ul, newLr);
	node->right = buildTree(s, newUl, lr);

	return node;
}


PNG twoDtree::render(){
 	PNG pic(width, height);
	renderHelper(pic, root);

	return pic;
}

void twoDtree::renderHelper(PNG &png, Node* subroot){
	if (subroot->left == NULL && subroot->right == NULL) {
		int x1 = subroot->upLeft.first;
		int y1 = subroot->upLeft.second;
		int x2 = subroot->lowRight.first;
		int y2 = subroot->lowRight.second;

		for (int i = x1; i <= x2; i++) {
			for (int j = y1; j <= y2; j++) {

				*png.getPixel(i, j) = subroot->avg;
			}
		}
	} else {
		renderHelper(png, subroot->left);
		renderHelper(png, subroot->right);
	}

}




void twoDtree::prune(double pct, int tol){
	executePrune(pct,tol,root);
}


void twoDtree::executePrune(double& pct, int& tol, Node *subroot){
	if(subroot==NULL){
		return;
	}
	if(subroot->left==NULL && subroot->right==NULL){
		return;
	}
	RGBAPixel passAvg = subroot->avg;
	int nodeShouldPrune = pruneNodes(subroot, passAvg,tol);
	int leafSum = leafNodes(subroot);
	double percent = (double)nodeShouldPrune/(double)leafSum;

	if(percent>=pct){
		clearHelper(subroot->left);
		subroot->left=NULL;
		clearHelper(subroot->right);
		subroot->right=NULL;

	}else{
		executePrune(pct,tol,subroot->left);
		executePrune(pct,tol,subroot->right);	
	}

}


int twoDtree::pruneNodes(Node *subroot, RGBAPixel& a, int& tol){
	if(subroot==NULL) 
		return 0;
	
    if(subroot->left==NULL && subroot->right==NULL){
	 	int r = subroot->avg.r;
		int g = subroot->avg.g;
		int b = subroot->avg.b;

		int dis = (r - a.r)*(r - a.r)
				 +(g - a.g)*(g - a.g)
				 +(b - a.b)*(b - a.b);
		if(dis <=tol){
			return 1;
		}else{
			return 0;
		}  	
    }
    return pruneNodes(subroot->left, a, tol)+pruneNodes(subroot->right, a, tol);

}

int twoDtree::leafNodes(Node *subroot){
	if(subroot==NULL) 
		return 0;

	if(subroot->left==NULL && subroot->right==NULL){
		return 1;
	}
	return leafNodes(subroot->left)+leafNodes(subroot->right);

}

void twoDtree::clear() {
	clearHelper(root);
	root = NULL;
	height = 0;
	width = 0;
}

void twoDtree::clearHelper(Node *subroot){
	if(subroot == NULL){
		return;
	}
	clearHelper(subroot->left);
	clearHelper(subroot->right);
	delete subroot;
	subroot=NULL;


}


void twoDtree::copy(const twoDtree & orig){
	root   = nodeCopy(orig.root);
	height = orig.height;
	width  = orig.width;
}

twoDtree::Node * twoDtree::nodeCopy(Node *subroot){
	if(subroot==NULL){
		return NULL;
	}

	Node *newNode = new Node(subroot->upLeft,subroot->lowRight,subroot->avg);
	newNode->left = nodeCopy(subroot->left);
	newNode->right= nodeCopy(subroot->right);
	return newNode;
}


