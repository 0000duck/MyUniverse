
// My_MFC_OpenGL_testDlg.h : ͷ�ļ�
//

#pragma once


// CMy_MFC_OpenGL_testDlg �Ի���
class CMy_MFC_OpenGL_testDlg : public CDialogEx
{
// ����
public:
	CMy_MFC_OpenGL_testDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_MFC_OPENGL_TEST_DIALOG };

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
};
