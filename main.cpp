#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <pdh.h>
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

    return;
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

// Функции для сбора метрик

double getCPUUsage()
    {
        PDH_STATUS status;
        PDH_HQUERY query;
        PDH_HCOUNTER counter;
        PDH_FMT_COUNTERVALUE value;

        // Open a query object.
        status = PdhOpenQuery(nullptr, 0, &query);
        if (status != ERROR_SUCCESS) {
            std::cerr << "PdhOpenQuery failed with error code " << status << std::endl;
            return -1.0;
        }

        // Add counter to the query.
        status = PdhAddEnglishCounter(query, "\\Processor(_Total)\\% Processor Time",
            0, &counter);
        if (status != ERROR_SUCCESS) {
            std::cerr << "PdhAddCounter failed with error code " << status << std::endl;
            PdhCloseQuery(query);
            return -1.0;
        }

        // Collect data for the counter.
        status = PdhCollectQueryData(query);
        if (status != ERROR_SUCCESS) {
            std::cerr << "PdhCollectQueryData failed with error code " << status << std::endl;
            PdhCloseQuery(query);
            return -1.0;
        }

        // Wait for one second to get a valid value.
        Sleep(100);

        // Collect data again to get the current value.
        status = PdhCollectQueryData(query);
        if (status != ERROR_SUCCESS) {
            std::cerr << "PdhCollectQueryData failed with error code " << status << std::endl;
            PdhCloseQuery(query);
            return -1.0;
        }

        // Get the value of the counter.
        status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, nullptr, &value);
        if (status != ERROR_SUCCESS) {
            std::cerr << "PdhGetFormattedCounterValue failed with error code " << status << std::endl;
            PdhCloseQuery(query);
            return -1.0;
        }

        // Close the query and return the CPU usage percentage.
        PdhCloseQuery(query);
        return value.doubleValue;
    }


double getMemoryUsage() {
    // Реализация сбора метрики использования памяти
    return 0.0;
}