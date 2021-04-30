// Author: Young-hoon Ji 210430

#include "KinectAzure.h"
#include "utils.h"

using namespace sen;

KinectAzure::KinectAzure() {
	std::cout << "Start to initialize Kinect Azure" << std::endl;

	const uint32_t deviceCount = k4a::device::get_installed_count();
	if (deviceCount == 0){
		std::cout << "No azure kinect devices detected!" << std::endl;
	}

	//root_path = ".\\results\\raw_data";

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
	std::cout << "Press ESC to quit View Mode" << std::endl;

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

	while (1){
		if (device.get_capture(&capture, std::chrono::milliseconds(0))){
			colorImage = capture.get_color_image();
			//depthImage = capture.get_depth_image();
			//irImage = capture.get_ir_image();

			colorTextureBuffer = colorImage.get_buffer();
			//ColorizeDepthImage(depthImage, DepthPixelColorizer::ColorizeBlueToRed, GetDepthModeRange(config.depth_mode), &depthTextureBuffer);
			//ColorizeDepthImage(irImage, DepthPixelColorizer::ColorizeGreyscale, GetIrLevels(K4A_DEPTH_MODE_PASSIVE_IR), &irTextureBuffer);
			
			colorFrame = cv::Mat(colorImage.get_height_pixels(), colorImage.get_width_pixels(), CV_8UC2, colorTextureBuffer);
			//colorFrame = cv::Mat(colorImage.get_height_pixels(), colorImage.get_width_pixels(), CV_8UC4, colorTextureBuffer);
			//depthFrame = cv::Mat(depthImage.get_height_pixels(), depthImage.get_width_pixels(), CV_8UC4, depthTextureBuffer.data());
			//irFrame = cv::Mat(irImage.get_height_pixels(), irImage.get_width_pixels(), CV_8UC4, irTextureBuffer.data());
			
			cvtColor(colorFrame, colorFrame, cv::COLOR_YUV2BGR_YUY2); //COLOR_YUV2BGR_YUY2, COLOR_YUV2BGR_UYVY
			cv::imshow("kinect color frame master", colorFrame);
			//cv::imshow("kinect depth map master", depthFrame);
			//cv::imshow("kinect ir frame master", irFrame);
		}
		if (cv::waitKey(30) == 27 || cv::waitKey(30) == 'q'){
			cv::destroyAllWindows();
			break;
		}
	}
}

void KinectAzure::RecordData(std::string outputfile) //record data
{
	std::cout << "Start Recording Data..." << std::endl;
	std::cout << "Press R to record" << std::endl;

	bool record_switch = true;
	int record_flag = 0; // 0:  not recording, 1: recording, 2: saving
	char key_input;

	//Record data in mkv
	k4a::capture capture;
	std::string filename(root_path + outputfile + ".mkv");

	recorder = k4a::record::create(filename.c_str(), device, config);
	if (record_switch == true)
	{
		recorder.write_header();
		if (recorder.is_valid() == false) {
			std::cout << "Recorder not opened" << std::endl;

			return;
		}
	}
	while (1) {
		if (device.get_capture(&capture, std::chrono::milliseconds(0))) {
			//std::cout << "flag: " << flag_record << std::endl;
			// Activate flag_record to record file
			if (_kbhit()) {
				key_input = _getch();
				if ((key_input == 82) || (key_input == 114)) { // ASC code : "R", "r"
					record_flag += 1;
					if(record_flag == 1)
						std::cout << "Recording..." << std::endl;
				}
			}
			//Record file when switch on and record instance on
			// Add one more condition
			if ((record_switch == true) && (recorder.is_valid() == true) && (record_flag == 1)) {
				recorder.write_capture(capture);
			}
		}

		if (record_flag == 2) { // if-statement, to save recording.
			std::cout << "Saving..." << std::endl;

			recorder.close();
			recorder.flush();

			return;
		}
	}
	return;
}

void KinectAzure::ConsoleController() //Console managing
{
	std::cout << "Console Controller started" << std::endl;
	//std::cout << "Set Dir" << std::endl;
	//SetDir();
	std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;

	char controller_input;

	while (true) {
		if (_kbhit()) {
			controller_input = _getch();
			if ((controller_input==86) || (controller_input==118)) { // ASC code : "V", "v"
				std::cout << "View Mode" << std::endl;
				ShowData();
				std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;
			}
			else if ((controller_input==82) || (controller_input==114)) { // ASC code : "R", "r"
				std::cout << "Recording Mode" << std::endl;
				std::cout << "File name: ";
				std::string test_file;
				std::getline(std::cin, test_file);
				RecordData(test_file);
				std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;
			}
			else if ((controller_input==83) || (controller_input==115)) { // ASC code : "S", "s"
				std::cout << "Set Dir" << std::endl;
				SetDir();
				std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;
			}
			else if ((controller_input==69) || (controller_input==101)) { // ASC code : "E", "e"
				std::cout << "Extract Data" << std::endl;
				ExtractData();
				std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;
			}
			else if (controller_input==81 || controller_input==113) { // ASC code : "Q", "q"
				std::cout << "Closing Program..." << std::endl;
				CloseDevice();

				return;
			}
			else {
				std::cout << "Retype the command" << std::endl;
			}
		}
	}
	return;
}

void KinectAzure::SetDir()
{
	std::string string_input;
	
	std::cout << "Type the root directory to save files: " << std::endl;
	std::getline(std::cin, string_input);

	while (IsPathExist(string_input) == false) {
		std::cout << "Directory not existed, Retype it." << std::endl;
		std::cout << "Type the root directory to save files: ";
		std::getline(std::cin, string_input);
	}
	root_path = string_input + "\\";
	std::cout << "Root Directory set to " << root_path << std::endl;

	return;
}

void KinectAzure::ExtractData() //extract data
{
	std::string data_dir;
	std::vector<std::string> dir_list;

	std::cout << "Type the directory where raw file saved: " << std::endl;
	std::getline(std::cin, data_dir);

	while (IsPathExist(data_dir) == false) {
		std::cout << "Directory not existed, Retype it." << std::endl;
		std::cout << "Type the directory where raw file saved: ";
		std::getline(std::cin, data_dir);
	}
	std::cout << "Extracted file will be saved into " << data_dir << std::endl;

	dir_list = get_files_inDirectory(root_path, "mkv");

	// for loop here
	for(int i=0;i<dir_list.size();i++){
		std::string target_video(root_path +"\\"+dir_list[i]);
		std::string outputfile(dir_list[i].substr(0, dir_list[i].rfind(".")) +".csv");

		std::ofstream file(data_dir + "\\extracted_" + outputfile);
		//file.open(save_dir+outputfile, std::ios::in);
		if (!(file.is_open())) {
			std::cout << "Path: " << data_dir + outputfile << std::endl;
			std::cout << "File not opened" << std::endl;

			return;
		}
		try {
			std::cout << "Extracting..." << std::endl;
			playback = k4a::playback::open((target_video).c_str());
			k4a::calibration sensor_calibration = playback.get_calibration();
			tracker = k4abt::tracker::create(sensor_calibration);

			k4a::capture video_capture;

			int frame_count = 0;

			while (1) {
				if (playback.get_next_capture(&video_capture)) {
					frame_count++;
					//std::cout << "Start processing frame " << frame_count << std::endl;
					if (!tracker.enqueue_capture(video_capture)) {
						std::cout << "Error! Add capture to tracker process queue timeout!" << std::endl;
						break;
					}
					k4abt::frame body_frame = tracker.pop_result();
					if (body_frame != nullptr) {
						uint32_t num_bodies = body_frame.get_num_bodies();

						for (uint32_t i = 0; i < num_bodies; i++) {
							k4abt_body_t body = body_frame.get_body(i);
							//print_body_information(body);
							file << frame_count << std::endl;
							for (int i = 0; i < (int)K4ABT_JOINT_COUNT; i++)
							{
								k4a_float3_t position = body.skeleton.joints[i].position;
								k4a_quaternion_t orientation = body.skeleton.joints[i].orientation;
								//k4abt_joint_confidence_level_t confidence_level = body.skeleton.joints[i].confidence_level; // line for joint confidence outputs

								file << position.v[0] << "," << position.v[1] << "," << position.v[2] << "," <<
									orientation.v[0] << "," << orientation.v[1] << "," << orientation.v[2] << "," << orientation.v[3] << std::endl;
							}
						}
					}
					else
						break;
				}
				else
					break;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed with exception:" << std::endl
				<< "    " << e.what() << std::endl;
			return;
		}

		file.close();
	}
	return;
}

void KinectAzure::CloseDevice()
{
	std::cout << "Close Device" << std::endl;
	device.close();
}