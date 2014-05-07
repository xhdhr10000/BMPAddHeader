// BMP_Add_HeaderDlg.h : header file
//

#pragma once


// CBMP_Add_HeaderDlg dialog
class CBMP_Add_HeaderDlg : public CDialog
{
// Construction
public:
	CBMP_Add_HeaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BMP_ADD_HEADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CEdit m_srcFile, m_dstFile;
	CButton m_RD32, m_RD16565, m_RD16555;
	CEdit m_TBX, m_TBY;
	int m_x, m_y;
	char *m_Buffer;
	TCHAR srcfile[MAX_PATH], dstfile[MAX_PATH];

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void add_32();
	void add_16_565();
	void add_16_555();
	afx_msg void OnBnClickedSrc();
	afx_msg void OnBnClickedDst();
	afx_msg void OnBnClickedOk();
};
