
// MFC_OpenGLDlg.h : ͷ�ļ�
//

#pragma once
#include "OpenGL.h"

// CMFC_OpenGLDlg �Ի���
class CMFC_OpenGLDlg : public CDialogEx
{
private:
      COpenGL m_OpenglWindow;
// ����
public:
	CMFC_OpenGLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_OPENGL_DIALOG };

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
