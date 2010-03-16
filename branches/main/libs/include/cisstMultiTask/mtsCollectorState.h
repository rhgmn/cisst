/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Min Yang Jung, Anton Deguet
  Created on: 2009-03-20

  (C) Copyright 2009-2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#ifndef _mtsCollectorState_h
#define _mtsCollectorState_h

#include <cisstMultiTask/mtsTaskPeriodic.h>
#include <cisstMultiTask/mtsCollectorBase.h>
#include <cisstMultiTask/mtsHistory.h>
#include <cisstMultiTask/mtsCommandVoid.h>
#include <cisstMultiTask/mtsStateTable.h>

#include <string>

// Always include last
#include <cisstMultiTask/mtsExport.h>

/*!
  \ingroup cisstMultiTask

  This class provides a way to collect data in the state table without
  loss and make a log file. The type of a log file can be plain text
  (ascii), csv, or binary.  A state table of which data is to be
  collected can be specified in the constructor.  This is intended for
  future usage where a task can have more than two state tables.
*/
class CISST_EXPORT mtsCollectorState : public mtsCollectorBase
{
    friend class mtsCollectorStateTest;
    friend class mtsStateTable;

    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_LOD_RUN_ERROR);

    /*! Structure and container definition to manage the list of signals to be
        collected by this collector. */
    typedef struct {
        std::string Name;
        unsigned int ID;
    } SignalElement;

    typedef std::vector<SignalElement> RegisteredSignalElementType;
    RegisteredSignalElementType RegisteredSignalElements;

    /*! Offset for a sampling stride. */
    unsigned int OffsetForNextRead;

    /*! Data index which should be read at the next time. */
    int LastReadIndex;

    /*! Local copy to reduce the number of reference in Collect() method. */
    unsigned int TableHistoryLength;

    /*! Flag to determine is the state collector is connected.  Once
      the state collector is connected, it becomes impossible to
      change to task/state table being collected. */
    bool ConnectedFlag;

    /*! Flag for PrintHeader() method. */
    bool FirstRunningFlag;

    /*! A stride value for data collector to skip several records. */
    unsigned int SamplingInterval;

    /*! Output file name. */
    std::string OutputFileName;

    /*! Pointer on output stream, can be create and managed by this
      class or provided by a user. */
    std::ostream * OutputStream;
    std::ofstream * OutputFile;

    /*! Delimiter used in a log file. Set by the constructor according
      to mtsCollectorBase::CollectorLogFormat. */
    char Delimiter;

    /*! Pointers to the target task and the target state table. */
    mtsTask * TargetTask;
    mtsStateTable * TargetStateTable;

    /*! String stream buffer for serialization. */
    std::stringstream StringStreamBufferForSerialization;

    /*! Serializer for binary logging. DeSerializer is used only at
      ConvertBinaryToText() method so we don't define it here. */
    cmnSerializer * Serializer;

    /*! Thread-related methods */
    void Run(void);

    void Startup(void);

    /*! Initialization */
    void Initialize(void);

    mtsFunctionWrite StateTableStartCollection;
    mtsFunctionWrite StateTableStopCollection;

    /*! Check if the signal specified by a user has been already registered.
        This is to avoid duplicate signal registration. */
    bool IsRegisteredSignal(const std::string & signalName) const;

    /*! Add a signal element. Called internally by mtsCollectorState::AddSignal(). */
    bool AddSignalElement(const std::string & signalName, const unsigned int signalID);

    /*! Fetch state table data */
    bool FetchStateTableData(const mtsStateTable * table,
                             const unsigned int startIdx,
                             const unsigned int endIdx);

    /*! Update the delimiter used in output files based on file
      format.  Should be used everytime FileFormat is set. */
    void SetDelimiter(void);

    /*! Print out the signal names which are being collected. */
    void PrintHeader(const CollectorFileFormat & fileFormat);

    /*! Mark the end of the header. Called in case of binary log file. */
    void MarkHeaderEnd(std::ostream & logFile);

    /*! Check if the given buffer contains the header mark. */
    static bool IsHeaderEndMark(const char * buffer);

    /*! When this function is called (called by the data thread as a form of an event),
        bulk-fetch is performed and data is dumped to a log fie. */
    // void DataCollectionEventHandler(); // performs thread wake up
    void BatchReadyHandler(const mtsStateTable::IndexRange & range); // performs collect, for implementation as continuous, no sleep

    /*! Fetch bulk data from StateTable. */
    void BatchCollect(const mtsStateTable::IndexRange & range);

public:
    /*! Constructor using the task name and table name. */
    mtsCollectorState(const std::string & collectorName);

    /*! Constructor using a task pointer and table name. */
    mtsCollectorState(const std::string & targetTaskName,
                      const std::string & targetStateTableName,
                      const CollectorFileFormat fileFormat);

    ~mtsCollectorState(void);

    /*! Define the output file and format.  If a file is already in
      use, this method will close the current one. */
    void SetOutput(const std::string & fileName, const CollectorFileFormat fileFormat);

    /*! Define the output using an existing ostream, the collector
      will not open nor close the stream. */
    void SetOutput(std::ostream & outputStream, const CollectorFileFormat fileFormat);

    /*! Define the output using an existing ostream, the collector
      will not open nor close the stream.  If this method is called
      for the first time, the format will be
      COLLECTOR_FILE_FORMAT_CSV, otherwise it will use the previously
      used format. */
    void SetOutput(std::ostream & outputStream);

    /*! Creates a default file name using the task name, table name
      and date.  The suffix depends on the file format. */
    void SetOutputToDefault(const CollectorFileFormat fileFormat);

    /*! Creates a default file name using the task name, table name
      and date.  If this method is called for the first time, the
      format will be COLLECTOR_FILE_FORMAT_CSV, otherwise it will use
      the previously used format. */
    void SetOutputToDefault(void);

    /*! Defines which table to collect data from.  This is defined by
        the task name and the table name.  If the table name is not
        provided, the collector will use the default task's state
        table. */
    bool SetStateTable(const std::string & taskName,
                       const std::string & stateTableName = "");

    /*! Add the signal specified to a list of registered signals. */
    bool AddSignal(const std::string & signalName = "");

    /*! Set a sampling interval so that data collector can skip
      several values.  This is useful when a frequency of the task is
      somewhat high and you don't want to collect ALL data from it. */
    void SetSamplingInterval(const unsigned int samplingInterval) {
        SamplingInterval = (samplingInterval > 0 ? samplingInterval : 1);
    }

    /*! Connect.  Once the state collector has been configured,
      i.e. the methods SetStateTable and SetOutput have been use,
      the collector should be added to the manager and then the
      Connect method should be called. */
    bool Connect(void);

    /*! Convert a binary log file into a plain text one. */
    static bool ConvertBinaryToText(const std::string sourceBinaryLogFileName,
                                    const std::string targetPlainTextLogFileName,
                                    const char delimiter = ',');

    /*! Get the name of log file currently being written. */
    inline const std::string & GetOutputFileName(void) const {
        return this->OutputFileName;
    }

    /*! Methods defined as virtual in base class to control stop/start
      collection with delay.  For the state table collection, these
      methods are mainly pass through, i.e. they call the
      corresponding commands from the state table task.  */
    //@{
    void StartCollection(const mtsDouble & delayInSeconds);
    void StopCollection(const mtsDouble & delayInSeconds);
    //@}
};

CMN_DECLARE_SERVICES_INSTANTIATION(mtsCollectorState)

#endif // _mtsCollectorState_h
