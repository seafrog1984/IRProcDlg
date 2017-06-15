#pragma once

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>



using namespace std;
using namespace cv;
// CImgDLG 对话框

class CImgDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CImgDLG)

public:
	CImgDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImgDLG();

// 对话框数据
	enum { IDD = IDD_IMG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	virtual BOOL OnInitDialog();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	unsigned short *ori;
	afx_msg void OnBnClickedShow();
	BOOL m_showTemper;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void rot90(Mat src, Mat& tmpimg);
	void rot90RGB(Mat src, Mat& tmpimg);
};
