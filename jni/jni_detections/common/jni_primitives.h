#ifndef JNI_PRIMITIVES_H
#define JNI_PRIMITIVES_H

#include <jni.h>
#include <glog/logging.h>

// Java Integer/Float
#define CLASSNAME_INTEGER "java/lang/Integer"
#define CONSTSIG_INTEGER "(I)V"
#define CLASSNAME_FLOAT "java/lang/Float"
#define CONSTSIG_FLOAT "(F)V"
#define CLASSNAME_POINTF "android/graphics/PointF"
#define CONSTSIG_POINTF "()V"

#define CLASSNAME_VISION_DET_RET "com/tzutalin/dlib/VisionDetRet"
#define CONSTSIG_VISION_DET_RET "()V"

#define CLASSNAME_FACE_DET "com/tzutalin/dlib/FaceDet"
#define CLASSNAME_PEDESTRIAN_DET "com/tzutalin/dlib/PedestrianDet"

class JavaPeer {
 public:
  JavaPeer(JNIEnv* env, const char* className, const char* constSig) {
    cls =
        reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass(className)));
    if (env->ExceptionCheck())
      DLOG(INFO) << "Failed to find class " << className;

    constructor = env->GetMethodID(cls, "<init>", constSig);
    if (env->ExceptionCheck())
      DLOG(INFO) << "Failed to find method " << constSig;

    env->GetJavaVM(&vm);
  }

  ~JavaPeer() {
    if (!vm)
      return;

    JNIEnv* env;
    vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    env->DeleteGlobalRef(cls);
  }

  jobjectArray ConstructArray(JNIEnv* env, int size) const {
    return env->NewObjectArray(size, cls, nullptr);
  }

  jobject Construct(JNIEnv* pEnv, ...) const {
    va_list args;
    va_start(args, pEnv);
    jobject obj = pEnv->NewObjectV(cls, constructor, args);
    va_end(args);

    return obj;
  }

  JavaVM* vm;
  jclass cls;
  jmethodID constructor;
};

// Java Integer/Float
class JNI_Integer : public JavaPeer {
 public:
  JNI_Integer(JNIEnv* pEnv)
      : JavaPeer(pEnv, CLASSNAME_INTEGER, CONSTSIG_INTEGER) {}
};

class JNI_Float : public JavaPeer {
 public:
  JNI_Float(JNIEnv* pEnv) : JavaPeer(pEnv, CLASSNAME_FLOAT, CONSTSIG_FLOAT) {}
};

struct PointF {
  float x;
  float y;
};

class JNI_PointF : public JavaPeer {
 public:
  JNI_PointF(JNIEnv* pEnv) : JavaPeer(pEnv, CLASSNAME_POINTF, CONSTSIG_POINTF) {
    m_id_x = pEnv->GetFieldID(cls, "x", "F");
    m_id_y = pEnv->GetFieldID(cls, "y", "F");
  }

  void Set(JNIEnv* pEnv, const PointF& pointF, jobject objPointF) const {
    pEnv->SetFloatField(objPointF, m_id_x, pointF.x);
    pEnv->SetFloatField(objPointF, m_id_y, pointF.y);
  }

  PointF Get(JNIEnv* pEnv, jobject objPointF) const {
    PointF pointF;
    pointF.x = pEnv->GetFloatField(objPointF, m_id_x);
    pointF.y = pEnv->GetFloatField(objPointF, m_id_y);
    return pointF;
  }

 private:
  jfieldID m_id_x;
  jfieldID m_id_y;
};


class JNI_VisionDetRet {
 public:
  JNI_VisionDetRet(JNIEnv* env) {
    jclass detRetClass = env->FindClass(CLASSNAME_VISION_DET_RET);
    CHECK_NOTNULL(detRetClass);
    jID_label = env->GetFieldID(detRetClass, "mLabel", "Ljava/lang/String;");
    jID_confidence = env->GetFieldID(detRetClass, "mConfidence", "F");
    jID_left = env->GetFieldID(detRetClass, "mLeft", "I");
    jID_top = env->GetFieldID(detRetClass, "mTop", "I");
    jID_right = env->GetFieldID(detRetClass, "mRight", "I");
    jID_bottom = env->GetFieldID(detRetClass, "mBottom", "I");
    jMethodID_addLandmark =
        env->GetMethodID(detRetClass, "addLandmark", "(II)Z");
  }

  void setLabel(JNIEnv* env, jobject& jDetRet, const std::string& label) {
    jstring jstr = (jstring)(env->NewStringUTF(label.c_str()));
    env->SetObjectField(jDetRet, jID_label, (jobject)jstr);
  }

  void setRect(JNIEnv* env, jobject& jDetRet, const int& left, const int& top,
               const int& right, const int& bottom) {
    env->SetIntField(jDetRet, jID_left, left);
    env->SetIntField(jDetRet, jID_top, top);
    env->SetIntField(jDetRet, jID_right, right);
    env->SetIntField(jDetRet, jID_bottom, bottom);
  }

  void addLandmark(JNIEnv* env, jobject& jDetRet, const int& x, const int& y) {
    env->CallBooleanMethod(jDetRet, jMethodID_addLandmark, x, y);
  }

  static jobject createJObject(JNIEnv* env) {
    jclass detRetClass = env->FindClass(CLASSNAME_VISION_DET_RET);
    jmethodID mid =
        env->GetMethodID(detRetClass, "<init>", CONSTSIG_VISION_DET_RET);
    return env->NewObject(detRetClass, mid);
  }

  static jobjectArray createJObjectArray(JNIEnv* env, const int& size) {
    jclass detRetClass = env->FindClass(CLASSNAME_VISION_DET_RET);
    return (jobjectArray)env->NewObjectArray(size, detRetClass, NULL);
  }

 private:
  jfieldID jID_label;
  jfieldID jID_confidence;
  jfieldID jID_left;
  jfieldID jID_top;
  jfieldID jID_right;
  jfieldID jID_bottom;
  jmethodID jMethodID_addLandmark;
};
#endif  // JNI_PRIMITIVES_H
