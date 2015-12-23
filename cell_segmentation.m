function [ cells ] = cell_segmentation( im, h, R, W )
%CELL_SEGMENTATION Color and shape based white blood cell segmentation
%   Segments white blood cells in peripheral blood and bone marrow images
%   as proposed in [1]. 
%
%   INPUT
%   im:     input RGB image which will be segmented
%   h:      depth h of h-transformation
%   R:      radius of circle for circle fitting
%   W:      majority filter kernel size   
%
%   OUTPUT
%   cells:  segmented and labeled cells
%   
%   REFERENCES
%   [1] Arslan et al. 2014, A Color and Shape Based Algorithm for
%   Segmentation of White Blood Cells in Peripheral Blood and Bone Marrow 
%   Images 

fprintf('Segmentation process started.\n');

%% Color transformation
G = im(:,:,2);
B = im(:,:,3);
M = B - G;

%% Initial segmentation
mask = initial_segmentation(M);
% mask = dlmread(['data/mask' num2str(1)], '', 1, 0); % Or read from the a
% set of pre-computed masks.

%% Distance transformation and cell segmentation
labeled = intensity_based_watershed(mask, double(M), h);

%% Post-processing and final segmented cells
[circlesFitted] = circle_fit(labeled, R);
[r, c] = find(circlesFitted >= 1);
elim = bwselect(labeled,c,r,8);
elim = bwlabeln(elim);
cells = majority_filter(elim, W);
fprintf('White blood cells segmented.\n');


