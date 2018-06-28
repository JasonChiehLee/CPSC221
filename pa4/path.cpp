#include "path.h"
#include <queue>
#include <stack>
using namespace std;

path::path(const PNG & im, pair<int,int> s, pair<int,int> e) {
	start = s;
	end = e;
	image = im;
	BFS();
}

void path::BFS(){
	vector<vector<bool>> V(image.height(), vector<bool> (image.width(),false));
	vector<vector<pair<int,int>>> P(image.height(), vector<pair<int,int>> (image.width(),end));
	queue<pair<int,int>> locationQ;

	V[start.second][start.first] = true;
	P[start.second][start.first] = start;
	locationQ.push(start);

	while(!locationQ.empty()){
		pair<int,int> pix = locationQ.front();
        locationQ.pop();
        vector<pair<int,int>> neighbor = neighbors(pix);
        for(unsigned int i=0; i < neighbor.size(); i++){
            if(good(V,pix,neighbor[i])){
                V[neighbor[i].second][neighbor[i].first] = true;
                locationQ.push(neighbor[i]);
                P[neighbor[i].second][neighbor[i].first]=pix;
            }   
                  
        }

	}
	
	pathPts = assemble(P,start,end);
}

PNG path::render(){
	PNG img(image);
    vector<pair<int,int>> paths = getPath();
    for(int i = 0; i < length();i++){
        RGBAPixel* pixel = img.getPixel(paths[i].first,paths[i].second);
        pixel->r = 255;
        pixel->g = 0;
        pixel->b = 0;
    }
    return img;
}

bool path::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
    int nextFirst = next.first;
    int nextSecond = next.second;
    RGBAPixel pix1;
    RGBAPixel pix2;
    bool ifInsideImage = nextFirst >= 0 && nextFirst < (int) image.width() && nextSecond >= 0 && nextSecond < (int) image.height();
    if(ifInsideImage){
        //An entry in table V is true if a cell has previously been visited.
        bool isVisited = v[nextSecond][nextFirst];
        pix1 = *image.getPixel(curr.first,curr.second);
        pix2 = *image.getPixel(nextFirst,nextSecond);
        bool ifNearInColor = closeEnough(pix1,pix2);
        if(!isVisited && ifNearInColor){
            return true;
        }else{
            return false;
        }

    }else{
        return false;
    }
}

vector<pair<int,int>> path::neighbors(pair<int,int> curr) {
	vector<pair<int,int>> n;
	
    //up
    n.push_back(make_pair(curr.first,curr.second-1));
    //down
    n.push_back(make_pair(curr.first,curr.second+1));
    //left 
    n.push_back(make_pair(curr.first-1,curr.second));
    //right 
    n.push_back(make_pair(curr.first+1,curr.second));
    return n;
}

vector<pair<int,int>> path::assemble(vector<vector<pair<int,int>>> & p,pair<int,int> s, pair<int,int> e) {
	vector<pair<int,int>> allPts;
	stack<pair<int,int>> resultStack;
	pair<int,int> curr = e;

    //if there is no path
	if(p[curr.second][curr.first]==e){
        allPts.push_back(s);
        return allPts;
    }

    resultStack.push(curr);
    pair<int,int> parent = p[curr.second][curr.first];
    while(parent!=s){
        resultStack.push(parent);
        parent = p[parent.second][parent.first];
    }
    resultStack.push(s);

    pair<int,int> path;
    while(!resultStack.empty()){
        path = resultStack.top();
        allPts.push_back(path);
        resultStack.pop();
    }


	return allPts;
}


/* PA4 - Completed for you for 2018S Semester */

bool path::closeEnough(RGBAPixel p1, RGBAPixel p2){
	int dist = (p1.r-p2.r)*(p1.r-p2.r);
	dist    += (p1.g-p2.g)*(p1.g-p2.g);
	dist    += (p1.b-p2.b)*(p1.b-p2.b);
	return (dist <= 80);
}

vector<pair<int,int>> path::getPath() { 
	return pathPts;
}

int path::length() { 
	return pathPts.size();
}
