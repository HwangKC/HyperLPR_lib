#ifndef __USING_OPENCV_INCLUDE__
#define __USING_OPENCV_INCLUDE__

//#define __USING_OPENCV_WORLD__
//#define __SURPOPRT_CUDA__

#include <opencv2/opencv.hpp>

#define CV_VERSION_ID       CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#define CV_VERSION_STR		CV_VERSION


#ifdef _DEBUG
#   define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#   define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif

#if 0
#define LINKLIB(name)	__pragma(message("** AUTO LINKING " cvLIB(name))) \
						__pragma(comment( lib, cvLIB(name)) )
#else
#   pragma message("** AUTO LINKING OPENCV Library Version:" CV_VERSION_ID )
#   define LINKLIB(name)	__pragma(comment( lib, cvLIB(name)) )
#endif

#ifdef _USING_OPENCV_WORLD_
    LINKLIB("world")
#else
    LINKLIB("aruco")
    LINKLIB("bgsegm")
    LINKLIB("bioinspired")
    LINKLIB("calib3d")
    LINKLIB("ccalib")
    LINKLIB("core")
    LINKLIB("datasets")
    LINKLIB("dnn")
    LINKLIB("dpm")
    LINKLIB("face")
    LINKLIB("features2d")
    LINKLIB("flann")
    LINKLIB("fuzzy")
    LINKLIB("highgui")
    LINKLIB("img_hash")
    LINKLIB("imgcodecs")
    LINKLIB("imgproc")
    LINKLIB("line_descriptor")
    LINKLIB("ml")
    LINKLIB("objdetect")
    LINKLIB("optflow")
    LINKLIB("phase_unwrapping")
    LINKLIB("photo")
    LINKLIB("plot")
    LINKLIB("reg")
    LINKLIB("rgbd")
    LINKLIB("saliency")
    LINKLIB("shape")
    LINKLIB("stereo")
    LINKLIB("stitching")
    LINKLIB("structured_light")
    LINKLIB("superres")
    LINKLIB("surface_matching")
    LINKLIB("text")
    LINKLIB("tracking")
    //LINKLIB("ts")
    LINKLIB("video")
    LINKLIB("videoio")
    LINKLIB("videostab")
    LINKLIB("xfeatures2d")
    LINKLIB("ximgproc")
    LINKLIB("xobjdetect")
    LINKLIB("xphoto")

#   ifdef __SURPOPRT_CUDA__
        LINKLIB("cudaarithm")
        LINKLIB("cudabgsegm")
        LINKLIB("cudacodec")
        LINKLIB("cudafeatures2d")
        LINKLIB("cudafilters")
        LINKLIB("cudaimgproc")
        LINKLIB("cudalegacy")
        LINKLIB("cudaobjdetect")
        LINKLIB("cudaoptflow")
        LINKLIB("cudastereo")
        LINKLIB("cudawarping")
        LINKLIB("cudev")
#   endif
#endif
#endif
