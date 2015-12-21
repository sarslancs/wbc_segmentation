%% 
% This script is to test the cell segmentation program cell_segmentation,
% proposed in Arslan et al. 2014, "A Color and Shape Based Algorithm for
% Segmentation of White Blood Cells inPeripheral Blood and Bone Marrow 
% Images". 

tic
%% Load image
im = imread('data/b1.jpg'); % OR
% im = imread('data/b35.jpg'); 

%% Set parameters
% You may need to tune these parameters for your own set of images.
h = 2;
R = 20;
W = 4;

%% Run segmentation 
cells = cell_segmentation( im, h, R, W );
toc

%% Show some results
% Show labeled cells and their ids 
imshow_labels_on(cells, 12);
% Show segmented cells imposed on image
figure, imshow(impose_mask_on_image(im, cells, [255 0 0]));


