TARGET := WDR

SOURCE := WDR.cpp

FLAGS := -I../../Include -I../../Thirdparty/opencv-3.4.1/include -Wl,-rpath,./:../../Lib/x64/:../../Thirdparty/opencv-3.4.1/lib/x64:../../Thirdparty/ffmpeg/lib/x64 -Wall -Wconversion -O3 -L../../Lib/x64 -lpicozense_api -L../../Thirdparty/opencv-3.4.1/lib/x64 -lopencv_core -lopencv_highgui -lopencv_imgproc

$(TARGET):$(SOURCE)
	g++ -std=c++11 -o $(TARGET) $(SOURCE) $(FLAGS) 

clean:
	rm -rf *.o $(TARGET)
