## Examples

[face_landmark_detection_ex.cpp](http://blog.dlib.net/2014/08/real-time-face-pose-estimation.html) is a real-time face pose estimation which dlib implement the paper, [One Millisecond Face Alignment with an Ensemble of Regression Trees by Vahid Kazemi and Josephine Sulliva](https://www.google.com.tw/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&uact=8&ved=0CCEQFjAAahUKEwihnIDZg-nHAhWkGKYKHZh1Bbw&url=http%3A%2F%2Fwww.csc.kth.se%2F~vahidk%2Fpapers%2FKazemiCVPR14.pdf&usg=AFQjCNF2xdEKMOTLZ0wY5VilcpU8NgYH5w&sig2=SGYVK5pVrzn4-NB0sA9yIg)

Test it:

`adb push $ROOT/data/shape_predictor_68_face_landmarks.dat /sdcard/`

`adb push $ROOT/data/lena.bmp /sdcard/`

`adb push libs/armeabi-v7a/face_landmark /data/local/tmp/`

`adb shell /data/local/tmp/face_landmark /sdcard/shape_predictor_68_face_landmarks.dat /sdcard/lena.bmp`

Result:
```
take 0.00635206 seconds
number of parts: 68
pixel position of first part:  (220, 269)
pixel position of second part: (218, 298)
```
