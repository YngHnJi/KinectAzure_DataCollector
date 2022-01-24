// Author: Young-hoon Ji 210422

#ifndef KINECTAZURE_H
#define KINECTAZURE_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <conio.h>
#include <ctime>
#include <vector>
#include <chrono> // added 210728 yhji

#include "opencv2/opencv.hpp"
#include <k4a/k4a.hpp>
#include <k4arecord/record.hpp>
#include <k4arecord/playback.hpp>
#include <k4abt.hpp>

#include "Pixel.h"
#include "DepthPixelColorizer.h"
#include "StaticImageProperties.h"

class KinectAzure {
public:
	KinectAzure();
	//~KinectAzure();

	//void RunKinect(); // for future to intergrate functions
	void ConsoleController(); // kinect controller

private:
	k4a_device_configuration_t config;
	k4a::device device;
	k4a::record recorder;
	k4a::playback playback;
	k4abt::tracker tracker;

	std::string root_path;
	//std::string save_path_extracted;

	bool item_manual;
	int item_index; // for auto file name
	float fps;

	void ShowData(); //show kinect output to aim the target
	void RecordData(std::string outputfile); //save record data into files
	void SetDir(); // set filesave directory
	void ExtractData(); // extraxt video data to skeleton joint data
	void CloseDevice();
};

#endif KINECTAZURE_H