//============================================================================
// Name        : face_landmark_detection_ex.cpp
// Author      : Tzutalin
// Version     : 1.0
// Copyright   : Tzutalin
// Description : face_landmark_detection_ex in C++, Ansi-style

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

int main(int argc, char** argv)
{
    try
    {
        if (argc == 1)
        {
            cout << "Call this program like this:" << endl;
            cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
            cout << "http://sourceforge.net/projects/dclib/files/dlib/v18.10/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            return 0;
        }

        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor sp;
        deserialize(argv[1]) >> sp;
        std::string path = argv[2];
        // TODO: Replace load_image with opencv
        // cv::Mat inputImage = cv::imread(path);
        // dlib::cv_image<dlib::rgb_pixel> img(inputImage);
        array2d<rgb_pixel> img;
        load_image(img, path);
        pyramid_up(img);
        std::vector<rectangle> dets = detector(img);
        std::vector<full_object_detection> shapes;
        for (unsigned long j = 0; j < dets.size(); ++j)
        {
            int64 t0 = cv::getTickCount();
            full_object_detection shape = sp(img, dets[j]);
            int64 t1 = cv::getTickCount();
            double secs = (t1-t0)/cv::getTickFrequency();
            cout << "take " << secs << " seconds "<< endl;
            cout << "number of parts: "<< shape.num_parts() << endl;
            cout << "pixel position of first part:  " << shape.part(0) << endl;
            cout << "pixel position of second part: " << shape.part(1) << endl;
            shapes.push_back(shape);
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}
