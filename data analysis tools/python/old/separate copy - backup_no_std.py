import os
import sys
date_folder = '04-07-22'
data_location = os.path.join('T:\Documents\Clemson\Deep Orange\\test_rig_data_analysis\\test_rig_imu_data', date_folder)
# data_location = './test_rig_imu_data/03-30-22'

def analyse(data_loc, filename):
    file_split = os.path.splitext(filename)
    if file_split[1] == '.txt':
        with open(os.path.join(data_loc, filename), 'r') as f0, \
        open(os.path.join(data_loc, 'acc', file_split[0]+'_acc'+file_split[1]),'w') as f1, \
        open(os.path.join(data_loc, 'gps', file_split[0]+'_gps'+file_split[1]),'w') as f2:
            for line in f0.readlines():
                if line[0] == '$':
                    if prev_line[0] != '$':
                        f2.write(prev_line.split(',')[0])
                        f2.write('\n')
                    f2.write(line)
                else:
                    f1.write(line)
                prev_line = line

def main(td=None):
    if td != None:
        data_location = td
    print(td)
    if not os.path.exists(os.path.join(data_location,'acc')):
        os.mkdir(os.path.join(data_location,'acc'))
    if not os.path.exists(os.path.join(data_location,'gps')):
        os.mkdir(os.path.join(data_location,'gps'))

    for filename in os.listdir(data_location):
        # print(filename)
        analyse(data_location, filename)

if __name__ == '__main__':
    td = None
    if len(sys.argv) > 1:
        td = sys.argv[1]
    main(td)