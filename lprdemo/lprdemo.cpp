#include "stdafx.h"

#include <sys/stat.h>
#include <algorithm>
#include <functional>

#include <lprlib.h>

#define DEFAULT_DISPLAY_WIDTH (800)
#define DEFAULT_DISPLAT_HEIGHT (600)


std::vector<std::string> CH_PLATE_CODE{ "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "³", "ԥ", "��", "��", "��", "��",
                                       "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A",
                                      "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
                                   "Y", "Z","��","ѧ","ʹ","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��" };



using namespace std;

template<class T>
static unsigned int levenshtein_distance(const T &s1, const T &s2)
{
    const size_t len1 = s1.size(), len2 = s2.size();
    std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);

    for (unsigned int i = 0; i < prevCol.size(); i++) prevCol[i] = i;
    for (unsigned int i = 0; i < len1; i++)
    {
        col[0] = i + 1;
        for (unsigned int j = 0; j < len2; j++)
            col[j + 1] = min(
                min(prevCol[1 + j] + 1, col[j] + 1),
                prevCol[j] + (s1[i] == s2[j] ? 0 : 1));
        col.swap(prevCol);
    }
    return prevCol[len2];
}

bool compare(const pr::PlateInfo & m1, const pr::PlateInfo & m2)
{
    return m1.confidence > m2.confidence;
}

void TEST_ACC()
{

    pr::PipelinePR prc("model/cascade.xml",
        "model/HorizonalFinemapping.prototxt", 
        "model/HorizonalFinemapping.caffemodel",
        "model/Segmentation.prototxt", 
        "model/Segmentation.caffemodel",
        "model/CharacterRecognization.prototxt", 
        "model/CharacterRecognization.caffemodel",
        "model/SegmentationFree.prototxt", 
        "model/SegmentationFree.caffemodel"
    );

    ifstream file;
    string imagename;
    int n = 0, correct = 0, j = 0, sum = 0;
    char filename[] = "E:\\general_test\\1.txt";
    string pathh = "E:\\general_test\\";
    file.open(filename, ios::in);
    //	cout << filename << endl;
    while (!file.eof())
    {
        file >> imagename;
        string imgpath = pathh + imagename;
        //	cout << imgpath << endl;
        std::cout << "------------------------------------------------" << endl;
        cout << "ͼƬ����" << imagename << endl;
        cv::Mat image = cv::imread(imgpath);
        //		cv::imshow("image", image);
        //		cv::waitKey(0);

        std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image, pr::SEGMENTATION_FREE_METHOD);

        float conf = 0;
        vector<float> con;
        vector<string> name;
        for (auto st : res)
        {
            if (st.confidence > 0.1)
            {
                //std::cout << st.getPlateName() << " " << st.confidence << std::endl;
                con.push_back(st.confidence);
                name.push_back(st.getPlateName());
                //conf += st.confidence;
            }
            else
                cout << "no string" << endl;
        }
        //	std::cout << conf << std::endl;
        int num = con.size();
        float max = 0;
        string platestr, chpr, ch;
        int diff = 0, dif = 0;
        for (int i = 0; i < num; i++)
        {

            if (con.at(i) > max)
            {
                max = con.at(i);
                platestr = name.at(i);
            }

        }
        //	cout << "max:"<<max << endl;
        cout << "string:" << platestr << endl;
        chpr = platestr.substr(0, 2);
        ch = imagename.substr(0, 2);
        diff = levenshtein_distance(imagename, platestr);
        dif = diff - 4;
        cout << "���:" << dif << endl;
        sum += dif;
        if (ch != chpr) n++;
        if (diff == 0)	correct++;
        j++;
    }
    float cha = 1 - float(n) / float(j);
    std::cout << "------------------------------------------------" << endl;
    cout << "����������" << j << endl;
    cout << "����ʶ��׼ȷ�ʣ�" << cha << endl;
    float chaccuracy = 1 - float(sum - n * 2) / float(j * 8);
    cout << "�ַ�ʶ��׼ȷ�ʣ�" << chaccuracy << endl;
    cv::waitKey(0);

}


void TEST_PIPELINE(char* pszImagePath, int default_width)
{
    if (pszImagePath == NULL)
    {
        std::cout << "NULL plate image!" << std::endl;
        return;
    }

    struct _stat fileStat;

    int ret = _stat(pszImagePath, &fileStat);
    if (ret != 0) //�ļ�������
    {
        std::cout << "Image " << pszImagePath << " do not exist!" << std::endl;
        return;
    }
    //��ʱ�����㴦��һ��ͼƬ��Ҫ��ʱ��
    CWatchTimer wt;
    wt.start();

    pr::PipelinePR prc(
        "model/cascade.xml",
        "model/HorizonalFinemapping.prototxt",
        "model/HorizonalFinemapping.caffemodel",
        "model/Segmentation.prototxt",
        "model/Segmentation.caffemodel",
        "model/CharacterRecognization.prototxt",
        "model/CharacterRecognization.caffemodel",
        "model/SegmentationFree.prototxt",
        "model/SegmentationFree.caffemodel"
    );

    std::cout << "PipelinePR initialze elapsed (us):" << wt.elapsed()*1e6 << std::endl;

    std::cout << "Processing " << pszImagePath << std::endl;

    wt.start();

    cv::Mat image = cv::imread(pszImagePath);

    std::cout << "reading image elapsed (us):" << wt.elapsed()*1e6 << std::endl;

    std::cout << "Source Image size (w*h): " << image.cols << " * " << image.rows << std::endl;

    cv::Mat image2;
    bool bScaled = false;
    if (image.cols > image.rows) //w>h
    {
        if (image.cols > DEFAULT_DISPLAY_WIDTH)
        {
            cv::resize(image, image2, cv::Size(DEFAULT_DISPLAY_WIDTH, image.rows * DEFAULT_DISPLAY_WIDTH / image.cols), cv::INTER_LINEAR);
            bScaled = true;
        }
    }
    else
    {
        if (image.rows > DEFAULT_DISPLAT_HEIGHT)
        {
            cv::resize(image, image2, cv::Size(image.cols * DEFAULT_DISPLAT_HEIGHT / image.rows, DEFAULT_DISPLAT_HEIGHT), cv::INTER_LINEAR);
            bScaled = true;
        }
    }

    //�Ƿ���Ҫ���ͷֱ��ʽ���ʶ��
    if (default_width != 0)
    {
        if (image.cols > image.rows) //w>h
        {
            if (image.cols > default_width)
                cv::resize(image, image, cv::Size(default_width, image.rows * default_width / image.cols), cv::INTER_LINEAR);
        }
        else
        {
            if (image.rows > default_width)
                cv::resize(image, image, cv::Size(image.cols * default_width / image.rows, default_width), cv::INTER_LINEAR);
        }

        std::cout << "Scaled image size (w*h): " << image.cols << " * " << image.rows << std::endl;
    }

    wt.start();

    //std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image, pr::SEGMENTATION_BASED_METHOD/*pr::SEGMENTATION_FREE_METHOD*/);
    std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image, pr::SEGMENTATION_FREE_METHOD);

    //���ʶ����Ҫ�ĵ�ʱ�䣬��λΪ�룬����Ϊ΢��
    double elapsed = wt.elapsed() * 1000;

    std::cout << "Total Elapsed (ms):" << elapsed << std::endl;

    //�����ŶȽ��дӴ�С�������
    std::sort(res.begin(), res.end(), compare);

    for (auto st : res)
    {
        if (st.confidence > 0.75)
        {
            std::cout << "Plate: " << st.getPlateName() << "\tconfidence: " << st.confidence << std::endl;
            cv::Rect region = st.getPlateRect();

            cv::rectangle(image, cv::Point(region.x, region.y), cv::Point(region.x + region.width, region.y + region.height), cv::Scalar(255, 255, 0), 2);
        }
    }

    if (bScaled)
        cv::imshow("image", image2);
    else
        cv::imshow("image", image);

    cv::waitKey(0);

}


void TEST_CAM(char* pszImagePath)
{
    if (pszImagePath == NULL)
    {
        std::cout << "NULL plate video file path name!" << std::endl;
        return;
    }

    struct _stat fileStat;

    int ret = _stat(pszImagePath, &fileStat);
    if (ret != 0) //�ļ�������
    {
        std::cout << "Video " << pszImagePath << " do not exist!" << std::endl;
        return;
    }

    cv::VideoCapture capture(pszImagePath);
    cv::Mat frame;

    pr::PipelinePR prc(
        "model/cascade.xml",
        "model/HorizonalFinemapping.prototxt",
        "model/HorizonalFinemapping.caffemodel",
        "model/Segmentation.prototxt",
        "model/Segmentation.caffemodel",
        "model/CharacterRecognization.prototxt",
        "model/CharacterRecognization.caffemodel",
        "model/SegmentationFree.prototxt",
        "model/SegmentationFree.caffemodel"
    );

    while (1)
    {
        //��ȡ��һ֡
        if (!capture.read(frame))
        {
            std::cout << "��ȡ��Ƶʧ��" << std::endl;
            exit(1);
        }
        //
        //        cv::transpose(frame,frame);
        //        cv::flip(frame,frame,2);

        //        cv::resize(frame,frame,cv::Size(frame.cols/2,frame.rows/2));

        std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(frame, pr::SEGMENTATION_FREE_METHOD);

        for (auto st : res)
        {
            if (st.confidence > 0.75)
            {
                std::cout << st.getPlateName() << " " << st.confidence << std::endl;
                cv::Rect region = st.getPlateRect();

                cv::rectangle(frame, cv::Point(region.x, region.y), cv::Point(region.x + region.width, region.y + region.height), cv::Scalar(255, 255, 0), 2);
            }
        }

        cv::imshow("image", frame);
        cv::waitKey(1);

    }
}


int main(int argc, char** argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cout << "Usage:" << argv[0] << " plate_image_path [SCALE_MAX_IMAGE_WIDTH|0]" << std::endl;
        return 1;
    }

    int default_width = 0;

    if (argc == 3)
        default_width = atoi(argv[2]);

    TEST_PIPELINE(argv[1], default_width);

    return 0;


}