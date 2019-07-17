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

	//Set output data mode to PsIRAndRGB_30 to get IR frame
	//The PsDepthAndIR_30, PsDepthAndIRAndRGB_30 are also available to output IR frame
	PsSetDataMode(deviceIndex, PsIRAndRGB_30);

	PsFrame irFrame = { 0 };

	//Frame Main Loop
	for (;;)
	{
		//Get IR Frame
		PsReadNextFrame(deviceIndex);
		PsGetFrame(deviceIndex, PsIRFrame, &irFrame);

		//Image Process: print the center pixel value of the IR image
		if (irFrame.pFrameData != NULL)
		{
			//Read the IRFrameData in uint_16
			PsGray16Pixel * IRFrameData = (PsGray16Pixel *)irFrame.pFrameData;

			PsGray16Pixel grayValue = IRFrameData[irFrame.height / 2 * irFrame.width
				+ irFrame.width / 2];
			cout << "The center gray value of IR image: " << grayValue << endl;
		}
		else
			cout << "IR Image is invalid " << endl;
	}

	PsCloseDevice(deviceIndex);

	PsShutdown();

	return 0;
}
