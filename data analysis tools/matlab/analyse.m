clc;
clear all;
close all;

%% get data
date = '03-30-22';
file = '16';
t = readtable(strcat('../test_rig_imu_data/',date,'/acc/',file,'_acc.txt'));

%% select range
full_range = 1;

lw = '-';
if full_range == 1
    tstart = 0;
    tend = t.time(end);
else
    tstart = 10.7;
    tend = 13.5;
end
ind = (t.time>tstart & t.time<tend);
figure(1);
plot(t.time(ind), (t.gx(ind)));
hold on
plot(t.time(ind), (t.gy(ind)));
plot(t.time(ind), (t.gz(ind)));
% return

%% analyse
d=diff(t.time);
T = mean(d(2:end));
Fs = 1/T;
L = length(t.gx);

y = abs(fft(t.gx));
yspan = length(y)/2;
y_norm = y(1:yspan)/max(y);
freqs = 0:(Fs/L):(Fs/2-Fs/L);
maj = 1;
[~, ind2] = sort(y_norm,'descend');
f_major = freqs(ind2(1:3));
fprintf("major frequency = %.2f\n",f_major(maj));
hold on;

tt = t.time(ind);
tgx = t.gx(ind);
is = find(islocalmin(tgx));
t_min = tt(is(1));           % should be -1 at this point in time
t_offset = 3/(4*f_major(maj))-t_min;

plot(t.time(ind), 10*sin(f_major(maj)*(t.time(ind)-t_offset)*2*pi));

return
figure;

plot(freqs, y_norm);
%%
% hold on
% [b, a] = butter(9, 0.075, 'low');
% H = freqz(b,a, floor(yspan/2));
% plot([0:1/(yspan/2-1):1]*max(freqs), max(y_norm)*abs(H),'r');
% 
% return;
% figure;
% filtered = filter(b,a,t.gx);
% plot(t.time, filtered);
% hold on;
% plot(t.time, t.gx)
% legend({'filtered','original'})