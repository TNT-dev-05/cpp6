#include    "Windows.h" 
#include    "lab5.h"
#include    "ShapeEditor.h"

#define     OBJECTRECTANGLE 1 
#define     OBJECTELLIPSE   2 
#define     OBJECTLINE      3 
#define     OBJECTPOINT     4

#include    "CommCtrl.h" 
#pragma comment(lib, "comctl32.lib") 

TBBUTTON tbButtons[4]; 
HIMAGELIST g_hImageList = NULL; 

ShapeObjectsEditor soe; 

LRESULT __stdcall WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
            case TTN_NEEDTEXT:
                {
                    LPNMTTDISPINFO lpttdi = (LPNMTTDISPINFO)lParam;
                    switch (lpttdi->hdr.idFrom) 
                    {
                    case OBJECTRECTANGLE:
                        lstrcpy(lpttdi->szText, L"Інструмент \"Прямокунткик\"");
                        break;
                    case OBJECTELLIPSE:
                        lstrcpy(lpttdi->szText, L"Інструмент \"Еліпс\"");
                        break;
                    case OBJECTLINE:
                        lstrcpy(lpttdi->szText, L"Інструмент \"Лінія\"");
                        break;
                    case OBJECTPOINT:
                        lstrcpy(lpttdi->szText, L"Інструмент \"Крапка\"");
                        break;
                    default:
                        lstrcpy(lpttdi->szText, L"Оберіть інструмент");
                        break;
                    }
                }
            break;
        }
        break;
        case WM_PAINT:
            soe.OnPaint(hwnd); 
            break; 
        case WM_LBUTTONUP:
            soe.OnLBup(hwnd, lParam); 
            break; 
        case WM_MOUSEMOVE:
            soe.OnMouseMove(hwnd, lParam); 
            break; 
        case WM_LBUTTONDOWN:
            soe.OnLBDown(hwnd, lParam); 
            break; 
        case WM_CREATE:
            break;
        case WM_COMMAND:
            switch (wParam)
            {
                case OBJECTRECTANGLE:
                {
                    soe.StartRectEditor(hwnd, OBJECTRECTANGLE); 
                    break; 
                }
                case OBJECTELLIPSE: 
                {
                    soe.StartEllipseEditor(hwnd, OBJECTELLIPSE);
                    break;
                }
                case OBJECTLINE:
                {
                    soe.StartLineEditor(hwnd, OBJECTLINE);
                    break;
                }
                case OBJECTPOINT: 
                    soe.StartPointEditor(hwnd, OBJECTPOINT);
                    break; 
            }
            break; 
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"mainWindow";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(WS_EX_LEFT, L"mainWindow", L"Редактор фігур", WS_OVERLAPPEDWINDOW, 200, 200, 800, 400, NULL, NULL, hInstance, NULL);

    const int ImageListID = 0;
    const int numButtons = 4;
    const int bitmapSize = 16;

    const DWORD buttonStyles = BTNS_AUTOSIZE;

    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
        WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS, 0, 0, 0, 0,
        hWnd, NULL, hInstance, NULL);

    HBITMAP hBitMapRectangle = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
    HBITMAP hBitMapEllipse = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7)); 
    HBITMAP hBitMapLine = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
    HBITMAP hBitMapPoint = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP9));

    g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   
        ILC_COLOR32 | ILC_MASK,   
        numButtons, 0);

    int indexRectangle = ImageList_Add(g_hImageList, hBitMapRectangle, NULL);
    int indexEllipse = ImageList_Add(g_hImageList, hBitMapEllipse, NULL); 
    int indexLine = ImageList_Add(g_hImageList, hBitMapLine, NULL); 
    int indexPoint = ImageList_Add(g_hImageList, hBitMapPoint, NULL);

    SendMessage(hWndToolbar, TB_SETIMAGELIST,
        (WPARAM)ImageListID,
        (LPARAM)g_hImageList);


    SendMessage(hWndToolbar, TB_LOADIMAGES,
        (WPARAM)IDB_STD_SMALL_COLOR,
        (LPARAM)HINST_COMMCTRL);

    tbButtons[0] = { MAKELONG(indexRectangle,  ImageListID), OBJECTRECTANGLE,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Прямокутник" }; 
    tbButtons[1] = { MAKELONG(indexEllipse,  ImageListID), OBJECTELLIPSE,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Еліпс" }; 
    tbButtons[2] = { MAKELONG(indexLine, ImageListID), OBJECTLINE, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Ліная" }; 
    tbButtons[3] = { MAKELONG(indexPoint, ImageListID), OBJECTPOINT, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Крапка" }; 

    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
    ShowWindow(hWndToolbar, TRUE);


    ShowWindow(hWnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}