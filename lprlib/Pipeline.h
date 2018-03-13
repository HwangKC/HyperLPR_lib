//
// Created by 庾金科 on 22/10/2017.
//

#ifndef SWIFTPR_PIPLINE_H
#define SWIFTPR_PIPLINE_H

#include "PlateDetection.h"
#include "PlateSegmentation.h"
#include "CNNRecognizer.h"
#include "PlateInfo.h"
#include "FastDeskew.h"
#include "FineMapping.h"
#include "Recognizer.h"
#include "SegmentationFreeRecognizer.h"


extern std::vector<std::string> CH_PLATE_CODE;
namespace pr
{

    //   const std::vector<std::string> CH_PLATE_CODE{"京", "沪", "津", "渝", "冀", "晋", "蒙", "辽", "吉", "黑", "苏", "浙", "皖", "闽", "赣", "鲁", "豫", "鄂", "湘", "粤", "桂",
    //                                       "琼", "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A",
     //                                      "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
       //                                    "Y", "Z","港","学","使","警","澳","挂","军","北","南","广","沈","兰","成","济","海","民","航","空"};


    const int SEGMENTATION_FREE_METHOD = 0;
    const int SEGMENTATION_BASED_METHOD = 1;

    class PipelinePR
    {
    public:
        GeneralRecognizer *generalRecognizer;
        PlateDetection *plateDetection;
        PlateSegmentation *plateSegmentation;
        FineMapping *fineMapping;
        SegmentationFreeRecognizer *segmentationFreeRecognizer;

        PipelinePR(const std::string& detector_filename,
            const std::string& finemapping_prototxt, 
            const std::string& finemapping_caffemodel,
            const std::string& segmentation_prototxt, 
            const std::string& segmentation_caffemodel,
            const std::string& charRecognization_proto, 
            const std::string& charRecognization_caffemodel,
            const std::string& segmentationfree_proto, 
            const std::string& segmentationfree_caffemodel
        );

        ~PipelinePR();

        std::vector<std::string> plateRes;
        std::vector<PlateInfo> RunPiplineAsImage(cv::Mat plateImage, int method);

    private:
        std::string m_detector_filename;
        std::string m_finemapping_prototxt;
        std::string m_finemapping_caffemodel;
        std::string m_segmentation_prototxt;
        std::string m_segmentation_caffemodel;
        std::string m_charRecognization_proto;
        std::string m_charRecognization_caffemodel;
        std::string m_segmentationfree_proto;
        std::string m_segmentationfree_caffemodel;
    };
}
#endif //SWIFTPR_PIPLINE_H
