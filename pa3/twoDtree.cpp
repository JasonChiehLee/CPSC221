
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
	stats s = stats(imIn);
	root = buildTree(s,make_pair(0,0),make_pair(width-1,height-1));

}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	// std::cout<<"ul.first"<<ul.first<<std::endl;
	// std::cout<<"ul.second"<<ul.second<<std::endl;
	// std::cout<<"lr.first"<<lr.first<<std::endl;
	// std::cout<<"lr.second"<<lr.second<<std::endl;
	RGBAPixel a = s.getAvg(ul,lr);
	Node *node = new Node(ul,lr,a);

	//base case 
	if(ul.first==lr.first && ul.second==lr.second)
		return node;

	pair<int,int> newUl;
	pair<int,int> newLr;
	long double lineBreaker;	

	//vertical
	int xStartsAt = ul.first;
	int xEndsAt   = lr.first;

	long double score1;
	long double score2;

	for(int i = xStartsAt; i < xEndsAt; i++){
		score1 = s.getScore(ul,make_pair(i,lr.second));
		score2 = s.getScore(make_pair(i+1,ul.second),lr);
		
		if(i==xStartsAt){
			lineBreaker = score1 + score2;
			newUl = make_pair(i+1,ul.second);
			newLr = make_pair(i,lr.second);
		}else if(score1+score2<lineBreaker){
			lineBreaker = score1 + score2;
			newUl = make_pair(i+1,ul.second);
			newLr = make_pair(i,lr.second);

		}
	}

	//horizantal 
	int yStartsAt = ul.second;
	int yEndsAt   = lr.second;

	long double score3;
	long double score4;

	for(int i = yStartsAt; i < yEndsAt; i++){
		score3 = s.getScore(ul,make_pair(lr.first,i));
		score4 = s.getScore(make_pair(ul.first,i+1),lr);

		if(i==yStartsAt){
			lineBreaker = score3 + score4;
			newUl = make_pair(ul.first,i+1);
			newLr = make_pair(lr.first,i);
		}else if(score3+score4<lineBreaker){
			lineBreaker = score3 + score4;
			newUl = make_pair(ul.first,i+1);
			newLr = make_pair(lr.first,i);
		}
	}
		node->left = buildTree(s,ul,newLr);
		node->right = buildTree(s,newUl,lr);

	return node;

}


PNG twoDtree::render(){
 	PNG png = PNG(width, height);
	renderHelper(png, root);

	return png;
}

void twoDtree::renderHelper(PNG &png, Node* subroot){
	if(subroot==NULL){
		return;
	}

	int x1 = subroot->upLeft.first;
	int y1 = subroot->upLeft.second;
	int x2 = subroot->lowRight.first;
	int y2 = subroot->lowRight.second;

	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {

			*png.getPixel(i, j) = subroot->avg;
		}
	}
	renderHelper(png, subroot->left);
	renderHelper(png, subroot->right);

}




void twoDtree::prune(double pct, int tol){
	executePrune(pct,tol,root);
}

void twoDtree::executePrune(double pct, int tol, Node *subroot){
	if(subroot==NULL){
		return;
	}
	if(subroot->left==NULL && subroot->right==NULL){
		return;
	}

	long double nodeShouldPrune = pruneNodes(root,root->avg,tol);
	long double leafSum = leafNodes(root);

	long double percent = nodeShouldPrune/leafSum;

	if(percent>=pct){
		clearHelper(root->left);
		root->left=NULL;
		clearHelper(root->right);
		root->right=NULL;
	}else{
		executePrune(pct,tol,subroot->left);
		executePrune(pct,tol,subroot->right);	
	}

}



long twoDtree::pruneNodes(Node *subroot, RGBAPixel a, int tol){
	if(subroot==NULL) 
		return 0;
	
    if(subroot->left==NULL && subroot->right==NULL){
	 	long r = subroot->avg.r;
		long g = subroot->avg.g;
		long b = subroot->avg.b;

		long long dis = (r - a.r)*(r - a.r) 
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

long twoDtree::leafNodes(Node *subroot){
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


