/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: $
  
  Author(s):  Balazs Vagvolgyi
  Created on: 2010

  (C) Copyright 2006-2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#ifndef _svlSampleImageTypes_h
#define _svlSampleImageTypes_h

#include <cisstMultitask/mtsGenericObjectProxy.h>
#include <cisstStereoVision/svlSampleImageCustom.h>

// Always include last!
#include <cisstStereoVision/svlExport.h>


typedef svlSampleImageCustom<unsigned char,  1, 1>   svlSampleImageMono8;
typedef svlSampleImageCustom<unsigned char,  1, 2>   svlSampleImageMono8Stereo;
typedef svlSampleImageCustom<unsigned short, 1, 1>   svlSampleImageMono16;
typedef svlSampleImageCustom<unsigned short, 1, 2>   svlSampleImageMono16Stereo;
typedef svlSampleImageCustom<unsigned char,  3, 1>   svlSampleImageRGB;
typedef svlSampleImageCustom<unsigned char,  4, 1>   svlSampleImageRGBA;
typedef svlSampleImageCustom<unsigned char,  3, 2>   svlSampleImageRGBStereo;
typedef svlSampleImageCustom<unsigned char,  4, 2>   svlSampleImageRGBAStereo;
typedef svlSampleImageCustom<float,          3, 1>   svlSampleImage3DMap;

CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono8)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono8Stereo)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono16)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono16Stereo)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGB)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGBA)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGBStereo)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGBAStereo)
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImage3DMap)

typedef mtsGenericObjectProxy<svlSampleImageMono8> svlSampleImageMono8Proxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono8Proxy);
typedef mtsGenericObjectProxy<svlSampleImageMono8Stereo> svlSampleImageMono8StereoProxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono8StereoProxy);
typedef mtsGenericObjectProxy<svlSampleImageMono16> svlSampleImageMono16Proxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono16Proxy);
typedef mtsGenericObjectProxy<svlSampleImageMono16Stereo> svlSampleImageMono16StereoProxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageMono16StereoProxy);
typedef mtsGenericObjectProxy<svlSampleImageRGB> svlSampleImageRGBProxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGBProxy);
typedef mtsGenericObjectProxy<svlSampleImageRGBA> svlSampleImageRGBAProxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGBAProxy);
typedef mtsGenericObjectProxy<svlSampleImageRGBStereo> svlSampleImageRGBStereoProxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGBStereoProxy);
typedef mtsGenericObjectProxy<svlSampleImageRGBAStereo> svlSampleImageRGBAStereoProxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImageRGBAStereoProxy);
typedef mtsGenericObjectProxy<svlSampleImage3DMap> svlSampleImage3DMapProxy;
CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlSampleImage3DMapProxy);

#endif // _svlSampleImageTypes_h

