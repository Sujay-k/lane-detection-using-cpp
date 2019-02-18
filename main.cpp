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

    Canny(src, src, minThreshold, maxThreshold, kernelSize);
    imshow("Edge Map", src);

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
