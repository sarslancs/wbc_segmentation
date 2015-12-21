function filtered = majority_filter(seg, W)
%MAJORITY_FILTER A disk-based majority filter for post-processing
%   Applied on the remaining cells to smooth their boundaries. Here, a 
%   circle-shaped kernel with a radius of W is used. 

domain = fspecial('disk',W) > 0;
filtered = zeros(size(seg));
cnt = 1;
for i = 1 : max(max(seg))
    A = seg == i;
    B = ordfilt2(A,round(0.5*numel(find(domain))),domain);
    CC = bwconncomp(B == 1);
    if CC.NumObjects > 1
        BW = false(CC.ImageSize);
        numPixels = cellfun(@numel,CC.PixelIdxList);
        [~,idx] = max(numPixels);
        BW(CC.PixelIdxList{idx}) = 1;
    else
        BW = B == 1;
    end

    filled = imfill(BW, 'holes') ;
    filtered(filled == 1) = cnt;
    cnt = cnt + 1; 
end
