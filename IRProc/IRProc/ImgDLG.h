#pragma once


// CImgDLG �Ի���

class CImgDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CImgDLG)

public:
	CImgDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImgDLG();

// �Ի�������
	enum { IDD = IDD_IMG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	virtual BOOL OnInitDialog();
};
