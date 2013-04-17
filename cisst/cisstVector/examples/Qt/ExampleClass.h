/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2012-04-01

  (C) Copyright 2013 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#ifndef _Example_Class_h
#define _Example_Class_h

// include what is needed from cisst
#include <cisstConfig.h>
#include <cisstCommon.h>
#include <cisstVector.h>
#include <cisstVectorQt.h>

#include <QApplication.h>
#include <QVBoxLayout>

class ExampleClass: public QObject
{
    Q_OBJECT;
public:
    ExampleClass(void);

protected:
    enum {NUMBER_OF_ELEMENTS = 4};
    QFrame * frame;
    QVBoxLayout * layout;
    vctQtWidgetDynamicVectorDoubleRead * dynamicVectorDoubleRead;
    vctQtWidgetDynamicVectorDoubleWrite * dynamicVectorDoubleWriteText;
    vctQtWidgetDynamicVectorDoubleWrite * dynamicVectorDoubleWriteSpinBox;
    vctQtWidgetDynamicVectorDoubleWrite * dynamicVectorDoubleWriteSlider;
    vctQtWidgetDynamicVectorIntRead * dynamicVectorIntRead;
    vctQtWidgetDynamicVectorIntWrite * dynamicVectorIntWriteText;
    vctQtWidgetDynamicVectorIntWrite * dynamicVectorIntWriteSpinBox;
    vctQtWidgetDynamicVectorIntWrite * dynamicVectorIntWriteSlider;
    vctQtWidgetRotationDoubleRead * rotationDoubleRead;
protected slots:
    void DoubleTextValueChangedSlot(void);
    void DoubleSpinBoxValueChangedSlot(void);
    void DoubleSliderValueChangedSlot(void);
    void IntTextValueChangedSlot(void);
    void IntSpinBoxValueChangedSlot(void);
    void IntSliderValueChangedSlot(void);
};

#endif // _Example_Class_h
