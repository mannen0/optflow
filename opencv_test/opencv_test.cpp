// opencv_test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>

using namespace cv;
using namespace cv::superres;

int _tmain(int argc, _TCHAR* argv[])
{
	// 動画ファイルの読み込み
	VideoCapture capture = VideoCapture("C:\\Users\\mannen\\Downloads\\lib\\opencv\\sources\\samples\\data\\vtest.avi");
	if (!capture.isOpened()){
		return -1;
	}
	/*while (1){
		Mat frame;
		capture >> frame;
		imshow("window", frame);
	}*/
	
	// TV-L1アルゴリズムによるオプティカルフロー計算オブジェクトの生成
	Ptr<DenseOpticalFlowExt> opticalFlow = superres::createOptFlow_DualTVL1();

	// 前のフレームを保存しておく
	Mat prev;
	capture >> prev;

	while (1)
	{
		// 現在のフレームを保存
		Mat curr;
		capture >> curr;
		
		// オプティカルフローの計算
		Mat flowX, flowY;
		opticalFlow->calc(prev, curr, flowX, flowY);

		// オプティカルフローの可視化（色符号化）
		//  オプティカルフローを極座標に変換（角度は[deg]）
		Mat magnitude, angle;
		cartToPolar(flowX, flowY, magnitude, angle, true);
		//  色相（H）はオプティカルフローの角度
		//  彩度（S）は0～1に正規化したオプティカルフローの大きさ
		//  明度（V）は1
		Mat hsvPlanes[3];
		hsvPlanes[0] = angle;
		normalize(magnitude, magnitude, 0, 1, NORM_MINMAX); // 正規化
		hsvPlanes[1] = magnitude;
		hsvPlanes[2] = Mat::ones(magnitude.size(), CV_32F);
		//  HSVを合成して一枚の画像にする
		Mat hsv;
		merge(hsvPlanes, 3, hsv);
		//  HSVからBGRに変換
		Mat flowBgr;
		cvtColor(hsv, flowBgr, cv::COLOR_HSV2BGR);

		// 表示
		//imshow("input", curr);
		imshow("optical flow", flowBgr);

		// 前のフレームを保存
		prev = curr;
		waitKey(1);
	}
	
	return 0;
}

