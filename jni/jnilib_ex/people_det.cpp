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
            return 0;

        cv::HOGDescriptor hog;
        hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
        std::vector<cv::Rect> found, found_filtered;
        hog.detectMultiScale(src_img, found, 0, cv::Size(8, 8),
                cv::Size(32, 32), 1.05, 2);
        size_t i, j;
        for (i = 0; i < found.size(); i++) {
            cv::Rect r = found[i];
            for (j = 0; j < found.size(); j++)
                if (j != i && (r & found[j]) == r)
                    break;
            if (j == found.size())
                found_filtered.push_back(r);
        }

        for (i = 0; i < found_filtered.size(); i++) {
            cv::Rect r = found_filtered[i];
            r.x += cvRound(r.width * 0.1);
            r.width = cvRound(r.width * 0.8);
            r.y += cvRound(r.height * 0.06);
            r.height = cvRound(r.height * 0.9);
            cv::rectangle(src_img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
        }
        mResultMat = src_img;
        //cv::imwrite(path, mResultMat);
        LOGD("det ends");
        mRets = found_filtered;
        return found_filtered.size();
    }

    inline cv::Mat& getResultMat() {
        return mResultMat;
    }

    inline std::vector<cv::Rect> getResult() {
        return mRets;
    }

private:
    cv::Mat mResultMat;
    std::vector<cv::Rect> mRets;
};

class DLibHOGFaceDetector {
public:
    DLibHOGFaceDetector() {

    }

    inline int det(std::string path) {
        dlib::frontal_face_detector detector =
                dlib::get_frontal_face_detector();

        cv::Mat src_img = cv::imread(path, 1);
        dlib::cv_image<dlib::bgr_pixel> img(src_img);

        mRets = detector(img);
        LOGD("Dlib HOG face det size : %d", mRets.size());

        /*
        // Draw on the input for test
        int i = 0;
        for (i = 0; i < mRets.size(); i++) {
            dlib::rectangle dlibrect = mRets[i];
            cv::Rect r(dlibrect.left(), dlibrect.top(), dlibrect.width(),
                    dlibrect.height());
            r.x += cvRound(r.width * 0.1);
            r.width = cvRound(r.width * 0.8);
            r.y += cvRound(r.height * 0.06);
            r.height = cvRound(r.height * 0.9);
            cv::rectangle(src_img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
        }
        cv::imwrite(path, src_img);
        */
        return mRets.size();
    }

    inline std::vector<dlib::rectangle> getResult() {
        return mRets;
    }

private:
    std::string mModel;
    std::vector<dlib::rectangle> mRets;
};

OpencvHOGDetctor* mOpencvHOGDetctor = NULL;
DLibHOGFaceDetector* mDlibFaceDetector = NULL;

#ifdef __cplusplus
extern "C" {
#endif

struct VisionDetRetOffsets {
    jfieldID label;
    jfieldID confidence;
    jfieldID left;
    jfieldID top;
    jfieldID right;
    jfieldID bottom;
} gVisionDetRetOffsets;

// ========================================================
// JNI Mapping Methods
// ========================================================
jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGE("JNI On Load");
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("GetEnv failed!");
        return result;
    }

    return JNI_VERSION_1_6;
}

void JNIEXPORT
JNICALL Java_com_tzutalin_dlib_PeopleDet_jniNativeClassInit(JNIEnv *_env,
        jclass _this) {
    jclass detRetClass = _env->FindClass("com/tzutalin/dlib/VisionDetRet");
    gVisionDetRetOffsets.label = _env->GetFieldID(detRetClass, "mLabel",
            "java/lang/String");
    gVisionDetRetOffsets.confidence = _env->GetFieldID(detRetClass,
            "mConfidence", "F");
    gVisionDetRetOffsets.left = _env->GetFieldID(detRetClass, "mLeft", "I");
    gVisionDetRetOffsets.top = _env->GetFieldID(detRetClass, "mTop", "I");
    gVisionDetRetOffsets.right = _env->GetFieldID(detRetClass, "mRight", "I");
    gVisionDetRetOffsets.bottom = _env->GetFieldID(detRetClass, "mBottom", "I");
    LOGD("JniNativeClassIni Success");
}

jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniOpencvHOGDetect(
        JNIEnv* env, jobject thiz, jstring imgPath)
{
    LOGD("com_tzutalin_dlib_PeopleDet jniOpencvHOGDetect");
    const char *img_path = env->GetStringUTFChars(imgPath, 0);
    if(mOpencvHOGDetctor == NULL)
        mOpencvHOGDetctor = new OpencvHOGDetctor();

    int nums = mOpencvHOGDetctor->det(std::string(img_path));
    env->ReleaseStringUTFChars(imgPath, img_path);
    return nums;
}

jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniGetOpecvHOGRet(JNIEnv* env,
        jobject thiz, jobject detRet, jint index)
{
    if (mOpencvHOGDetctor) {
        cv::Rect rect = mOpencvHOGDetctor->getResult()[index];
        env->SetIntField(detRet, gVisionDetRetOffsets.left, rect.x);
        env->SetIntField(detRet, gVisionDetRetOffsets.top, rect.y);
        env->SetIntField(detRet, gVisionDetRetOffsets.right, rect.x + rect.width);
        env->SetIntField(detRet, gVisionDetRetOffsets.bottom, rect.y + rect.height);
        env->SetFloatField(detRet, gVisionDetRetOffsets.confidence, 0);
        jstring jstr=(jstring)(env->NewStringUTF("person"));
        env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);
        return 0;
    }
    return -1;
}

jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniDLibHOGDetect(
        JNIEnv* env, jobject thiz, jstring imgPath, jobject detRet)
{
    LOGD("com_tzutalin_dlib_PeopleDet jniDLibHOGDetect");
    const char *img_path = env->GetStringUTFChars(imgPath, 0);
    if(mDlibFaceDetector == NULL)
        mDlibFaceDetector = new DLibHOGFaceDetector();

    int size = mDlibFaceDetector->det(std::string(img_path));
    env->ReleaseStringUTFChars(imgPath, img_path);
    return size;
}

jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniGetDLibRet(JNIEnv* env,
        jobject thiz, jobject detRet, jint index)
{
    if (mDlibFaceDetector) {
        dlib::rectangle rect = mDlibFaceDetector->getResult()[index];
        env->SetIntField(detRet, gVisionDetRetOffsets.left, rect.left());
        env->SetIntField(detRet, gVisionDetRetOffsets.top, rect.top());
        env->SetIntField(detRet, gVisionDetRetOffsets.right, rect.right());
        env->SetIntField(detRet, gVisionDetRetOffsets.bottom, rect.bottom());
        env->SetFloatField(detRet, gVisionDetRetOffsets.confidence, 0);
        jstring jstr=(jstring)(env->NewStringUTF("face"));
        env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);
        return 0;
    }
    return -1;
}

jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniInit(JNIEnv* env, jobject thiz)
{

    if(mDlibFaceDetector == NULL)
        mDlibFaceDetector = new DLibHOGFaceDetector();

    if (mOpencvHOGDetctor == NULL)
        mOpencvHOGDetctor = new OpencvHOGDetctor();

    return 0;
}


jint JNIEXPORT JNICALL
Java_com_tzutalin_dlib_PeopleDet_jniDeInit(JNIEnv* env, jobject thiz)
{
    if(mDlibFaceDetector)
        delete mDlibFaceDetector;

    if (mOpencvHOGDetctor)
        delete mOpencvHOGDetctor;

    return 0;
}


#ifdef __cplusplus
}
#endif
