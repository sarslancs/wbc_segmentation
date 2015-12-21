function [ labeled, segs ] = intensity_based_watershed(mask, intensityMap, h0) 
%INTENSITY_BASED_WATERSHED Cell segmentation using distance function and 
%marker-controlled watershed
%
%   [INPUT]
%   mask:           initial mask
%   h0:             depth h of h-minima transform 
%   intensityMap:   2D intensity image
%
%   [OUTPUT]
%   seg:            binary segmented map 
%   labeled:        labeled segmentation map

fprintf('Marker-controlled watershed done.\n');
intensityMap(~mask) = 0;
bounds = bwmorph(mask,'remove'); 
D = -bwdist(bounds);
D(~mask)= 1;
Do = imhmin(D, h0); % H-minima transform: suppresses all minima in the 
% intensity image I whose depth is less than h0

markers = imregionalmin(Do);
 
distanceFunc = imimposemin(Do.*intensityMap, markers); 
L = watershed(distanceFunc);
segs = mask;
segs(L==0)=0;
labeled = bwlabeln(segs);