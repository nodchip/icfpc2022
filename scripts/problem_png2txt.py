# data/problems/*.png を テキスト形式に変換する
import sys
import os
import glob
import cv2

def to_txt(path, arr):
	with open(path, 'w') as fo:
		fo.write(f'{arr.shape[1]} {arr.shape[0]}\n')
		for y in range(arr.shape[0]):
			for x in range(arr.shape[1]):
				fo.write(f'{arr[y, x, 0]} {arr[y, x, 1]} {arr[y, x, 2]} {arr[y, x, 3]}   ')
			fo.write('\n')
				
def main():
	for png_file in glob.glob('../data/problems/*.png'):
		arr = cv2.imread(png_file, cv2.IMREAD_UNCHANGED)
		print(png_file, arr.shape)
		to_txt(os.path.splitext(png_file)[0] + '.txt', arr)

if __name__ == '__main__':
	main()
