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
	std::string string_input;

	root_path = ".\\results\\raw_data\\";
	std::cout << "Default root_path: " << root_path << std::endl;
	std::cout << "Choose manual/auto: ";
	std::getline(std::cin, string_input);
	while(1){
		if (string_input == "manual") {
			item_manual = true;
			break;
		}
		else if (string_input == "auto") {
			item_manual = false;
			break;
		}
		else {
			std::cout << "Retype" << std::endl;
			std::cout << "Choose manual/auto: ";
			std::getline(std::cin, string_input);
		}
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

	fps = 30; // used in extract joint data

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
	std::cout << "Press R to save" << std::endl;

	bool record_switch = true;
	int record_flag = 0; // 0: recording, 1: saving
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
					/*if(record_flag == 1) // commented 210728 yhji, for sync the process same with other devices
						std::cout << "Recording..." << std::endl;*/
				}
			}
			//Record file when switch on and record instance on
			// Add one more condition
			if ((record_switch == true) && (recorder.is_valid() == true) && (record_flag == 0)) {
				recorder.write_capture(capture);
			}
		}

		if (record_flag == 1) { // if-statement, to save recording.
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
				std::string test_file;
				if(item_manual==true){
					std::cout << "File name: ";
					std::getline(std::cin, test_file);
				}
				else {
					test_file = "Test_" + std::to_string(item_index);
					item_index++;
				}
				RecordData(test_file);
				std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;
			}
			else if ((controller_input==83) || (controller_input==115)) { // ASC code : "S", "s"
				std::cout << "Set Dir" << std::endl;
				SetDir();
				//std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;
				std::cout << "option: v(view), s(set directory), r(record), e(extract data): ";
			}
			else if ((controller_input==69) || (controller_input==101)) { // ASC code : "E", "e"
				std::cout << "Extract Data" << std::endl;
				ExtractData();
				std::cout << "option: v(view), s(set directory), r(record), e(extract data)" << std::endl;
			}
			else if (controller_input==81 || controller_input==113) { // ASC code : "Q", "q"
				std::cout << std::endl << "Closing Program..." << std::endl;
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

	std::cout << "Type the root directory to save files: ";
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

void KinectAzure::ExtractData() // extract data
{
	// joint information
	// ref: https://microsoft.github.io/Azure-Kinect-Body-Tracking/release/0.9.x/k4abttypes_8h_source.html
	k4abt::frame body_frame;
	uint32_t num_bodies;
	k4abt_body_t body;
	k4a_float3_t position;

	std::vector<std::string> joint_info = { "JOINT_PELVIS", "JOINT_SPINE_NAVEL", "JOINT_SPINE_CHEST", "JOINT_NECK",
	 "JOINT_CLAVICLE_LEFT", "JOINT_SHOULDER_LEFT", "JOINT_ELBOW_LEFT", "JOINT_WRIST_LEFT", "JOINT_HAND_LEFT",
	 "JOINT_HANDTIP_LEFT", "JOINT_THUMB_LEFT", "JOINT_CLAVICLE_RIGHT", "JOINT_SHOULDER_RIGHT", "JOINT_ELBOW_RIGHT",
	 "JOINT_WRIST_RIGHT", "JOINT_HAND_RIGHT", "JOINT_HANDTIP_RIGHT", "JOINT_THUMB_RIGHT", "JOINT_HIP_LEFT",
	 "JOINT_KNEE_LEFT", "JOINT_ANKLE_LEFT", "JOINT_FOOT_LEFT", "JOINT_HIP_RIGHT", "JOINT_KNEE_RIGHT",
	 "JOINT_ANKLE_RIGHT", "JOINT_FOOT_RIGHT", "JOINT_HEAD", "JOINT_NOSE", "JOINT_EYE_LEFT", "JOINT_EAR_LEFT",
	 "JOINT_EYE_RIGHT", "JOINT_EAR_RIGHT" };

	std::string data_dir;
	std::vector<std::string> dir_list;

	//std::cout << "Type the directory where to save extracted files: ";
	//std::getline(std::cin, data_dir);

	//while (IsPathExist(data_dir) == false) {
	//	std::cout << "Directory not existed, Retype it." << std::endl;
	//	std::cout << "Type the directory where raw file saved: ";
	//	std::getline(std::cin, data_dir);}
	//std::cout << "Extracted file will be saved into " << data_dir << std::endl;

	dir_list = get_files_inDirectory(root_path, "mkv");
	//dir_list = get_files_inDirectory(data_dir, "mkv");

	// for loop here
	for(int i=0;i<dir_list.size();i++){
		std::string target_video(root_path +"\\"+dir_list[i]);
		std::string outputfile(dir_list[i].substr(0, dir_list[i].rfind(".")) +".csv");

		//std::ofstream file(data_dir + "\\extracted_" + outputfile);
		std::ofstream file(root_path + "\\extracted_" + outputfile); // edited 210730
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
			file << "sec";
			for (int i = 0; i < joint_info.size(); i++) {
				file << "," << joint_info[i]+".X" << "," << joint_info[i] + ".Y" << "," << joint_info[i] + ".Z";
			}
			file << std::endl;

			while (1) {
				if (playback.get_next_capture(&video_capture)) {
					frame_count++;
					//std::cout << "Start processing frame " << frame_count << std::endl;
					if (!tracker.enqueue_capture(video_capture)) {
						std::cout << "Error! Add capture to tracker process queue timeout!" << std::endl;
						break;
					}
					body_frame = tracker.pop_result();
					if (body_frame != nullptr) {
						num_bodies = body_frame.get_num_bodies();

						for (uint32_t i = 0; i < num_bodies; i++) {
							body = body_frame.get_body(i);
							//print_body_information(body);
							//file << frame_count << std::endl;
							file << frame_count * (1/fps);
							for (int i = 0; i < (int)K4ABT_JOINT_COUNT; i++)
							{
								position = body.skeleton.joints[i].position;
								//k4a_quaternion_t orientation = body.skeleton.joints[i].orientation;
								//k4abt_joint_confidence_level_t confidence_level = body.skeleton.joints[i].confidence_level; // line for joint confidence outputs

								file << "," << position.xyz.x << "," << position.xyz.y << "," << position.xyz.z;
							}
							file << std::endl;
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