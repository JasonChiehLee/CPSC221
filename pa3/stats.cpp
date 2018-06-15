
#include "stats.h"

/**
 *
 * stats (pa3)
 * This file will be used for grading.
 *
 */

stats::stats(PNG & im){

	sumRed.clear();
	sumRed.resize(im.height(), vector<long> (im.width(),0));
	sumGreen.clear();
	sumGreen.resize(im.height(), vector<long> (im.width(),0));
	sumBlue.clear();
	sumBlue.resize(im.height(), vector<long> (im.width(),0));
	sumsqRed.clear();
	sumsqRed.resize(im.height(), vector<long> (im.width(),0));
	sumsqGreen.clear();
	sumsqGreen.resize(im.height(), vector<long> (im.width(),0));
	sumsqBlue.clear();
	sumsqBlue.resize(im.height(), vector<long> (im.width(),0));


	for (unsigned int y = 0; y < im.height(); y++) {
		for (unsigned int x = 0; x < im.width(); x++) {
		//std::cout<<"sumRed: "<<sumRed[y][x]<<std::endl;
			RGBAPixel * pix = im.getPixel(x,y);

			if ((y == 0) && (x == 0)) {
				sumRed[y][x] = pix->r;
				sumGreen[y][x] = pix->g;
				sumBlue[y][x] = pix->b;
				sumsqRed[y][x] = pix->r * pix->r;
				sumsqGreen[y][x] = pix->g * pix->g;
				sumsqBlue[y][x] = pix->b * pix->b;
			}
			else if (y==0) {
				sumRed[y][x] = sumRed[y][x-1] + pix->r;
				sumGreen[y][x] = sumGreen[y][x-1] + pix->g;
				sumBlue[y][x] = sumBlue[y][x-1] + pix->b;
				sumsqRed[y][x] = sumsqRed[y][x-1] + pix->r * pix->r;
				sumsqGreen[y][x] = sumsqGreen[y][x-1] + pix->g * pix->g;
				sumsqBlue[y][x] = sumsqBlue[y][x-1] + pix->b * pix->b;
			}
			else if (x==0) {
				sumRed[y][x] = sumRed[y-1][x] + pix->r;
				sumGreen[y][x] = sumGreen[y-1][x] + pix->g;
				sumBlue[y][x] = sumBlue[y-1][x] + pix->b;
				sumsqRed[y][x] = sumsqRed[y-1][x] + pix->r * pix->r;
				sumsqGreen[y][x] = sumsqGreen[y-1][x] + pix->g * pix->g;
				sumsqBlue[y][x] = sumsqBlue[y-1][x] + pix->b * pix->b;
			}
			else {

				sumRed[y][x] = sumRed[y][x-1] + sumRed[y-1][x] - sumRed[y-1][x-1] + pix->r;
				// std::cout<<"sumRed: x:"<<x<<std::endl;
				// std::cout<<"sumRed: y:"<<y<<std::endl;
	   // 			 std::cout<<std::endl;
				sumGreen[y][x] = sumGreen[y][x-1] + sumGreen[y-1][x] - sumGreen[y-1][x-1] + pix->g;
				sumBlue[y][x] = sumBlue[y][x-1] + sumBlue[y-1][x] - sumBlue[y-1][x-1] + pix->b;
				sumsqRed[y][x] = sumsqRed[y][x-1] + sumsqRed[y-1][x] - sumsqRed[y-1][x-1] + pix->r * pix->r;
				sumsqGreen[y][x] = sumsqGreen[y][x-1] + sumsqGreen[y-1][x] - sumsqGreen[y-1][x-1] + pix->g * pix->g;
				sumsqBlue[y][x] = sumsqBlue[y][x-1] + sumsqBlue[y-1][x] - sumsqBlue[y-1][x-1] + pix->b * pix->b;
			}
		}

	}

	//	std::cout<<"sumRed: x:"<<x<<std::endl;


	//std::cout<<"sumRed result is "<<sumRed[im.width()-1][im.height()-1]<<std::endl;
}

long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
	long result = 0;
	
	switch(channel){
		case 'r':

	// std::cout<<"getSum: ul->first:"<<ul.first<<std::endl;
	// std::cout<<"getSum: ul->second:"<<ul.second<<std::endl;
	// std::cout<<"getSum: lr->first:"<<lr.first<<std::endl;
	// std::cout<<"getSum: lr->second:"<<lr.second<<std::endl;
	// std::cout<<std::endl;
			if(ul.first!=0 && ul.second!=0){
			 	result = sumRed[lr.second][lr.first]
			 			-sumRed[lr.second][ul.first-1]
			 			-sumRed[ul.second-1][lr.first]
			 			+sumRed[ul.second-1][ul.first-1];
			}
			else if(ul.first==0 && ul.second!=0){
				result = sumRed[lr.second][lr.first]
						-sumRed[ul.second-1][lr.first];
			}
			else if(ul.first!=0 && ul.second==0){
				result = sumRed[lr.second][lr.first]
						-sumRed[lr.second][ul.first-1];
			}else if(ul.first==0 && ul.second==0){
				result = sumRed[lr.second][lr.first];
			}


		case 'g':
			if(ul.first!=0 && ul.second!=0){
			 	result = sumGreen[lr.second][lr.first]
			 			-sumGreen[lr.second][ul.first-1]
			 			-sumGreen[ul.second-1][lr.first]
			 			+sumGreen[ul.second-1][ul.first-1];
			}
			else if(ul.first==0 && ul.second!=0){
				result = sumGreen[lr.second][lr.first]
						-sumGreen[ul.second-1][lr.first];
			}
			else if(ul.first!=0 && ul.second==0){
				result = sumGreen[lr.second][lr.first]
						-sumGreen[lr.second][ul.first-1];
			}else if(ul.first==0 && ul.second==0){
				result = sumGreen[lr.second][lr.first];
			}


		case 'b':
			if(ul.first!=0 && ul.second!=0){
			 	result = sumBlue[lr.second][lr.first]
			 			-sumBlue[lr.second][ul.first-1]
			 			-sumBlue[ul.second-1][lr.first]
			 			+sumBlue[ul.second-1][ul.first-1];
			}
			else if(ul.first==0 && ul.second!=0){
				result = sumBlue[lr.second][lr.first]
						-sumBlue[ul.second-1][lr.first];
			}
			else if(ul.first!=0 && ul.second==0){
				result = sumBlue[lr.second][lr.first]
						-sumBlue[lr.second][ul.first-1];
			}else if(ul.first==0 && ul.second==0){
				result = sumBlue[lr.second][lr.first];
			}
		}
		return result;

}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
	long result = 0;
	switch(channel){
		case 'r':
			if(ul.first!=0 && ul.second!=0){
			 	result = sumsqRed[lr.second][lr.first]
			 			-sumsqRed[lr.second][ul.first-1]
			 			-sumsqRed[ul.second-1][lr.first]
			 			+sumsqRed[ul.second-1][ul.first-1];
			}
			else if(ul.first==0 && ul.second!=0){
				result = sumsqRed[lr.second][lr.first]
						-sumsqRed[ul.second-1][lr.first];
			}
			else if(ul.first!=0 && ul.second==0){
				result = sumsqRed[lr.second][lr.first]
						-sumsqRed[lr.second][ul.first-1];
			}else if(ul.first==0 && ul.second==0){
				result = sumsqRed[lr.second][lr.first];
			}

		case 'g':
			if(ul.first!=0 && ul.second!=0){
			 	result = sumsqGreen[lr.second][lr.first]
			 			-sumsqGreen[lr.second][ul.first-1]
			 			-sumsqGreen[ul.second-1][lr.first]
			 			+sumsqGreen[ul.second-1][ul.first-1];
			}
			else if(ul.first==0 && ul.second!=0){
				result = sumsqGreen[lr.second][lr.first]
						-sumsqGreen[ul.second-1][lr.first];
			}
			else if(ul.first!=0 && ul.second==0){
				result = sumsqGreen[lr.second][lr.first]
						-sumsqGreen[lr.second][ul.first-1];
			}else if(ul.first==0 && ul.second==0){
				result = sumsqGreen[lr.second][lr.first];
			}
	

		case 'b':
			if(ul.first!=0 && ul.second!=0){
			 	result = sumsqBlue[lr.second][lr.first]
			 			-sumsqBlue[lr.second][ul.first-1]
			 			-sumsqBlue[ul.second-1][lr.first]
			 			+sumsqBlue[ul.second-1][ul.first-1];
			}
			else if(ul.first==0 && ul.second!=0){
				result = sumsqBlue[lr.second][lr.first]
						-sumsqBlue[ul.second-1][lr.first];
			}
			else if(ul.first!=0 && ul.second==0){
				result = sumsqBlue[lr.second][lr.first]
						-sumsqBlue[lr.second][ul.first-1];
			}else if(ul.first==0 && ul.second==0){
				result = sumsqBlue[lr.second][lr.first];
			}
		
			
	}
	return result;
}

long stats::getScore(pair<int,int> ul, pair<int,int> lr){
	long double rsq = getSumSq('r',ul,lr);
	long double gsq = getSumSq('g',ul,lr);
	long double bsq = getSumSq('b',ul,lr);

	long double rsum = getSum('r',ul,lr);
	long double gsum = getSum('g',ul,lr);
	long double bsum = getSum('b',ul,lr);
	
	int R = rectArea(ul,lr);

	long double r = rsq - rsum * rsum / R;
	long double g = gsq - gsum * gsum / R;
	long double b = bsq - bsum * bsum / R;

	return r+g+b;
 
}

RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	// std::cout<<"getAvg: ul->first:"<<ul.first<<std::endl;
	// std::cout<<"getAvg: ul->second:"<<ul.second<<std::endl;
	// std::cout<<"getAvg: lr->first:"<<lr.first<<std::endl;
	// std::cout<<"getAvg: lr->second:"<<lr.second<<std::endl;
	int pixNum = rectArea(ul,lr);
		
	long double rav = (double)getSum('r',ul,lr)/(double)pixNum;
	long double gav = (double)getSum('g',ul,lr)/(double)pixNum;
	long double bav = (double)getSum('b',ul,lr)/(double)pixNum;

	return RGBAPixel(rav,gav,bav);

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
	return (lr.first-ul.first) * (lr.second-ul.second);
}
