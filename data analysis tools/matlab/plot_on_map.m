close all
clear all
clc

% dataset
date = '04-07-22';
lognum = 8; % 6 for turns 1 and 2
folder = strcat('T:\Documents\Clemson\Deep Orange\test_rig_data_analysis\test_rig_imu_data\',date,'\gps\');
file = strcat(string(lognum), '_gps.txt');
t_pos = readtable(strcat(folder, file));

% convert to latitude from lat and degrees
tposbuf = str2double(split(t_pos.latitude,'°')).*[1 1/60];
t_pos.latitude = sign(tposbuf(:,1)).*sum(abs(tposbuf),2);
tposbuf = str2double(split(t_pos.longitude,'°')).*[1 1/60];
t_pos.longitude = sign(tposbuf(:,1)).*sum(abs(tposbuf),2);

% crop
start_time = 23;
end_time = 125;

t_pos = t_pos(t_pos.time > start_time & t_pos.time < end_time,:);

% plot
geoplot(t_pos.latitude, t_pos.longitude,'linewidth',2,'color', 'white');
geobasemap satellite

%% animated line
figure;
an = animatedline;
ylim([34.8061 34.8065])
xlim([-82.3182 -82.3179])
daspect([1 1 1])
for k = 1:height(t_pos)
    addpoints(an,t_pos.longitude(k),t_pos.latitude(k));
    drawnow

    fprintf(string(t_pos.time(k)));
    fprintf('\n')
    pause((t_pos.time(k)-t_pos.time(max(k-1,1)))/10)
end
