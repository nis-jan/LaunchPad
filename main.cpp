#include "mainwindow.h"
#include <iostream>
#include <string>
#include <QApplication>

#include <thread>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>

#include <windows.h>
#include <shellapi.h>

#define _WIN32_WINNT 0x050

using namespace std;


static bool winShown = false;
static bool runFlag  = true;

HHOOK hhkLowLevelKybd;


MainWindow * p_w;

void showWindow();


LRESULT keyboardcallback(int nCode, WPARAM wParam, LPARAM lParam){
    static bool winKeyDown;

    if (nCode == HC_ACTION){
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        switch(wParam){
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            cout << "key down" << endl;
            if (p->vkCode == VK_ESCAPE){
                cout << "ESCAPE" << endl;
                p_w->EscapeWindow();
                winShown = false;
            }
            if (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN) winKeyDown = true;
            if (p->vkCode == VK_SPACE && winKeyDown)showWindow();
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN) winKeyDown = false;
            break;
        }
    }


    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void showWindow(){
    if (winShown) return;
    winShown = true;

    QMetaObject::invokeMethod(p_w, "show", Qt::QueuedConnection);
    QMetaObject::invokeMethod(p_w, "raise", Qt::QueuedConnection);
    QMetaObject::invokeMethod(p_w, "activateWindow", Qt::QueuedConnection);
    cout << "activating launchpad" << endl;
}

void keybdThreadFn(){
    cout << "listening on keyboard..." << endl;
    hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardcallback, 0, 0);
    MSG msg;
    while ((!GetMessage(&msg, NULL, NULL, NULL))&&runFlag) {    //this while loop keeps the hook
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}

void winHiddenCallback(std::string prompt){
    winShown = false;
    cout << "Prompt: " << prompt << endl;
    int index = prompt.find(" ");
    string query = prompt.substr(index+1);
    stringstream ss(query);
    string segment;

    if (prompt.find("YouTube")!= string::npos){
        wstring url = L"https://www.youtube.com/results?search_query=";
        while(getline(ss, segment, ' ')) url += wstring(segment.begin(), segment.end()) + L"+";
        ShellExecute(0, 0, url.c_str(), 0, 0 , SW_SHOW );
    }
    if (prompt.find("Web") != string::npos){
        wstring url = L"https://www.google.com/search?q=";
        while(getline(ss, segment, ' ')) url += wstring(segment.begin(), segment.end()) + L"+";
        ShellExecute(0, 0, url.c_str(), 0, 0 , SW_SHOW );
    }
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.winHiddenCallback = winHiddenCallback;
    p_w = &w;


    std::thread keybdThread(keybdThreadFn);

    cout << "launchpad started..." << endl;

    int ret = a.exec();
    UnhookWindowsHookEx(hhkLowLevelKybd);
    cout << "launchpad stopped" << endl;
    return ret;
}
