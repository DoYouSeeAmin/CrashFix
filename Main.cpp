#include <windows.h>
#include <iostream>
#include <stdlib.h>

HHOOK hKeyboardHook;

void RunExplorer() {
    system("start explorer.exe");
}

void KillExplorer() {
    system("taskkill /f /im explorer.exe");
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

        // Проверяем сообщения о нажатии клавиш
        if (wParam == WM_KEYDOWN) {
            // Если нажата клавиша "+", запускаем Explorer.exe
            if (pKeyboard->vkCode == VK_ADD) {
                RunExplorer();
            }
            // Если нажата клавиша "-", завершаем процесс Explorer.exe
            else if (pKeyboard->vkCode == VK_SUBTRACT) {
                KillExplorer();
            }
            else if (((pKeyboard->vkCode == VK_RCONTROL) && (GetKeyState(VK_MULTIPLY) & 0x8000))) {
                exit(0);
            }
        }
    }
    // Передаём управление следующему хуку в цепочке
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    // Устанавливаем глобальный хук клавиатуры
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    if (hKeyboardHook == NULL) {
        return 1;
    }

    // Цикл обработки сообщений (можно использовать для фонового выполнения)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Освобождаем хук перед завершением программы
    UnhookWindowsHookEx(hKeyboardHook);

    return 0;
}
