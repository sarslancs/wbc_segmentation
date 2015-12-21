function [ ] = imshow_labels_on(labeled, fontSize)
%IMSHOW_LABELS_ON Shows labeled connected components with their labels
%imposed on top of them
%   Shows the connected components (e.g. obtained via bwlabeln) and their labels imposed 
%   on top of them. The size of the labels (numbers) are set by fontSize.
%
% author: Salim Arslan (name.surname@imperial.ac.uk)
% last updated: 22.06.2014
%
% input
%   labeled: Labeled connected components
%   fontSize: Font size of the labels (numbers)
%
% output
%   no output
%

num = max(max(labeled));
figure, imshow(label2rgb(labeled, rand(num,3)));
s = regionprops(labeled, 'Centroid');

for k = 1 : numel(s)
    c = s(k).Centroid;
    text(c(1), c(2), sprintf('%d', k), ...
        'HorizontalAlignment', 'center', ...
        'VerticalAlignment', 'middle',...
        'FontSize', fontSize);
end
hold off