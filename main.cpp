#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <vector>
#include <glob.h>

#define FOR(begin,limit) for(auto i=begin; i<limit; i++)

using namespace std;
using namespace cv;

// Thresholds 
const int minThreshold = 50;
const int maxThreshold = 200;
const int kernelSize = 3;

//houghLines Thresholds
int rho = 1;
int theta = CV_PI / 180;
int Threshold = 40;
int minLineLength = 100;
int maxLineGap = 300;

vector<Mat> stringToMat(vector<string> fileName){

    vector<Mat> src;

    FOR(0,fileName.size())  src.push_back(imread(fileName[i],0));

    return src;

}

void imShow(vector<string> fileName){

    vector<Mat> src;

    cout << "Reading " << fileName.size() << " number of images..."<<endl;
    src = stringToMat(fileName);

    while(true){

        FOR(0,fileName.size())  imshow(fileName[i],src[i]);

        int k = waitKey(1);
        if(k==27) return;
    }

}

vector<string> readImages(string path){
    
    glob_t r;
    vector<string> res;
    
    path = path+"/*";
    glob(path.c_str(), GLOB_TILDE, NULL, &r);
    
    FOR(0,r.gl_pathc)   res.push_back(r.gl_pathv[i]);

    return res;

}

void processOne(Mat src){

    Mat edges,roi;
    int numberOfpoints[] = {8};
    int shiftx = 46;
    int shifty = 45;

    Point points[1][8];

    points[0][0] = Point(src.size().width/2 - shiftx,src.size().height/2 + shifty);
    points[0][1] = Point(50, src.size().height);
    points[0][2] = Point(50, src.size().height);
    points[0][3] = Point(src.size().width - 50,src.size().height);
    points[0][4] = Point(src.size().width - 50,src.size().height);
    points[0][5] = Point(src.size().width/2 + shiftx,src.size().height/2 + shifty);
    points[0][6] = Point(src.size().width/2 + shiftx,src.size().height/2 + shifty);
    points[0][7] = Point(src.size().width/2 - shiftx,src.size().height/2 + shifty);

    const Point* vertices[1] = { points[0] };

    // Detect edges
    //cout << "Image size: " << src.size() << endl;
    imshow("Input image", src);
    Canny(src, edges, minThreshold, maxThreshold, kernelSize);
    //imshow("Edge Map", edges);

    // Select ROI
    //roi.zeros(src.size(),src.type());
    roi = Mat::zeros(src.size(),src.type());
    fillPoly(roi, vertices, numberOfpoints, 1, Scalar(255), 8);
    //imshow("ROI_",roi);
    bitwise_and(edges,roi,roi);
    //imshow("ROI",roi);

    std::vector<cv::Vec4i> lines;
    HoughLinesP( roi, lines, 1, CV_PI/360, 40, 120, 280);
    
    cvtColor(src, src, COLOR_GRAY2BGR);

    FOR(0,lines.size()){
        Vec4i l = lines[i];
        line(src, Point(l[0],l[1]), Point(l[2],l[3]), Scalar(0,0,255), 1, LINE_AA);
    }
    imshow("Detected Lane", src);
    int k = waitKey(0); 
    if(k==27) return;

}

bool processImages(vector<string> fileName){

    vector<Mat> src = stringToMat(fileName);

    FOR(0,fileName.size()){
        try{
            processOne(src[i]);
        }
        catch(...){
            return false;
        }

    }

    return true;

}


int main(int argc, char* argv[]){
    
    string path = "./test_images";
    
    vector<string> fileName = readImages(path);

    bool state = processImages(fileName);

    if(state){ cout << "Images processed correctly!" << endl;}
    else{ cout << "Error: Watch out, man! look back at your code." << endl; return -1;}

    return 0;

}
