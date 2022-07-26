## Introduction

---

  This is a repository to collect human skeleton data through Azure Kinect depth camera with C++ OpenCV library in Windows 10.

## Environments

---

The data collection tool using Azure Kinect was developed using Windows 10, Visual Studio 2019, and nvidia GPU 1080ti. The overall development environment setting and programming are referred to the list and in the links below.

- [Microsoft Kinect Azure required](https://docs.microsoft.com/hr-hr/azure/kinect-dk/depth-camera)
- Windows 10
- Visual Studio 2019
- C/C++
- OpenCV 4.5.1
- Microsoft.Azure.Kinect.BodyTracking.1.0.1
- Microsoft.Azure.Kinect.BodyTracking.Dependencies.0.9.1
- Microsoft.Azure.Kinect.BodyTracking.Dependencies.cuDNN.0.9.1
- Microsoft.Azure.Kinect.Sensor.1.4.0

Azure Kinect DK Official Documents

- [https://docs.microsoft.com/ko-kr/azure/kinect-dk/](https://docs.microsoft.com/ko-kr/azure/kinect-dk/)

Code Reference

- [https://github.com/microsoft/Azure-Kinect-Samples](https://github.com/microsoft/Azure-Kinect-Samples)
- [https://github.com/forestsen/KinectAzureDKProgramming](https://github.com/forestsen/KinectAzureDKProgramming)

Azure Kinect PC hardward and system requirements([Link](https://docs.microsoft.com/hr-hr/azure/kinect-dk/system-requirements))

- Seventh Gen Intel® CoreTM i5 Processor (Quad Core 2.4 GHz or faster)
- 4 GB Memory
- NVIDIA GEFORCE GTX 1050 or equivalent
- Dedicated USB3 port

## Overall Program Pipeline

---

![pipeline](Attachment/pipeline.png)

## Program Details

---

Program Control Options for file system

![program init cmd components](Attachment/program_init.png)

program init cmd components

Data Saving Option

: When the program is initially executed, it can be divided into manual and auto according to the file organization structure of the collected data.

- manual: When data is collected through the keyboard input “r”, the name of the file can be set separately to store image information

![manual_cmd](Attachment/manual_cmd.png)

- auto:  When collecting data through the keyboard input “r”, the name of the file is stored in order as shown in the picture below

![auto_cmd](Attachment/auto_cmd.png)


Program Options

- v(view) : ****Before data collection, the ability to ensure that objects are captured by the camera
- s(set directory) : Before data collection, the ability to set the collection location
- r(record) : Image data collection when transmitting r commands through the keyboard
- e(extract data) : The collected image data is extracted through the Pose Estimation model within Azure Kinect, and the extracted information is stored in csv format (to convert files in the directory specified by the s command)

Example of Azure Kinect Skeleton data

[https://docs.microsoft.com/hr-hr/azure/kinect-dk/body-joints](https://docs.microsoft.com/hr-hr/azure/kinect-dk/body-joints)

![skeleton_info](Attachment/skeleton_info.png)

![skeleton_info2](Attachment/skeleton_info2.png)

Example of data extracting from video(.mkv) to skeleton(.csv)

![extracting_cmd](Attachment/extracting_cmd.png)

![Example of extracted skeleton data](Attachment/extracting_csv.png)

Example of extracted skeleton data

## Reference

---

Azure Kinect DK official document

- [https://docs.microsoft.com/hr-hr/azure/kinect-dk/](https://docs.microsoft.com/hr-hr/azure/kinect-dk/)

Azure Kinect code reference

- [https://github.com/microsoft/Azure-Kinect-Samples](https://github.com/microsoft/Azure-Kinect-Samples)
- [https://github.com/forestsen/KinectAzureDKProgramming](https://github.com/forestsen/KinectAzureDKProgramming)

Azure Kinect hardware and system requirements

- [https://docs.microsoft.com/hr-hr/azure/kinect-dk/system-requirements](https://docs.microsoft.com/hr-hr/azure/kinect-dk/system-requirements)

Skeleton data example

- [https://docs.microsoft.com/hr-hr/azure/kinect-dk/body-joints](https://docs.microsoft.com/hr-hr/azure/kinect-dk/body-joints)