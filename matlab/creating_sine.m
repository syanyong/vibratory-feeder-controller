% sin_table = int16(sind(0:0.045009:180)*655); %4000 0.045009
sin_table = int16(sind(0:0.09001:180)*640); %2000 0.09001
size(sin_table,2);
plot(sin_table,'*');

fprintf('\n\n{')
sin_table_size = size(sin_table,2);
for k = 1 : sin_table_size
    fprintf('%d',sin_table(k))
    if(k~=sin_table_size)
        fprintf(',')
    end
end
fprintf('};\n\n')
fprintf('\n%d\n',k)