
// GrabDemoDlg.h : ͷ�ļ�
//

#pragma once

// ����ͷ�ļ�
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "afxwin.h"

//ccf��أ���̬ȫ��
static TCHAR *ConfigKeyName ={  _T("Camera Name") };
static TCHAR *CompanyKeyName ={  _T("Company Name") };
static TCHAR *ModelKeyName = { _T("Model Name") };
static TCHAR *VicName = { _T("Vic Name") };

// CGrabDemoDlg �Ի���
class CGrabDemoDlg : public CDialogEx
{
// ����
public:
	CGrabDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GRABDEMO_DIALOG };

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

//ccf���
public:
	CString m_configFile;			//ccf����·��



public:
	afx_msg void OnGrab();

//�Զ��庯��
public:
	static void XferCallback(SapXferCallbackInfo *pInfo);
	static void CGrabDemoDlg::StartFrameCallback(SapAcqCallbackInfo *pInfo);
	static void AcqCallback(SapXferCallbackInfo *pInfo); //cpp����
	void UpdateMenu(); //����ؼ����������
	void StartFrameCallback();

//�Զ������
public:
	CImage image; //ʹ��ͼƬ��
	CRect rectControl; //�ؼ����ζ���	
	//CStatic m_statusWnd; //��ʾ���ã�Text�ؼ�

	DWORD	m_scaleHeight; 
	DWORD	m_scaleWidth;
	float	m_scaleHeightFactor;
	float	m_scaleWidthFactor;
		 
public:
	//���
	SapAcquisition	*m_Acq;		//��ȡ
	SapTransfer		*m_Xfer;	//����ȡ�ķŵ�����
	SapBuffer		*m_Buffers; //����
	SapView			*m_View;	//���桪���ؼ�
	//չʾͼƬ
	//CImageWnd		*m_ImageWnd;

	CString  m_appTitle; //���ڱ���

public:
	afx_msg void OnBnClickedLoadAcqConfig();
	//Ϊ�ؼ���ӱ���
	CStatic m_viewWnd; //PictureControl
	CScrollBar m_verticalScr;
	CScrollBar m_horizontalScr;


	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSnap();
	afx_msg void OnBnClickedFileLoad();
	afx_msg void OnBnClickedFileSave();
	afx_msg void OnBnClickedFreeze();
};
