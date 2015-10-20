/*
 * people_det.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: Tzutalin
 */

#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_loader/load_image.h>

#define  LOG_TAG    "People_Det-JNI"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)


class OpencvHOGDetctor {
public:
    OpencvHOGDetctor() {
    }

    inline int det(std::string path) {
        LOGD("det path %s", path.c_str());
        cv::Mat src_img = cv::imread(path, 1);
        if (src_img.empty())
            return -1;

        cv::HOGDescriptor hog;
        hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
        std::vector<cv::Rect> found, found_filtered;
        hog.detectMultiScale(src_img, found, 0, cv::Size(8,8), cv::Size(32,32), 1.05, 2);
        size_t i, j;
        for (i=0; i<found.size(); i++)
        {
            cv::Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j])==r)
                    break;
            if (j==found.size())
                found_filtered.push_back(r);
        }

        for (i=0; i<found_filtered.size(); i++)
        {
            cv::Rect r = found_filtered[i];
            r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.06);
            r.height = cvRound(r.height*0.9);
            cv::rectangle(src_img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
        }
        mResultMat = src_img;
        cv::imwrite( "/sdcard/testret.jpg", mResultMat);
        LOGD("det ends");
        return 0;
    }

    inline cv::Mat& getResultMat() {
        return mResultMat;
    }
private:
    cv::Mat mResultMat;
};

class DLibHOGFaceDetector{
public:
    DLibHOGFaceDetector() {
  
    }    
    
    inline void det(std::string path) {
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
        dlib::array2d<dlib::rgb_pixel> img;        
        load_image(img, path);        
        dlib::pyramid_up(img);        
        std::vector<dlib::rectangle> dets = detector(img);
        LOGD("Dlib HOG face det size : %d", dets.size());
    }
private:
    std::string mModel;
};

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================
// JNI Mapping Methods
// ========================================================
jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGE("JNI On Load");
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("GetEnv failed!");
        return result;
    }

    return JNI_VERSION_1_6;
}

void JNIEXPORT
JNICALL Java_com_tzutalin_dlib_PeopleDet_jniNativeClassInit(
    JNIEnv *_env, jclass _this) {
     LOGD("JniNativeClassIni");
}

jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniOpencvHOGDetect(
    JNIEnv* env, jobject thiz, jstring imgPath)
{
    LOGD("com_tzutalin_dlib_PeopleDet jniOpencvHOGDetect");
    const char *img_path = env->GetStringUTFChars(imgPath, 0);
    OpencvHOGDetctor hogdet;
    hogdet.det(std::string(img_path));
    env->ReleaseStringUTFChars(imgPath, img_path);
    return 0;
}


jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniDLibHOGDetect(
    JNIEnv* env, jobject thiz, jstring imgPath)
{
    LOGD("com_tzutalin_dlib_PeopleDet jniDLibHOGDetect");
    const char *img_path = env->GetStringUTFChars(imgPath, 0);
    DLibHOGFaceDetector hogdet;    
    hogdet.det(std::string(img_path));
    env->ReleaseStringUTFChars(imgPath, img_path);
    return 0;
}

#ifdef __cplusplus
}
#endif
