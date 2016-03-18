clear;
clc;

h=1;
total_time=200;
total_samples = total_time/h;

A = csvread('/home/eegrad/mislam/project/ucr-ee213/starter_code/matlab_spice_parser/MatrixA.csv'); A(:,end)=[];
C = csvread('/home/eegrad/mislam/project/ucr-ee213/starter_code/matlab_spice_parser/MatrixC.csv'); C(:,end)=[];
RHS = csvread('/home/eegrad/mislam/project/ucr-ee213/starter_code/matlab_spice_parser/VectorB.csv');
%bForIsrc = zeroes(size(bRHS,1),total_samples);
%bForIsrc = getRHSforIsrc(size(bRHS,1),h,total_time);
b = RHS


V_all = zeros(total_samples,size(A,2));

for ii=1:1:total_samples
    ii;
    V = (A+C./h)^-1*b;
    V_all(ii,:) = V';
    if (ii<total_samples)
        b=RHS+(C*V)./h;
    end
    clc;
end
plot(V_all(:,:));





