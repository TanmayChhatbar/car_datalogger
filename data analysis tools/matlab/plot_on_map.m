close all
clear all
clc

date = '2022-04-16_from_sassafras';
lognum = 3;

folder = strcat('T:\Documents\Projects\Arduino\data_logger\data\',date,'\gps\');
file = strcat(string(lognum), '_gps.txt');
t_pos = readtable(strcat(folder, file));

tposbuf = str2double(split(t_pos.latitude,'°')).*[1 1/60];
t_pos.latitude = sign(tposbuf(:,1)).*sum(abs(tposbuf),2);
tposbuf = str2double(split(t_pos.longitude,'°')).*[1 1/60];
t_pos.longitude = sign(tposbuf(:,1)).*sum(abs(tposbuf),2);

geoplot(t_pos.latitude, t_pos.longitude,'linewidth',2,'color', 'white','LineStyle','*');
geobasemap satellite