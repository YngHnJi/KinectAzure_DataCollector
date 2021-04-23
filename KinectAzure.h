// Author: Young-hoon Ji 210422

#ifndef KINECTAZURE_H
#define KINECTAZURE_H

#include "opencv2/opencv.hpp"
#include <k4a/k4a.hpp>
//#include <k4arecord/record.h>
#include <k4arecord/record.hpp>


#include "Pixel.h"
#include "DepthPixelColorizer.h"
#include "StaticImageProperties.h"

class KinectAzure {
public:
	KinectAzure();
	//~KinectAzure();

	//show data
	void ShowData();
	//record data
	void RecordData(std::string outputfile);
	//extract data
	//Console managing
	void CloseDevice();

private:
	k4a_device_configuration_t config;
	k4a::device device;
	k4a::record recorder;
};

#endif KINECTAZURE_H