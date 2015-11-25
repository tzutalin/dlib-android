#!/usr/bin/env python
import os
import sys
import subprocess
import shutil
import argparse

def parse_args():
    """
    Parse input arguments
    """
    parser = argparse.ArgumentParser(description='Build dlib-android')
    parser.add_argument('--jobs', dest='jobs', help='Jobs to compile',
                        default=1, type=int)

    parser.add_argument('--android_project', dest='android_project',
                        help='Android JNI folder path',
                        default=None, type=str)

    parser.add_argument('--clean', dest='clean',
                        help='clean',
                        default=False, type=bool)

    parser.add_argument('--test', dest='test',
                        help='Push TestSelectiveSearch sample to arm device to test',
                        default=False, type=bool)



    args = parser.parse_args()
    return args

def build(jobs):
    subprocess.call(['ndk-build', '-j', str(jobs)])

def clean():
    subprocess.call(['ndk-build', 'clean'])

def test():
    # Test max_cost_assignment_ex daemon example
    print '----max_cost_assignment_ex daemon test'
    print '----Push svm_ex to arm device'
    srcFolder = os.path.join('libs', 'armeabi-v7a', 'max_cost_assignment_ex')
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp'])
    print '----Execute /data/local/tmp/max_cost_assignment_ex'
    subprocess.call(['adb', 'shell', './data/local/tmp/max_cost_assignment_ex'])

    print '\n\n'
    # Test dlib's selective search
    print '----selective search algorithm'
    srcFolder = os.path.join('jni','other_daemon_ex', 'data', 'lena.jpg')
    print '----Push test image to sdcard'
    subprocess.call(['adb', 'push', srcFolder, '/sdcard/lena.jpg'])
    srcFolder = os.path.join('libs', 'armeabi-v7a', 'TestSelectiveSearch')
    print '----Push daemon to /data/local/tmp'
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp'])
    print '----Execute /data/local/tmp/TestSelectiveSearch'
    subprocess.call(['adb', 'shell', './data/local/tmp/TestSelectiveSearch','/sdcard/lena.jpg'])

def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)

if __name__ == '__main__':
    args = parse_args()
    if args.clean is True:
        clean()
        exit()
    else:
        build(args.jobs)

    if args.android_project:
        srcFolder = os.path.join('libs')
        copytree(srcFolder, args.android_project)

    if args.test:
        test()

