function [ imposed_im, boundaries ] = impose_mask_on_image(im, mask, c)
%IMPOSEMASK Imposes a binary mask on an RGB image 
%   Delineates the boundaries of a binary mask and imposes them on the RGB
%   image.
%
% author: Salim Arslan (name.surname@imperial.ac.uk)
% last updated: 21/12/2015
%
% input:
%   im: RGB image
%   mask: Binary mask
%   c: color vector (e.g. [255 0 0] for red)
%
% output:
%   imposed_im: Delienated image
%   boundaries: Mask boundaries
%

imposed_im = im;
mask = cast(mask, class(imposed_im)); 

  
mask = bwmorph(mask, 'remove');
boundaries = imdilate(mask, ones(2,2));

g = imposed_im(:,:,1); g(boundaries==1)=c(1); imposed_im(:,:,1)=g;
g = imposed_im(:,:,2); g(boundaries==1)=c(2); imposed_im(:,:,2)=g;
g = imposed_im(:,:,3); g(boundaries==1)=c(3); imposed_im(:,:,3)=g;

end