TARGET := RGBFrameReader

SOURCE := RGBFrameReader.cpp

FLAGS := -I../../Include -Wl,-rpath,./:../../Lib/x64/:../../Thirdparty/opencv-3.4.1/lib/x64:../../Thirdparty/ffmpeg/lib/x64 -Wall -Wconversion -O3 -L../../Lib/x64 -lpicozense_api

$(TARGET):$(SOURCE)
	g++ -std=c++11 -o $(TARGET) $(SOURCE) $(FLAGS) 

clean:
	rm -rf *.o $(TARGET)
