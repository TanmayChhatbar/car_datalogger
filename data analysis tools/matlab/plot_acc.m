clc;
clear all;
close all;

%% get data
date = '2022-04-16_from_sassafras';
lognum = 3;
logs_loc = 'T:\Documents\Projects\Arduino\data_logger\data\';
t = readtable(strcat(logs_loc,date,'/acc/',string(lognum),'_acc.txt'));

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

