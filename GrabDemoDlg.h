
// GrabDemoDlg.h : 头文件
//

#pragma once

// 引入头文件
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "afxwin.h"

//ccf相关，静态全局
static TCHAR *ConfigKeyName ={  _T("Camera Name") };
static TCHAR *CompanyKeyName ={  _T("Company Name") };
static TCHAR *ModelKeyName = { _T("Model Name") };
static TCHAR *VicName = { _T("Vic Name") };

// CGrabDemoDlg 对话框
class CGrabDemoDlg : public CDialogEx
{
// 构造
public:
	CGrabDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GRABDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//ccf相关
public:
	CString m_configFile;			//ccf完整路径



public:
	afx_msg void OnGrab();

//自定义函数
public:
	static void XferCallback(SapXferCallbackInfo *pInfo);
	static void CGrabDemoDlg::StartFrameCallback(SapAcqCallbackInfo *pInfo);
	static void AcqCallback(SapXferCallbackInfo *pInfo); //cpp案例
	void UpdateMenu(); //界面控件的整体控制
	void StartFrameCallback();

//自定义变量
public:
	CImage image; //使用图片类
	CRect rectControl; //控件矩形对象	
	//CStatic m_statusWnd; //提示作用，Text控件

	DWORD	m_scaleHeight; 
	DWORD	m_scaleWidth;
	float	m_scaleHeightFactor;
	float	m_scaleWidthFactor;
		 
public:
	//组合
	SapAcquisition	*m_Acq;		//获取
	SapTransfer		*m_Xfer;	//将获取的放到缓存
	SapBuffer		*m_Buffers; //缓存
	SapView			*m_View;	//缓存——控件
	//展示图片
	//CImageWnd		*m_ImageWnd;

	CString  m_appTitle; //窗口标题

public:
	afx_msg void OnBnClickedLoadAcqConfig();
	//为控件添加变量
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
