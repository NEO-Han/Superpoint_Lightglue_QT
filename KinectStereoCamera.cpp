#include "KinectStereoCamera.h"

KinectStereoCamera::KinectStereoCamera()
{
	/*this->Initialize();*/
}
KinectStereoCamera::~KinectStereoCamera()
{
    this->Close();
}
void KinectStereoCamera::Initialize()
{
    // ��ʼ����һ�� Kinect
    HRESULT hr = GetDefaultKinectSensor(&this->sensor1);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize first Kinect sensor." << std::endl;
        return;
    }

    hr = sensor1->Open();
    if (FAILED(hr)) {
        std::cerr << "Failed to open first Kinect sensor." << std::endl;
        return;
    }

    // ��ʼ���ڶ��� Kinect
    hr = GetDefaultKinectSensor(&this->sensor2);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize second Kinect sensor." << std::endl;
        return;
    }

    hr = sensor2->Open();
    if (FAILED(hr)) {
        std::cerr << "Failed to open second Kinect sensor." << std::endl;
        return;
    }
}
void KinectStereoCamera::Close() {
    if (this->sensor1) {
        this->sensor1->Close();
        this->sensor1->Release();
        this->sensor1 = nullptr;
    }

    if (this->sensor2) {
        this->sensor2->Close();
        this->sensor2->Release();
        this->sensor2 = nullptr;
    }
}
cv::Mat KinectStereoCamera::GetColorImageFromSensor(IKinectSensor* sensor)
{
    // ��ȡ��ɫ֡
    IColorFrameSource* colorFrameSource;
    HRESULT hr = sensor->get_ColorFrameSource(&colorFrameSource);
    if (FAILED(hr)) {
        std::cerr << "Failed to get color frame source." << std::endl;
        return cv::Mat();
    }

    IColorFrameReader* colorFrameReader;
    hr = colorFrameSource->OpenReader(&colorFrameReader);
    if (FAILED(hr)) {
        std::cerr << "Failed to open color frame reader." << std::endl;
        return cv::Mat();
    }

    // ��ȡ���µĲ�ɫ֡
    IColorFrame* colorFrame;
    hr = colorFrameReader->AcquireLatestFrame(&colorFrame);
    if (SUCCEEDED(hr)) {
        UINT bufferLength;
        RGBQUAD* buffer;

        // ������ɫͼ��֡������
        IFrameDescription* frameDescription;
        hr = colorFrame->get_FrameDescription(&frameDescription);
        if (SUCCEEDED(hr)) {
            int colorWidth, colorHeight;
            frameDescription->get_Width(&colorWidth);
            frameDescription->get_Height(&colorHeight);

            bufferLength = colorWidth * colorHeight * sizeof(RGBQUAD);
            buffer = new RGBQUAD[colorWidth * colorHeight];

            colorFrame->CopyConvertedFrameDataToArray(bufferLength, reinterpret_cast<BYTE*>(buffer), ColorImageFormat_Bgra);

            cv::Mat colorImage(colorHeight, colorWidth, CV_8UC4, buffer); // ��ɫͼ���ʽΪ BGRA

            // ͼ������
            cv::Mat resizedImage;
            cv::resize(colorImage, resizedImage, cv::Size(480, 640));

            // �ͷ���Դ
            delete[] buffer;
            frameDescription->Release();
            colorFrame->Release();
            colorFrameReader->Release();
            colorFrameSource->Release();

            return resizedImage;
        }
        else {
            std::cerr << "Failed to get frame description." << std::endl;
            colorFrameReader->Release();
            colorFrameSource->Release();
            return cv::Mat();
        }
    }
    else {
        std::cerr << "Failed to acquire color frame." << std::endl;
        colorFrameReader->Release();
        colorFrameSource->Release();
        return cv::Mat();
    }
}

cv::Mat KinectStereoCamera::GetLeftColorImage()
{
    return this->GetColorImageFromSensor(this->sensor1);
}

cv::Mat KinectStereoCamera::GetRightColorImage()
{
    return this->GetColorImageFromSensor(this->sensor2);
}


