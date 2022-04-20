from datetime import date
import os
import shutil

# import files from SD card to local folder
def main():
    td = date.today()

    folder_name = f"{str(td.month).zfill(2)}-{str(td.day).zfill(2)}-{str(td.year)[2:4].zfill(2)}"
    data_loc = '../test_rig_imu_data'
    folder = os.path.join(data_loc,folder_name)
    if not os.path.isdir(folder):
        os.mkdir(folder)
        print(f"Folder {folder} created")
    else:
        print(f"Folder {folder} exists")

    if len(os.listdir(folder))!=0:
        print('Folder not empty')
    else:
        for file in os.listdir('E:\\'):
            if os.path.splitext(file)[1]=='.txt':
                # shutil.copy(f"E:\\{file}",os.path.join(folder, file))
                shutil.move(f"E:\\{file}",os.path.join(folder, file))
                print(f'Moved {file}')
    return folder

if __name__ == '__main__':
    main()
