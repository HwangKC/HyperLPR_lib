//
// Created by 庾金科 on 23/10/2017.
//

#include "Pipeline.h"


namespace pr
{
    const int HorizontalPadding = 4;
    PipelinePR::PipelinePR(const std::string& detector_filename,
        const std::string& finemapping_prototxt,
        const std::string& finemapping_caffemodel,
        const std::string& segmentation_prototxt,
        const std::string& segmentation_caffemodel,
        const std::string& charRecognization_proto,
        const std::string& charRecognization_caffemodel,
        const std::string& segmentationfree_proto,
        const std::string& segmentationfree_caffemodel
    )
    {
        m_detector_filename = detector_filename;
        m_finemapping_prototxt = finemapping_prototxt;
        m_finemapping_caffemodel = finemapping_caffemodel;
        m_segmentation_prototxt = segmentation_prototxt;
        m_segmentation_caffemodel = segmentation_caffemodel;
        m_charRecognization_proto = charRecognization_proto;
        m_charRecognization_caffemodel = charRecognization_caffemodel;
        m_segmentationfree_proto = segmentationfree_proto;
        m_segmentationfree_caffemodel = segmentationfree_caffemodel;

        plateDetection = NULL;
        fineMapping = NULL;
        plateSegmentation = NULL;
        generalRecognizer = NULL;
        segmentationFreeRecognizer = NULL;

    }

    PipelinePR::~PipelinePR()
    {
       if (plateDetection) delete plateDetection;
       if (fineMapping) delete fineMapping;
       if (plateSegmentation) delete plateSegmentation;
       if (generalRecognizer) delete generalRecognizer;
       if (segmentationFreeRecognizer) delete segmentationFreeRecognizer;

       plateDetection = NULL;
       fineMapping = NULL;
       plateSegmentation = NULL;
       generalRecognizer = NULL;
       segmentationFreeRecognizer = NULL;
    }

    std::vector<PlateInfo> PipelinePR::RunPiplineAsImage(cv::Mat plateImage, int method)
    {
        plateDetection = new PlateDetection(m_detector_filename);
        fineMapping = new FineMapping(m_finemapping_prototxt, m_finemapping_caffemodel);
        plateSegmentation = new PlateSegmentation(m_segmentation_prototxt, m_segmentation_caffemodel);
        generalRecognizer = new CNNRecognizer(m_charRecognization_proto, m_charRecognization_caffemodel);


        std::vector<PlateInfo> results;
        std::vector<pr::PlateInfo> plates;
        plateDetection->plateDetectionRough(plateImage, plates, 36, 700);

        for (pr::PlateInfo plateinfo : plates)
        {
            cv::Mat image_finemapping = plateinfo.getPlateImage();
            image_finemapping = fineMapping->FineMappingVertical(image_finemapping);
            image_finemapping = pr::fastdeskew(image_finemapping, 5);

            //Segmentation-based

            if (method == SEGMENTATION_BASED_METHOD)
            {
                image_finemapping = fineMapping->FineMappingHorizon(image_finemapping, 2, HorizontalPadding);
                cv::resize(image_finemapping, image_finemapping, cv::Size(136 + HorizontalPadding, 36));
                plateinfo.setPlateImage(image_finemapping);
                std::vector<cv::Rect> rects;

                plateSegmentation->segmentPlatePipline(plateinfo, 1, rects);
                plateSegmentation->ExtractRegions(plateinfo, rects);
                cv::copyMakeBorder(image_finemapping, image_finemapping, 0, 0, 0, 20, cv::BORDER_REPLICATE);
                plateinfo.setPlateImage(image_finemapping);
                generalRecognizer->SegmentBasedSequenceRecognition(plateinfo);
                plateinfo.decodePlateNormal(CH_PLATE_CODE);

            }

            //Segmentation-free
            else if (method == SEGMENTATION_FREE_METHOD)
            {
                segmentationFreeRecognizer = new SegmentationFreeRecognizer(m_segmentationfree_proto, m_segmentationfree_caffemodel);

                image_finemapping = fineMapping->FineMappingHorizon(image_finemapping, 4, HorizontalPadding + 3);

                cv::resize(image_finemapping, image_finemapping, cv::Size(136 + HorizontalPadding, 36));

                plateinfo.setPlateImage(image_finemapping);
                std::pair<std::string, float> res = segmentationFreeRecognizer->SegmentationFreeForSinglePlate(plateinfo.getPlateImage(), CH_PLATE_CODE);
                plateinfo.confidence = res.second;
                plateinfo.setPlateName(res.first);
            }

            results.push_back(plateinfo);
        }

        return results;

    }//namespace pr

}