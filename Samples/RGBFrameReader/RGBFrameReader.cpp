#include <iostream>
#include "PicoZense_api.h"
using namespace std;

int main(int argc, char *argv[])
{
	PsReturnStatus status;
	int32_t deviceIndex = 0;

	status = PsInitialize();
	if (status != PsReturnStatus::PsRetOK)
	{
		cout << "Initialize failed!" << endl;
		system("pause");
		return -1;
	}

	int32_t deviceCount = 0;
	status = PsGetDeviceCount(&deviceCount);
	if (deviceCount == 0)
	{
		cout << "Not Find any camera!" << endl;
		system("pause");
		return -1;
	}

	status = PsOpenDevice(deviceIndex);
	if (status != PsReturnStatus::PsRetOK)
	{
		cout << "OpenDevice failed!" << endl;
		system("pause");
		return -1;
	}

	//Set output data mode to PsDepthAndRGB_30 to get RGB frame
	//The PsIRAndRGB_30, PsDepthAndIRAndRGB_30 are also available to output RGB frame
	PsSetDataMode(deviceIndex, PsDepthAndRGB_30);

	PsFrame rgbFrame = { 0 };

	//Frame Main Loop
	for (;;)
	{
		//Get Depth Frame
		PsReadNextFrame(deviceIndex);
		PsGetFrame(deviceIndex, PsRGBFrame, &rgbFrame);

		//Image Process: print the center pixel value of the RGB image
		if (rgbFrame.pFrameData != NULL){

			//Read the RGBFrameData in PsRGB888Pixel
			PsRGB888Pixel * RGBFrameData = (PsRGB888Pixel *)rgbFrame.pFrameData;
			PsRGB888Pixel centerRGBPixel = *(RGBFrameData + rgbFrame.height / 2 * rgbFrame.width
				+ rgbFrame.width / 2);
            cout << "The centre pixel Value of RGB image,  r " << (int)centerRGBPixel.r << ", g  " << (int)centerRGBPixel.g << ", b " << (int)centerRGBPixel.b << endl;
		}
		else
			cout << "RGB Image is invalid " << endl;
	}

	PsCloseDevice(deviceIndex);

	PsShutdown();

	return 0;//a.exec();
}
