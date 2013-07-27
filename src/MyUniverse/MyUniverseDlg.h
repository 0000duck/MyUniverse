
// MyUniverseDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "GlbRot.h"
#include "resource.h"
#include "afxcmn.h"

#define GET_DATA 1 
#define PUT_DATA 0

//�Զ�����Ϣ������slider_rotz),��TimingThread����
#define WM_GLB_UPDATEDATA (WM_USER+100)
#define WM_GLB_UDPREADONEPAGE (WM_USER+101)

// CMyUniverseDlg �Ի���
class CMyUniverseDlg : public CDialogEx
{
// ����
public:
	CMyUniverseDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CMyUniverseDlg();
// �Ի�������
	enum { IDD = IDD_MYUNIVERSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
private:
    CString GetPageStructPath(CString StoryPath, CString ChapterName);
    void ReadOnePage(bool bUpdateDataFromUI);
    void ReadChapterStruct();
    void ReadPageStruct();
    void ReadFolderContent(CString folderPath, CString suffix);
    void GlobeRotate(int Horz, int Vert, int Axis, GlbRotmat &r);
    void ReadStoryConfigXML();
    CString FindXMLFilePath(CString pageStructPath);
    unsigned char* convert (unsigned char *in, char *encoding);
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    LRESULT OnGlbUpdateData(WPARAM wParam, LPARAM lParam);//�Զ�����Ϣ����Ӧ����
    LRESULT OnGlbUdpReadOnePage(WPARAM wParam, LPARAM lParam);//�Զ�����Ϣ����Ӧ����
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    int m_edit_rotx;
    int m_edit_roty;
    int m_edit_rotz;
    int m_slider_rotx;
    int m_slider_roty;
    int m_slider_rotz;
    afx_msg void OnNMCustomdrawSliderRotx(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSliderRoty(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSliderRotz(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditRotx();
    afx_msg void OnEnChangeEditRoty();
    afx_msg void OnEnChangeEditRotz();
    afx_msg void OnCbnSelchangeComboChapter();
    afx_msg void OnBnClickedButtonBrowse();
    afx_msg void OnCbnSelchangeComboPage();
    CComboBox m_chapter_select; //�½ڣ�����ѡ���
    CComboBox m_page_select;    //ҳ�棬����ѡ���
    CString m_story_path;       //�½ڵ���Ŀ¼��
    CString m_chapter_value;    //�½ڵ���Ŀ¼��
    CString m_page_struct_path; //ҳ���ļ���Ŀ¼
    CString m_page_value;       //ҳ���ļ����ļ������������
    afx_msg void OnBnClickedButtonStep();
    afx_msg void OnBnClickedButtonPlay();
    afx_msg void OnBnClickedButtonPause();
    int m_frame_rate;
    afx_msg void OnEnChangeEditFrameRate();
    afx_msg void OnBnClickedCheckAutoRot();
    BOOL m_rotating_UI;
    CSliderCtrl m_slider_rotz_ctrl;
    CEdit m_edit_rotz_ctrl;
    CSliderCtrl m_slider_rotx_ctrl;
    CSliderCtrl m_slider_roty_ctrl;
    int m_rotationRate_UI;
    afx_msg void OnEnChangemrotationrate();
    CButton m_unclockwise_UI;
    CButton m_clockwise_UI;
    afx_msg void OnBnClickedUnclockwise();
    afx_msg void OnBnClickedClockwise();
};
