/*
 * jni_facelandmark_det.cpp
 *
 *  Created on: March 26, 2015
 *      Author: Tzutalin
 */

#include <dlib/image_loader/load_image.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/opencv.h>
#include <dlib/opencv/cv_image.h>
#include <glog/logging.h>
#include <jni.h>
#include <memory>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================
// JNI Mapping Methods
// ========================================================
#define DLIB_JNI_METHOD(METHOD_NAME) \
	Java_com_tzutalin_dlib_FaceLandmark_##METHOD_NAME

#define DLIB_FS_NUM_LANDMARKS	68
#define LANDMARK_COLOR    CV_RGB(0, 255, 0)
#define LANDMARK_THICKNESS  1

#define FACE_ROI_COLOR    CV_RGB(255, 0, 0)
#define FACE_ROI_THICKNESS  2

class FaceLandmarkDetector {
 public:
  FaceLandmarkDetector() {
  }
  virtual ~FaceLandmarkDetector() {
  }

  void loadModel(const char *pszModel) {
    dlib::deserialize(pszModel) >> m_sp;
  }

  bool detectLandmarks(const cv::Mat& input, const cv::Rect& roi,
                       std::vector<cv::Point> &vPoint) {

    dlib::cv_image<uchar> img(input);

    dlib::rectangle rect;
    rect.left() = roi.x;
    rect.top() = roi.y;
    rect.right() = roi.x + roi.width;
    rect.bottom() = roi.y + roi.height;

    m_det = m_sp(img, rect);

    vPoint.reserve(DLIB_FS_NUM_LANDMARKS);
    for (int i = 0; i < DLIB_FS_NUM_LANDMARKS; i++) {
      vPoint.push_back(cv::Point(m_det.part(i).x(), m_det.part(i).y()));
    }

    return (m_det.num_parts() == DLIB_FS_NUM_LANDMARKS);
  }

 private:
  dlib::shape_predictor m_sp;
  dlib::full_object_detection m_det;
};

std::shared_ptr<FaceLandmarkDetector> gFaceLandmarkDetectorPtr;

jint JNIEXPORT JNICALL
DLIB_JNI_METHOD(jniGetLandmark)(
    JNIEnv* env, jobject thiz, jstring imgPath)
{
  LOG(INFO) << "jniGetLandmark";
  return 0;
}

jint JNIEXPORT JNICALL
DLIB_JNI_METHOD(jniInit)(JNIEnv* env, jobject thiz)
{

  return JNI_OK;
}

jint JNIEXPORT JNICALL
DLIB_JNI_METHOD(jniDeInit)(JNIEnv* env, jobject thiz)
{

  return JNI_OK;
}

#ifdef __cplusplus
}
#endif
