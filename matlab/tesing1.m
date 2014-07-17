clc;
clear all;

% Adjusting Amplitude
spwm_amp = 100;
sin_table = sind(0:0.09/2:180)*1000; %4000=0.045009:180

% Creating sin table
peak_tri = 1000;
%sin_table=int16(sin_table);       % Convert to int16
sprintf('sin_table size = %d',size(sin_table,2))

%% Timer Interupt Simulation    
tri_count = 1000;
tri_slop = 0;
sin_ptr = 1;

% Timer starting here!!!!!!!
for k = 1:1000
    % Creating Triangle
    if(tri_count >= 1000)			%/*Slop control*/
        tri_slop = 1;
    elseif(tri_count <= 0)
        tri_slop = 0;
    end
    if(tri_slop == 0)			%/*tri count control*/
        tri_count = tri_count+50;
    else
        tri_count = tri_count-50;
    end
    
    %Getting Sin from table
    sin_sig_temp = sin_table(sin_ptr);
    sin_ptr = sin_ptr+1;
    if(sin_ptr > size(sin_table,2))
        sin_ptr = 1;
    end
    
    %Adjusting Amplitude
    sin_sig_temp = sin_sig_temp*spwm_amp/100;
    
    %Comparing
    if(sin_sig_temp > tri_count)
        spwm_temp = 1;
    else
        spwm_temp = 0;
    end
    tri_sig(k) = tri_count;
    sin_sig(k) = sin_sig_temp;
    spwm_sig(k) = spwm_temp;
end
subplot(2,1,1), plot(tri_sig,'-*');
hold on;
subplot(2,1,1), plot(sin_sig,'-*r');
hold off;

subplot(2,1,2), plot(spwm_sig,'*');
