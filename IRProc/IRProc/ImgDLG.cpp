// ImgDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "IRProc.h"
#include "ImgDLG.h"
#include "afxdialogex.h"
#include <fstream>

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

#define F_TO_C(x)  ((((x)-32.0) * 5.0) / 9.0)
#define C_TO_F(x)    ((32.0) + ((x) * (9.0/5.0)))

Mat img;
Mat dst;
Mat g_dst;
bool showTemper = 0;
float topvalue = (int)(F_TO_C(130) * 100.0);
float bottomvalue = (int)(F_TO_C(50) * 100.0);
long width = 320, height = 240;

// CImgDLG 对话框

IMPLEMENT_DYNAMIC(CImgDLG, CDialogEx)

CImgDLG::CImgDLG(CWnd* pParent /*=NULL*/)
: CDialogEx(CImgDLG::IDD, pParent)
, m_showTemper(FALSE)
{

}

CImgDLG::~CImgDLG()
{
}

void CImgDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_TEMPER, m_showTemper);
}


BEGIN_MESSAGE_MAP(CImgDLG, CDialogEx)
	ON_BN_CLICKED(IDC_OPEN, &CImgDLG::OnBnClickedOpen)
	//	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_SHOW, &CImgDLG::OnBnClickedShow)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CImgDLG 消息处理程序


void CImgDLG::OnBnClickedOpen()
{
	// TODO:  在此添加控件通知处理程序代码

	unsigned short *tmp = (unsigned short*)malloc(320 * 240 * sizeof(short));
	ori = tmp;
	ifstream fin("b.dat");

	for (int i = 0; i < width*height; i++)
	{
		fin >> *(tmp + i);
		if (*(tmp + i) == ' ')
			i--;
	}

	fin.close();

	Mat src(height, width, CV_8UC1, tmp);
	//namedWindow("src");
	//imshow("src", src);

	//	img.create(height, width, CV_16UC1, tmp);
	src.copyTo(img);
	//	cvtColor(src,img , CV_BGR2GRAY);

	Mat g_src(img);
	dst.create(img.size(), CV_8UC3);
	g_dst.create(img.size(), CV_8UC1);
	//cvtColor(img, g_src, CV_BGR2GRAY);

	if (tmp)
	{


		//	SetDlgItemText(IDC_CENTER_TEMP, buf);

		// highly non-optimal conversion from 16-bit to 8-bit

		// Values are in degrees C * 100.0
		// So, 1 deg C would == 100
		// My mind doesn't work well in deg C so I have a F_TO_C conversion
		//
		// topvalue represents the highest temperature value we want to display
		// bottomvalue represents the lowest temperature value we want to display
		//

		bool auto_range = true; // set to false to use manual range...

		// Manual range values



		int i, end, col, row;
		float value;
		unsigned short *src = (unsigned short *)tmp;
		//unsigned char  *dest = (unsigned char  *)m_ImageData8;
		unsigned char displayValue;
		end = width * height;  // image size...

		if (auto_range)
		{
			bottomvalue = 99999;
			topvalue = -99999;
			for (i = 0; i < end; i++)
			{
				value = src[i];
				bottomvalue = min(bottomvalue, value);
				topvalue = max(topvalue, value);
			}
		}

		float range = topvalue - bottomvalue;

		if (range != 0)
		{

			for (i = 0; i < end; i++)
			{
				value = *src++;
				// Limit upper + lower values
				if (value < bottomvalue) value = bottomvalue;
				if (value > topvalue)    value = topvalue;
				// Scale to 0..255 for display
				displayValue = ((value - bottomvalue) * 255) / range;
				float tmp = ((value - bottomvalue) * 255) / range;
				row = i / width;
				col = i%width;

				g_dst.at<uchar>(row, col) = displayValue;

				dst.at<Vec3b>(row, col)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
				dst.at<Vec3b>(row, col)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
				dst.at<Vec3b>(row, col)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);


				/*if ((value - bottomvalue) / range <= 0.25)
				{
				dst.at<Vec3b>(row, col)[0] = 255;
				dst.at<Vec3b>(row, col)[1] = displayValue;
				dst.at<Vec3b>(row, col)[2] = 0;
				}
				else if ((value - bottomvalue) / range <= 0.5)
				{
				dst.at<Vec3b>(row, col)[0] = (range-(value-range/4)*2)/range*255;
				dst.at<Vec3b>(row, col)[1] = 255;
				dst.at<Vec3b>(row, col)[2] = 0;
				}
				else if ((value - bottomvalue) / range <= 0.75)
				{
				dst.at<Vec3b>(row, col)[0] = 0;
				dst.at<Vec3b>(row, col)[1] = 255;
				dst.at<Vec3b>(row, col)[2] = (value-range/2)*2/range*255;
				}
				else
				{
				dst.at<Vec3b>(row, col)[0] = (range - (value - range*3 / 4) * 2) / range * 255;
				dst.at<Vec3b>(row, col)[1] = 0;
				dst.at<Vec3b>(row, col)[2] = 255;
				}
				*/
			}
		}
	}
	//int i, j;
	//for (i = 0; i < dst.rows; i++)
	//{
	//	for (j = 0; j < dst.cols; j++)
	//	{
	//		int tmp = g_src.at<uchar>(i, j);

	//		dst.at<Vec3b>(i, j)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
	//		dst.at<Vec3b>(i, j)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
	//		dst.at<Vec3b>(i, j)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);
	//	}
	//}


	imshow("view", g_dst);

	imshow("color", dst);

	//新建一个名为expanded的Mat容器。高度和img1相同，宽度为两倍  
	//Mat expanded(Size((dst.cols + dst.cols), dst.rows), CV_8UC3);

	//Mat ROI = expanded(Rect(0, 0, dst.cols, dst.rows));

	//Mat ROI1 = expanded(Rect(dst.cols, 0, dst.cols, dst.rows));
	////cout << "ROI1.cols" << ROI1.cols << endl;  
	////cout << "ROI1.rows" << ROI1.rows << endl;  
	//addWeighted(ROI, 0, dst, 1, 0., ROI);

	//addWeighted(ROI1, 0, dst, 1, 0., ROI1);

	//namedWindow("picture");
	//imshow("picture", expanded);


}


BOOL CImgDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	CRect rect;
	GetDlgItem(IDC_PIC)->GetWindowRect(&rect);           //IDC_WAVE_DRAW为Picture Control的ID  
	ScreenToClient(&rect);
	GetDlgItem(IDC_PIC)->MoveWindow(30, 100, 320, 240, true);    //固定Picture Control控件的大小  

	namedWindow("color", WINDOW_AUTOSIZE);
	hWnd = (HWND)cvGetWindowHandle("color");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC2)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	GetDlgItem(IDC_PIC2)->GetWindowRect(&rect);           //IDC_WAVE_DRAW为Picture Control的ID  
	ScreenToClient(&rect);
	GetDlgItem(IDC_PIC2)->MoveWindow(360, 100, 320, 240, true);    //固定Picture Control控件的大小  


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void on_mouse(int event, int x, int y, int flags, void *p)
{
	IplImage *img0, *img1;
	img0 = &IplImage(g_dst);
	img1 = cvCloneImage(img0);
	CvFont font, font2;
	uchar *ptr;
	int tmp;
	float temper;
	char label[1000];
	char label2[1000];
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, 1);
	cvInitFont(&font2, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, 1);
	if (event == CV_EVENT_MOUSEMOVE&&showTemper)
	{
		//	ptr = cvPtr2D(img1, y, x, NULL);	//获取像素的灰度值
		tmp = g_dst.at<uchar>(y, x);
		temper = 25 + tmp / 255.0 * 12.0;
		sprintf_s(label, "Temperature:%.2f)", temper);
		sprintf_s(label2, "Pixel:(%d, %d)", x, y);	//获取坐标点位置
		cvRectangle(img1, cvPoint(x, y - 12), cvPoint(x + 180, y + 20),
			CV_RGB(255, 0, 0), CV_FILLED, 8, 0);
		cvPutText(img1, label, cvPoint(x, y), &font, CV_RGB(255, 255, 255));
		cvPutText(img1, label2, cvPoint(x, y + 20), &font2, CV_RGB(255, 255, 255));
		cvShowImage("view", img1);
	}
}

void CImgDLG::OnBnClickedShow()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_showTemper)
		showTemper = 1;
	else
		showTemper = 0;
	cvSetMouseCallback("view", on_mouse, 0);
	cvSetMouseCallback("color", on_mouse, 0);
}


BOOL CImgDLG::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if ((bottomvalue<topvalue-1)&&bottomvalue>1)
	bottomvalue += zDelta / 120;

	int i, end, col, row;
	float value;
	unsigned short *src = (unsigned short *)ori;
	//unsigned char  *dest = (unsigned char  *)m_ImageData8;
	unsigned char displayValue;
	end = width * height;  // image size...

	float range = topvalue - bottomvalue;

	if (range != 0)
	{

		for (int i = 0; i < end; i++)
		{
			float value = *src++;
			// Limit upper + lower values
			if (value < bottomvalue) value = bottomvalue;
			if (value > topvalue)    value = topvalue;
			// Scale to 0..255 for display
			int displayValue = ((value - bottomvalue) * 255) / range;
			float tmp = ((value - bottomvalue) * 255) / range;
			int row = i / width;
			int col = i%width;


			dst.at<Vec3b>(row, col)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
			dst.at<Vec3b>(row, col)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
			dst.at<Vec3b>(row, col)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);

		}
	}
	
	imshow("color", dst);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
