#ifndef __newCircleFit_h
#define __newCircleFit_h

#include "matrix.h"
#include "imageProcessing.h"

struct TNewCircle{
	int centX, centY;
	int radius;
	double criterion;
	int selected;
};
typedef struct TNewCircle NEW_CIRCLE;

struct TNewCircleSet{
	int no;
	NEW_CIRCLE *set;
};
typedef struct TNewCircleSet NEW_CIRCLE_SET;


int findComponentSize(MATRIX M, int minx, int maxx, int miny, int maxy, int regId);
NEW_CIRCLE_SET findNewCircleSet(MATRIX M, int minx, int maxx, int miny, int maxy, 
								int regId, int radiusThr, int downRate);

int findMaximumRadius(int minx, int maxx, int miny, int maxy);
CIRCLE_BND *createCircularBoundaries(int maxRadius);
void freeCircularBoundaries(CIRCLE_BND *B, int maxRadius);
NEW_CIRCLE_SET findGrowingNewCircleSet(MATRIX M, int minx, int maxx, int miny, int maxy, 
									   int regId, int radiusThr, int distThr, int downRate,
									   int innerRegThr, MATRIX sobelRed, MATRIX sobelBlue, 
									   MATRIX sobelGreen, MATRIX sobelYellow);
int computeBoundaryColors(int x, int y, int R, CIRCLE_BND *B, int distThr, int innerRegThr,
						  MATRIX sobRed, MATRIX sobBlue, MATRIX sobGreen, MATRIX sobYellow,
						  int *selRed, int *selBlue, int *selGreen, int *selYellow,
						  int maxRed, int maxBlue, int maxGreen, int maxYellow);


// ----------------------------------------------------------- //
// ----------------- Locate Circles -------------------------- //
// ----------------------------------------------------------- //
int findMaxPossibleRadius(MATRIX M, MATRIX *out);
void findExactRadius(MATRIX *out, CIRCLE_BND *B, MATRIX *distMap, int dist_thr);
MATRIX locateCircles(MATRIX M, int radiusMinThr, int radiusMaxThr,
					 int componentLabel, FILE *fid, MATRIX *distMap);
void locateOneCircle(MATRIX *radii, MATRIX *map, int x, int y, CIRCLE_BND *B, 
					 int R, int label);
// ----------------------------------------------------------- //
// ----------------------------------------------------------- //
// ----------------------------------------------------------- //

#endif