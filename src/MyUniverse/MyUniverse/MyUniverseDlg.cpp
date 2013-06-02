
// MyUniverseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyUniverse.h"
#include "MyUniverseDlg.h"
#include "afxdialogex.h"
#include "..\..\GlbCore\GlbCore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )
#endif

//extern int g_rot_x;
//extern int g_rot_y;
//extern int g_rot_z;
DWORD WINAPI GlobeThread(LPVOID lpParam);

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


// CMyUniverseDlg �Ի���




CMyUniverseDlg::CMyUniverseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyUniverseDlg::IDD, pParent)
	, m_edit_rot_x_value(0)
	, m_edit_rot_y_value(0)
	, m_edit_rot_z_value(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyUniverseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_ROT_X, m_slide_rot_x);
	DDX_Control(pDX, IDC_SLIDER_ROT_Y, m_slide_rot_y);
	DDX_Control(pDX, IDC_SLIDER_ROT_Z, m_slide_rot_z);
	DDX_Control(pDX, IDC_EDIT_ROT_X, m_edit_rot_x);
	DDX_Control(pDX, IDC_EDIT_ROT_Y, m_edit_rot_y);
	DDX_Control(pDX, IDC_EDIT_ROT_Z, m_edit_rot_z);
	DDX_Text(pDX, IDC_EDIT_ROT_X, m_edit_rot_x_value);
	DDX_Text(pDX, IDC_EDIT_ROT_Y, m_edit_rot_y_value);
	DDV_MinMaxInt(pDX, m_edit_rot_y_value, 0, 100);
	DDX_Text(pDX, IDC_EDIT_ROT_Z, m_edit_rot_z_value);
}

BEGIN_MESSAGE_MAP(CMyUniverseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROT_X, &CMyUniverseDlg::OnNMCustomdrawSliderRotX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROT_Y, &CMyUniverseDlg::OnNMCustomdrawSliderRotY)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROT_Z, &CMyUniverseDlg::OnNMCustomdrawSliderRotZ)
	ON_BN_CLICKED(IDOK, &CMyUniverseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMyUniverseDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMyUniverseDlg ��Ϣ�������

BOOL CMyUniverseDlg::OnInitDialog()
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
	
	CreateThread(0, 0, GlobeThread, 0,0,0);//����OpenGL��ʾ�߳�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyUniverseDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyUniverseDlg::OnPaint()
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyUniverseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edit_rot_x_value = (m_slide_rot_x.GetPos());
	UpdateData(FALSE);
	//printf("%d\n", m_edit_rot_x_value);
	*pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edit_rot_y_value = (m_slide_rot_y.GetPos());
	UpdateData(FALSE);
	*pResult = 0;
}



void CMyUniverseDlg::OnNMCustomdrawSliderRotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edit_rot_z_value = (m_slide_rot_z.GetPos());
	UpdateData(FALSE);
	*pResult = 0;
}


void CMyUniverseDlg::OnBnClickedOk()
{
	return;
}


void CMyUniverseDlg::OnBnClickedCancel()
{
	if( 2 == MessageBox("���Ҫ�˳���", NULL, MB_OKCANCEL))
	{
		return;
	}
	else
	{
		CDialogEx::OnCancel();
	}
}

DWORD WINAPI GlobeThread(LPVOID lpParam)
{
    //CreateGLWindow("MyUniverse",200,200,16,false);//glOrtho�Ĳ�����˶�Ӧ
    glbCreateWindow(0);

    //GlbImage Image = glbLoadImage("a001.dds");
    GlbImage Image = glbLoadImage("earth.jpg");

    do
    {
        glbDrawImage(Image);
        glbUpdateWindow(0);
    }while(1);

    glbReleaseImage(&Image);
    glbDestoryWindow(0);
    return 1;
}