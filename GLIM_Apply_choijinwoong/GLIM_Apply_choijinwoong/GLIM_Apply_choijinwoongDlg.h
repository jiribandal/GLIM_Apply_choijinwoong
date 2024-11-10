#pragma once
#include "afxdialogex.h"
#include <atlimage.h>

class CGLIMApplychoijinwoongDlg : public CDialogEx
{
public:
    CGLIMApplychoijinwoongDlg(CWnd* pParent = nullptr);

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;
    afx_msg void OnBnClickedLoadImage();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedDraw();
    afx_msg void OnBnClickedAction();
    void DrawImage(CDC* pDC);
    void SaveCurrentImage();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    DECLARE_MESSAGE_MAP()

private:
    bool m_imageLoaded = false;
    CImage m_image;
    HICON m_hIcon;
    int m_x = -1;          // 원의 중심 x 좌표
    int m_y = -1;          // 원의 중심 y 좌표
    int m_radius = 0;      // 원의 반지름
    int m_x2, m_y2;        // 목표 좌표 (x2, y2)
    int m_stepX, m_stepY;  // x와 y 방향의 이동 단계 크기
    bool m_moving = false; // 원이 이동 중인지 여부
};
