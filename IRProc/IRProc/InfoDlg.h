#pragma once


// CInfoDlg �Ի���

class CInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoDlg();

// �Ի�������
	enum { IDD = IDD_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
