/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*

  Author(s):  Min Yang Jung
  Created on: 2010-01-20

  (C) Copyright 2010-2012 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include <cisstCommon/cmnPortability.h>
#include <cisstCommon/cmnLogger.h>
#include <cisstCommon/cmnGetChar.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <cisstMultiTask/mtsManagerGlobal.h>
#include <cisstMultiTask/mtsManagerLocal.h>

#if (CISST_OS == CISST_LINUX_XENOMAI)
#include <sys/mman.h>
#endif

int main(int CMN_UNUSED(argc), char ** CMN_UNUSED(argv))
{
#if (CISST_OS == CISST_LINUX_XENOMAI)
    mlockall(MCL_CURRENT|MCL_FUTURE);
#endif

    // log configuration
    cmnLogger::SetMask(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskFunction(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskDefaultLog(CMN_LOG_ALLOW_ALL);
    cmnLogger::AddChannel(std::cout, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);
    // specify a higher, more verbose log level for these classes
    cmnLogger::SetMaskClassMatching("mts", CMN_LOG_ALLOW_ALL);
    // enable system-wide thread-safe logging
    mtsManagerLocal::SetLogForwarding(true);

    // Create and start global component manager
    mtsManagerGlobal * globalComponentManager = new mtsManagerGlobal;
    if (!globalComponentManager->StartServer()) {
        CMN_LOG_INIT_ERROR << "Failed to start global component manager." << std::endl;
        return 1;
    }
    CMN_LOG_INIT_VERBOSE << "Global component manager started..." << std::endl;

#if CISST_HAS_SAFECASS_EXT
    mtsManagerLocal::InstallSafetyCoordinator();
#endif
    // Get local component manager instance
    mtsManagerLocal * componentManager;
    try {
        componentManager = mtsManagerLocal::GetInstance(*globalComponentManager);
    } catch (...) {
        CMN_LOG_INIT_ERROR << "Failed to initialize local component manager" << std::endl;
        return 1;
    }

    // create the tasks, i.e. find the commands
    componentManager->CreateAll();
    componentManager->WaitForStateAll(mtsComponentState::READY);

    // start the periodic Run
    componentManager->StartAll();
    componentManager->WaitForStateAll(mtsComponentState::ACTIVE);

    // loop until 'q' is pressed
    int key = ' ';
    std::cout << "Press 'q' to quit" << std::endl;
    std::cout << "Press 't' to print time synchronization statistics" << std::endl;
    while (key != 'q') {
        key = cmnGetChar();
        if (key == 't') {
            std::vector<std::string> procNames;
            std::vector<double> offsets;
            if (!componentManager->GetGCMProcTimeSyncInfo(procNames,offsets)) {
               std::cout<< " Could not retrieve process time sync info " <<std::endl; 
            }
            else {

                std::cout<< " The system has " <<procNames.size() << " processes "<<std::endl; 
                if (procNames.size() == 0)
                      continue;

                std::cout<< "  -- n --  Process Name : time offset wrt GCM " <<std::endl; 
                for (unsigned int i = 0; i < procNames.size(); i++) {
                        std::cout << "  -- "<< i << " -- " <<procNames[i] << " : "
                        <<std::setprecision(8) <<std::setprecision(4) << std::setiosflags(std::ios::fixed | std::ios::showpoint)<< offsets[i] * 1000.0 << " milliSeconds"
                                              << std::endl;
                }
            }
        }
            
    }
    std::cout << "Quitting ..." << std::endl;

    // Cleanup global component manager
    if (!globalComponentManager->StopServer()) {
        CMN_LOG_RUN_ERROR << "Failed to stop global component manager." << std::endl;
    }

    // Cleanup local component manager
    componentManager->KillAll();
    componentManager->WaitForStateAll(mtsComponentState::FINISHED, 5.0 * cmn_s);
    componentManager->Cleanup();

    return 0;
}
