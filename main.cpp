#include "TaskManager.h"
#include "resources.h"
#include <windows.h>
#include <string>

TaskManager manager;

const char CLASS_NAME[] = "TaskManager"; // Nome della classe della finestra principale

// Prototipi delle funzioni
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void UpdateTaskList(HWND hwndList);
void ShowAddTaskDialog(HWND hwnd);
INT_PTR CALLBACK AddTaskDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Funzione principale WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Registrazione della classe della finestra principale
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    // Creazione della finestra principale
    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Task Manager",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    // Mostra e aggiorna la finestra principale
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Loop principale dei messaggi
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Funzione di callback per la finestra principale
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            AddControls(hwnd); // Aggiungi i controlli alla finestra principale al momento della creazione
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 2: // Pulsante "Add Task"
                    ShowAddTaskDialog(hwnd); // Mostra la finestra di dialogo per l'aggiunta del compito
                    break;

                case 3: // Pulsante "Complete Task"
                {
                    HWND hwndList = GetDlgItem(hwnd, 1); // Ottieni l'handle della lista dei compiti
                    int index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                    if (index != LB_ERR) {
                        int taskId = SendMessage(hwndList, LB_GETITEMDATA, index, 0);
                        manager.completeTask(taskId); // Completa il compito selezionato
                        UpdateTaskList(hwndList); // Aggiorna la lista dei compiti
                    }
                    break;
                }

                case 4: // Pulsante "Delete Task"
                {
                    HWND hwndList = GetDlgItem(hwnd, 1); // Ottieni l'handle della lista dei compiti
                    int index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                    if (index != LB_ERR) {
                        int taskId = SendMessage(hwndList, LB_GETITEMDATA, index, 0);
                        manager.deleteTask(taskId); // Elimina il compito selezionato
                        UpdateTaskList(hwndList); // Aggiorna la lista dei compiti
                    }
                    break;
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0); // Termina il loop dei messaggi quando la finestra viene distrutta
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Aggiungi i controlli alla finestra principale
void AddControls(HWND hwnd) {
    // Etichetta per la lista dei compiti
    CreateWindowW(L"Static", L"Task List", WS_VISIBLE | WS_CHILD, 10, 10, 80, 20, hwnd, nullptr, nullptr, nullptr);

    // Lista dei compiti
    HWND hwndList = CreateWindowW(L"ListBox", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 10, 30, 470, 250, hwnd, (HMENU)1, nullptr, nullptr);

    // Pulsanti
    CreateWindowW(L"Button", L"Add Task", WS_VISIBLE | WS_CHILD, 10, 290, 80, 25, hwnd, (HMENU)2, nullptr, nullptr);
    CreateWindowW(L"Button", L"Complete Task", WS_VISIBLE | WS_CHILD, 100, 290, 120, 25, hwnd, (HMENU)3, nullptr, nullptr);
    CreateWindowW(L"Button", L"Delete Task", WS_VISIBLE | WS_CHILD, 230, 290, 100, 25, hwnd, (HMENU)4, nullptr, nullptr);

    // Aggiorna la lista dei compiti
    UpdateTaskList(hwndList);
}

// Aggiorna la lista dei compiti nella finestra principale
void UpdateTaskList(HWND hwndList) {
    SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
    const auto& tasks = manager.getTasks();
    for (const auto& task : tasks) {
        std::wstring taskDesc = std::to_wstring(task.id) + L": " + std::wstring(task.description.begin(), task.description.end()) + (task.completed ? L" (Completed)" : L"");
        SendMessage(hwndList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(taskDesc.c_str()));
    }
}

// Mostra la finestra di dialogo per l'aggiunta di un compito
void ShowAddTaskDialog(HWND hwnd) {
    wchar_t buffer[256] = {0}; // Buffer per il testo del compito

    // Mostra la finestra di dialogo per l'aggiunta del compito
    if (DialogBoxParamW(nullptr, MAKEINTRESOURCEW(IDD_ADDTASK), hwnd, AddTaskDialogProc, reinterpret_cast<LPARAM>(buffer))  == IDOK) {
        // Gestisci il testo del compito qui
        std::wstring taskDesc = buffer;
        if (!taskDesc.empty()) {
            manager.addTask(std::string(taskDesc.begin(), taskDesc.end())); // Aggiungi il compito al manager
            HWND hwndList = GetDlgItem(hwnd, 1); // Ottieni l'handle della lista dei compiti
            UpdateTaskList(hwndList); // Aggiorna la lista dei compiti nella finestra principale
        }
    }
}

// Finestra di dialogo procedura di callback per l'aggiunta di un compito
INT_PTR CALLBACK AddTaskDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static wchar_t buffer[256] = {0}; // Buffer per il testo del compito

    switch (message) {
        case WM_INITDIALOG:
            SetFocus(GetDlgItem(hDlg, IDC_TASKDESCRIPTION)); // Imposta il focus sul campo di testo per la descrizione del compito
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK:
                    GetDlgItemTextW(hDlg, IDC_TASKDESCRIPTION, buffer, 256); // Ottieni il testo inserito
                    EndDialog(hDlg, IDOK); // Chiudi la finestra di dialogo con IDOK
                    return TRUE;

                case IDCANCEL:
                    EndDialog(hDlg, IDCANCEL); // Chiudi la finestra di dialogo con IDCANCEL
                    return TRUE;
            }
            break;
    }

    return FALSE; // Restituisci FALSE per indicare che il messaggio non è stato gestito
}
