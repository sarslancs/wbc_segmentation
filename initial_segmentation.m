function [ mask ] = initial_segmentation( intensityMap )
% INITIAL_SEGMENTATION Computes a binary mask given an intensity image
%   A two-stage segmentation algorithm. In the first stage, the intensity  
%   map is thresholded by the Otsu method to generate a "coarse" mask  
%   where the cellular regions and the background are clearly separated  
%   from each other. In the second stage, this mask is refined by applying 
%   active contours without edges, in which an active contour is evolved
%   by minimizing an energy function on the intensity map. 

%% Internal parameters for initial segmentation
smoothness  = 0.1;    % Weight of smoothing term
numPixels   = 100;    % Size of the objects that will be cleared out
maxIts      = 700;    % Max iterations for region_based_active_contours
verbose     = 0;      % Show graphical progress

%% Initial segmentation
fprintf('Initial segmentation called.\n');
initialMap = im2bw(intensityMap, graythresh(intensityMap));
innerThr = graythresh(intensityMap(initialMap == 1));
initialBoundaries = intensityMap > innerThr * 255;    
map = double(intensityMap).*initialMap;
fprintf('Active contours without edges running.');
[bw, err] = region_based_active_contours(map, initialBoundaries, ...
                 initialMap, maxIts, smoothness, verbose);
bw = bwareaopen(bw, numPixels);
mask = ~bwareaopen(~bw, numPixels);
if err == 1
    mask = initialMap;
end       
fprintf('\n');

