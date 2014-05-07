// BMP_Add_HeaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMP_Add_Header.h"
#include "BMP_Add_HeaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBMP_Add_HeaderDlg dialog




CBMP_Add_HeaderDlg::CBMP_Add_HeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMP_Add_HeaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBMP_Add_HeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBMP_Add_HeaderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(BT_SRC, &CBMP_Add_HeaderDlg::OnBnClickedSrc)
	ON_BN_CLICKED(BT_DST, &CBMP_Add_HeaderDlg::OnBnClickedDst)
	ON_BN_CLICKED(IDOK, &CBMP_Add_HeaderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBMP_Add_HeaderDlg message handlers

BOOL CBMP_Add_HeaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if (!m_srcFile.SubclassDlgItem(TB_SRC, this)) return FALSE;
	if (!m_dstFile.SubclassDlgItem(TB_DST, this)) return FALSE;
	if (!m_RD32.SubclassDlgItem(RD_32, this)) return FALSE;
	if (!m_RD16565.SubclassDlgItem(RD_16565, this)) return FALSE;
	if (!m_RD16555.SubclassDlgItem(RD_16555, this)) return FALSE;
	if (!m_TBX.SubclassDlgItem(TB_X, this)) return FALSE;
	if (!m_TBY.SubclassDlgItem(TB_Y, this)) return FALSE;
	m_RD32.SetCheck(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBMP_Add_HeaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBMP_Add_HeaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBMP_Add_HeaderDlg::OnBnClickedSrc()
{
	OPENFILENAME ofn;

	memset(&ofn, 0, sizeof(ofn));
	srcfile[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER;
	ofn.hwndOwner = this->GetSafeHwnd();
	ofn.lpstrFilter = _T("Any File\0*.*\0\0");
	ofn.lpstrFile = srcfile;
	GetOpenFileName(&ofn);
	if (_tcslen(srcfile)!=0) {
		m_srcFile.SetWindowText(srcfile);
		_tcscpy_s(dstfile, MAX_PATH, srcfile);
		if (dstfile[_tcslen(dstfile)-4] == '.')
			_tcscpy(&dstfile[_tcslen(dstfile)-3], _T("bmp"));
		else
			_tcscat(dstfile, _T(".bmp"));
		m_dstFile.SetWindowText(dstfile);
	}
}

void CBMP_Add_HeaderDlg::OnBnClickedDst()
{
	OPENFILENAME ofn;

	memset(&ofn, 0, sizeof(ofn));
	dstfile[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER|OFN_OVERWRITEPROMPT;
	ofn.hwndOwner = this->GetSafeHwnd();
	ofn.lpstrFilter = _T("Bitmap(*.bmp)\0*.bmp\0\0");
	ofn.lpstrFile = dstfile;
	ofn.lpstrDefExt = _T("bmp");
	GetSaveFileName(&ofn);
	if (_tcslen(srcfile)!=0) m_dstFile.SetWindowText(dstfile);
}

void CBMP_Add_HeaderDlg::add_32()
{
	int *tmp;
	m_Buffer = (char*)malloc(m_x*m_y*4+0x36);
	memset(m_Buffer, 0, 0x36);

	m_Buffer[0x0] = 0x42;
	m_Buffer[0x1] = 0x4d;
	tmp = (int*)&m_Buffer[0x2];
	*tmp = m_x*m_y*4+0x36;
	m_Buffer[0xa] = 0x36;
	m_Buffer[0xe] = 0x28;
	tmp = (int*)&m_Buffer[0x12];
	*tmp = m_x;
	tmp = (int*)&m_Buffer[0x16];
	*tmp = m_y;
	m_Buffer[0x1a] = 1;
	m_Buffer[0x1c] = 32;
	tmp = (int*)&m_Buffer[0x22];
	*tmp = m_x*m_y*4;
	tmp = (int*)&m_Buffer[0x26];
	*tmp = 0xb13;
	tmp = (int*)&m_Buffer[0x2a];
	*tmp = 0xb13;

	FILE *fpbmp = _tfopen(srcfile, _T("rb"));
	if (fpbmp == NULL) {
		MessageBox(_T("Open src file failed"));
		return;
	}
	fread(&m_Buffer[0x36], 1, m_x*m_y*4, fpbmp);
	fclose(fpbmp);

	fpbmp = _tfopen(dstfile, _T("wb"));
	if (fpbmp == NULL) {
		MessageBox(_T("Open dst file failed"));
		return;
	}
	fwrite(m_Buffer, 1, m_x*m_y*4+0x36, fpbmp);
	fclose(fpbmp);

	free(m_Buffer);
}

void CBMP_Add_HeaderDlg::add_16_565()
{
	int *tmp;
	m_Buffer = (char*)malloc(m_x*m_y*2+0x46);
	memset(m_Buffer, 0, 0x46);

	m_Buffer[0x0] = 0x42;
	m_Buffer[0x1] = 0x4d;
	tmp = (int*)&m_Buffer[0x2];
	*tmp = m_x*m_y*2+0x46;
	m_Buffer[0xa] = 0x46;
	m_Buffer[0xe] = 0x38;
	tmp = (int*)&m_Buffer[0x12];
	*tmp = m_x;
	tmp = (int*)&m_Buffer[0x16];
	*tmp = m_y;
	m_Buffer[0x1a] = 1;
	m_Buffer[0x1c] = 16;
	m_Buffer[0x1e] = 3;
	tmp = (int*)&m_Buffer[0x22];
	*tmp = m_x*m_y*2;
	tmp = (int*)&m_Buffer[0x26];
	*tmp = 0xb13;
	tmp = (int*)&m_Buffer[0x2a];
	*tmp = 0xb13;
	tmp = (int*)&m_Buffer[0x36];
	*tmp = 0xf800;
	tmp++;
	*tmp = 0x7e0;
	tmp++;
	*tmp = 0x1f;

	FILE *fpbmp = _tfopen(srcfile, _T("rb"));
	if (fpbmp == NULL) {
		MessageBox(_T("Open src file failed"));
		return;
	}
	fread(&m_Buffer[0x46], 1, m_x*m_y*2, fpbmp);
	fclose(fpbmp);

	fpbmp = _tfopen(dstfile, _T("wb"));
	if (fpbmp == NULL) {
		MessageBox(_T("Open dst file failed"));
		return;
	}
	fwrite(m_Buffer, 1, m_x*m_y*2+0x46, fpbmp);
	fclose(fpbmp);

	free(m_Buffer);
}

void CBMP_Add_HeaderDlg::add_16_555()
{
	int *tmp;
	m_Buffer = (char*)malloc(m_x*m_y*2+0x46);
	memset(m_Buffer, 0, 0x46);

	m_Buffer[0x0] = 0x42;
	m_Buffer[0x1] = 0x4d;
	tmp = (int*)&m_Buffer[0x2];
	*tmp = m_x*m_y*2+0x46;
	m_Buffer[0xa] = 0x46;
	m_Buffer[0xe] = 0x38;
	tmp = (int*)&m_Buffer[0x12];
	*tmp = m_x;
	tmp = (int*)&m_Buffer[0x16];
	*tmp = m_y;
	m_Buffer[0x1a] = 1;
	m_Buffer[0x1c] = 16;
	m_Buffer[0x1e] = 3;
	tmp = (int*)&m_Buffer[0x22];
	*tmp = m_x*m_y*2;
	tmp = (int*)&m_Buffer[0x26];
	*tmp = 0xb13;
	tmp = (int*)&m_Buffer[0x2a];
	*tmp = 0xb13;
	tmp = (int*)&m_Buffer[0x36];
	*tmp = 0x7c00;
	tmp++;
	*tmp = 0x3e0;
	tmp++;
	*tmp = 0x1f;
	tmp++;
	*tmp = 0x800;

	FILE *fpbmp = _tfopen(srcfile, _T("rb"));
	if (fpbmp == NULL) {
		MessageBox(_T("Open src file failed"));
		return;
	}
	fread(&m_Buffer[0x46], 1, m_x*m_y*2, fpbmp);
	fclose(fpbmp);

	fpbmp = _tfopen(dstfile, _T("wb"));
	if (fpbmp == NULL) {
		MessageBox(_T("Open dst file failed"));
		return;
	}
	fwrite(m_Buffer, 1, m_x*m_y*2+0x46, fpbmp);
	fclose(fpbmp);

	free(m_Buffer);
}

void CBMP_Add_HeaderDlg::OnBnClickedOk()
{
	if ( (m_TBX.GetWindowTextLength() == 0) || (m_TBY.GetWindowTextLength() == 0) ) {
		MessageBox(_T("请输入分辨率"));
		return;
	}
	TCHAR tmp[10];
	m_TBX.GetWindowText(tmp, 10);
	m_x = _ttoi(tmp);
	m_TBY.GetWindowText(tmp, 10);
	m_y = _ttoi(tmp);
	m_srcFile.GetWindowText(srcfile, MAX_PATH);
	m_dstFile.GetWindowText(dstfile, MAX_PATH);
	if (m_RD32.GetCheck()) add_32();
	else if (m_RD16565.GetCheck()) add_16_565();
	else if (m_RD16555.GetCheck()) add_16_555();

	MessageBox(_T("成功"));
}
