#include <memory>
#define NOMINIMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fast_io.h>

// 定义触控输入上限
#define MAX_TOUCH_POINTS 10

// 存储触控点的信息
TOUCHINPUT touchInputs[MAX_TOUCH_POINTS];

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    using namespace fast_io::io;
    auto currenttimestamp{::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::realtime)};
    auto localiso8601{local(currenttimestamp)};
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_LBUTTONDOWN: // 处理鼠标左键按下事件
            perr(std::source_location::current(),"\t",localiso8601,": WM_LBUTTONDOWN\n");
            return 0;
        case WM_LBUTTONUP: // 处理鼠标左键按下事件
            perr(std::source_location::current(),"\t",localiso8601,": WM_LBUTTONUP\n");
            return 0;
        case WM_RBUTTONDOWN: // 处理鼠标右键按下事件
            perr(std::source_location::current(),"\t",localiso8601,": WM_RBUTTONDOWN\n");
            return 0;
        case WM_KEYDOWN: // 处理键盘按下事件
            if (wParam == VK_ESCAPE) { // 如果按下Esc键
                PostQuitMessage(0);
            }
            return 0;
        case WM_POINTERDOWN: // 处理指针按下事件
            perr(std::source_location::current(),"\t",localiso8601,": WM_POINTERDOWN\n");
            return 0;
        case WM_POINTERUP: // 处理指针抬起事件
            perr(std::source_location::current(),"\t",localiso8601,": WM_POINTERUP\n");
            return 0;
        case WM_POINTERUPDATE: // 处理指针移动事件
            perr(std::source_location::current(),"\t",localiso8601,": WM_POINTERUPDATE\n");
            return 0;
        case WM_TOUCH: // 处理触控事件
        {
            UINT cInputs = LOWORD(wParam);
            perrln(std::source_location::current(),"\t",localiso8601,": WM_TOUCH: ", cInputs);
            ::std::unique_ptr<TOUCHINPUT[]> pInputs(new TOUCHINPUT[cInputs]);
            if (GetTouchInputInfo((HTOUCHINPUT)lParam, cInputs, pInputs.get(), sizeof(TOUCHINPUT))) {
                for (UINT i = 0; i != cInputs; ++i) {
                    TOUCHINPUT ti = pInputs[i];
                    if (ti.dwFlags & TOUCHEVENTF_DOWN) {
                        perrln("\t",i,"\tTOUCHEVENTF_DOWN :\tpinputs[i].dwID:",ti.dwID," pinputs[i].x:",ti.x," pinputs[i].y:",ti.y);
                        // 处理触控按下事件
                    }
                    if (ti.dwFlags & TOUCHEVENTF_UP) {
                        perrln("\t",i,"\tTOUCHEVENTF_UP ",i,":\tpinputs[i].dwID:",ti.dwID," pinputs[i].x:",ti.x," pinputs[i].y:",ti.y);
                        // 处理触控抬起事件
                    }
                    if (ti.dwFlags & TOUCHEVENTF_MOVE) {
                        perrln("\t",i,"\tTOUCHEVENTF_MOVE ",i,":\tpinputs[i].dwID:",ti.dwID," pinputs[i].x:",ti.x," pinputs[i].y:",ti.y);
                        // 处理触控移动事件
                    }
                }
                perr("\n\n");
                CloseTouchInputHandle((HTOUCHINPUT)lParam);
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Sample Window Class";

    // 注册窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Learn to Program Windows",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // 启用触控功能
//    RegisterTouchWindow(hwnd, 0);

    // 运行消息循环
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
