// Author: Young-hoon Ji 210422

#include <conio.h>
#include "KinectAzure.h"


using namespace sen;

KinectAzure::KinectAzure() {
	std::cout << "Start to initialize Kinect Azure" << std::endl;

	const uint32_t deviceCount = k4a::device::get_installed_count();
	if (deviceCount == 0)
	{
		std::cout << "No azure kinect devices detected!" << std::endl;
	}

	config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.camera_fps = K4A_FRAMES_PER_SECOND_30;
	config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	//config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG; // for recording
	config.color_format = K4A_IMAGE_FORMAT_COLOR_YUY2; //for imshow and recording
	//config.color_format = K4A_IMAGE_FORMAT_COLOR_NV12; //for imshow, recording
	//config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32; //for imshow
	config.color_resolution = K4A_COLOR_RESOLUTION_720P;
	config.synchronized_images_only = true;

	std::cout << "Started opening K4A device..." << std::endl;
	device = k4a::device::open(K4A_DEVICE_DEFAULT);
	device.start_cameras(&config);
	
	std::cout << "Finished opening K4A device." << std::endl;
}
//KinectAzure::~KinectAzure() = default;

void KinectAzure::ShowData() //show data
{
	std::cout << "Press ESC to ecape" << std::endl;

	std::vector<Pixel> depthTextureBuffer;
	std::vector<Pixel> irTextureBuffer;
	uint8_t* colorTextureBuffer;

	k4a::capture capture;

	k4a::image depthImage;
	k4a::image colorImage;
	k4a::image irImage;

	cv::Mat depthFrame;
	cv::Mat colorFrame;
	cv::Mat irFrame;

	while (1)
	{
		if (device.get_capture(&capture, std::chrono::milliseconds(0)))
		{
			{
				depthImage = capture.get_depth_image();
				colorImage = capture.get_color_image();
				irImage = capture.get_ir_image();

				ColorizeDepthImage(depthImage, DepthPixelColorizer::ColorizeBlueToRed, GetDepthModeRange(config.depth_mode), &depthTextureBuffer);
				ColorizeDepthImage(irImage, DepthPixelColorizer::ColorizeGreyscale, GetIrLevels(K4A_DEPTH_MODE_PASSIVE_IR), &irTextureBuffer);
				colorTextureBuffer = colorImage.get_buffer();

				depthFrame = cv::Mat(depthImage.get_height_pixels(), depthImage.get_width_pixels(), CV_8UC4, depthTextureBuffer.data());
				colorFrame = cv::Mat(colorImage.get_height_pixels(), colorImage.get_width_pixels(), CV_8UC4, colorTextureBuffer);
				irFrame = cv::Mat(irImage.get_height_pixels(), irImage.get_width_pixels(), CV_8UC4, irTextureBuffer.data());
				cv::imshow("kinect depth map master", depthFrame);
				cv::imshow("kinect color frame master", colorFrame);
				cv::imshow("kinect ir frame master", irFrame);
			}
		}
		if (cv::waitKey(30) == 27 || cv::waitKey(30) == 'q')
		{
			break;
		}
	}
}

void KinectAzure::RecordData(std::string outputfile)//record data
{
	bool record_switch = true;
	int flag_record = 0; // 0:  not recording, 1: recording, 2: saving
	char key_input;
	int file_index = 0;
	
	//Record data in mkv
	k4a::capture capture;
	std::string root_path(".\\results\\");
	std::string filename(root_path+outputfile+".mkv");
	recorder = k4a::record::create(filename.c_str(), device, config);

	if (record_switch == true)
	{
		recorder.write_header();
		if (recorder.is_valid() == false) {
			std::cout << "Recorder not opened" << std::endl;

			return;
		}
	}
	while (1){
		if (device.get_capture(&capture, std::chrono::milliseconds(0))){
			//std::cout << "flag: " << flag_record << std::endl;
			// Activate flag_record to record file
			if (_kbhit()){
				key_input = _getch();
				if ((key_input == 82) || (key_input == 114)){ // ASC code : "R", "r"
					flag_record += 1;
					std::cout << "Recording started" << std::endl;
					//std::cout << "flag: " << flag_record << std::endl;
				}
			}
			//Record file when switch on and record instance on
			// Add one more condition
			if ((record_switch == true) && (recorder.is_valid() == true) && (flag_record == 1)) {
				recorder.write_capture(capture);
			}
		}

		if (flag_record == 2){ // if-statement, to save recording.
			std::cout << "Record file saving" << std::endl;
			//std::cout << "flag: " << flag_record << std::endl;
			recorder.close();
			recorder.flush();

			return;
		}
	}

	return;
}

//extract data
//Console managing

void KinectAzure::CloseDevice()
{
	std::cout << "Close Device" << std::endl;
	device.close();
}