// opencv_test.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>

using namespace cv;
using namespace cv::superres;

int _tmain(int argc, _TCHAR* argv[])
{
	// ����t�@�C���̓ǂݍ���
	VideoCapture capture = VideoCapture("C:\\Users\\mannen\\Downloads\\lib\\opencv\\sources\\samples\\data\\vtest.avi");
	if (!capture.isOpened()){
		return -1;
	}
	/*while (1){
		Mat frame;
		capture >> frame;
		imshow("window", frame);
	}*/
	
	// TV-L1�A���S���Y���ɂ��I�v�e�B�J���t���[�v�Z�I�u�W�F�N�g�̐���
	Ptr<DenseOpticalFlowExt> opticalFlow = superres::createOptFlow_DualTVL1();

	// �O�̃t���[����ۑ����Ă���
	Mat prev;
	capture >> prev;

	while (1)
	{
		// ���݂̃t���[����ۑ�
		Mat curr;
		capture >> curr;
		
		// �I�v�e�B�J���t���[�̌v�Z
		Mat flowX, flowY;
		opticalFlow->calc(prev, curr, flowX, flowY);

		// �I�v�e�B�J���t���[�̉����i�F�������j
		//  �I�v�e�B�J���t���[���ɍ��W�ɕϊ��i�p�x��[deg]�j
		Mat magnitude, angle;
		cartToPolar(flowX, flowY, magnitude, angle, true);
		//  �F���iH�j�̓I�v�e�B�J���t���[�̊p�x
		//  �ʓx�iS�j��0�`1�ɐ��K�������I�v�e�B�J���t���[�̑傫��
		//  ���x�iV�j��1
		Mat hsvPlanes[3];
		hsvPlanes[0] = angle;
		normalize(magnitude, magnitude, 0, 1, NORM_MINMAX); // ���K��
		hsvPlanes[1] = magnitude;
		hsvPlanes[2] = Mat::ones(magnitude.size(), CV_32F);
		//  HSV���������Ĉꖇ�̉摜�ɂ���
		Mat hsv;
		merge(hsvPlanes, 3, hsv);
		//  HSV����BGR�ɕϊ�
		Mat flowBgr;
		cvtColor(hsv, flowBgr, cv::COLOR_HSV2BGR);

		// �\��
		//imshow("input", curr);
		imshow("optical flow", flowBgr);

		// �O�̃t���[����ۑ�
		prev = curr;
		waitKey(1);
	}
	
	return 0;
}

