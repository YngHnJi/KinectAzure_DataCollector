// Main Reference: https://github.com/forestsen/KinectAzureDKProgramming/blob/master/OpenCV_OneKinect/Source_1.cpp
/*
Author: Younghoon Ji 
210422, initial commit
210430 3 functions added, ConsoleController(), void ExtractData(), SetDir()
*/


#include <iostream>
#include <string>
#include <vector>

#include "KinectAzure.h"


int main(void)
{
	std::cout << "Running Kinect" << std::endl;
	//std::string test_file("HelloWorld");

	KinectAzure Kinect = KinectAzure();
	Kinect.ConsoleController();


	return 0;
}