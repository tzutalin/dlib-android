//============================================================================
// Name        : face_landmark_detection_ex.cpp
// Author      : Tzutalin
// Version     : 1.1
// Copyright   : Tzutalin
// Description : face_landmark_detection_ex in C++, Google-Style

/* The pose estimator was created by
   using dlib's implementation of the paper:
       One Millisecond Face Alignment with an Ensemble of Regression Trees by
       Vahid Kazemi and Josephine Sullivan, CVPR 2014
   and was trained on the iBUG 300-W face landmark dataset.
*/
//============================================================================
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_loader/load_image.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace dlib;
using namespace std;

int main(int argc, char** argv) {
  try {
    if (argc == 1) {
      cout << "Call this program like this:" << endl;
      cout << "./face_landmark_detection_ex "
              "shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
      cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file "
              "from:\n";
      cout << "http://sourceforge.net/projects/dclib/files/dlib/v18.10/"
              "shape_predictor_68_face_landmarks.dat.bz2" << endl;
      return -1;
    }

    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor sp;
    deserialize(argv[1]) >> sp;
    const std::string path = argv[2];
    // Read a image using opencv
    cv::Mat inputImage = cv::imread(path, CV_LOAD_IMAGE_COLOR);

    // Limit minimal detected face size so you can downscale input image several
    // times to skip small faces.
    unsigned min_face_size = 200;  // px
    // double scale = 80.0 / min_face_size;
    // if scale is 2.5, it will be faster 2.5x , but the landmark will not fix
    // well sometimes
    double scale = 1.0;
    double inverse_scale = 1 / scale;
    cv::resize(inputImage, inputImage, cv::Size(), scale, scale);

    // Check for invalid input
    if (!inputImage.data) {
      cout << "Could not open or find the image" << std::endl;
      return -1;
    }
    long t0 = cv::getTickCount();

    // Convert mat to dlib's bgr pixel
    dlib::cv_image<dlib::bgr_pixel> img(inputImage);
    // Start detecting
    std::vector<rectangle> dets = detector(img);
    std::vector<full_object_detection> shapes;
    for (unsigned long j = 0; j < dets.size(); ++j) {
      full_object_detection shape = sp(img, dets[j]);
      cout << "number of parts: " << shape.num_parts() << endl;
      int x = shape.part(0).x();
      int y = shape.part(0).y();
      cout << "pixel position  0 index, x: " << x << endl;
      cout << "pixel position  0 index, y: " << y << endl;
      // If input is lena.bmp, 0 index is supposed to be (109, 135)
      shapes.push_back(shape);
    }
    long t1 = cv::getTickCount();
    double secs = (t1 - t0) / cv::getTickFrequency();
    cout << "take " << secs << " seconds " << endl;
    return 0;
  } catch (exception& e) {
    cout << "\nexception thrown!" << endl;
    cout << e.what() << endl;
  }
}
