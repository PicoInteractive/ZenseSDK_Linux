#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "PicoZense_api.h"

using namespace std;
using namespace cv;

static void Opencv_Depth(uint32_t slope, int height, int width, uint8_t*pData, cv::Mat& dispImg)
{
	dispImg = cv::Mat(height, width, CV_16UC1, pData);
	Point2d pointxy(width / 2, height / 2);
	int val = dispImg.at<ushort>(pointxy);
	char text[20];
#ifdef _WIN32
	sprintf_s(text, "%d", val);
#else
	snprintf(text, sizeof(text), "%d", val);
#endif
	dispImg.convertTo(dispImg, CV_8U, 255.0 / slope);
	applyColorMap(dispImg, dispImg, cv::COLORMAP_RAINBOW);
	int color;
	if (val > 2500)
		color = 0;
	else
		color = 4096;
	circle(dispImg, pointxy, 4, Scalar(color, color, color), -1, 8, 0);
	putText(dispImg, text, pointxy, FONT_HERSHEY_DUPLEX, 2, Scalar(color, color, color));
}

int main(int argc, char *argv[])
{
	PsReturnStatus status;
	int32_t deviceIndex = 0;
	uint32_t wdrSlope = 4400;

	PsInitialize();
	
	status = PsOpenDevice(deviceIndex);
	if (status != PsReturnStatus::PsRetOK)
	{
		cout << "OpenDevice failed!" << endl;
		system("pause");
		return -1;
	}

	//Set data mode to PsWDR_Depth, WDR depth frame only output in this mode
	PsDataMode dataMode = PsWDR_Depth;
	PsSetDataMode(deviceIndex, dataMode);

	//Set WDR Output Mode, two ranges Near(Range0)/Far(Range2) output from device every one frame, no care for range3 and range3Count in PsWDRTotalRange_Two
	PsWDROutputMode wdrMode = { PsWDRTotalRange_Two, PsNearRange, 1, PsFarRange, 1, PsNearRange, 1 };
	PsSetWDROutputMode(deviceIndex, &wdrMode);

	//Set WDR fusion threshold, no care for threshold2 in PsWDRTotalRange_Two
	PsSetWDRFusionThreshold(deviceIndex, 1000, 2500);

	cv::Mat imageMat;
	const string wdrDepthImageWindow = "WDR Depth Image";
	bool f_bInvalidDepth2Zero = false;

	cout << "\n--------------------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "Press following key to set corresponding feature:" << endl;
	cout << "1: WDR mode 1, Near(Range0)/Far(Range2) " << endl;
	cout << "2: WDR mode 2, Near(Range0)/XFar(Range5) " << endl;
	cout << "3: WDR mode 3, Near(Range0)/Mid(Range1)/Far(Range2) " << endl;
	cout << "V/v: Enable or disable the WDR depth fusion feature " << endl;
	cout << "Esc: Program quit " << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------------------\n" << endl;

	for (;;)
	{
		PsFrame wdrFrame = {0};
		status = PsReadNextFrame(deviceIndex);
		if (status != PsReturnStatus::PsRetOK)
		{
			goto KEY;
		}
		if(dataMode == PsWDR_Depth)
 		{
			status = PsGetFrame(deviceIndex, PsWDRDepthFrame, &wdrFrame);
			if (status == PsReturnStatus::PsRetOK)
			{
				if (wdrFrame.pFrameData != NULL)
				{
					Opencv_Depth(wdrSlope, wdrFrame.height, wdrFrame.width, wdrFrame.pFrameData, imageMat);
					cv::imshow(wdrDepthImageWindow, imageMat);
				}
			}
		}	
	 
KEY:
		unsigned char key = waitKey(1);
		imageMat.release();

		if (key == 27)
		{
			PsCloseDevice(deviceIndex);
			PsShutdown();
			cv::destroyAllWindows();
			return 0;
		}	
		else if(key == 'V' || key == 'v')
		{		
			static bool bWDRStyle = true;

			status = PsSetWDRStyle(deviceIndex, bWDRStyle ? PsWDR_ALTERNATION : PsWDR_FUSION);
			if (PsRetOK == status)
			{
				if (!bWDRStyle)
				{
					cout << "WDR Fusion image output" << endl;
				}
				else
				{
					cout << "WDR image output alternatively in multi range" << endl;
				}
			}
			bWDRStyle = !bWDRStyle;
		}
		else if ((key == '1') || (key == '2') || (key == '3'))
		{
			switch (key)
			{
			case '1':
				//WDR mode 1: Near(Range0)/Far(Range2),  no care for range3 and range3Count in PsWDRTotalRange_Two
				wdrMode.totalRange = PsWDRTotalRange_Two;
				wdrMode.range1 = PsNearRange;
				wdrMode.range1Count = 1;
				wdrMode.range2 = PsFarRange;
				wdrMode.range2Count = 1;
				PsSetWDROutputMode(deviceIndex, &wdrMode);
				//No care for threshold2 in PsWDRTotalRange_Two
				PsSetWDRFusionThreshold(deviceIndex, 1000, 2500);	
				wdrSlope = 4400;
				cout << "Switch to WDR mode 1: Near(Range0)/Far(Range2)" << endl;
				break;
			case '2':
				//WDR mode 2: Near(Range0)/XFar(Range5), no care for range3 and range3Count in PsWDRTotalRange_Two
				wdrMode.totalRange = PsWDRTotalRange_Two;
				wdrMode.range1 = PsNearRange;
				wdrMode.range1Count = 1;
				wdrMode.range2 = PsXFarRange;
				wdrMode.range2Count = 1;
				PsSetWDROutputMode(deviceIndex, &wdrMode);
				//No care for threshold2 in PsWDRTotalRange_Two
				PsSetWDRFusionThreshold(deviceIndex, 1250, 2500);
				wdrSlope = 7500;
				cout << "Switch to WDR mode 2: Near(Range0)/XFar(Range5)" << endl;
				break;
			case '3':
				//WDR mode 3: Near(Range0)/Mid(Range1)/Far(Range2)
				wdrMode.totalRange = PsWDRTotalRange_Three;
				wdrMode.range1 = PsNearRange;
				wdrMode.range1Count = 1;
				wdrMode.range2 = PsMidRange;
				wdrMode.range2Count = 1;
				wdrMode.range3 = PsFarRange;
				wdrMode.range3Count = 1;
				PsSetWDROutputMode(deviceIndex, &wdrMode);
				PsSetWDRFusionThreshold(deviceIndex, 1000, 2500);
				wdrSlope = 4400;
				cout << "Switch to WDR mode 3: Near(Range0)/Mid(Range1)/Far(Range2)" << endl;
				break;
			default:
				cout << "Unsupported WDR mode!" << endl;
				continue;
			}
		}
	}

	PsCloseDevice(deviceIndex);
	PsShutdown();
	cv::destroyAllWindows();
	return 0;
}
