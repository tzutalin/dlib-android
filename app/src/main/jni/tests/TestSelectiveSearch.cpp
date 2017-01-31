//============================================================================
// Name        : TestSelectiveSearch.cpp
// Author      : Tzutalin
// Version     : 1.0
// Copyright   : Tzutalin
// Description : TestSelectiveSearch in C++, Ansi-style
//============================================================================
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dlib/matrix/matrix_utilities.h>
#include <dlib/image_transforms/segment_image.h>
#include <dlib/opencv/cv_image.h>

#include <iostream>
#include <ctime>
#include <vector>
std::vector<dlib::rectangle> getProposals(const cv::Mat& inputImage) {
  dlib::cv_image<dlib::rgb_pixel> image(inputImage);
  // Find candidate objects
  std::vector<dlib::rectangle> rects;
  dlib::find_candidate_object_locations(image, rects);
  return rects;
}

void detectByFilePath(std::string path) {
  // Find candidate objects
  cv::Mat inputImage = cv::imread(path);
  const clock_t begin_time = clock();
  std::vector<dlib::rectangle> rects = getProposals(inputImage);
  const clock_t end_time = clock();
  std::cout << "Find proposals size: " << rects.size() << " takes "
            << (double(end_time - begin_time) / CLOCKS_PER_SEC) << " seconds"
            << std::endl;
}

int main(int argc, char** argv) {
  std::cout << "TestSelectiveSearch" << std::endl;
  if (argc == 1) {
    std::cout << "Give some image files as arguments to this program."
              << std::endl;
    return 0;
  }
  // Find proposals
  detectByFilePath(std::string(argv[1]));
  return 0;
}

#ifdef UBUNTU_DEV
#include <boost/python.hpp>
BOOST_PYTHON_MODULE(sss) {
  using namespace boost::python;
  def("test", detectByFilePath, arg("path"));
}
#endif
