
// MyUniverseDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


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
    CString m_story_path;
    CString m_page_struct_path;
    afx_msg void OnBnClickedButtonBrowse();
    CComboBox m_chapter_select;
    CString m_chapter_value;
    CComboBox m_page_select;
    CString m_page_value;
    afx_msg void OnCbnSelchangeComboPage();
};
