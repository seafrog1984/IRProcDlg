// ImgDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "IRProc.h"
#include "ImgDLG.h"
#include "afxdialogex.h"
#include <fstream>


using namespace std;
using namespace cv;

#define F_TO_C(x)  ((((x)-32.0) * 5.0) / 9.0)
#define C_TO_F(x)    ((32.0) + ((x) * (9.0/5.0)))
#define HEIGHT 240
#define WIDTH 320

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
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, _T("data files (*.dat) |*.dat | All Files (*.*) |*.*||"), NULL);                            // 选项图片的约定     
	dlg.m_ofn.lpstrTitle = _T("Open Image");// 打开文件对话框的标题名     
	if (dlg.DoModal() != IDOK)             // 判断是否获得图片         
		return;
	CString m_path = dlg.GetPathName();

	unsigned short *tmp = (unsigned short*)malloc(320 * 240 * sizeof(short));
	ori = tmp;
	ifstream fin((LPSTR)(LPCSTR)m_path);

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

	Mat g_src(img),g_tmpdst,tmpdst;
	tmpdst.create(img.size(), CV_8UC3);
	g_tmpdst.create(img.size(), CV_8UC1);
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

				g_tmpdst.at<uchar>(row, col) = displayValue;

				tmpdst.at<Vec3b>(row, col)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
				tmpdst.at<Vec3b>(row, col)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
				tmpdst.at<Vec3b>(row, col)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);


		
			}
		}
	}

//	Mat g_dstImage2, g_dstImage3, g_dstImage4, tempImage,g_tempImage;
		g_dst.create(Size(g_tmpdst.rows, g_tmpdst.cols), CV_8UC1);
		for (int i = 0; i < g_tmpdst.rows; i++)
		{
			for (int j = 0; j < g_tmpdst.cols; j++)
			{
				g_dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
			}

		}

		dst.create(Size(tmpdst.rows, tmpdst.cols), CV_8UC3);
		for (int i = 0; i < tmpdst.rows; i++)
		{
			for (int j = 0; j < tmpdst.cols; j++)
			{
				dst.at<Vec3b>(j, HEIGHT - 1 - i)[0] = tmpdst.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(j, HEIGHT - 1 - i)[1] = tmpdst.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(j, HEIGHT - 1 - i)[2] = tmpdst.at<Vec3b>(i, j)[2];
			}

		}

//	rot90(g_tmpdst, g_dst);
//	rot90RGB(tmpdst, dst);

	/*pyrUp(tempImage, dst, Size(tempImage.cols * 2, tempImage.rows * 2));
	pyrUp(g_tempImage, g_dst, Size(tempImage.cols * 2, tempImage.rows * 2));*/
	//pyrDown(tempImage, g_dstImage4, Size(tempImage.cols / 2, tempImage.rows / 2));

	
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
	GetDlgItem(IDC_PIC)->MoveWindow(30, 100, 240, 320, true);    //固定Picture Control控件的大小  

	namedWindow("color", WINDOW_AUTOSIZE);
	hWnd = (HWND)cvGetWindowHandle("color");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC2)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	GetDlgItem(IDC_PIC2)->GetWindowRect(&rect);           //IDC_WAVE_DRAW为Picture Control的ID  
	ScreenToClient(&rect);
	GetDlgItem(IDC_PIC2)->MoveWindow(320, 100, 240, 320, true);    //固定Picture Control控件的大小  


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
	static Point pre_pt = (-1, -1);//初始坐标  
	static Point cur_pt = (-1, -1);//实时坐标  
	char temp[50];
	Mat img, imgt;

	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		g_dst.copyTo(img);//将原始图片复制到img中  
		sprintf_s(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
		imshow("view", img);
	}
	//else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	//{

	//	g_dst.copyTo(imgt);//将img复制到临时图像tmp上，用于显示实时坐标  
	//	sprintf_s(temp, "(%d,%d)", x, y);
	//	cur_pt = Point(x, y);
	//	putText(imgt, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
	//	imshow("view",imgt);
	//}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		g_dst.copyTo(imgt);
		sprintf_s(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(imgt, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(imgt, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		imshow("view", imgt);
	}
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{
		g_dst.copyTo(img);
		
		cur_pt = Point(x, y);
		double max=0, min=9999, aver, dif,sum=0;

		for (int i = pre_pt.y; i < cur_pt.y; i++)
		{
			for (int j = pre_pt.x; j < cur_pt.x; j++)
			{
				double value= g_dst.at<uchar>(i, j);
				sum += value;
				if (value>max)
				{
					max = value;
				}
				if (value < min)
				{
					min = value;
				}
			}

		}

		aver = sum / ((cur_pt.x - pre_pt.x)*(cur_pt.y - pre_pt.y));
		max = 25 + max / 255.0 * 12.0;
		min = 25 + min / 255.0 * 12.0;
		aver = 25 + aver / 255.0 * 12.0;
		
	//	circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		sprintf_s(temp, "(%.2lf,%.2lf,%.2lf)", max, min,aver);
		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上

		imshow("view", img);
		//img.copyTo(imgt);
		//截取矩形包围的图像，并保存到dst中  
	/*	int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);
		if (width == 0 || height == 0)
		{
			printf("width == 0 || height == 0");
			return;
		}
		dst = img(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
		namedWindow("dst");
		imshow("dst", dst);
		waitKey(0);*/
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

	Mat tmpdst;
	tmpdst.create(img.size(), CV_8UC3);

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


			tmpdst.at<Vec3b>(row, col)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
			tmpdst.at<Vec3b>(row, col)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
			tmpdst.at<Vec3b>(row, col)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);

		}
	}
	dst.create(Size(tmpdst.rows, tmpdst.cols), CV_8UC3);
	for (int i = 0; i < tmpdst.rows; i++)
	{
		for (int j = 0; j < tmpdst.cols; j++)
		{
			dst.at<Vec3b>(j, HEIGHT - 1 - i)[0] = tmpdst.at<Vec3b>(i, j)[0];
			dst.at<Vec3b>(j, HEIGHT - 1 - i)[1] = tmpdst.at<Vec3b>(i, j)[1];
			dst.at<Vec3b>(j, HEIGHT - 1 - i)[2] = tmpdst.at<Vec3b>(i, j)[2];
		}

	}
	imshow("color", dst);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
void CImgDLG::rot90(Mat src, Mat& tmpimg)
{
	tmpimg.create(Size(g_dst.rows, g_dst.cols), CV_8UC1);
	for (int i = 0; i < g_dst.rows; i++)
	{
		for (int j = 0; j < g_dst.cols; j++)
		{
			tmpimg.at<uchar>(j, HEIGHT - 1 - i) = g_dst.at<uchar>(i, j);
		}

	}
}

void CImgDLG::rot90RGB(Mat src, Mat& tmpimg)
{
	tmpimg.create(Size(src.rows, src.cols), CV_8UC3);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			tmpimg.at<Vec3b>(j, HEIGHT - 1 - i)[0] = src.at<Vec3b>(i, j)[0];
			tmpimg.at<Vec3b>(j, HEIGHT - 1 - i)[1] = src.at<Vec3b>(i, j)[1];
			tmpimg.at<Vec3b>(j, HEIGHT - 1 - i)[2] = src.at<Vec3b>(i, j)[2];
		}

	}
}