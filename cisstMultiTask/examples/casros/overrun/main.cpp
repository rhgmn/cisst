/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*

  Author(s):  Min Yang Jung
  Created on: 2014-08-05

  (C) Copyright 2014 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/
// For cisst
#include <cisstCommon/cmnGetChar.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <cisstMultiTask/mtsTaskPeriodic.h>
#include <cisstMultiTask/mtsTaskManager.h>
#if (CISST_OS == CISST_LINUX_XENOMAI)
#include <sys/mman.h>
#endif

// For casros
#include "config.h"
#include "common.h"
#include "json.h"

using namespace SC;

// Periodic component
class PeriodicTask: public mtsTaskPeriodic
{
public:
    double Delay;

    PeriodicTask(const std::string & name, double period)
        : mtsTaskPeriodic(name, period, false, 5000), Delay(0.0)
    {}

    void Configure(const std::string & CMN_UNUSED(filename) = "") {}
    void Startup(void) {}
    void Run(void) {
        ProcessQueuedCommands();
        ProcessQueuedEvents();

        if (Delay > 0.0) {
            std::cout << "Periodic task sleeps for " << Delay << " seconds\n";
            osaSleep(Delay);
            Delay = 0.0;
        }

        std::cout << "." << std::flush;
    }
    void Cleanup(void) {}
};


int main(int, char *[])
{
    cmnLogger::SetMask(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskFunction(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskDefaultLog(CMN_LOG_ALLOW_ALL);
    cmnLogger::AddChannel(std::cout, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);
    //cmnLogger::SetMaskClassMatching("mtsSafetyCoordinator", CMN_LOG_ALLOW_ALL);
    //cmnLogger::SetMaskClassMatching("mtsMonitorComponent", CMN_LOG_ALLOW_ALL);
    
    // Enable casros framework
    mtsComponentManager::InstallSafetyCoordinator();

    // Get instance of local component manager
    mtsManagerLocal * ComponentManager = 0;
    try {
        ComponentManager = mtsComponentManager::GetInstance();
    } catch (std::exception & e) {
        std::cerr << "Failed to initialize local component manager: " << e.what() << std::endl;
        return 1;
    }

    // Create two test components
    PeriodicTask task("task", 1.0);
    ComponentManager->AddComponent(&task);

    ComponentManager->CreateAll();
    ComponentManager->WaitForStateAll(mtsComponentState::READY);

    ComponentManager->StartAll();
    ComponentManager->WaitForStateAll(mtsComponentState::ACTIVE);

    std::cout << "1: do 0.5 second pause\n";
    std::cout << "2: do 1.5 second pause\n";
    std::cout << "q: quit\n";

    // loop until 'q' is pressed
    int key = ' ';
    while (key != 'q') {
        key = cmnGetChar();
        if (key == '1') {
            task.Delay = 0.5;
        } else if (key == '2') {
            task.Delay = 1.5;
        }
        osaSleep(100 * cmn_ms);
    }
    std::cout << std::endl;

    // Clean up resources
    std::cout << "Cleaning up..." << std::endl;

    ComponentManager->KillAll();
    ComponentManager->WaitForStateAll(mtsComponentState::FINISHED, 2.0 * cmn_s);
    ComponentManager->Cleanup();

    return 0;
}
