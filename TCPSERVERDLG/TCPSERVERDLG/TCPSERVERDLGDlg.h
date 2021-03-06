
#pragma once
#include <afxsock.h>

// CTCPSERVERDLGDlg 对话框
class CTCPSERVERDLGDlg : public CDialogEx
{
// 构造
public:
	CTCPSERVERDLGDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPSERVERDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnSocket(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSend();
	static DWORD WINAPI RecvProc(LPVOID lParam);
	void InitSocket();
	CString AnsiToUnicode(char * szAnsi);
	SOCKET s_1,s_2;
	sockaddr_in addr_1, addr_2;
	int num_connection;
};


struct RECVPARAM
{
	SOCKET sock;
	HWND   hWnd;
};