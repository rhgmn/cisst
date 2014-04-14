/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2012-07-09

  (C) Copyright 2012-2013 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#pragma once

#ifndef _vctDataFunctionsTransformationsJSON_h
#define _vctDataFunctionsTransformationsJSON_h

#include <cisstVector/vctDataFunctionsFixedSizeVectorJSON.h>
#include <cisstVector/vctDataFunctionsFixedSizeMatrixJSON.h>

#if CISST_HAS_JSON
template <class _rotationType>
void
cmnDataToJSON(const vctFrameBase<_rotationType> & data,
              Json::Value & jsonValue)
{
    cmnDataToJSON(data.Translation(), jsonValue["Translation"]);
    cmnDataToJSON(data.Rotation(), jsonValue["Rotation"]);
}

template <class _rotationType>
void
cmnDataFromJSON(vctFrameBase<_rotationType> & data,
                const Json::Value & jsonValue)
    throw (std::runtime_error)
{
    cmnDataFromJSON(data.Translation(), jsonValue["Translation"]);
    cmnDataFromJSON(data.Rotation(), jsonValue["Rotation"]);
}
#endif // CISST_HAS_JSON

#endif // _vctDataFunctionsTransformationsJSON_h
