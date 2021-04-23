// Main Reference: https://github.com/forestsen/KinectAzureDKProgramming/blob/master/OpenCV_OneKinect/Source_1.cpp
// Younghoon Ji 20210422

#include <iostream>
#include <string>
#include <vector>

#include "KinectAzure.h"



int main(void)
{
	std::cout << "Running Kinect" << std::endl;

	KinectAzure Kinect = KinectAzure();
	Kinect.ShowData();
	
	//std::string test_file("HelloWorld");
	//Kinect.RecordData(test_file);
	
	Kinect.CloseDevice();

	return 0;
}