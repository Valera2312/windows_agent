//
// Created by Valery on 08.12.2024.
//
#include <pdh.h>
#include <iostream>
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