
// MyUniverseDlg.h : ͷ�ļ�
//

#pragma once


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
    int edit_rotx;
    int edit_roty;
    int edit_rotz;
    int slider_rotx;
    int slider_roty;
    int slider_rotz;
    afx_msg void OnNMCustomdrawSliderRotx(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSliderRoty(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSliderRotz(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditRotx();
    afx_msg void OnEnChangeEditRoty();
    afx_msg void OnEnChangeEditRotz();
};
