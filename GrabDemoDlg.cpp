
// GrabDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GrabDemo.h"
#include "GrabDemoDlg.h"
#include "afxdialogex.h"
#include <math.h>  //for floor

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGrabDemoDlg 对话框




CGrabDemoDlg::CGrabDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrabDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGrabDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_WND, m_viewWnd);
	DDX_Control(pDX, IDC_VERT_SCROLLBAR, m_verticalScr);
	DDX_Control(pDX, IDC_HORZ_SCROLLBAR, m_horizontalScr);
}

//消息映射表
BEGIN_MESSAGE_MAP(CGrabDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GRAB, &CGrabDemoDlg::OnGrab)
	ON_BN_CLICKED(IDC_LOAD_ACQ_CONFIG, &CGrabDemoDlg::OnBnClickedLoadAcqConfig)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_SNAP, &CGrabDemoDlg::OnBnClickedSnap)
	ON_BN_CLICKED(IDC_FILE_LOAD, &CGrabDemoDlg::OnBnClickedFileLoad)
	ON_BN_CLICKED(IDC_FILE_SAVE, &CGrabDemoDlg::OnBnClickedFileSave)
	ON_BN_CLICKED(IDC_FREEZE, &CGrabDemoDlg::OnBnClickedFreeze)
END_MESSAGE_MAP()


//回调函数，刷新view
void CGrabDemoDlg::XferCallback(SapXferCallbackInfo *pInfo)
{
	CGrabDemoDlg *pDlg= (CGrabDemoDlg *) pInfo->GetContext();
	pDlg->m_View->Show();
	//SapView *pView=(SapView *) pInfo->GetContext();
	//pView->Show();
}
void CGrabDemoDlg::AcqCallback(SapXferCallbackInfo *pInfo)
{
	SapView *pView= (SapView *) pInfo->GetContext();

	// Resfresh view
	pView->Show();
}

//m_Acq的回调函数
int start,end,duration,fps  = 0;
BOOL firstFrame = TRUE;
void CGrabDemoDlg::StartFrameCallback(SapAcqCallbackInfo *pInfo)
{
	//for the first start of frame record the start time.
	if (firstFrame)
	{
		firstFrame = FALSE;
		start = GetTickCount();
		return;
	}
	end = GetTickCount();
	duration = end - start;
	start = end; 

	//update FPS only if the value changed. 1000 is used because the duration is in ms.
	if (fps != 1000/duration)
	{
		fps = 1000/duration;
		printf("Approximate FPS = %d \r",fps);
	}
}

// CGrabDemoDlg 消息处理程序

BOOL CGrabDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_View=NULL;//重绘
	m_Xfer=NULL;
	m_Buffers=NULL;
	//GetWindowText(m_appTitle); //窗口标题
	////初始化，重绘使用
	//m_Buffers=new SapBuffer(); //默认640*480
	//m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd()); //缓存与控件绑定
	//m_Buffers->Create();
	//m_View->Create();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGrabDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGrabDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//if (m_View!=NULL&&m_View->GetBuffer()!=NULL&&m_View->GetBuffer()->IsMapped())
		if (m_View!=NULL)
		{
			m_View->OnPaint();
		}
		


		//if (!image.IsNull())
		//{
		//	//显示
		//	CDC *pDc = m_viewWnd.GetWindowDC();//获得显示控件的DC
		//	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE); //设置缩放，否则图片显示会失真
		//	image.Draw(pDc->m_hDC, rectControl);   //将图片绘制到Picture控件表示的矩形区域
		//	//释放资源
		//	ReleaseDC(pDc);
		//}

		//
		//if (m_ImageWnd)
		//{
		//	// 展示最新的图片
		//	m_ImageWnd->OnPaint();
		//}
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGrabDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGrabDemoDlg::OnGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("hh"));

	//m_statusWnd.SetWindowText(_T("")); //忽略此功能（提示信息用）

	if(m_Xfer!=NULL) //连续
	{
		m_Xfer->Grab();
		//UpdateMenu();	//忽略此功能（界面控件失效与否逻辑）
	}
	else
	{
		MessageBox(_T("未检测到相机"));
	}
}

//忽略此功能（界面控件失效与否逻辑）
void CGrabDemoDlg::UpdateMenu()
{

}


void CGrabDemoDlg::OnBnClickedLoadAcqConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	//加载ccf界面
	//CAcqConfigDlg dlg(this, NULL);
	//INT_PTR nResponse = dlg.DoModal();

	//if (IDOK==nResponse)
	//{
	//	m_Acq=new SapAcquisition(dlg.GetAcquisition());

	//	m_Buffers=new SapBufferWithTrash(2,m_Acq);
	//	m_Xfer=new SapAcqToBuf(m_Acq,m_Buffers,XferCallback,this);
	//}
	////// 缓存图片——桌面控件，PictureControl(m_viewWnd)
	//m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());

	//////创建所有对象
	//m_Acq->Create();
	//m_Buffers->Create();
	//m_Xfer->Create();
	//m_View->Create();
	//////m_Xfer->Grab(); //实时展示

	////图片展示窗体（CImageWnd类功能很强大），让m_View与3个控件关联，并展示。
	//m_ImageWnd = new CImageWnd(m_View, &m_viewWnd, &m_horizontalScr, &m_verticalScr, this);


	//—————————————————————————————begin	
	//【流程】扫描采集卡→扫描设备（相机）→加载ccf
	BOOL serverFound = FALSE;
	BOOL deviceFound = FALSE;
	BOOL cameraFound = FALSE;
	//扫描采集卡（插槽上插卡了没有），只插了1个	
	int serverCount = SapManager::GetServerCount(); //插槽数，板子上有2个插槽
	for (int serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq) != 0) //如果插卡了，把采集卡名字拿到
		{
			char serverName[30]; //采集卡名
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName)); //获取采集卡名
			printf("%d: %s\n", serverIndex, serverName);
			serverFound = TRUE;
		}
	}
	if (!serverFound)
	{
		MessageBox(_T("没有发现采集卡"));
		return;
	}
	//选择1个采集卡去使用，我们选择Xcelera-CL_PX4_1
	char *acqServerName="Xcelera-CL_PX4_1";
	//扫描采集卡上有几个设备（注意Sapera对设备和相机是有区分的），有4个设备
	int deviceCount = SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcq); //采集卡上有4个设备
	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		char deviceName[50]; //设备名
		SapManager::GetResourceName(acqServerName, SapManager::ResourceAcq, deviceIndex, deviceName, sizeof(deviceName)); //获取设备名
		printf("%d: %s\n", deviceIndex+1, deviceName);
		deviceFound = TRUE;
	}
	if (!deviceFound)
	{		
		MessageBox(_T("采集卡上没有发现设备"));
	}
	//选择1个设备去使用，我们选择CameraLink Base Mono #1
	char *acqDeviceName="CameraLink Base Mono #1";
	//它所对应的设备索引是0
	int acqDeviceIndex = 0;

	//加载ccf
	CFileDialog fileOpen(TRUE, _T("ccf"), NULL,0,_T("Camera Configuration Files(*.ccf)|*.ccf|All Files (*.*)|*.*||"),this);
	INT_PTR ret = fileOpen.DoModal();
	if (ret == IDCANCEL)
		return;
	m_configFile = fileOpen.GetPathName();//ccf文件
	USES_CONVERSION; //CString 转char*
	char* configFileName=T2A(m_configFile.GetBuffer(0));

	//选择ccf文件
	//char *configFileName="C:\\DALSA\\Sapera\\CamFiles\\User\\ss.ccf"; //特别注意，必须“\\”或“/”。

	//************至此，准备完毕：采集卡、设备、配置文件
	SapLocation loc(acqServerName,acqDeviceIndex); //选择的采集卡、选择的设备
	//创建对象
	m_Acq=new SapAcquisition(loc,configFileName);
	m_Buffers=new SapBuffer(1,m_Acq);
	//m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());// 缓存图片——桌面控件，PictureControl(m_viewWnd)
	//m_Xfer=new SapAcqToBuf(m_Acq,m_Buffers,XferCallback,this);
	m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());// 
	m_Xfer=new SapAcqToBuf(m_Acq,m_Buffers,AcqCallback,m_View);
	
	//Creat
	m_Acq->Create();
	//m_Acq->RegisterCallback(SapAcquisition::EventStartOfFrame,StartFrameCallback,NULL);//注册采集的回调函数

	m_Buffers->Create();
	m_Xfer->Create();
	m_View->Create();
	////图片展示窗体（CImageWnd类功能很强大），让m_View与3个控件关联，并展示。
	//m_ImageWnd = new CImageWnd(m_View, &m_viewWnd, &m_horizontalScr, &m_verticalScr, this);

	/////////////////m_Xfer->Grab(); //连续采集
	//———————————————————————————end

	//——————————————————读取ccf里的内容
	//SapAcquisition类的构造函数的第一个参数是SapLocation类型的值，参考《C:\DALSA\Sapera\Help\Sapera++Prog.chm》
	//SapLocation(const char *serverName, int resourceIndex = 0);的第一个参数是板卡
	//所以在new SapAcquisition类前，先把ccf中的板卡拿到
	//char* serverName=new char[30]; //存储板卡名
	//char deviceIndex; //设备序号，GetPrivateProfileString函数只支持char(char*)类型的参数
	//GetPrivateProfileString(_T("Board"), _T("Server Name"), _T("Unknow"), serverName, 30, m_configFile_char); //从ccf拿到板卡名
	//GetPrivateProfileString(_T("Board"), _T("Device Index"), _T("Unknow"), &deviceIndex, 2, m_configFile_char); //从ccf拿到设备序号
	//int deviceIndex=GetPrivateProfileInt(_T("Board"), _T("Device Index"),0,m_configFile_char); //获取设备序号（整型值）


	////读取文件内容如下：
	//TCHAR szServerName[CORSERVER_MAX_STRLEN] = { 0 };//Xcelera-CL_PX4
	//TCHAR szCameraName[_MAX_PATH] = { 0 }; //Default Area Scan 1 tap Mono
	//TCHAR szVicName[_MAX_PATH] = { 0 };//Default Area Scan 1 tap Mono
	//TCHAR szCompanyName[_MAX_PATH] = { 0 };//No Name
	//TCHAR szModelName[_MAX_PATH] = { 0 };//No Name

	//TCHAR *keyName = ConfigKeyName;
	//CString strConfigFile(m_configFile);
	//GetPrivateProfileString(_T("Board"), _T("Server name"), _T("Unknow"), szServerName, CORSERVER_MAX_STRLEN, strConfigFile);
	//GetPrivateProfileString(_T("General"), CString(keyName), _T("Unknown"), szCameraName, _MAX_PATH, strConfigFile);
	//GetPrivateProfileString(_T("General"), CString(CompanyKeyName), _T(""), szCompanyName, _MAX_PATH, strConfigFile);
	//GetPrivateProfileString(_T("General"), CString(ModelKeyName), _T(""), szModelName, _MAX_PATH, strConfigFile);
	//GetPrivateProfileString(_T("General"), CString(VicName), _T(""), szVicName, _MAX_PATH, strConfigFile);

}



//横滚动条
void CGrabDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
//	//如果是横滚动条
//	if (pScrollBar->GetDlgCtrlID() == IDC_HORZ_SCROLLBAR)
//	{
//		//获取当前位置和边界	
//		SCROLLINFO scrollInfo= {0};//GetScrollInfo/SetScrollInfo函数要用到SCROLLINFO结构体
//		scrollInfo.cbSize= sizeof( scrollInfo); //结构体的大小，28
//
//		//scrollInfo.nMin = 0;
//		//scrollInfo.nMax = m_Buffers->GetWidth();
//
//		scrollInfo.fMask= SIF_TRACKPOS | SIF_RANGE | SIF_POS; //指定有效的成员
//
//		int viewWidth  = m_View->GetWidth();
//		int viewHeight = m_View->GetHeight();
//		int pageWidth  = m_View->GetWidth();
//		int pageHeight = m_View->GetHeight();
//		if (m_View->GetBuffer())
//		{
//			viewWidth = m_View->GetBuffer()->GetWidth();
//			viewHeight= m_View->GetBuffer()->GetHeight();
//		}
//		pageWidth = m_View->GetScaleParamsSrc().Width(); 
//		pageHeight= m_View->GetScaleParamsSrc().Height(); 
//
//
//
//
//		GetScrollInfo( SB_HORZ, &scrollInfo);
//		int left= scrollInfo.nPos; //滚动框的位置，nTrackPos是拖动时的实时位置（只读）
//		int hMin= scrollInfo.nMin; //滚动范围的最小值
//		int hMax= scrollInfo.nMax; //滚动范围的最大值
//		//int left=m_horizontalScr.GetScrollPos();
//		//注意case陷阱，left出case就被重置为0，赋值失效。
//		switch (nSBCode)
//		{
//			case SB_THUMBTRACK: //消息：是否拉动滚动条
//				//left = scrollInfo.nTrackPos;break;
//			case SB_THUMBPOSITION: 
//				left = scrollInfo.nTrackPos; //拖动时，滚动框的位置（只能get不能set）
//				break;
//			case SB_LINELEFT:  //消息：左箭头
//				if (left > hMin) 
//					left--; 
//				break;
//			case SB_LINERIGHT: //消息：右箭头
//				if (left < hMax) 
//					left++;
//				break;
//			case SB_PAGELEFT: //消息：左翻页（单击滚动条空白区域），每次左移10
//				left -= 10; 
//				if (left < hMin) 
//					left = hMin; 
//				break; //防止越界
//			case SB_PAGERIGHT: //消息：右翻页（单击滚动条空白区域），每次右移10
//					left += 10; 
//					if (left > hMax) 
//						left = hMax; 
//					break; //防止越界
//								
//		}
//		// scrollInfo.nPos= left;
//		//m_horizontalScr.SetScrollPos(left);
//		//SetScrollInfo( SB_HORZ, &scrollInfo, TRUE);
//		//保存最新位置
//		scrollInfo.nPos= left;
//		SetScrollInfo( SB_HORZ, &scrollInfo, TRUE);
//		if (m_View)
//			m_View->OnHScroll(left);
//
//		//OnPaint(); //重绘，否则消失
//	}
//
//
//
//	//if (pScrollBar->GetDlgCtrlID() == IDC_HORZ_SCROLLBAR)
//	//{
//	//	m_ImageWnd->OnHScroll(nSBCode, nPos);
//	//	OnPaint(); //重绘，否则消失
//	//}
//
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGrabDemoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	//纵滚动条
//	//if (pScrollBar->GetDlgCtrlID() == IDC_VERT_SCROLLBAR)
//	//{
//	//	m_ImageWnd->OnVScroll(nSBCode, nPos);
//	//	OnPaint();
//	//}
//
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

//实时显示坐标情况
void CGrabDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if (!image.IsNull())
//	{
//		CString str=m_appTitle;
//		str+=" "+m_ImageWnd->GetPixelString(point); //获取坐标
//
//		SetWindowText(str); //展示在窗口标题处
//	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CGrabDemoDlg::OnBnClickedSnap()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Xfer!=NULL)
	{
		m_Xfer->Snap(); //抓拍
	}
	else
	{
		MessageBox(_T("未检测到相机"));
	}
}


void CGrabDemoDlg::OnBnClickedFileLoad()
{
	//MFC读图，给buffer
	CFileDialog fileDlg(TRUE, _T("bmp"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
	fileDlg.DoModal();
	//获取图片路径和图片名称
	CString strFilePath = fileDlg.GetPathName(); //"D:\1.jpg"
	//判断路径不为空
	if (strFilePath == _T(""))
	{
		return;
	}
	////控件呈现原始的灰色，防止图片重叠	
	//m_viewWnd.ShowWindow(FALSE);
	//m_viewWnd.ShowWindow(TRUE);

	USES_CONVERSION; //CString 转char*
	char* image=T2A(strFilePath.GetBuffer(0));
	m_Buffers=new SapBuffer(image);//用image初始化buffer对象。否则默认640*480
	m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());
	m_Buffers->Create();//创建缓冲区对象所需的所有资源
	m_View->Create();
	//对于buffer，new、create、load三步，否则展示有问题
	m_Buffers->Load(image,-1);//要加载的缓冲区(或第一个缓冲区)的索引，其中-1等效于当前索引。

	//int w=m_Buffers->GetWidth(); //图的宽高
	//int h=m_Buffers->GetHeight();
	//缩放
	int viewWidth; //控件宽高
	int viewHeight;

	m_View->GetViewArea( &viewWidth, &viewHeight);

	m_scaleWidthFactor = 100.0f*viewWidth/m_View->GetBuffer()->GetWidth(); //4.88
	m_scaleHeightFactor= 100.0f*viewHeight/m_View->GetBuffer()->GetHeight(); //5.3269


	if( m_scaleWidthFactor < m_scaleHeightFactor)
	{
		m_scaleHeightFactor= m_scaleWidthFactor; //取小的缩放数据
	}
	else
	{
		m_scaleWidthFactor= m_scaleHeightFactor;
	}
	//用小的缩放数据重新计算大小
	m_scaleWidth = (DWORD)floor(m_View->GetBuffer()->GetWidth()  * m_scaleWidthFactor/100 + 0.5f);
	m_scaleHeight= (DWORD)floor(m_View->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);
	//重新计算缩放倍数
	m_scaleWidthFactor = 100.0f * m_scaleWidth / m_View->GetBuffer()->GetWidth();
	m_scaleHeightFactor= 100.0f * m_scaleHeight / m_View->GetBuffer()->GetHeight();

	m_View->SetScalingMode( m_scaleWidthFactor/100.0f, m_scaleHeightFactor/100.0f);
	//控件呈现原始的灰色，防止图片重叠	
	m_viewWnd.ShowWindow(FALSE);
	m_viewWnd.ShowWindow(TRUE);

	//使用官方界面读图
	////读图前m_Buffers对象必须存在，先点击“加载ccf”按钮即可。
	//CLoadSaveDlg dlg(this,m_Buffers,TRUE); //Sapera官方库,FALSE存取、TRUE读取
	//if (IDOK==dlg.DoModal())
	//{
	//	int w=m_Buffers->GetWidth(); //图的宽高
	//	int h=m_Buffers->GetHeight();

	//	int viewWidth; //控件宽高
	//	int viewHeight;

	//	m_View->GetViewArea( &viewWidth, &viewHeight);

	//	m_scaleWidthFactor = 100.0f * viewWidth/m_View->GetBuffer()->GetWidth(); //4.88
	//	m_scaleHeightFactor= 100.0f * viewHeight/m_View->GetBuffer()->GetHeight(); //5.3269


	//	if( m_scaleWidthFactor < m_scaleHeightFactor)
	//	{
	//		m_scaleHeightFactor= m_scaleWidthFactor; //取小的缩放数据
	//	}
	//	else
	//	{
	//		m_scaleWidthFactor= m_scaleHeightFactor;
	//	}
	//	//用小的缩放数据重新计算大小
	//	m_scaleWidth = (DWORD)floor(m_View->GetBuffer()->GetWidth()  * m_scaleWidthFactor/100 + 0.5f);
	//	m_scaleHeight= (DWORD)floor(m_View->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);
	//	//重新计算缩放倍数
	//	m_scaleWidthFactor = 100.0f * m_scaleWidth / m_View->GetBuffer()->GetWidth();
	//	m_scaleHeightFactor= 100.0f * m_scaleHeight / m_View->GetBuffer()->GetHeight();
	//	m_View->SetScalingMode( m_scaleWidthFactor/100.0f, m_scaleHeightFactor/100.0f);
	//}


	//// **************** 纯MFC获取图片
	//CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
 //   fileDlg.DoModal();
	////获取图片路径和图片名称
 //   CString strFilePath = fileDlg.GetPathName(); //"D:\1.jpg"
	////判断路径不为空
 //   if (strFilePath == _T(""))
 //   {
 //       return;
 //   }
	//if (!image.IsNull()) //防止再次加载报错
	//{
	//	image.Destroy();
	//}
 //   image.Load(strFilePath); //装载路径下图片信息到图片类

	////******************* 图片缩放适应控件大小
 //   m_viewWnd.GetClientRect(&rectControl);	//控件区域
	////图片不缩放
	////int imageWidth = image.GetWidth();
 //   //int imageHeight = image.GetHeight();
	////rectControl = CRect(rectControl.TopLeft(), CSize(imageWidth, imageHeight)); //显示未缩放原图，超出控件区域不显示
	//OnPaint();//重绘
}



void CGrabDemoDlg::OnBnClickedFileSave()
{
	if (m_Buffers!=NULL)
	{
		CFileDialog dlg(FALSE,_T("bmp"),_T("untitled.bmp"),NULL,_T("位图文件|*.bmp|JPEG 图像文件|*.jpg|PNG 图像文件|*.png||"));
		if ( IDOK != dlg.DoModal())
			return;
		// 文件名
		CString strFileName;
		strFileName = dlg.m_ofn.lpstrFile;// 如"D:\1.bmp"
		USES_CONVERSION; //CString 转char*
		char* image=T2A(strFileName.GetBuffer(0));
		m_Buffers->Save(image,"-format bmp");
	}
	else
	{
		MessageBox(_T("还未加载图片"));
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	//if (image.IsNull()) 
	//{
	//	MessageBox(_T("你还没有打开一个要保存的图像文件！"));
	//	return;
	//}
	//CString strFilter;
	//strFilter = _T("位图文件|*.bmp|JPEG 图像文件|*.jpg|PNG 图像文件|*.png||");
	//CFileDialog dlg(FALSE,_T("bmp"),_T("untitled.bmp"),NULL,strFilter);
	//if ( IDOK != dlg.DoModal())
	//	return;

	//// 文件名
	//CString strFileName;
	//strFileName = dlg.m_ofn.lpstrFile;// 如"D:\1.jpg"

	//// 图像保存
	//HRESULT hResult = image.Save(strFileName);
	//if (FAILED(hResult))
	//	MessageBox(_T("保存图像文件失败！"));
}


void CGrabDemoDlg::OnBnClickedFreeze()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Xfer!=NULL)
	{
		m_Xfer->Freeze(); //暂停
	}
	else
	{
		MessageBox(_T("未检测到相机"));
	}
}
