function [ ] = save_file(im, fileName)
%SAVE_FILE Writes matrices onto disk

[dimx, dimy] = size(im);
myStr = [];
for i = 1 : dimy
    myStr = [myStr  '%d '];
end

myStr = [myStr '\n'];    

im = double(im);
id = fopen(fileName,'w');
fprintf(id,'%d %d\n',dimx, dimy);
fprintf(id, myStr, im');
fclose(id);



