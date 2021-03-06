
#include "stdafx.h"
#include "TCPSERVERDLG.h"
#include "TCPSERVERDLGDlg.h"
#include "afxdialogex.h"

#pragma comment(lib,"ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SOCKET WM_USER + 1000


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()




CTCPSERVERDLGDlg::CTCPSERVERDLGDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPSERVERDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPSERVERDLGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTCPSERVERDLGDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CTCPSERVERDLGDlg::OnBnClickedSend)
	ON_MESSAGE(WM_SOCKET, &CTCPSERVERDLGDlg::OnSocket)
END_MESSAGE_MAP()



BOOL CTCPSERVERDLGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitSocket();

	//Set acyncronous socket
	//::WSAAsyncSelect(s_1,this->m_hWnd,WM_SOCKET,FD_ACCEPT|FD_READ);

	//create a thread to send message 
	RECVPARAM *tmp_param = new RECVPARAM;
	tmp_param->sock = s_1;
	tmp_param->hWnd = this->m_hWnd;

	HANDLE h_thread = ::CreateThread(NULL,0,RecvProc,(LPVOID)tmp_param,0,NULL);

	CloseHandle(h_thread);
	GetDlgItem(IDC_TEXT)->EnableWindow(false);
	GetDlgItem(IDC_ADDR)->SetWindowTextW(L"TCP server starts... ...");

	return TRUE;  
}

void CTCPSERVERDLGDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTCPSERVERDLGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CTCPSERVERDLGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//////////////////////////////////////////////
afx_msg LRESULT CTCPSERVERDLGDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	CString str,str_text,str_char;
	char cs_text[200] = {0};
	int length = 0;
#if 0
	switch (lParam)
	{
	case FD_ACCEPT:
		length = sizeof(addr_2);
		s_2 = ::accept(s_1,(sockaddr*)&addr_2,&length);
		num_connection++;
		str.Format(L"Total %d Clients connected to the server.\n",num_connection);
		str += ::inet_ntoa(addr_1.sin_addr);
		str += L"\r\n sign in \r\n";
		GetDlgItem(IDC_TEXT)->SetWindowTextW(str);

	case FD_READ:
		::recv(s_2,cs_text,200,0);
		str_char.Format(L"%s",cs_text);
		GetDlgItem(IDC_TEXT)->GetWindowText(str_text);
		str_text += L"\r\n";
		str_text += ::inet_ntoa(addr_2.sin_addr);
		str_text += L"SAID:";
		str_text += str_char;

		GetDlgItem(IDC_TEXT)->SetWindowTextW(str_text);
	}
#endif

	//2018.12.6
	str_char = AnsiToUnicode((char*)lParam);
	GetDlgItem(IDC_TEXT)->GetWindowText(str_text);
	str_text += L"\r\n";
	str_text += str_char;

	GetDlgItem(IDC_TEXT)->SetWindowTextW(str_text);

	return 0;
}


//Cstring to char
char* UsrCStringToChar(CString str)
{
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *cs_text = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, cs_text, len, NULL, NULL);
	return cs_text;
	//delete[] ptxtTemp;
}




//Send the msg to client
void CTCPSERVERDLGDlg::OnBnClickedSend()
{
	CString str; char * cs_text = NULL;
	GetDlgItem(IDC_SENDTEXT)->GetWindowText(str);
	if (str.IsEmpty())
	{
		AfxMessageBox(L"EMPTY... ...");
	}
	else
	{
		char *cs_text = UsrCStringToChar(str);
		if (::send(s_2, cs_text,str.GetLength(),0) != SOCKET_ERROR)
		{
			GetDlgItem(IDC_TEXT)->GetWindowText(str);
			str += L"MSG ALREADY SENT TO CLIENT.";
			GetDlgItem(IDC_TEXT)->SetWindowTextW(str);
		}
		else
		{
			GetDlgItem(IDC_TEXT)->SetWindowTextW(L"Failed to send msg...\r\n");
		}		
	}
}


CString CTCPSERVERDLGDlg::AnsiToUnicode(char * szAnsi)
{
	CString str;
	int wcsLen=0;
	wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
	wszString[wcsLen] = '\0'; 
	str = wszString;
	delete wszString;
	return str;
}


DWORD WINAPI CTCPSERVERDLGDlg::RecvProc(LPVOID lParam)
{
	SOCKET sock = ((RECVPARAM*)lParam)->sock;
	HWND hWnd = ((RECVPARAM*)lParam)->hWnd;

	char tmp_buff[1024] = {0};
	int ret_val = -1;

	SOCKADDR_IN addr_from;
	int len = sizeof(addr_from);

	char cs_text[1024] = {0};
	while (true)
	{
		SOCKET s_client = ::accept(sock, (sockaddr*)&addr_from, &len);		
 		ret_val = ::recv(s_client, cs_text, 1024, 0);
		if (SOCKET_ERROR != ret_val && INVALID_SOCKET != ret_val)
		{                                      
			sprintf_s(tmp_buff, "%s said:%s", inet_ntoa(addr_from.sin_addr), cs_text);
			::PostMessage(hWnd, WM_SOCKET, 0, (LPARAM)tmp_buff);
		}

		closesocket(s_client);
	}
	return 0;
}



void CTCPSERVERDLGDlg::InitSocket()
{
	WSADATA data;
	WORD w = MAKEWORD(2, 0);

	::WSAStartup(w, &data);

	addr_1.sin_family = AF_INET;
	addr_1.sin_port = htons(8888);
	addr_1.sin_addr.S_un.S_addr = INADDR_ANY;
	s_1 = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == s_1)
	{
		MessageBox(L"failed to create socket");
	}
	::bind(s_1, (sockaddr*)&addr_1, sizeof(addr_1));
	::listen(s_1, 5);
}