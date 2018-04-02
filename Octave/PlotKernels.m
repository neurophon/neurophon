plotlinewidth = 3.5;
linewidth = 2;
fontsize = 22;

figure('units','normalized','outerposition',[0 0 1 1])
load RealKernel008.mat 
plot(Aux, "linewidth", plotlinewidth)
hold on
load RealKernel016.mat 
plot(Aux, "linewidth", plotlinewidth)
load RealKernel032.mat 
plot(Aux, "linewidth", plotlinewidth)
load RealKernel064.mat 
plot(Aux, "linewidth", plotlinewidth)
load RealKernel128.mat 
plot(Aux, "linewidth", plotlinewidth)
grid on
h=get(gcf, "currentaxes");
set(h, "fontsize", fontsize, "linewidth", linewidth);
axis ([-5 136 -0.3 0.65])
axis ([-5 136 -0.28 0.65])
xlabel ("Sample number")
ylabel ("Kernel value")
%legend("8 ms time window","16 ms time window","32 ms time window","64 ms time window","128 ms time window")
title ("Real part")
title ("Kernel real part")

figure('units','normalized','outerposition',[0 0 1 1])
load ImagKernel008.mat 
plot(Aux, "linewidth", plotlinewidth)
hold on
load ImagKernel016.mat 
plot(Aux, "linewidth", plotlinewidth)
load ImagKernel032.mat 
plot(Aux, "linewidth", plotlinewidth)
load ImagKernel064.mat 
plot(Aux, "linewidth", plotlinewidth)
load ImagKernel128.mat 
plot(Aux, "linewidth", plotlinewidth)
grid on
h=get(gcf, "currentaxes");
set(h, "fontsize", fontsize, "linewidth", linewidth);
axis ([-5 136 -0.3 0.65])
axis ([-5 136 -0.5 0.5])
xlabel ("Sample number")
ylabel ("Kernel value")
%legend("8 ms time window","16 ms time window","32 ms time window","64 ms time window","128 ms time window")
title ("Real part")
title ("Kernel imaginary part")
