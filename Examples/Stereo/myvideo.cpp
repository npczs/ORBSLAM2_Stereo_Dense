#include "System.h"
#include <string>
#include <chrono>   // for time stamp
#include <iostream>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>    
#include <opencv2/opencv.hpp> 
using namespace std;
 
#define Image_Width   1280     //【可能需要修改的程序参数2】：定义单目输出图像的宽度，相机支持的双目分辨率见产品参数表（双目分辨率的宽度除以2即为单目宽度）
#define Image_height  720      //【可能需要修改的程序参数3】：定义单目输出图像的高度
// 参数文件与字典文件
// 如果你系统上的路径不同，请修改它
string parameterFile = "./video.yaml";
string vocFile = "./Vocabulary/ORBvoc.txt";
// 视频文件
string videoFileLeft = "./left.avi";
string videoFileRight = "./right.avi";
int main(int argc, char **argv) {
 // 声明 ORB-SLAM2 系统
 ORB_SLAM2::System SLAM(vocFile, parameterFile, ORB_SLAM2::System::STEREO, true);
 // 获取视频图像
  cv::VideoCapture capl(videoFileLeft);    // change to 1 if you want to use USB camera.
cv::VideoCapture capr(videoFileRight);    // change to 1 if you want to use USB camera.
  // 记录系统时间
  auto start = chrono::system_clock::now();
//...
       char file_name[999];                         //定义文件名称，用于自动保存图像时命名，100表示抓拍的图像不超过100张
	
 
	uchar Resize_Flag = 0;
	int key = 0;                                          //键盘按下按键后获取的键值
//...
    // Read rectification parameters
    cv::FileStorage fsSettings(parameterFile, cv::FileStorage::READ);
    if(!fsSettings.isOpened())
    {
        cerr << "ERROR: Wrong path to settings" << endl;
        return -1;
    }
 
    cv::Mat K_l, K_r, P_l, P_r, R_l, R_r, D_l, D_r;
    fsSettings["LEFT.K"] >> K_l;
    fsSettings["RIGHT.K"] >> K_r;
 
    fsSettings["LEFT.P"] >> P_l;
    fsSettings["RIGHT.P"] >> P_r;
 
    fsSettings["LEFT.R"] >> R_l;
    fsSettings["RIGHT.R"] >> R_r;
 
    fsSettings["LEFT.D"] >> D_l;
    fsSettings["RIGHT.D"] >> D_r;
 
    int rows_l = fsSettings["LEFT.height"];
    int cols_l = fsSettings["LEFT.width"];
    int rows_r = fsSettings["RIGHT.height"];
    int cols_r = fsSettings["RIGHT.width"];
 
    if(K_l.empty() || K_r.empty() || P_l.empty() || P_r.empty() || R_l.empty() || R_r.empty() || D_l.empty() || D_r.empty() ||
            rows_l==0 || rows_r==0 || cols_l==0 || cols_r==0)
    {
        cerr << "ERROR: Calibration parameters to rectify stereo are missing!" << endl;
        return -1;
    }
 
    cv::Mat M1l,M2l,M1r,M2r;
    cv::initUndistortRectifyMap(K_l,D_l,R_l,P_l.rowRange(0,3).colRange(0,3),cv::Size(cols_l,rows_l),CV_32F,M1l,M2l);
    cv::initUndistortRectifyMap(K_r,D_r,R_r,P_r.rowRange(0,3).colRange(0,3),cv::Size(cols_r,rows_r),CV_32F,M1r,M2r);
 // Vector for tracking time statistics
    vector<float> vTimesTrack;
 
 cv::Mat imLeft, imRight, imLeftRect, imRightRect;
int num = 1;
while (1) {
        cv::Mat framel,framer;
        capl >> framel;   // 读取相机数据
        capr >> framer;   // 读取相机数据
        if ((framer.data==nullptr) ||(framel.data==nullptr))
            break;
       imLeft = framel;         //获取左Camera的图像
       imRight = framer;//获取右Camera的图像
                        file_name[num]=num;
			//imwrite("left.bmp",imLeft);
 
			//imwrite("right.bmp",imRight);
			num++;
			key = 0;
        cv::remap(imLeft,imLeftRect,M1l,M2l,cv::INTER_LINEAR);
        cv::remap(imRight,imRightRect,M1r,M2r,cv::INTER_LINEAR);
        // rescale because image is too large
        cv::Mat framel_resized,framer_resized;
        cv::resize(framel, framel_resized, cv::Size(640,360));
        cv::resize(framer, framer_resized, cv::Size(640,360));
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
         SLAM.TrackStereo(imLeftRect,imRightRect,double(timestamp.count())/1000.0);
        //cv::waitKey(30);
    }
//SLAM.SaveMap("/home/ORB_SLAM2/map.bin");
//SLAM.LoadMap("/home/ORB_SLAM2/map.bin");
 
   SLAM.Shutdown();
    return 0;
 
}
 
