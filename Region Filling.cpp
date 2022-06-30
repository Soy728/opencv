#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>

using namespace cv;
using namespace std;

POINT point;
Mat original = imread("C:/Users/user/Desktop/Profile.png", 0);
Mat output_boundary;
Mat output_filling;

void boundary(InputArray input, OutputArray& output, int morph_size) //find boundary 
{
    const Mat& img = input.getMat();
    output.create(img.size(), img.type());
    Mat dst1;
    dst1.create(img.size(), img.type());
    Mat dst2 = output.getMat();
    dst2.create(img.size(), img.type());

    Mat element = getStructuringElement(
        MORPH_RECT, Size(2 * morph_size + 1,
            2 * morph_size + 1),
        Point(morph_size, morph_size));

    erode(img, dst1, element, Point(-1, -1), 1);


    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            dst2.at<uchar>(y, x) = img.at<uchar>(y, x) - dst1.at<uchar>(y, x);
        }
    }


}

void RegionFilling(int y, int x, InputArray input, OutputArray& output, int morph_size)
{
    int y_ = y;
    int x_ = x;
    const Mat& original = input.getMat();


    Mat blackimg = Mat(original.size(), CV_8UC1, Scalar(0, 0));
    blackimg.at<uchar>(y_, x_) = 255; // 흰 점 찍기

    Mat dst = output.getMat();
    dst = blackimg;


    Mat dst_copy; // 반복문을 계속 할지 말지 판단하는 X(k-1)변수

    Mat element = getStructuringElement(
        MORPH_ELLIPSE, Size(2 * morph_size + 1,
            2 * morph_size + 1),
        Point(morph_size, morph_size));


    int same = 1;
    while (same >= 1) {

        imshow("output_boundary", dst);

        blackimg.copyTo(dst_copy);
        dilate(blackimg, dst, element, Point(-1, -1), 1);

        for (int y = 0; y < original.rows; y++) {
            for (int x = 0; x < original.cols; x++) {
                if (original.at<uchar>(y, x) > 100 && dst.at<uchar>(y, x) > 100) {
                    blackimg.at<uchar>(y, x) = 255;
                }
                else blackimg.at<uchar>(y, x) = 0;
            }
        }
        same = 0;
        for (int y = 0; y < original.rows; y++) {
            for (int x = 0; x < original.cols; x++) {
                if (dst_copy.at<uchar>(y, x) - blackimg.at<uchar>(y, x) == 0) {
                    continue;

                }
                else // 다른게 하나라도 있으면 same 이 1이상
                    same += 1;
            }
        }
        waitKey(40);
    }

}


void on_mouse(int event, int x, int y, int flags, void*)
{
    if (event == EVENT_LBUTTONDOWN) {

        point.x = x;
        point.y = y;
        cout << "x-coordinate: " << x << ", y-coordinate:" << y << endl;

        RegionFilling(point.y, point.x, original, output_boundary, 2);
    }
}

int main()
{
    boundary(original, output_boundary, 2);
    imshow("output_boundary", output_boundary);
    cout << "Please click on the screen" << endl;
    cout << "If you click outside the area, Region filling does not proceed." << endl;
    setMouseCallback("output_boundary", on_mouse);
    waitKey();


}