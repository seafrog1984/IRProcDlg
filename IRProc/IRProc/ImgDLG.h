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
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	unsigned short *ori;
	afx_msg void OnBnClickedShow();
	BOOL m_showTemper;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
