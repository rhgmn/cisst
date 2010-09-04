/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: CounterEvenComponent.h 

  Author(s):  Min Yang Jung
  Created on: 2010-09-01

  (C) Copyright 2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstCommon/cmnConstants.h>
#include <cisstOSAbstraction/osaGetTime.h>

#include "CounterEvenComponent.h"

CMN_IMPLEMENT_SERVICES(CounterEvenComponent);

CounterEvenComponent::CounterEvenComponent() : 
    mtsTaskPeriodic("CounterEvenObject", 1 * cmn_s, false, 1000)
{
}

CounterEvenComponent::CounterEvenComponent(const std::string & componentName, double period):
    mtsTaskPeriodic(componentName, period, false, 1000)
{
}

void CounterEvenComponent::Run(void) 
{
    ProcessQueuedCommands();

    static unsigned int counter = 0;

    if (counter++ % 2 == 0) {
        std::cout << this->GetName() << ") " << counter << std::endl;
        std::flush(std::cout);
    }
}

