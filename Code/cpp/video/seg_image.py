import cv2
import subprocess
import os
import sys

def lerp(start, end, t):
    return start + (end - start) * t

def process_frame(file, method, param1, param2):
    temp_file = "tmp.png"

    m = method[0].lower()
    if m == "s" : # SLIC
        subprocess.run(["../../bin/slic", file, temp_file, str(int(param1)), str(int(param2))])
    elif m == "f" : # Fenzelszwalb
        subprocess.run(["../../bin/felzenszwalb", file, temp_file, str(param1), str(int(param2))])
    elif m == "q": # QuickShift
        subprocess.run(["../../bin/quickshift", file, temp_file, str(param2 / 1000.0), str(int(param1))])

    processed_frame = cv2.imread(temp_file)
    os.remove(temp_file)

    return processed_frame

def process_video(input_file, output_file, method, frames_count, param1start, param2start, param1end, param2end):
    image = cv2.imread(input_file)
    height, width, channels = image.shape

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video_writer = cv2.VideoWriter(output_file, fourcc, 24.0, (width, height))

    for i in range(frames_count):
        t = i / frames_count
        processed_frame = process_frame(input_file, method, lerp(param1start, param1end, t), lerp(param2start, param2end, t))

        video_writer.write(processed_frame)

        print(i + 1, "/", frames_count)

    video_writer.release()

if __name__ == "__main__":
    input_file = sys.argv[1]
    output_file = "output_video.mp4"
    method = sys.argv[2]
    frame_count = int(sys.argv[3])
    param1start = float(sys.argv[4])
    param2start = float(sys.argv[5])
    param1end = float(sys.argv[6])
    param2end = float(sys.argv[7])

    process_video(input_file, output_file, method, frame_count, param1start, param2start, param1end, param2end)