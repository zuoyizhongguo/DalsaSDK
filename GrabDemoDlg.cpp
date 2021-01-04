
// GrabDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GrabDemo.h"
#include "GrabDemoDlg.h"
#include "afxdialogex.h"
#include <math.h>  //for floor

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CGrabDemoDlg �Ի���




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

//��Ϣӳ���
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


//�ص�������ˢ��view
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

//m_Acq�Ļص�����
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

// CGrabDemoDlg ��Ϣ�������

BOOL CGrabDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_View=NULL;//�ػ�
	m_Xfer=NULL;
	m_Buffers=NULL;
	//GetWindowText(m_appTitle); //���ڱ���
	////��ʼ�����ػ�ʹ��
	//m_Buffers=new SapBuffer(); //Ĭ��640*480
	//m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd()); //������ؼ���
	//m_Buffers->Create();
	//m_View->Create();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGrabDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
		//	//��ʾ
		//	CDC *pDc = m_viewWnd.GetWindowDC();//�����ʾ�ؼ���DC
		//	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE); //�������ţ�����ͼƬ��ʾ��ʧ��
		//	image.Draw(pDc->m_hDC, rectControl);   //��ͼƬ���Ƶ�Picture�ؼ���ʾ�ľ�������
		//	//�ͷ���Դ
		//	ReleaseDC(pDc);
		//}

		//
		//if (m_ImageWnd)
		//{
		//	// չʾ���µ�ͼƬ
		//	m_ImageWnd->OnPaint();
		//}
	}

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGrabDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGrabDemoDlg::OnGrab()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(_T("hh"));

	//m_statusWnd.SetWindowText(_T("")); //���Դ˹��ܣ���ʾ��Ϣ�ã�

	if(m_Xfer!=NULL) //����
	{
		m_Xfer->Grab();
		//UpdateMenu();	//���Դ˹��ܣ�����ؼ�ʧЧ����߼���
	}
	else
	{
		MessageBox(_T("δ��⵽���"));
	}
}

//���Դ˹��ܣ�����ؼ�ʧЧ����߼���
void CGrabDemoDlg::UpdateMenu()
{

}


void CGrabDemoDlg::OnBnClickedLoadAcqConfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ccf����
	//CAcqConfigDlg dlg(this, NULL);
	//INT_PTR nResponse = dlg.DoModal();

	//if (IDOK==nResponse)
	//{
	//	m_Acq=new SapAcquisition(dlg.GetAcquisition());

	//	m_Buffers=new SapBufferWithTrash(2,m_Acq);
	//	m_Xfer=new SapAcqToBuf(m_Acq,m_Buffers,XferCallback,this);
	//}
	////// ����ͼƬ��������ؼ���PictureControl(m_viewWnd)
	//m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());

	//////�������ж���
	//m_Acq->Create();
	//m_Buffers->Create();
	//m_Xfer->Create();
	//m_View->Create();
	//////m_Xfer->Grab(); //ʵʱչʾ

	////ͼƬչʾ���壨CImageWnd�๦�ܺ�ǿ�󣩣���m_View��3���ؼ���������չʾ��
	//m_ImageWnd = new CImageWnd(m_View, &m_viewWnd, &m_horizontalScr, &m_verticalScr, this);


	//����������������������������������������������������������begin	
	//�����̡�ɨ��ɼ�����ɨ���豸�������������ccf
	BOOL serverFound = FALSE;
	BOOL deviceFound = FALSE;
	BOOL cameraFound = FALSE;
	//ɨ��ɼ���������ϲ忨��û�У���ֻ����1��	
	int serverCount = SapManager::GetServerCount(); //���������������2�����
	for (int serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq) != 0) //����忨�ˣ��Ѳɼ��������õ�
		{
			char serverName[30]; //�ɼ�����
			SapManager::GetServerName(serverIndex, serverName, sizeof(serverName)); //��ȡ�ɼ�����
			printf("%d: %s\n", serverIndex, serverName);
			serverFound = TRUE;
		}
	}
	if (!serverFound)
	{
		MessageBox(_T("û�з��ֲɼ���"));
		return;
	}
	//ѡ��1���ɼ���ȥʹ�ã�����ѡ��Xcelera-CL_PX4_1
	char *acqServerName="Xcelera-CL_PX4_1";
	//ɨ��ɼ������м����豸��ע��Sapera���豸������������ֵģ�����4���豸
	int deviceCount = SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcq); //�ɼ�������4���豸
	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		char deviceName[50]; //�豸��
		SapManager::GetResourceName(acqServerName, SapManager::ResourceAcq, deviceIndex, deviceName, sizeof(deviceName)); //��ȡ�豸��
		printf("%d: %s\n", deviceIndex+1, deviceName);
		deviceFound = TRUE;
	}
	if (!deviceFound)
	{		
		MessageBox(_T("�ɼ�����û�з����豸"));
	}
	//ѡ��1���豸ȥʹ�ã�����ѡ��CameraLink Base Mono #1
	char *acqDeviceName="CameraLink Base Mono #1";
	//������Ӧ���豸������0
	int acqDeviceIndex = 0;

	//����ccf
	CFileDialog fileOpen(TRUE, _T("ccf"), NULL,0,_T("Camera Configuration Files(*.ccf)|*.ccf|All Files (*.*)|*.*||"),this);
	INT_PTR ret = fileOpen.DoModal();
	if (ret == IDCANCEL)
		return;
	m_configFile = fileOpen.GetPathName();//ccf�ļ�
	USES_CONVERSION; //CString תchar*
	char* configFileName=T2A(m_configFile.GetBuffer(0));

	//ѡ��ccf�ļ�
	//char *configFileName="C:\\DALSA\\Sapera\\CamFiles\\User\\ss.ccf"; //�ر�ע�⣬���롰\\����/����

	//************���ˣ�׼����ϣ��ɼ������豸�������ļ�
	SapLocation loc(acqServerName,acqDeviceIndex); //ѡ��Ĳɼ�����ѡ����豸
	//��������
	m_Acq=new SapAcquisition(loc,configFileName);
	m_Buffers=new SapBuffer(1,m_Acq);
	//m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());// ����ͼƬ��������ؼ���PictureControl(m_viewWnd)
	//m_Xfer=new SapAcqToBuf(m_Acq,m_Buffers,XferCallback,this);
	m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());// 
	m_Xfer=new SapAcqToBuf(m_Acq,m_Buffers,AcqCallback,m_View);
	
	//Creat
	m_Acq->Create();
	//m_Acq->RegisterCallback(SapAcquisition::EventStartOfFrame,StartFrameCallback,NULL);//ע��ɼ��Ļص�����

	m_Buffers->Create();
	m_Xfer->Create();
	m_View->Create();
	////ͼƬչʾ���壨CImageWnd�๦�ܺ�ǿ�󣩣���m_View��3���ؼ���������չʾ��
	//m_ImageWnd = new CImageWnd(m_View, &m_viewWnd, &m_horizontalScr, &m_verticalScr, this);

	/////////////////m_Xfer->Grab(); //�����ɼ�
	//������������������������������������������������������end

	//��������������������������������������ȡccf�������
	//SapAcquisition��Ĺ��캯���ĵ�һ��������SapLocation���͵�ֵ���ο���C:\DALSA\Sapera\Help\Sapera++Prog.chm��
	//SapLocation(const char *serverName, int resourceIndex = 0);�ĵ�һ�������ǰ忨
	//������new SapAcquisition��ǰ���Ȱ�ccf�еİ忨�õ�
	//char* serverName=new char[30]; //�洢�忨��
	//char deviceIndex; //�豸��ţ�GetPrivateProfileString����ֻ֧��char(char*)���͵Ĳ���
	//GetPrivateProfileString(_T("Board"), _T("Server Name"), _T("Unknow"), serverName, 30, m_configFile_char); //��ccf�õ��忨��
	//GetPrivateProfileString(_T("Board"), _T("Device Index"), _T("Unknow"), &deviceIndex, 2, m_configFile_char); //��ccf�õ��豸���
	//int deviceIndex=GetPrivateProfileInt(_T("Board"), _T("Device Index"),0,m_configFile_char); //��ȡ�豸��ţ�����ֵ��


	////��ȡ�ļ��������£�
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



//�������
void CGrabDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
//	//����Ǻ������
//	if (pScrollBar->GetDlgCtrlID() == IDC_HORZ_SCROLLBAR)
//	{
//		//��ȡ��ǰλ�úͱ߽�	
//		SCROLLINFO scrollInfo= {0};//GetScrollInfo/SetScrollInfo����Ҫ�õ�SCROLLINFO�ṹ��
//		scrollInfo.cbSize= sizeof( scrollInfo); //�ṹ��Ĵ�С��28
//
//		//scrollInfo.nMin = 0;
//		//scrollInfo.nMax = m_Buffers->GetWidth();
//
//		scrollInfo.fMask= SIF_TRACKPOS | SIF_RANGE | SIF_POS; //ָ����Ч�ĳ�Ա
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
//		int left= scrollInfo.nPos; //�������λ�ã�nTrackPos���϶�ʱ��ʵʱλ�ã�ֻ����
//		int hMin= scrollInfo.nMin; //������Χ����Сֵ
//		int hMax= scrollInfo.nMax; //������Χ�����ֵ
//		//int left=m_horizontalScr.GetScrollPos();
//		//ע��case���壬left��case�ͱ�����Ϊ0����ֵʧЧ��
//		switch (nSBCode)
//		{
//			case SB_THUMBTRACK: //��Ϣ���Ƿ�����������
//				//left = scrollInfo.nTrackPos;break;
//			case SB_THUMBPOSITION: 
//				left = scrollInfo.nTrackPos; //�϶�ʱ���������λ�ã�ֻ��get����set��
//				break;
//			case SB_LINELEFT:  //��Ϣ�����ͷ
//				if (left > hMin) 
//					left--; 
//				break;
//			case SB_LINERIGHT: //��Ϣ���Ҽ�ͷ
//				if (left < hMax) 
//					left++;
//				break;
//			case SB_PAGELEFT: //��Ϣ����ҳ�������������հ����򣩣�ÿ������10
//				left -= 10; 
//				if (left < hMin) 
//					left = hMin; 
//				break; //��ֹԽ��
//			case SB_PAGERIGHT: //��Ϣ���ҷ�ҳ�������������հ����򣩣�ÿ������10
//					left += 10; 
//					if (left > hMax) 
//						left = hMax; 
//					break; //��ֹԽ��
//								
//		}
//		// scrollInfo.nPos= left;
//		//m_horizontalScr.SetScrollPos(left);
//		//SetScrollInfo( SB_HORZ, &scrollInfo, TRUE);
//		//��������λ��
//		scrollInfo.nPos= left;
//		SetScrollInfo( SB_HORZ, &scrollInfo, TRUE);
//		if (m_View)
//			m_View->OnHScroll(left);
//
//		//OnPaint(); //�ػ棬������ʧ
//	}
//
//
//
//	//if (pScrollBar->GetDlgCtrlID() == IDC_HORZ_SCROLLBAR)
//	//{
//	//	m_ImageWnd->OnHScroll(nSBCode, nPos);
//	//	OnPaint(); //�ػ棬������ʧ
//	//}
//
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGrabDemoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	//�ݹ�����
//	//if (pScrollBar->GetDlgCtrlID() == IDC_VERT_SCROLLBAR)
//	//{
//	//	m_ImageWnd->OnVScroll(nSBCode, nPos);
//	//	OnPaint();
//	//}
//
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

//ʵʱ��ʾ�������
void CGrabDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	if (!image.IsNull())
//	{
//		CString str=m_appTitle;
//		str+=" "+m_ImageWnd->GetPixelString(point); //��ȡ����
//
//		SetWindowText(str); //չʾ�ڴ��ڱ��⴦
//	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CGrabDemoDlg::OnBnClickedSnap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Xfer!=NULL)
	{
		m_Xfer->Snap(); //ץ��
	}
	else
	{
		MessageBox(_T("δ��⵽���"));
	}
}


void CGrabDemoDlg::OnBnClickedFileLoad()
{
	//MFC��ͼ����buffer
	CFileDialog fileDlg(TRUE, _T("bmp"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
	fileDlg.DoModal();
	//��ȡͼƬ·����ͼƬ����
	CString strFilePath = fileDlg.GetPathName(); //"D:\1.jpg"
	//�ж�·����Ϊ��
	if (strFilePath == _T(""))
	{
		return;
	}
	////�ؼ�����ԭʼ�Ļ�ɫ����ֹͼƬ�ص�	
	//m_viewWnd.ShowWindow(FALSE);
	//m_viewWnd.ShowWindow(TRUE);

	USES_CONVERSION; //CString תchar*
	char* image=T2A(strFilePath.GetBuffer(0));
	m_Buffers=new SapBuffer(image);//��image��ʼ��buffer���󡣷���Ĭ��640*480
	m_View=new SapView(m_Buffers,m_viewWnd.GetSafeHwnd());
	m_Buffers->Create();//�������������������������Դ
	m_View->Create();
	//����buffer��new��create��load����������չʾ������
	m_Buffers->Load(image,-1);//Ҫ���صĻ�����(���һ��������)������������-1��Ч�ڵ�ǰ������

	//int w=m_Buffers->GetWidth(); //ͼ�Ŀ��
	//int h=m_Buffers->GetHeight();
	//����
	int viewWidth; //�ؼ����
	int viewHeight;

	m_View->GetViewArea( &viewWidth, &viewHeight);

	m_scaleWidthFactor = 100.0f*viewWidth/m_View->GetBuffer()->GetWidth(); //4.88
	m_scaleHeightFactor= 100.0f*viewHeight/m_View->GetBuffer()->GetHeight(); //5.3269


	if( m_scaleWidthFactor < m_scaleHeightFactor)
	{
		m_scaleHeightFactor= m_scaleWidthFactor; //ȡС����������
	}
	else
	{
		m_scaleWidthFactor= m_scaleHeightFactor;
	}
	//��С�������������¼����С
	m_scaleWidth = (DWORD)floor(m_View->GetBuffer()->GetWidth()  * m_scaleWidthFactor/100 + 0.5f);
	m_scaleHeight= (DWORD)floor(m_View->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);
	//���¼������ű���
	m_scaleWidthFactor = 100.0f * m_scaleWidth / m_View->GetBuffer()->GetWidth();
	m_scaleHeightFactor= 100.0f * m_scaleHeight / m_View->GetBuffer()->GetHeight();

	m_View->SetScalingMode( m_scaleWidthFactor/100.0f, m_scaleHeightFactor/100.0f);
	//�ؼ�����ԭʼ�Ļ�ɫ����ֹͼƬ�ص�	
	m_viewWnd.ShowWindow(FALSE);
	m_viewWnd.ShowWindow(TRUE);

	//ʹ�ùٷ������ͼ
	////��ͼǰm_Buffers���������ڣ��ȵ��������ccf����ť���ɡ�
	//CLoadSaveDlg dlg(this,m_Buffers,TRUE); //Sapera�ٷ���,FALSE��ȡ��TRUE��ȡ
	//if (IDOK==dlg.DoModal())
	//{
	//	int w=m_Buffers->GetWidth(); //ͼ�Ŀ��
	//	int h=m_Buffers->GetHeight();

	//	int viewWidth; //�ؼ����
	//	int viewHeight;

	//	m_View->GetViewArea( &viewWidth, &viewHeight);

	//	m_scaleWidthFactor = 100.0f * viewWidth/m_View->GetBuffer()->GetWidth(); //4.88
	//	m_scaleHeightFactor= 100.0f * viewHeight/m_View->GetBuffer()->GetHeight(); //5.3269


	//	if( m_scaleWidthFactor < m_scaleHeightFactor)
	//	{
	//		m_scaleHeightFactor= m_scaleWidthFactor; //ȡС����������
	//	}
	//	else
	//	{
	//		m_scaleWidthFactor= m_scaleHeightFactor;
	//	}
	//	//��С�������������¼����С
	//	m_scaleWidth = (DWORD)floor(m_View->GetBuffer()->GetWidth()  * m_scaleWidthFactor/100 + 0.5f);
	//	m_scaleHeight= (DWORD)floor(m_View->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);
	//	//���¼������ű���
	//	m_scaleWidthFactor = 100.0f * m_scaleWidth / m_View->GetBuffer()->GetWidth();
	//	m_scaleHeightFactor= 100.0f * m_scaleHeight / m_View->GetBuffer()->GetHeight();
	//	m_View->SetScalingMode( m_scaleWidthFactor/100.0f, m_scaleHeightFactor/100.0f);
	//}


	//// **************** ��MFC��ȡͼƬ
	//CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
 //   fileDlg.DoModal();
	////��ȡͼƬ·����ͼƬ����
 //   CString strFilePath = fileDlg.GetPathName(); //"D:\1.jpg"
	////�ж�·����Ϊ��
 //   if (strFilePath == _T(""))
 //   {
 //       return;
 //   }
	//if (!image.IsNull()) //��ֹ�ٴμ��ر���
	//{
	//	image.Destroy();
	//}
 //   image.Load(strFilePath); //װ��·����ͼƬ��Ϣ��ͼƬ��

	////******************* ͼƬ������Ӧ�ؼ���С
 //   m_viewWnd.GetClientRect(&rectControl);	//�ؼ�����
	////ͼƬ������
	////int imageWidth = image.GetWidth();
 //   //int imageHeight = image.GetHeight();
	////rectControl = CRect(rectControl.TopLeft(), CSize(imageWidth, imageHeight)); //��ʾδ����ԭͼ�������ؼ�������ʾ
	//OnPaint();//�ػ�
}



void CGrabDemoDlg::OnBnClickedFileSave()
{
	if (m_Buffers!=NULL)
	{
		CFileDialog dlg(FALSE,_T("bmp"),_T("untitled.bmp"),NULL,_T("λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|PNG ͼ���ļ�|*.png||"));
		if ( IDOK != dlg.DoModal())
			return;
		// �ļ���
		CString strFileName;
		strFileName = dlg.m_ofn.lpstrFile;// ��"D:\1.bmp"
		USES_CONVERSION; //CString תchar*
		char* image=T2A(strFileName.GetBuffer(0));
		m_Buffers->Save(image,"-format bmp");
	}
	else
	{
		MessageBox(_T("��δ����ͼƬ"));
		return;
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if (image.IsNull()) 
	//{
	//	MessageBox(_T("�㻹û�д�һ��Ҫ�����ͼ���ļ���"));
	//	return;
	//}
	//CString strFilter;
	//strFilter = _T("λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|PNG ͼ���ļ�|*.png||");
	//CFileDialog dlg(FALSE,_T("bmp"),_T("untitled.bmp"),NULL,strFilter);
	//if ( IDOK != dlg.DoModal())
	//	return;

	//// �ļ���
	//CString strFileName;
	//strFileName = dlg.m_ofn.lpstrFile;// ��"D:\1.jpg"

	//// ͼ�񱣴�
	//HRESULT hResult = image.Save(strFileName);
	//if (FAILED(hResult))
	//	MessageBox(_T("����ͼ���ļ�ʧ�ܣ�"));
}


void CGrabDemoDlg::OnBnClickedFreeze()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Xfer!=NULL)
	{
		m_Xfer->Freeze(); //��ͣ
	}
	else
	{
		MessageBox(_T("δ��⵽���"));
	}
}
