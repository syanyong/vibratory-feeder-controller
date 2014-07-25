x = 100;
sin = uint16(x*(1:100))./100;
dv = uint16(100./(1:100));
sin2 = x./dv;
plot(sin)
hold on
plot(sin2,'r')
hold off