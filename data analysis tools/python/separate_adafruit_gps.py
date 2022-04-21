import os
import sys

date_folder = '2022-04-16_from_sassafras'
data_location = os.path.join('../', date_folder)
print(data_location)
# data_location = './test_rig_imu_data/03-30-22'
nums = str(range(0,10))
def analyse(data_loc, filename):
    file_split = os.path.splitext(filename)
    if file_split[1] == '.txt':
        with open(os.path.join(data_loc, filename), 'r') as f0, \
        open(os.path.join(data_loc, 'acc', file_split[0]+'_acc'+file_split[1]),'w') as f_acc, \
        open(os.path.join(data_loc, 'gps', file_split[0]+'_gps'+file_split[1]),'w') as f_gps:
            f_acc.write('time,ax,ay,az,gx,gy,gz,mx,my,mz\n')
            f_gps.write('time,latitude,longitude\n')

            for line in f0.readlines():
                if 'GPS:' in line:
                    lat = "-" if line[13] == 'S' else ""
                    lat = lat+line[4:6]+'°'+line[6:13]+','
                    lon = "-" if line[24] == 'W' else ""
                    lon = lon+line[15:17]+'°'+line[17:24]+'\n'
                    time = prev_line.split(',')[0]+','
                    # print(time)
                    f_gps.write(time+lat+lon)
                else:
                    try:
                        int(line[0])
                        f_acc.write(line)
                    except:
                        continue

                prev_line = line

def main(td):
    # if td != None:
    #     data_location = td
    print(td)
    # global td
    if not os.path.exists(os.path.join(td,'acc')):
        os.mkdir(os.path.join(td,'acc'))
    if not os.path.exists(os.path.join(td,'gps')):
        os.mkdir(os.path.join(td,'gps'))

    for filename in os.listdir(td):
        # print(filename)
        analyse(td, filename)

if __name__ == '__main__':
    td = None
    if len(sys.argv) > 1:
        td = sys.argv[1]
    else:
        td = data_location
    main(td)