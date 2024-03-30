#pragma once
#include <vector>
#include <string>
#include <memory>
#include<iostream>
#include "opencv2/stitching.hpp"
#include "opencv2/calib3d.hpp"
#include "common.h"
#include <onnxruntime_cxx_api.h>
using namespace cv::detail;
using namespace cv;
using namespace std;

class  FEATURE_MATCHER_EXPORTS SuperPoint :public Feature2D
{
protected:
	
	std::wstring m_modelPath;
	vector<float> ApplyTransform(const Mat& image, float& mean, float& std);
public:
	SuperPoint(std::wstring modelPath);
	~SuperPoint();
	Ort::Env m_env;
	Ort::SessionOptions m_sessionOptions;
	Ort::Session* m_extractorSession;
	Ort::MemoryInfo* m_memoryInfo;
	Ort::RunOptions run_options;
	//Ort::Value m_inputTensor;
	virtual void detectAndCompute(InputArray image,InputArray mask,
		std::vector<KeyPoint>& keypoints,
		OutputArray descriptors,
		bool useProvidedKeypoints = false);
	virtual void detect(InputArray image,
		std::vector<KeyPoint>& keypoints,
		InputArray mask = noArray());
	virtual void compute(InputArray image,
		std::vector<KeyPoint>& keypoints,
		OutputArray descriptors);


};

