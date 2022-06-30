#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

void myMedianFilter(InputArray input, OutputArray& output, int windowSize)
{
    int neighbor = (windowSize - 1) / 2;
    const Mat& img = input.getMat();
    output.create(img.size(), img.type());
    Mat dst = output.getMat();
    dst.create(img.size(), img.type());
    


    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            vector<uchar>medianArr(windowSize * windowSize);
            int num = 0;
            for (int s = -neighbor; s <= neighbor; s++) {
                for (int t = -neighbor; t <= neighbor; t++) {
                    if (t + x >= img.cols || t + x < 0 || s + y >= img.rows || s + y < 0) {
                        medianArr.at(num) = img.at<uchar>(y, x);
                        num++;
                    }
                    else {
                        medianArr.at(num) = img.at<uchar>((s + y), (t + x));
                        num++;

                    }
                }
            }

            
            sort(medianArr.begin(), medianArr.end());
            uchar median = medianArr.at(medianArr.size() / 2);
            dst.at<uchar>(y, x) = median;

        }
    }

}


int main()
{
    Mat image = imread("C:/Users/user/Desktop/Profile.png", 0);
    //Mat output_func;
    Mat output;
    myMedianFilter(image, output, 5);
    //medianBlur(image, output_func, 5);
    //imshow("output_func", output_func);
    imshow("origin", image);
    imshow("output", output);

    waitKey();



}
