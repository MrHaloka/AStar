//
// Created by MrHaloka
//
#include "../../Public/Managers/UIWindowManager.h"
#include "../../Public/Navigation/AStar.h"
#include <memory>
#include <chrono>
#include <gdiplus.h>

using namespace Gdiplus;

UIWindowManager::UIWindowManager() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    mObjectsManager = std::make_unique<ObjectsManager>(GRID_SIZE, CELL_SIZE);
    mEnd = Vector2d(GRID_SIZE - 1, GRID_SIZE - 1);
}

int UIWindowManager::UMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            OnCreateWindow(hwnd);
            return 0;
        }
        case WM_SIZE: {
            OnSizeChange(hwnd);
            return 0;
        }
        case WM_PAINT: {
            OnPaint(hwnd);
            return 0;
        }
        case WM_EXITSIZEMOVE: {
            OnSizeChangeFinish(hwnd);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            OnLButtonDown(hwnd, lParam);
            return 0;
        }
        case WM_RBUTTONDOWN: {
            OnRButtonDown(hwnd, lParam);
            return 0;
        }
        case WM_SETCURSOR: {
            OnSetCursor(hwnd, uMsg, wParam, lParam);
            return 0;
        }
        case WM_DESTROY: {
            OnWindowsDestroy();
            return 0;
        }
        case WM_KEYDOWN: {
            OnKeyDown(hwnd, wParam, lParam);
            return 0;
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

void UIWindowManager::RedrawLine(HDC hdc, int x, int y) {
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN) SelectObject(hdc, hPen);
    int startX = GetAdjusted(x, mCellWidth, mCellRemainWidth);
    int StartY = GetAdjusted(y, mCellHeight, mCellRemainHeight);
    int endX = GetAdjusted(x + 1, mCellWidth, mCellRemainWidth);
    int endY = GetAdjusted(y + 1, mCellHeight, mCellRemainHeight);
    MoveToEx(hdc, startX, StartY, nullptr);
    LineTo(hdc, startX, endY);
    MoveToEx(hdc, startX, StartY, nullptr);
    LineTo(hdc, endX, StartY);
    MoveToEx(hdc, endX, endY, nullptr);
    LineTo(hdc, startX, endY);
    MoveToEx(hdc, endX, endY, nullptr);
    LineTo(hdc, endX, StartY);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void UIWindowManager::FillGrid(HDC hdc, HBRUSH hBrush, int x, int y) {
    int startX = GetAdjusted(x, mCellWidth, mCellRemainWidth);
    int StartY = GetAdjusted(y, mCellHeight, mCellRemainHeight);
    int endX = GetAdjusted(x + 1, mCellWidth, mCellRemainWidth);
    int endY = GetAdjusted(y + 1, mCellHeight, mCellRemainHeight);
    RECT rect = {startX, StartY, endX, endY};
    FillRect(hdc, &rect, hBrush);
    RedrawLine(hdc, x, y);
}

void UIWindowManager::DrawGrid(HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN) SelectObject(hdc, hPen);
    for (int x = 0; x <= GRID_SIZE; x++) {
        const int adjustedX = GetAdjusted(x, mCellWidth, mCellRemainWidth);
        MoveToEx(hdc, adjustedX, 0, nullptr);
        LineTo(hdc, adjustedX, mHeight);
    }
    for (int y = 0; y <= GRID_SIZE; y++) {
        const int adjustedY = GetAdjusted(y, mCellHeight, mCellRemainHeight);
        MoveToEx(hdc, 0, adjustedY, nullptr);
        LineTo(hdc, mWidth, adjustedY);
    }
    HBRUSH hBrush = CreateSolidBrush(BLACK);
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            if (mCells[x * GRID_SIZE + y]) {
                FillGrid(hdc, hBrush, x, y);
            }
        }
    }
    DeleteObject(hBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void UIWindowManager::OnCreateWindow(const HWND &hwnd) {
}

void UIWindowManager::OnSizeChange(const HWND &hwnd) {
    InvalidateRect(hwnd, nullptr, TRUE);
}

void UIWindowManager::OnSizeChangeFinish(const HWND &hwnd) {
    InvalidateRect(hwnd, nullptr, TRUE);
}

void UIWindowManager::OnPaint(const HWND &hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    RECT rect;
    GetClientRect(hwnd, &rect);
    FillRect(hdc, &rect, (HBRUSH) (COLOR_WINDOW + 1));
    mWidth = rect.right - rect.left;
    mHeight = rect.bottom - rect.top;
    mCellHeight = mHeight / GRID_SIZE;
    mCellWidth = mWidth / GRID_SIZE;
    mCellRemainHeight = mHeight % GRID_SIZE;
    mCellRemainWidth = mWidth % GRID_SIZE;
    DrawGrid(hdc);
    DrawImages(hdc);
    EndPaint(hwnd, &ps);
}

void UIWindowManager::OnLButtonDown(const HWND &hwnd, const LPARAM &lParam) {
    HDC hdc = GetDC(hwnd);
    int xPos = LOWORD(lParam);
    int yPos = HIWORD(lParam);
    int x = CalculatePointClicked(xPos, mCellWidth, mCellRemainWidth);
    int y = CalculatePointClicked(yPos, mCellHeight, mCellRemainHeight);
    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0){
        mStart = Vector2d(x, y);
        InvalidateRect(hwnd, nullptr, TRUE);
        return;
    }
    if (mCells[x * GRID_SIZE + y]) {
        return;
    }
    HBRUSH hBrush = CreateSolidBrush(BLACK);
    FillGrid(hdc, hBrush, x, y);
    mCells[x * GRID_SIZE + y] = true;
    mObjectsManager->AddNewObject(x * GRID_SIZE + y, Vector2d(x * CELL_SIZE, y * CELL_SIZE));
    DeleteObject(hBrush);
    InvalidateRect(hwnd, nullptr, TRUE);
}

void UIWindowManager::OnRButtonDown(const HWND &hwnd, const LPARAM &lParam) {
    int xPos = LOWORD(lParam);
    int yPos = HIWORD(lParam);
    int x = CalculatePointClicked(xPos, mCellWidth, mCellRemainWidth);
    int y = CalculatePointClicked(yPos, mCellHeight, mCellRemainHeight);
    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0){
        mEnd = Vector2d(x, y);
        InvalidateRect(hwnd, nullptr, TRUE);
        return;
    }
    if (!mCells[x * GRID_SIZE + y]) {
        return;
    }
    HDC hdc = GetDC(hwnd);
    HBRUSH hBrush = (HBRUSH) (COLOR_WINDOW + 1);
    FillGrid(hdc, hBrush, x, y);
    mCells[x * GRID_SIZE + y] = false;
    mObjectsManager->RemoveObject(x * GRID_SIZE + y, Vector2d(x * CELL_SIZE, y * CELL_SIZE));
    DeleteObject(hBrush);
    InvalidateRect(hwnd, nullptr, TRUE);
}

void UIWindowManager::OnSetCursor(const HWND &hwnd, const UINT &uMsg, const WPARAM &wParam, const LPARAM &lParam) {
    if (LOWORD(lParam) == HTCLIENT) {
        SetCursor(LoadCursor(nullptr, IDC_ARROW));
        return;
    }
    DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void UIWindowManager::OnWindowsDestroy() {
    PostQuitMessage(0);
}

int UIWindowManager::GetAdjusted(int point, int cellSize, int remaining) {
    return point <= remaining ? point * cellSize + point : point * cellSize + remaining;
}

int UIWindowManager::CalculatePointClicked(int point, int cellSize, int remaining) {
    int rawPoint = point / cellSize;
    if (rawPoint > remaining) {
        return (point - remaining) / cellSize;
    }
    return (point - rawPoint) / cellSize;
}

void UIWindowManager::OnKeyDown(HWND const &hwnd, const WPARAM &wParam, const LPARAM &lParam) {
    if (wParam == VK_RETURN) {
        AStar navigator(mObjectsManager.get(), mStart * CELL_SIZE, mEnd * CELL_SIZE);
        PathLinkedList* path = navigator.CalculateThePath();
        if (path == nullptr){
            return;
        }
        while(path->GetPrevious() != nullptr){
            PathLinkedList* currentPath = path;
            HBRUSH hBrush = CreateSolidBrush(GREEN);
            HDC hdc = GetDC(hwnd);
            FillGrid(hdc, hBrush, currentPath->GetPath().x / 100, currentPath->GetPath().y / 100);
            path = path->GetPrevious();
            delete currentPath;
        }
    }
}

UIWindowManager::~UIWindowManager() {
    GdiplusShutdown(gdiplusToken);
}

void UIWindowManager::DrawImages(HDC hdc) {
    Graphics graphics(hdc);

    int startX = GetAdjusted(mStart.x, mCellWidth, mCellRemainWidth);
    int startY = GetAdjusted(mStart.y, mCellHeight, mCellRemainHeight);
    Image image1(L"Start.png");
    graphics.DrawImage(&image1, startX, startY, mCellWidth, mCellHeight);

    int endX = GetAdjusted(mEnd.x, mCellWidth, mCellRemainWidth);
    int endY = GetAdjusted(mEnd.y, mCellHeight, mCellRemainHeight);
    Image image2(L"End.png");
    graphics.DrawImage(&image2, endX, endY, mCellWidth, mCellHeight);
}
