#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "util.h"
#include "imageProcessing.h"
#include "newCircleFit.h"

int findComponentSize(MATRIX M, int minx, int maxx, int miny, int maxy, int regId){
	int i, j, no = 0;
	
	for (i = minx; i <= maxx; i++)
		for (j = miny; j <= maxy; j++)
			if (M.data[i][j] == regId)
				no++;
	return no;
}
NEW_CIRCLE_SET findNewCircleSet(MATRIX M, int minx, int maxx, int miny, int maxy, 
								int regId, int radiusThr, int downRate){
	int N = findComponentSize(M,minx,maxx,miny,maxy,regId);
	NEW_CIRCLE_SET NCS;
	int x, y, found, r, i, j, minR;
	double maxR, d;

	NCS.no = 0;
	NCS.set = (NEW_CIRCLE *)calloc(N,sizeof(NEW_CIRCLE));
	
	for (x = minx; x <= maxx; x += downRate)
		for (y = miny; y <= maxy; y += downRate){
			if (M.data[x][y] != regId)
				continue;

			// ------------------------------------------------------------ //
			// It finds a square with a half edge of minR
			found = 0;
			r = 1;
			while ((x-r >= minx) && (x+r <= maxx) && (y-r >= miny) && (y-r <= maxy)){
				for (i = x-r; i <= x+r; i++)
					if ((M.data[i][y-r] != regId) || (M.data[i][y+r] != regId)){
						found = 1;
						break;
					}
				for (j = y-r; j <= y+r; j++)
					if ((M.data[x-r][j] != regId) || (M.data[x+r][j] != regId)){
						found = 1;
						break;
					}
				if (found)
					break;
				r++;
			}
			minR = r - 1;
			// ------------------------------------------------------------ //

			// ------------------------------------------------------------ //
			// It makes the square a circle
			// 1. First it finds the max possible radius 
			//    (the actual radius should be smaller than this)
			// 2. Extend the circle until it reaches the max possible radius
			if ((x-r >= minx) && (x+r <= maxx) && (y-r >= miny) && (y-r <= maxy)){
				
				maxR = -10.0;
				for (i = x-r; i <= x+r; i++)
					if (( (M.data[i][y-r] != regId) || (M.data[i][y+r] != regId) ) &&
						( (maxR < 0.0) || (r*r + (i-x)*(i-x) < maxR) ))
						maxR = r*r + (i-x)*(i-x);
				for (j = y-r; j <= y+r; j++)
					if (( (M.data[x-r][j] != regId) || (M.data[x+r][j] != regId) ) &&
						( (maxR < 0.0) || (r*r + (j-y)*(j-y) < maxR) ))
						maxR = r*r + (j-y)*(j-y);
				maxR = sqrt(maxR);


				if (maxR - r >= 1.0){
					r++;
					minR++;
				}
				while (r < maxR){
					if ((x-r < minx) || (x+r > maxx) || (y-r < miny) || (y+r > maxy))
						break;
					for (i = x-r; i <= x+r; i++){
						d = sqrt(r*r + (i-x)*(i-x));
						if (d >= maxR)
							continue;
						if ((M.data[i][y-r] != regId) || (M.data[i][y+r] != regId))
							maxR = d;
					}
					for (j = y-r; j <= y+r; j++){
						d = sqrt(r*r + (j-y)*(j-y));
						if (d >= maxR)
							continue;
						if ((M.data[x-r][j] != regId) || (M.data[x+r][j] != regId))
							maxR = d;
					}
					if (r < maxR){
						minR = r;
						r++;
					}
				}
			}
			// ------------------------------------------------------------ //
			if (minR >= radiusThr){
				NCS.set[NCS.no].centX = x;
				NCS.set[NCS.no].centY = y;
				NCS.set[NCS.no].radius = minR;
				NCS.set[NCS.no].criterion = 0.0;
				NCS.set[NCS.no].selected = 0;
				NCS.no += 1;
			}
		}
	return NCS;
}
int findMaximumRadius(int minx, int maxx, int miny, int maxy){
	int R1 = ((maxx-minx+1) / 2) + 1;
	int R2 = ((maxy-miny+1) / 2) + 1;
	if (R1 < R2)
		return R1;
	return R2;
}
int computeBoundaryColors(int x, int y, int R, CIRCLE_BND *B, int distThr, int innerRegThr,
						  MATRIX sobRed, MATRIX sobBlue, MATRIX sobGreen, MATRIX sobYellow,
						  int *selRed, int *selBlue, int *selGreen, int *selYellow,
						  int maxRed, int maxBlue, int maxGreen, int maxYellow){
	int i, k, cx, cy;
	int red, green, blue, yellow, innerReg, sum, two;
	double r2;

	for (k = 0; k <= maxRed; k++)				selRed[k] = 1;
	for (k = 0; k <= maxBlue; k++)				selBlue[k] = 1;
	for (k = 0; k <= maxGreen; k++)				selGreen[k] = 1;
	for (k = 0; k <= maxYellow; k++)			selYellow[k] = 1;
				
	red = green = blue = yellow = 0;
	for (k = R; k > R-distThr; k--){
		if (k < 0)
			break;
		r2 = k / sqrt(2.0);
		for (i = 0; i < B[k].N; i++){
			cx = B[k].x[i];
			cy = B[k].y[i];

			if ((cx >= r2) && (cy <= r2)){
				if (sobRed.data[x+cx][y-cy]){
					selRed[ sobRed.data[x+cx][y-cy] ] = 0;				red++;
				}
				if (sobRed.data[x+cx][y+cy]){
					selRed[ sobRed.data[x+cx][y+cy] ] = 0;				red++;
				}
				if (sobBlue.data[x-cx][y-cy]){
					selBlue[ sobBlue.data[x-cx][y-cy] ] = 0;			blue++;
				}
				if (sobBlue.data[x-cx][y+cy]){
					selBlue[ sobBlue.data[x-cx][y+cy] ] = 0;			blue++;
				}
			}
			if ((cy >= r2) && (cx <= r2)){
				if (sobGreen.data[x-cx][y+cy]){
					selGreen[ sobGreen.data[x-cx][y+cy] ] = 0;			green++;
				}
				if (sobGreen.data[x+cx][y+cy]){
					selGreen[ sobGreen.data[x+cx][y+cy] ] = 0;			green++;
				}
				if (sobYellow.data[x-cx][y-cy]){
					selYellow[ sobYellow.data[x-cx][y-cy] ] = 0;		yellow++;
				}
				if (sobYellow.data[x+cx][y-cy]){
					selYellow[ sobYellow.data[x+cx][y-cy] ] = 0;		yellow++;
				}
			}
		}
	}
	for (k = 0; k <= R-distThr; k++){
		for (i = 0; i < B[k].N; i++){
			cx = B[k].x[i];
			cy = B[k].y[i];
			
			if (sobRed.data[x+cx][y-cy] && selRed[sobRed.data[x+cx][y-cy]])
				selRed[sobRed.data[x+cx][y-cy]] = -1;
			if (sobRed.data[x+cx][y+cy] && selRed[sobRed.data[x+cx][y+cy]])
				selRed[sobRed.data[x+cx][y+cy]] = -1;
			if (sobRed.data[x-cx][y-cy] && selRed[sobRed.data[x-cx][y-cy]])
				selRed[sobRed.data[x-cx][y-cy]] = -1;
			if (sobRed.data[x-cx][y+cy] && selRed[sobRed.data[x-cx][y+cy]])
				selRed[sobRed.data[x-cx][y+cy]] = -1;
			
			if (sobBlue.data[x+cx][y-cy] && selBlue[sobBlue.data[x+cx][y-cy]])
				selBlue[sobBlue.data[x+cx][y-cy]] = -1;
			if (sobBlue.data[x+cx][y+cy] && selBlue[sobBlue.data[x+cx][y+cy]])
				selBlue[sobBlue.data[x+cx][y+cy]] = -1;
			if (sobBlue.data[x-cx][y-cy] && selBlue[sobBlue.data[x-cx][y-cy]])
				selBlue[sobBlue.data[x-cx][y-cy]] = -1;
			if (sobBlue.data[x-cx][y+cy] && selBlue[sobBlue.data[x-cx][y+cy]])
				selBlue[sobBlue.data[x-cx][y+cy]] = -1;

			if (sobGreen.data[x+cx][y-cy] && selGreen[sobGreen.data[x+cx][y-cy]])
				selGreen[sobGreen.data[x+cx][y-cy]] = -1;
			if (sobGreen.data[x+cx][y+cy] && selGreen[sobGreen.data[x+cx][y+cy]])
				selGreen[sobGreen.data[x+cx][y+cy]] = -1;
			if (sobGreen.data[x-cx][y-cy] && selGreen[sobGreen.data[x-cx][y-cy]])
				selGreen[sobGreen.data[x-cx][y-cy]] = -1;
			if (sobGreen.data[x-cx][y+cy] && selGreen[sobGreen.data[x-cx][y+cy]])
				selGreen[sobGreen.data[x-cx][y+cy]] = -1;

			if (sobYellow.data[x+cx][y-cy] && selYellow[sobYellow.data[x+cx][y-cy]])
				selYellow[sobYellow.data[x+cx][y-cy]] = -1;
			if (sobYellow.data[x+cx][y+cy] && selYellow[sobYellow.data[x+cx][y+cy]])
				selYellow[sobYellow.data[x+cx][y+cy]] = -1;
			if (sobYellow.data[x-cx][y-cy] && selYellow[sobYellow.data[x-cx][y-cy]])
				selYellow[sobYellow.data[x-cx][y-cy]] = -1;
			if (sobYellow.data[x-cx][y+cy] && selYellow[sobYellow.data[x-cx][y+cy]])
				selYellow[sobYellow.data[x-cx][y+cy]] = -1;
		}
	}
	innerReg = 0;
	for (k = 0; k <= maxRed; k++)				if (selRed[k] == -1)		innerReg++;
	for (k = 0; k <= maxBlue; k++)				if (selBlue[k] == -1)		innerReg++;
	for (k = 0; k <= maxGreen; k++)				if (selGreen[k] == -1)		innerReg++;
	for (k = 0; k <= maxYellow; k++)			if (selYellow[k] == -1)		innerReg++;
	
	sum = red + green + blue + yellow;
	if ((innerRegThr >= 0) && (innerReg > innerRegThr))
		sum = 0;
	
	two = (blue*green) + (blue*yellow) + (red*green) + (red*yellow);
	if (two == 0)
		sum = 0;
	
	return sum;
}
NEW_CIRCLE_SET findGrowingNewCircleSet(MATRIX M, int minx, int maxx, int miny, int maxy, 
									   int regId, int radiusThr, int distThr, int downRate,
									   int innerRegThr, MATRIX sobelRed, MATRIX sobelBlue, 
									   MATRIX sobelGreen, MATRIX sobelYellow){
	int maxRadius = findMaximumRadius(minx,maxx,miny,maxy);
	NEW_CIRCLE_SET NCS;
	int k, x, y, found, R, cx, cy, selectedR;
	double maxRGBY;
	CIRCLE_BND *B	= createCircularBoundaries(maxRadius);
	int N			= findComponentSize(M,minx,maxx,miny,maxy,regId);
	int *RGBYsum	= (int *)calloc(maxRadius,sizeof(int));
	int maxRed		= maxMatrixEntry(sobelRed);
	int maxBlue		= maxMatrixEntry(sobelBlue);
	int maxGreen	= maxMatrixEntry(sobelGreen);
	int maxYellow	= maxMatrixEntry(sobelYellow);
	int *selRed		= (int *)calloc(maxRed+1,sizeof(int));
	int *selBlue	= (int *)calloc(maxBlue+1,sizeof(int));
	int *selGreen	= (int *)calloc(maxGreen+1,sizeof(int));
	int *selYellow	= (int *)calloc(maxYellow+1,sizeof(int));
	
	NCS.no = 0;
	NCS.set = (NEW_CIRCLE *)calloc(N,sizeof(NEW_CIRCLE));
	

	for (x = minx; x <= maxx; x += downRate)
		for (y = miny; y <= maxy; y += downRate){
			if (M.data[x][y] != regId)
				continue;
			
			R = 0;
			while (R < maxRadius){
				if ((x-R < minx) || (x+R > maxx) || (y-R < miny) || (y+R > maxy)){
					R--;
					break;
				}
				found = 0;
				for (k = 0; k < B[R].N; k++){
					cx = B[R].x[k];
					cy = B[R].y[k];
					if ((M.data[x-cx][y-cy] != regId) || (M.data[x-cx][y+cy] != regId) ||
						(M.data[x+cx][y-cy] != regId) || (M.data[x+cx][y+cy] != regId)){
						found = 1;
						break;
					}
				}
				if (found){
					R--;
					break;
				}
				
				// -------------------------- COLOR ------------------------------------ //
				// -- Using R, compute red, green, blue, yellow falling in boundaries -- //
				// --------------------------------------------------------------------- //
				RGBYsum[R] = computeBoundaryColors(x,y,R,B,distThr,innerRegThr,
												   sobelRed,sobelBlue,sobelGreen,sobelYellow,
												   selRed,selBlue,selGreen,selYellow,
												   maxRed,maxBlue,maxGreen,maxYellow);
				// --------------------------------------------------------------------- //
				R++;
			}
			if (R == -1)
				terminateProgram("Error: findGrowingNewCircleSet (1)");

			// -------------------------- COLOR ------------------------------------ //
			// ------------ select R according to sobel sums ----------------------- //
			// --------------------------------------------------------------------- //
			selectedR = -1;
			for (k = radiusThr; k <= R; k++)
				if ((selectedR == -1) || (SQUARE(RGBYsum[k]) / (B[k].N + 0.0) > maxRGBY)){
					selectedR = k;
					maxRGBY = SQUARE(RGBYsum[k]) / (B[k].N + 0.0);
				}
			if (selectedR != -1){
				NCS.set[NCS.no].centX = x;
				NCS.set[NCS.no].centY = y;
				NCS.set[NCS.no].radius = selectedR;
				NCS.set[NCS.no].criterion = maxRGBY;
				NCS.set[NCS.no].selected = 0;
				NCS.no += 1;
			}	
		}

	for (k = 0; k < maxRadius; k++)
		if (B[k].N > 0){
			free(B[k].x);
			free(B[k].y);
		}
	free(B);
	free(RGBYsum);
	free(selRed);
	free(selBlue);
	free(selGreen);
	free(selYellow);
	
	return NCS;
}
double calcKomsuluk(MATRIX *distMap, int x, int y, CIRCLE_BND *B, int R, double th)
{
	double komsu = 0;
	int xc, yc,i;
	//th = R / 4.0;
	for (i=0;i<B[R].N;i++)
	{
		xc = B[R].x[i];
		yc = B[R].y[i];
		if (distMap->data[x+xc][y+yc] <= th)
			komsu++;
		if (distMap->data[x+xc][y-yc] <= th)
			komsu++;
		if (distMap->data[x-xc][y+yc] <= th)
			komsu++;
		if (distMap->data[x-xc][y-yc] <= th)
			komsu++;
	}
	//writeMatrixIntoFile(*distMap, "outDeneme", 0);
	//komsu = B[R].N;
	return komsu;
}
int findMaxPossibleRadius(MATRIX M, MATRIX *out){
	int maxR = 0, i, j, k;
	
	initializeMatrix(out,-1);
	for (i = 0; i < M.row; i++)
		for (j = 0; j < M.column; j++){
			if (M.data[i][j] == 0)
				continue;

			k = 1;
			while(1){
				if ((i-k < 0)			|| (M.data[i-k][j] == 0))		break;
				if ((i+k >= M.row)		|| (M.data[i+k][j] == 0))		break;
				if ((j-k < 0)			|| (M.data[i][j-k] == 0))		break;
				if ((j+k >= M.column)	|| (M.data[i][j+k] == 0))		break;
				k++;
			}
			out->data[i][j] = k-1;
			if (maxR < k-1)
				maxR = k-1;
		}
	return maxR;
}
void findExactRadius(MATRIX *out, CIRCLE_BND *B, MATRIX *distMap, int dist_thr){
	int x, y, k, i, r, cx, cy;
	double komsu, komsu_thr;

	for (x = 0; x < out->row; x++)
		for (y = 0; y < out->column; y++)
			if (out->data[x][y] > 1){
				r = out->data[x][y];
				for (k = 2; k <= r; k++){
					for (i = 0; i < B[k].N; i++){
						cx = B[k].x[i];
						cy = B[k].y[i];
						if ((out->data[x+cx][y+cy] == -1) || (out->data[x+cx][y-cy] == -1) ||
							(out->data[x-cx][y+cy] == -1) || (out->data[x-cx][y-cy] == -1))
							break;
					}
					if (i < B[k].N)
						break;
				}
				komsu = calcKomsuluk(distMap, x, y, B, k-1, dist_thr);
				komsu_thr = B[k-1].N * 3.0 ;
				if (komsu >= komsu_thr)
					out->data[x][y] = k-1;
				else
					out->data[x][y] = 1;
			}
}
void locateOneCircle(MATRIX *radii, MATRIX *map, int x, int y, CIRCLE_BND *B, 
					 int R, int label){
	int i, j, cx, cy, mx, my, r;
	int minx, miny, maxx, maxy;

	for (i = 0; i <= R; i++)
		for (j = 0; j < B[i].N; j++){
			cx = B[i].x[j];
			cy = B[i].y[j];

			map->data[x+cx][y+cy] = map->data[x+cx][y-cy] = label;
			map->data[x-cx][y+cy] = map->data[x-cx][y-cy] = label;

			radii->data[x+cx][y+cy] = radii->data[x+cx][y-cy] = -1;
			radii->data[x-cx][y+cy] = radii->data[x-cx][y-cy] = -1;
		}
	minx = x - 2*R;		if (minx < 0)				minx = 0;
	maxx = x + 2*R;		if (maxx >= map->row)		maxx = map->row - 1;
	miny = y - 2*R;		if (miny < 0)				miny = 0;
	maxy = y + 2*R;		if (maxy >= map->column)	maxy = map->column - 1;
	for (mx = minx; mx <= maxx; mx++)
		for (my = miny; my <= maxy; my++){
			if (radii->data[mx][my] < 0)
				continue;
			r = radii->data[mx][my];
			for (i = 0; i <= r; i++){
				for (j = 0; j < B[i].N; j++){
					cx = B[i].x[j];
					cy = B[i].y[j];

					if ((radii->data[mx+cx][my+cy] == -1) || (radii->data[mx+cx][my-cy] == -1) ||
						(radii->data[mx-cx][my+cy] == -1) || (radii->data[mx-cx][my-cy] == -1))
						break;
				}
				if (j < B[i].N)
					break;
			}
			radii->data[mx][my] = i - 1;
		}
}
MATRIX locateCircles(MATRIX M, int radiusMinThr, int radiusMaxThr,
					 int componentLabel, FILE *fid, MATRIX *distMap){
	MATRIX map		= allocateMatrix(M.row,M.column);
	MATRIX currR	= allocateMatrix(M.row,M.column);
	int maxR		= findMaxPossibleRadius(M,&currR) + 1; // find max possible circle
	CIRCLE_BND *B	= createCircularBoundaries(maxR);
	int cnt, i, x, y;
	
	findExactRadius(&currR,B, distMap, 0);
	initializeMatrix(&map,0);
	cnt = 1;
	if ((radiusMaxThr > 0) && (maxR > radiusMaxThr + 1))
		maxR = radiusMaxThr + 1;
	for (i = maxR - 1; i >= radiusMinThr; i--){
		for (x = 0; x < currR.row; x++)
			for (y = 0; y < currR.column; y++)
				if (currR.data[x][y] == i){
					locateOneCircle(&currR,&map,x,y,B,i,cnt);
					cnt++;
					// write the circle information into the file
					// centX   centY   type   radius
					if (fid)
						fprintf(fid,"%d\t%d\t%d\t%d\n",x,y,componentLabel,i);
				}
	}
	freeMatrix(currR);
	freeCircularBoundaries(B,maxR);
	return map;	
}
