
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
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRotZ(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slide_rot_x;
	CSliderCtrl m_slide_rot_y;
	CSliderCtrl m_slide_rot_z;
	CEdit m_edit_rot_x;
	CEdit m_edit_rot_y;
	CEdit m_edit_rot_z;
	int m_edit_rot_x_value;
	int m_edit_rot_y_value;
	int m_edit_rot_z_value;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
