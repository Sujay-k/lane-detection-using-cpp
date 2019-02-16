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

void imShow(vector<string> fileName){

    int numberOfImages = fileName.size();
    vector<Mat> src;

    cout << "Reading " << numberOfImages << " number of images..."<<endl;
    FOR(0,numberOfImages){
        src.push_back(imread(fileName[i]));
    }

    while(true){
        FOR(0,numberOfImages){
            imshow(fileName[i],src[i]);
        }

        int k = waitKey(1);
        if(k==27) return;
    }

}

vector<string> readImages(string path){
    
    glob_t r;
    vector<string> res;
    
    path = path+"/*";
    glob(path.c_str(), GLOB_TILDE, NULL, &r);
    
    FOR(0,r.gl_pathc){
        res.push_back(r.gl_pathv[i]);
    }

    return res;

}

int main(int argc, char* argv[]){
    
    string path = "./test_images";
    
    vector<string> fileName = readImages(path);
    imShow(fileName);
    
    return 0;

}
