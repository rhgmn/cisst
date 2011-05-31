/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Peter Kazanzides
  Created on: 2010-12-10

  (C) Copyright 2010 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstCommon/cmnObjectRegister.h>
#include <cisstCommon/cmnTokenizer.h>
#include <cisstInteractive/ireTask.h>
#include <cisstInteractive/ireFramework.h>

#include <cisstMultiTask/mtsInterfaceRequired.h>
#include <cisstMultiTask/mtsManagerComponentBase.h>
#include <cisstOSAbstraction/osaThreadSignal.h>   // PK TEMP

CMN_IMPLEMENT_SERVICES(ireTaskConstructorArg);

void ireTaskConstructorArg::SerializeRaw(std::ostream & outputStream) const
{
    mtsGenericObject::SerializeRaw(outputStream);
    cmnSerializeRaw(outputStream, Name);
    cmnSerializeRaw(outputStream, UseIPython);
    cmnSerializeRaw(outputStream, Startup);
}

void ireTaskConstructorArg::DeSerializeRaw(std::istream & inputStream)
{
    mtsGenericObject::DeSerializeRaw(inputStream);
    cmnDeSerializeRaw(inputStream, Name);
    cmnDeSerializeRaw(inputStream, UseIPython);
    cmnDeSerializeRaw(inputStream, Startup);
}

void ireTaskConstructorArg::ToStream(std::ostream & outputStream) const
{
    outputStream << "Name: " << Name
                 << ", UseIPython: " << UseIPython
                 << ", Startup: " << Startup << std::endl;
}

void ireTaskConstructorArg::ToStreamRaw(std::ostream & outputStream, const char delimiter,
                                        bool headerOnly, const std::string & headerPrefix) const
{
    mtsGenericObject::ToStreamRaw(outputStream, delimiter, headerOnly, headerPrefix);
    if (headerOnly) {
        outputStream << headerPrefix << "-name" << delimiter
                     << headerPrefix << "-useIPython" << delimiter
                     << headerPrefix << "-startup";
    } else {
        outputStream << this->Name << delimiter
                     << this->UseIPython << delimiter
                     << this->Startup;
    }
}

bool ireTaskConstructorArg::FromStreamRaw(std::istream & inputStream, const char delimiter)
{
    mtsGenericObject::FromStreamRaw(inputStream, delimiter);
    UseIPython = false;
    Startup.clear();
    if (inputStream.fail())
        return false;
    inputStream >> Name;
    if (inputStream.fail())
        return false;
    if (inputStream.eof())
        return (typeid(*this) == typeid(ireTaskConstructorArg));
    std::string useIPythonString;
    inputStream >> useIPythonString;
    if (inputStream.fail())
        return false;
    // Set UseIPython true if string is "1", "true", or if first 3 letters equal "ipy"
    if ((useIPythonString == "1") || (useIPythonString == "true") ||
        (useIPythonString.compare(0,3,"ipy") == 0))
        UseIPython = true;
    if (inputStream.eof())
        return (typeid(*this) == typeid(ireTaskConstructorArg));
    std::string buffer;
    getline(inputStream, buffer);
    if (inputStream.fail())
        return false;
    cmnTokenizer tokens;
    tokens.Parse(buffer);
    const char *line = tokens.GetToken(0);
    if (line)
        Startup.assign(line);
    return (typeid(*this) == typeid(ireTaskConstructorArg));
}

CMN_IMPLEMENT_SERVICES_DERIVED_ONEARG(ireTask, mtsTaskContinuous, ireTaskConstructorArg)

ireTask::ireTask(const std::string &name, bool useIPython, const std::string &startup) :
    mtsTaskContinuous(name), UseIPython(useIPython), StartupCommands(startup)
{
    EnableDynamicComponentManagement();
    SetInitializationDelay(30.0);  // Allow up to 30 seconds for it to start
}

ireTask::ireTask(const ireTaskConstructorArg &arg) :
    mtsTaskContinuous(arg.Name), UseIPython(arg.UseIPython), StartupCommands(arg.Startup)
{
    EnableDynamicComponentManagement();
    SetInitializationDelay(30.0);  // Allow up to 30 seconds for it to start
}

ireTask::~ireTask()
{
    this->Cleanup();
}

void ireTask::Startup(void)
{
    // PK TEMP: Set callbacks for osaThreadSignal::Wait
    osaThreadSignal::SetWaitCallbacks(Thread.GetId(), &ireFramework::UnblockThreads, &ireFramework::BlockThreads);

    cmnObjectRegister::Register(GetName(), this);
    std::stringstream startup;
    startup << "from cisstCommonPython import *; "
            << "from cisstMultiTaskPython import *; "
            << GetName() << " = cmnObjectRegister.FindObject('" << GetName() << "'); "
#if 0
            << "Manager = " << GetName() << ".GetInterfaceRequired('"
            << mtsManagerComponentBase::InterfaceNames::InterfaceInternalRequired << "'); "
            << "Manager.UpdateFromC(); "
#endif
            << "Manager = " << GetName() << ".GetManagerComponentServices(); "
            << StartupCommands;
    try {
        ireFramework::LaunchIREShell(startup.str().c_str(), true, UseIPython);
    }
    catch (...) {
        if (UseIPython)
            CMN_LOG_CLASS_INIT_ERROR << "Could not launch IPython shell" << std::endl;
        else
            CMN_LOG_CLASS_INIT_ERROR << "Could not launch IRE shell (wxPython)" << std::endl;
    }
    while (ireFramework::IsStarting()) {
        // Need following call to give the IRE thread some time to execute.
        ireFramework::JoinIREShell(0.1);
    }
}

void ireTask::Run(void)
{
    ProcessQueuedCommands();
    ProcessQueuedEvents();
    // Need following call to give the IRE thread some time to execute.
    ireFramework::JoinIREShell(0.1);
    if (ireFramework::IsFinished())
        Kill();
}

void ireTask::Cleanup(void)
{
    cmnObjectRegister::Remove(this->GetName());
}