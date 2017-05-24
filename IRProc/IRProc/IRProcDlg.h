
// IRProcDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ImgDLG.h"
#include "InfoDlg.h"
#include "SetDlg.h"

// CIRProcDlg �Ի���
class CIRProcDlg : public CDialogEx
{
// ����
public:
	CIRProcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IRPROC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	CImgDLG ImgDlg;
	CInfoDlg InfoDlg;
	CSetDlg SetDlg;
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
