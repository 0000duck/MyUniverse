
// FlashPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "cshockwaveflash.h"


// CFlashPlayerDlg �Ի���
class CFlashPlayerDlg : public CDialogEx
{
// ����
public:
	CFlashPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FLASHPLAYER_DIALOG };

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
    CShockwaveFlash m_flash_player;
    afx_msg void OnAcnStartAnimate1();
    afx_msg void OnBnClickedMfcbutton1();
};
