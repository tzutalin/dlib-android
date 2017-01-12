#!/usr/bin/env python
# Copyright (c) 2017 Tzutalin
# Create by TzuTaLin <tzu.ta.lin@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
import os
import sys
import argparse
import shutil
import subprocess
from subprocess import Popen, PIPE

DEVICE_ABI = 'armeabi-v7a'

class PrintColors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def parse_args():
    """
    Parse input arguments
    """
    parser = argparse.ArgumentParser(description='Build dlib-android')
    parser.add_argument('--jobs', dest='jobs', help='Jobs to compile',
                        default=4, type=int)

    parser.add_argument('--android_project', dest='android_project',
                        help='Android JNI folder path',
                        default=os.path.join('androidstudio-examples', 'dlib-android-app', 'dlib', 'src', 'main', 'jniLibs'), type=str)

    parser.add_argument('--clean', action='store_true',
                        help='clean obj and binaries')

    parser.add_argument('--test', action='store_true',
                        help='Push executable file to data/local/tmp/, and run them')

    parser.add_argument('--debug', action='store_true',
                        help='Enable debug build')

    args = parser.parse_args()
    return args


def ndk_build(args):
    jobs = str(args.jobs)
    isDebug = '1' if args.debug else '0'
    build_cmd = ['ndk-build',
                 '-j' + jobs,
                 'NDK_LOG=1',
                 'NDK_DEBUG=' + isDebug,
                 'V=0']
    # Print the build command
    print PrintColors.UNDERLINE + 'ndk build arguments:' + str(build_cmd) + PrintColors.ENDC
    ret = subprocess.call(build_cmd)
    if ret is not 0:
        print PrintColors.FAIL + 'Build Error' + PrintColors.ENDC
        os.sys.exit(1)
    else:
        print PrintColors.OKBLUE + 'Build Pass' + PrintColors.ENDC


def ndk_clean():
    subprocess.call(['ndk-build', 'clean'])


def setDeviceABI():
    global DEVICE_ABI
    p = subprocess.Popen(
        ['adb', 'shell', 'getprop', 'ro.product.cpu.abi'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    output, err = p.communicate(
        b"input data that is passed to subprocess' stdin")
    rc = p.returncode
    if rc == 0:
        if "x86" in output:
            DEVICE_ABI = 'x86'

    print PrintColors.OKBLUE + 'We will use ABI:' + DEVICE_ABI + ' binaries to test ' + PrintColors.ENDC


def test():
    global DEVICE_ABI
    # Test max_cost_assignment_ex daemon example
    print '----max_cost_assignment_ex daemon test'
    print '----Push svm_ex to phone device'
    srcFolder = os.path.join('libs', DEVICE_ABI, 'max_cost_assignment_ex')
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp'])
    print '----Execute /data/local/tmp/max_cost_assignment_ex'
    subprocess.call(['adb', 'shell', './data/local/tmp/max_cost_assignment_ex'])

    print '\n\n'
    print 'Test dlib selective search'
    print '----selective search algorithm'
    srcFolder = os.path.join('data', 'lena.jpg')
    print '----Push test image to data/local/tmp'
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp/lena.jpg'])
    print '----Push daemon to /data/local/tmp'
    srcFolder = os.path.join('libs', DEVICE_ABI, 'TestSelectiveSearch')
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp'])
    print '----Execute /data/local/tmp/TestSelectiveSearch'
    subprocess.call(
        ['adb', 'shell', './data/local/tmp/TestSelectiveSearch', '/data/local/tmp/lena.jpg'])

    print '\n\n'
    print 'Test face landmark'
    srcFolder = os.path.join('data', 'lena.bmp')
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp'])
    srcFolder = os.path.join('data', 'shape_predictor_68_face_landmarks.dat')
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp'])
    srcFolder = os.path.join('libs', DEVICE_ABI, 'face_landmark')
    subprocess.call(['adb', 'push', srcFolder, '/data/local/tmp'])
    print '----Execute /data/local/tmp/face_lanmark'
    subprocess.call(['adb', 'shell', './data/local/tmp/face_landmark',
                    '/data/local/tmp/shape_predictor_68_face_landmarks.dat', '/data/local/tmp/lena.bmp'])


def copytree(src, dst, symlinks=False, ignore=None):
    if not os.path.exists(dst):
        os.makedirs(dst)
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            copytree(s, d, symlinks, ignore)
        else:
            if not os.path.exists(d) or os.stat(s).st_mtime - os.stat(d).st_mtime > 1:
                shutil.copy2(s, d)


def copySoToAS(src, dst):
    for root, subdirs, files in os.walk(src):
        for f in files:
            if f.endswith('.so'):
                path = os.path.join(root, f)
                abis = os.path.basename(os.path.dirname(path))
                shutil.copy(path, os.path.join(dst, abis))

# TODO : Refactor
def test_cmake():
    curr = os.getcwd()
    buildFolder = 'build'
    if os.path.exists(buildFolder):
        shutil.rmtree(buildFolder)

    os.mkdir(buildFolder)
    os.chdir(buildFolder)
    cmake_cmd = ['cmake']
    build_cmd = ['make']
    ABI = 'arm64-v8a'
    # ABI = 'x86'
    # TODO
    NDK_PATH = '/home/darrenl/tools/android-ndk-r10e'
    cmake_cmd = cmake_cmd + ['-DCMAKE_SYSTEM_NAME=Android', '-DCMAKE_SYSTEM_VERSION=21',
                                '-DCMAKE_ANDROID_ARCH_ABI=' + ABI, '-DCMAKE_ANDROID_STL_TYPE=gnustl_static',
                                '-DCMAKE_ANDROID_NDK=' + NDK_PATH]
    cmake_cmd = cmake_cmd + ['-D', 'CMAKE_INSTALL_PREFIX=.']
    cmake_cmd = cmake_cmd + ['..']
    # Print the build command
    print PrintColors.UNDERLINE + 'cmake build arguments:' + str(cmake_cmd) + PrintColors.ENDC
    ret = subprocess.call(cmake_cmd)
    ret = subprocess.call(build_cmd)
    if ret is not 0:
        print PrintColors.FAIL + 'Build Error' + PrintColors.ENDC
        os.sys.exit(1)
    else:
        print PrintColors.OKBLUE + 'Build Pass' + PrintColors.ENDC


if __name__ == '__main__':
    # Move to top-level
    ROOT = os.path.dirname(os.path.abspath(__file__))
    os.chdir(ROOT)

    libs_dir = os.path.join(ROOT, 'libs')
    obj_dir = os.path.join(ROOT, 'obj')

    setDeviceABI()

    args = parse_args()

    if args.test:
        os.environ['Test'] = "true"

    if args.clean:
        ndk_clean()
    else:
        ndk_build(args)

    if args.android_project and os.path.exists(args.android_project):
        srcFolder = os.path.join('libs')
        copySoToAS(srcFolder, args.android_project)

    if args.test:
        test()

    os.sys.exit(0)
