#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "matrix.h"
#include "newCircleFit.h"
#include "imageProcessing.h"
#include <string.h>
#include <time.h>

#define LARGE_NO 500000

MATRIX locateCircles4CellSegmentation(MATRIX M, int radiusMinThr, int radiusMaxThr, MATRIX *distMap, int dist_thr, int startingLabel){
	MATRIX map		= allocateMatrix(M.row,M.column);
	MATRIX currR	= allocateMatrix(M.row,M.column);
	int maxR		= findMaxPossibleRadius(M,&currR) + 1; // find max possible circle
	CIRCLE_BND *B	= createCircularBoundaries(maxR);
	int i, x, y;
	
	findExactRadius(&currR,B, distMap, dist_thr);
	initializeMatrix(&map,0);
	if ((radiusMaxThr > 0) && (maxR > radiusMaxThr + 1))
		maxR = radiusMaxThr + 1;
	
	for (i = maxR - 1; i >= radiusMinThr; i--){
		for (x = 0; x < currR.row; x++)
			for (y = 0; y < currR.column; y++)
				if (currR.data[x][y] == i){
					locateOneCircle(&currR,&map,x,y,B,i,startingLabel);
					startingLabel++;
				}
	}
	freeMatrix(currR);
	freeCircularBoundaries(B,maxR);
	return map;	
}
void CALL_CIRCLE_FIT(int argc, char *argv[]){
	int maxThr		= LARGE_NO;
	int distThr		= 15;				// internal parameter
	int minThr		= atoi(argv[4]);
	int startingLbl	= atoi(argv[5]);
	MATRIX map		= readMatrix(argv[1]);
	MATRIX distMap	= readMatrix(argv[2]);
	FILE *file;
	clock_t start = clock();
	MATRIX cmap		= locateCircles4CellSegmentation(map,minThr,maxThr,&distMap,distThr,startingLbl);
	file = fopen("time-circle.txt","a+");
	fprintf(file, "%f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
	
	writeMatrixIntoFile(cmap,argv[3],0);

	freeMatrix(map);
	freeMatrix(cmap);
	freeMatrix(distMap);
}
int main(int argc, char *argv[]){
	
	if (argc != 6){
		printf("\n\nUsage: ");
		printf("\n\t1.  Bright file [br1]");
		printf("\n\t2.  Distance map file [dist1]");
		printf("\n\t3.  Result file [res1]");
		printf("\n\t4.  Min circle radius [20]");
		printf("\n\t5.  Starting label");
		printf("\n\n");
	}
	
	CALL_CIRCLE_FIT(argc,argv);

	return 0;
}

