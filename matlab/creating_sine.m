% sin_table = int16(sind(0:0.045009:180)*655); %4000 0.045009
% sin_table = int16(sind(0:0.09001:180)*640); %2000 0.09001
temp = int16(sind(0:(0.09001*2):360)*640/2);

div = 100;

% sin_table = (temp*div/100) + (320-320*div/100);
% tm1 = temp*div;
% tm2 = 320*div;
% sin_table = ((32000-tm2)+tm1)/100;
sin_table = temp;
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