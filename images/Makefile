.PHONY: all clean

all: scaled.mkv
	node gen_src.js

clean:
	rm -f video.mkv
	rm -f scaled.mkv
	rm -f frame_data.c
	rm -f frame_data.bin
	rm -rf images_jpg

video.mkv: 
	youtube-dl https://www.youtube.com/watch\?v\=FtutLA63Cp8 -o video

scaled.mkv: video.mkv
	ffmpeg -i video.mkv -vf "scale=480:272, fps=10" scaled.mkv
	mkdir images_jpg
	ffmpeg -i scaled.mkv "images_jpg/frame_%04d.png"

