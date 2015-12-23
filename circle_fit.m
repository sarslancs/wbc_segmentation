function [ circleFitMarkers ] = circle_fit(labeled, rthr)
%CIRCLE_FIT Fits circles within connected components in a labeled image and
%clears out those that cannot be fitted any  
%
%   Circles are iteratively located provided that their radii are greater 
%   than a circle threshold rthr [1]. Segmented cells are kept on which the 
%   circle-fit algorithm can locate at least one circle; the rest is 
%   eliminated from the results. Function is originally written for the
%   smart markers algorithm [2].
%
%   CAUTION!
%   This function calls an external executable located in circleFitNative. 
%   It is written in C and must be compiled prior to calling the circle_fit
%   function. We have already provided you with the Windows and Linux 64bit 
%   executables, but we would strongly suggest that you should compile the
%   source codes given in the circleFitNative folder excusively by following 
%   the instructions below:
%   1. Open a terminal
%   2. cd into <directory you extracted Matlab source>/circleFitNative
%   3. run gcc *.c -o circleFit_exe
%   If you change the name of the exe file, do not forget to apply the
%   same change to "exeName" below.
%
%   TROUBLESHOOT
%   If you encounter the "Permission denied" error, run the command
%   "chmod 755 circleFit_exe". This would change the read-write-execute
%   permission assigned to that file and (hopefully) solve the issue.
%
%   REFERENCES
%   [1]: Tosun et al, 2009, Object-oriented texture analysis for the 
%   unsupervised segmentation of biopsy images for cancer detection
%   [2]: Koyuncu et al, 2012, Smart markers for watershed-based cell 
%   segmentation.
    
exeName = '/circleFitNative/circleFit_unix64';

fprintf('Post-processing started.\n');
cnt = 1;
circleFitMarkers = zeros(size(labeled));
while (1)
    save_file(labeled,'dummyBright');
    distMap = bwdist(~labeled);
    save_file(distMap, 'dummyDist');

    str = ['./' exeName ' dummyBright  dummyDist  dummyRes ' ...
           num2str(rthr) ' ' num2str(cnt)];
    
    system(str);

    cMap = load('dummyRes');
    cnt = max(cMap(:)) + 1;

    if max(cMap(:)) == 0
        break;
    end

    labeled = xor(labeled, logical(cMap));
    circleFitMarkers = circleFitMarkers + cMap;
end

delete('dummyRes', 'dummyBright', 'dummyDist', 'time-circle.txt');
