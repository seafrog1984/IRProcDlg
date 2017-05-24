#pragma once


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
};
