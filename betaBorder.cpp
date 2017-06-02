#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
// static void help()
// {
//     cout << "\nThis program demonstrates kmeans clustering.\n"
//             "It generates an image with random points, then assigns a random number of cluster\n"
//             "centers and uses kmeans to move those cluster centers to their representitive location\n"
//             "Call\n"
//             "./kmeans\n" << endl;
// }
vector<vector<Point> >  contourBubbleSort(vector<vector<Point> > contours){
  //Inverse Bubble sort over contout area
  bool swaped;
  for(int i= 0; i < contours.size()-1; i++){
    swaped=false;
    for(int j=0;j <contours.size()-1;j++)
        if ((contourArea(contours[j]))<(contourArea(contours[j+1]))){
          vector<Point> aux_vector;
          aux_vector=contours[j+1];
          contours[j+1]=contours[j];
          contours[j]=aux_vector;
          swaped=true;
     }
     if(swaped==false)
        break;
  }
  return contours;
}



int main( int argc, char** argv )
{
  // Read command line arg
  char* impath;
  if (argc < 2)
  {
    std::cerr << "Usage: ./pers <PATH_TO_IMAGE_TO_TRANSFORM>" << std::endl;
    return -1;
  } else
  {
    impath = argv[1];
  }
  // Read source image.
  Mat img = imread(impath);

  // Image resize
  float val_magic=300;
  float relation;
  relation=val_magic/img.cols;

  cv::resize(img, img, cv::Size(int(val_magic),int(img.rows*relation)), 0, 0, CV_INTER_LINEAR);
  Mat img2 = img.clone();
  //Image preprocessing
  cv::cvtColor(img,img,CV_BGR2GRAY);
  cv::medianBlur(img,img,5);
  cv::threshold(img,img, 120, 255, CV_THRESH_BINARY);
  cv::Canny(img,img,100,200);

  //Find contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  contours=contourBubbleSort(contours);
  vector<Point2f> screenCnt;

  for(int i= 0; i < contours.size(); i++){
        float perimeter;
        cout << (contourArea(contours[i])) << endl;
        perimeter=cv::arcLength(cv::Mat(contours[i]),true);
        vector<Point2f> approx;
        cv::approxPolyDP(contours[i],approx, 0.05 * perimeter, true);

        cout << approx.size() << endl;
        if ((approx.size()) >= 4){
            screenCnt = approx;
            if ((approx.size()) ==4){
                //skip kmeans if len == 4

                break;
            }
            //Find best 4 corners by kmean
            Mat labels,centers;
            cout << "Holi"<< endl;
            kmeans(Mat(screenCnt), 4 , labels,
                TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
                   3, KMEANS_PP_CENTERS, centers);
            cout << centers << endl;
            //centers.copyTo(screenCnt);
            break;
        }



  }
for(int i=0; i<screenCnt.size(); i++){
circle( img2,
screenCnt[i],
5.0,
Scalar( 0, 0, 255 ),
-1,
8);
}
  imshow("clusters", img2);
  char key = (char)waitKey();

  return 0;
}
