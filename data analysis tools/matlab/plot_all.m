warning('off','all');

subplot(2,1,1);
hold on;
plot(t.time(ind), t.ax(ind),lw);
plot(t.time(ind), t.ay(ind),lw);
plot(t.time(ind), t.az(ind)+1,lw);
% t.acc = sqrt(t.ax.^2+t.ay.^2+t.az.^2); plot(t.time, t.acc);
lgd = legend({'ax','ay','az','net'});

subplot(2,1,2);
hold on;
plot(t.time(ind), t.gx(ind),lw);
plot(t.time(ind), t.gy(ind),lw);
plot(t.time(ind), t.gz(ind)+1,lw);
% t.acc = sqrt(t.ax.^2+t.ay.^2+t.az.^2); plot(t.time, t.acc);
lgd = legend({'gx','gy','gz','net'});