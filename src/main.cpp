#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "metrics/metrics.h"
#include <stdio.h>
#pragma comment(lib, "pdh.lib")


SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

// Функции для службы
void ServiceMain(int argc, char** argv);
void ControlHandler(DWORD request);
int InitService();

//Фукции сбора метрик
double getCPUUsage();

int main() {
    while (true) {
        double cpuUsage = getCPUUsage();
        std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
    }
    // return 0;
    // SERVICE_TABLE_ENTRY ServiceTable[1];
    // ServiceTable[0].lpServiceName = _T("Monitoring_Agent");
    // ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    //
    // StartServiceCtrlDispatcher(ServiceTable);
    return 0;
}
void ServiceMain(int argc, char** argv) {
    hStatus = RegisterServiceCtrlHandler(_T("Monitoring_Agent"), (LPHANDLER_FUNCTION)ControlHandler);

    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;

    SetServiceStatus(hStatus, &ServiceStatus);

    int result = InitService();
    if (result) {
        ServiceStatus.dwCurrentState = SERVICE_RUNNING;
        ServiceStatus.dwCheckPoint = 0;
        ServiceStatus.dwWaitHint = 0;
        SetServiceStatus(hStatus, &ServiceStatus);
    } else {
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = -1;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }

    // Основной цикл службы
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
        // Вызов функций для сбора метрик и их отправки на сервер
    }
}

void ControlHandler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
        case SERVICE_CONTROL_SHUTDOWN:
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
        default:
            break;
    }
    SetServiceStatus(hStatus, &ServiceStatus);
}

int InitService() {
    // Инициализация службы (например, настройка конфигурации)
    return 1; // Возвращает 1 при успешной инициализации
}


