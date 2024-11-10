#include "pch.h"
#include "framework.h"
#include "GLIM_Apply_choijinwoong.h"
#include "GLIM_Apply_choijinwoongDlg.h"
#include "afxdialogex.h"
#include <cstdlib>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CGLIMApplychoijinwoongDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_OPEN, &CGLIMApplychoijinwoongDlg::OnBnClickedLoadImage)
    ON_BN_CLICKED(IDC_BTN_DRAW, &CGLIMApplychoijinwoongDlg::OnBnClickedDraw)
    ON_BN_CLICKED(IDC_BTN_ACTION, &CGLIMApplychoijinwoongDlg::OnBnClickedAction)
    ON_WM_TIMER()
END_MESSAGE_MAP()

CGLIMApplychoijinwoongDlg::CGLIMApplychoijinwoongDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_GLIM_APPLY_CHOIJINWOONG_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CGLIMApplychoijinwoongDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CGLIMApplychoijinwoongDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return TRUE;
}


void CGLIMApplychoijinwoongDlg::OnPaint()
{
    CPaintDC dc(this);

    // 이미지가 로드된 경우 이미지를 먼저 그리기
    if (m_imageLoaded) 
    {
        DrawImage(&dc);
    }
    else 
    {
        CDialogEx::OnPaint();
    }

    // 원 또는 "X" 표시
    if (m_radius > 0) 
    {
        // 원 생성
        dc.Ellipse(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);
    }
    else if (m_x >= 0 && m_y >= 0) 
    {
        // 빨간색 펜을 선택하여 "X" 표시
        CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
        CPen* pOldPen = dc.SelectObject(&redPen);

        // X와 좌표값을 표시
        dc.MoveTo(m_x - 5, m_y - 5);
        dc.LineTo(m_x + 5, m_y + 5);
        dc.MoveTo(m_x - 5, m_y + 5);
        dc.LineTo(m_x + 5, m_y - 5);

        // 좌표 텍스트 표시
        CString coordText;
        coordText.Format(_T("(%d, %d)"), m_x, m_y);
        dc.TextOut(m_x + 10, m_y, coordText);

        // 펜 원상복구
        dc.SelectObject(pOldPen);
    }
}


HCURSOR CGLIMApplychoijinwoongDlg::OnQueryDragIcon()
{
    return m_hIcon;
}


void CGLIMApplychoijinwoongDlg::OnBnClickedDraw()
{
    // x1, y1 좌표값 가져오기
    m_x = GetDlgItemInt(IDC_EDIT_x1);
    m_y = GetDlgItemInt(IDC_EDIT_y1);

    // 반지름을 10에서 20 사이로 랜덤 설정
    srand((unsigned)time(0));
    m_radius = 10 + (rand() % 11);

    // 다이얼로그 크기에 맞춰 좌표가 범위를 벗어나지 않도록 제한
    int maxWidth = 640;
    int maxHeight = 480;

    if (m_x - m_radius < 0) m_x = m_radius;
    if (m_x + m_radius > maxWidth) m_x = maxWidth - m_radius;
    if (m_y - m_radius < 0) m_y = m_radius;
    if (m_y + m_radius > maxHeight) m_y = maxHeight - m_radius;

    // 화면 갱신 요청
    Invalidate();
    UpdateWindow();
}



void CGLIMApplychoijinwoongDlg::OnBnClickedAction()
{
    m_x = GetDlgItemInt(IDC_EDIT_x1);
    m_y = GetDlgItemInt(IDC_EDIT_y1);
    m_x2 = GetDlgItemInt(IDC_EDIT_x2);
    m_y2 = GetDlgItemInt(IDC_EDIT_y2);

    // 다이얼로그 또는 이미지 크기 설정
    int maxWidth = 640;
    int maxHeight = 480;

    // 목표 좌표가 이미지 경계를 넘지 않도록 조정
    if (m_x2 < 0) m_x2 = 0;
    if (m_x2 > maxWidth) m_x2 = maxWidth;
    if (m_y2 < 0) m_y2 = 0;
    if (m_y2 > maxHeight) m_y2 = maxHeight;

    int totalSteps = 50;
    m_stepX = (m_x2 - m_x) / totalSteps;
    m_stepY = (m_y2 - m_y) / totalSteps;

    m_moving = true;
    SetTimer(1, 50, NULL);
}


void CGLIMApplychoijinwoongDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (m_moving) 
    {
        // 목표 지점을 초과하지 않도록 조건 설정
        if ((m_stepX > 0 && m_x >= m_x2) || (m_stepX < 0 && m_x <= m_x2) ||
            (m_stepY > 0 && m_y >= m_y2) || (m_stepY < 0 && m_y <= m_y2)) 
        {
            m_x = m_x2;
            m_y = m_y2;
            m_moving = false;
            KillTimer(1);
        }
        else 
        {
            m_x += m_stepX;
            m_y += m_stepY;
        }

        // 이미지 경계를 넘지 않도록 추가 제한
        int maxWidth = 640;
        int maxHeight = 480;
        if (m_x < 0) m_x = 0;
        if (m_x > maxWidth) m_x = maxWidth;
        if (m_y < 0) m_y = 0;
        if (m_y > maxHeight) m_y = maxHeight;

        Invalidate();
        UpdateWindow();
        SaveCurrentImage();
    }

    CDialogEx::OnTimer(nIDEvent);
}



void CGLIMApplychoijinwoongDlg::SaveCurrentImage()
{
    CImage image;
    CRect rect;
    GetClientRect(&rect);

    CDC* pDC = GetDC();
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    memDC.SelectObject(&bitmap);

    memDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
    image.Attach((HBITMAP)bitmap.Detach());

    CString folderPath = _T(".\\image");
    CreateDirectory(folderPath, NULL);

    CString filePath;
    filePath.Format(_T("%s\\frame_%d.jpg"), folderPath, GetTickCount());
    image.Save(filePath, Gdiplus::ImageFormatJPEG);

    ReleaseDC(pDC);
}


void CGLIMApplychoijinwoongDlg::DrawImage(CDC* pDC)
{
    if (m_imageLoaded) 
    {
        int x_start = 10;
        int y_start = 10;
        int width = 640;
        int height = 480;

        m_image.Draw(pDC->m_hDC, x_start, y_start, width, height);

        // 원의 중심 좌표에 "X" 표시
        if (m_x >= 0 && m_y >= 0) 
        {
            pDC->MoveTo(m_x - 5, m_y - 5);
            pDC->LineTo(m_x + 5, m_y + 5);
            pDC->MoveTo(m_x - 5, m_y + 5);
            pDC->LineTo(m_x + 5, m_y - 5);

            // 중심 좌표 텍스트 표시
            CString coordText;
            coordText.Format(_T("(%d, %d)"), m_x, m_y);
            pDC->TextOut(m_x + 10, m_y, coordText);
        }
    }
}

void CGLIMApplychoijinwoongDlg::OnBnClickedLoadImage()
{
    CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_FILEMUSTEXIST, _T("Image Files (*.jpg;*.png)|*.jpg;*.png|All Files (*.*)|*.*||"));
    if (dlg.DoModal() == IDOK) {
        CString filePath = dlg.GetPathName();

        if (m_imageLoaded) {
            m_image.Destroy();
            m_imageLoaded = false;
        }

        HRESULT hr = m_image.Load(filePath);
        if (SUCCEEDED(hr)) {
            m_imageLoaded = true;

            // 마지막 원의 반지름을 0으로 설정하여 원을 숨김
            m_radius = 0;

            // 화면 갱신 요청
            Invalidate();
        }
        else {
            AfxMessageBox(_T("이미지를 로드할 수 없습니다."));
        }
    }
}
