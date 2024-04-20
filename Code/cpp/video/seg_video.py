import cv2
import subprocess
import os
import sys

def lerp(start, end, t):
    return start + (end - start) * t

def process_frame(frame, method, param1, param2):
    temp_file = "tmp.png"
    cv2.imwrite(temp_file, frame)

    m = method[0].lower()
    if m == "s" : # SLIC
        subprocess.run(["../../bin/slic", temp_file, temp_file, str(int(param1)), str(int(param2))])
    elif m == "f" : # Fenzelszwalb
        subprocess.run(["../../bin/felzenszwalb", temp_file, temp_file, str(param1), str(int(param2))])
    elif m == "q": # QuickShift
        subprocess.run(["../../bin/quickshift", temp_file, temp_file, str(param2 / 1000.0), str(int(param1))])

    processed_frame = cv2.imread(temp_file)
    os.remove(temp_file)

    return processed_frame

def process_video(input_file, output_file, method, param1start, param2start, param1end, param2end):
    video_capture = cv2.VideoCapture(input_file)

    frame_width = int(video_capture.get(cv2.CAP_PROP_FRAME_WIDTH))
    frame_height = int(video_capture.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = int(video_capture.get(cv2.CAP_PROP_FPS))
    frames_count = int(video_capture.get(cv2.CAP_PROP_FRAME_COUNT))

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video_writer = cv2.VideoWriter(output_file, fourcc, fps, (frame_width, frame_height))

    i = 0
    while True:
        ret, frame = video_capture.read()

        if not ret:
            break

        t = i / frames_count
        processed_frame = process_frame(frame, method, lerp(param1start, param1end, t), lerp(param2start, param2end, t))

        video_writer.write(processed_frame)

        i += 1

        print(i, "/", frames_count)

    video_capture.release()
    video_writer.release()

if __name__ == "__main__":
    input_file = sys.argv[1]
    output_file = "output_video.mp4"
    method = sys.argv[2]
    param1start = sys.argv[3]
    param2start = sys.argv[4]
    param1end = sys.argv[5]
    param2end = sys.argv[6]

    process_video(input_file, output_file, method, param1start, param2start, param1end, param2end)