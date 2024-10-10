//
//  Created by MrHaloka
//

#ifndef UIWINDOWMANAGER_H
#define UIWINDOWMANAGER_H
#include <cstdint>
#include <memory>
#include <windef.h>
#include <windows.h>
#include "../../Public/Managers/ObjectsManager.h"

class UIWindowManager {
protected:
    static constexpr COLORREF GREEN = RGB(0, 255, 0);
    static constexpr COLORREF DARK_RED = RGB(139, 0, 0);
    static constexpr COLORREF BLACK = RGB(0, 0, 0);

    static constexpr int GRID_SIZE = 25;
    static constexpr int CELL_SIZE = 100;

    bool mCells[GRID_SIZE * GRID_SIZE];
    int mHeight;
    int mWidth;
    int mCellHeight;
    int mCellWidth;
    int mCellRemainHeight;
    int mCellRemainWidth;

    ULONG_PTR gdiplusToken;

    std::unique_ptr<ObjectsManager> mObjectsManager;

    Vector2d mStart;
    Vector2d mEnd;
public:
    UIWindowManager();
    ~UIWindowManager();
    int UMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    void RedrawLine(HDC hdc, int x, int y);
    void FillGrid(HDC hdc, HBRUSH hBrush, int x, int y);
    void DrawGrid(HDC hdc);
    void DrawImages(HDC hdc);
    //Events
    void OnCreateWindow(const HWND& hwnd);
    void OnSizeChange(const HWND& hwnd);
    void OnPaint(const HWND& hwnd);
    void OnSizeChangeFinish(const HWND& hwnd);
    void OnLButtonDown(const HWND& hwnd, const LPARAM& lParam);
    void OnRButtonDown(const HWND& hwnd, const LPARAM& lParam);
    void OnSetCursor(const HWND& hwnd, const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam);
    void OnWindowsDestroy();
    void OnKeyDown(const HWND& hwnd, const WPARAM& wParam, const LPARAM& lParam);

    int GetAdjusted(int point, int cellSize, int remaining);
    int CalculatePointClicked(int point, int cellSize, int remaining);
};
#endif //UIWINDOWMANAGER_H
