
// MyUniverseDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "GlbRot.h"

#define GET_DATA 1 
#define PUT_DATA 0

// CMyUniverseDlg �Ի���
class CMyUniverseDlg : public CDialogEx
{
// ����
public:
	CMyUniverseDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYUNIVERSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
private:
    void ReadChapterStruct();
    void ReadPageStruct();
    void ReadOnePage();
    void ReadFolderContent(CString folderPath, CString suffix);
    void GlobeRotate(int Horz, int Vert, int Axis, GlbRotmat &r);
    void ReadStoryConfigXML();
    unsigned char* convert (unsigned char *in, char *encoding);
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
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
    CString m_page_value;       //ҳ���ļ����ļ���
};
