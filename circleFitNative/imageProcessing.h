#ifndef __imageProcessing_h
#define __imageProcessing_h

#define MAX_DIST 200000

#include "matrix.h"

/***************************************************************/
/***************************************************************/
/***************************************************************/
struct TBoundingBox{
	int minx, maxx, miny, maxy;
};
typedef struct TBoundingBox BNDBOX;
BNDBOX *calculateBoundingBoxes(MATRIX reg, int background);
BNDBOX *calculateBoundingBoxesAreas(MATRIX reg, int background, int **areas);
BNDBOX newBoundingBox(int minx, int maxx, int miny, int maxy);
void extendBoundingBoxes(BNDBOX *bnd, int no, int offset, int dimx, int dimy);
/***************************************************************/
/***************************************************************/
/***************************************************************/
struct TCircleBoundaries{
	int *x, *y;
	int N, r;
};
typedef struct TCircleBoundaries CIRCLE_BND;
CIRCLE_BND *createCircularBoundaries(int maxRadius);
void freeCircularBoundaries(CIRCLE_BND *B, int maxRadius);
/***************************************************************/
/***************************************************************/
/***************************************************************/
MATRIX fourConnectivity(MATRIX image);
MATRIX eightConnectivity(MATRIX image);
void eightConnectivityPartial(MATRIX marked, int id, BNDBOX bnd, MATRIX visited, 
							  int *queueX, int *queueY, MATRIX *reg, int *regNo);
MATRIX findComponentBoundingBoxes(MATRIX C); // minx, maxx, miny, maxy
void markRectangularRegions(MATRIX *reg, MATRIX bndbox);
void bwareaopen2(MATRIX *M, int areaThr);
void bwextend2(MATRIX *M, int d);

void relabelComponents(MATRIX *rmap);
void relabelComponentsWithSpecifiedBackground(MATRIX *rmap, int background, int startLabel);

int otsu(double *p, int no);
int otsuGrayImage(MATRIX M);
int otsuGraySubimage(MATRIX M, int minx, int maxx, int miny, int maxy);
int otsuLocalGrayImage(MATRIX M, MATRIX localMap, int regId, 
					   int minx, int maxx, int miny, int maxy);
int otsuLocalGraySubimage(MATRIX sub, MATRIX localMap, int regId, 
					   int minx, int maxx, int miny, int maxy);
int otsuGrayImageWithBackground(MATRIX M, int background);


MATRIX imthresh(MATRIX M, int thr);
void imthresh2(MATRIX *M, int thr);
void imsubthresh2(MATRIX *M, int thr, MATRIX mask, int no, BNDBOX bnd);
MATRIX imfilter(MATRIX M, MATRIX h, int is_0_255);
MATRIX imfilterWithMask(MATRIX M, MATRIX h, MATRIX mask, int label, int is_0_255,
						int minx, int miny, int maxx, int maxy);
void imfill2(MATRIX *L);	// 'holes'


MATRIX bwdilate(MATRIX M, MATRIX S);
void bwdilate2(MATRIX *M, MATRIX S);
void bwsubdilate2 (MATRIX *M, MATRIX S, int minx, int maxx, int miny, int maxy);
void bwerode2 (MATRIX *M, MATRIX S);
void bwsuberode2 (MATRIX *M, MATRIX S, int minx, int maxx, int miny, int maxy);
void bnderode2 (MATRIX *M, MATRIX original, MATRIX S);
void bwopen2 (MATRIX *M, MATRIX S);
MATRIX createDiskStructuralElement8();
MATRIX createDiskStructuralElement6();
MATRIX createDiskStructuralElement5();
MATRIX createDiskStructuralElement4();
MATRIX createDiskStructuralElement2();
MATRIX createOctagonStructuralElement3();
MATRIX createOctagonStructuralElement4();
MATRIX createOctagonStructuralElement6();
MATRIX createSquareStructuralElement(int a);

void fillInside(MATRIX *M, int label, int minX, int maxX, int minY, int maxY);
void markLineBetweenTwoPoints(MATRIX *M, int label, int x1, int y1, int x2, int y2);
MATRIX findRegionBoundaries(MATRIX R, int background);

void createSobels(MATRIX *hhor, MATRIX *hver);
MATRIX createCircularWindow(int thr);
struct Point{
	int r, c;
	double val; // non_zeroya olan uzaklýk tutuyorum.
};
typedef struct Point Pt;
MATRIX bwdist_(const MATRIX* bw);
#endif
MATRIX findEdge(const MATRIX L, int thickness);
MATRIX orMatrix(const MATRIX *a, const MATRIX *b);