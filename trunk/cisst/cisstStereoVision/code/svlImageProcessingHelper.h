/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$
  
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

#ifndef _svlImageProcessingHelper_h
#define _svlImageProcessingHelper_h

#include <cisstStereoVision/svlTypes.h>
#include <cisstStereoVision/svlTypes.h>
#include <cisstVector/vctFixedSizeMatrixTypes.h>
#include <cisstVector/vctFixedSizeVectorTypes.h>
#include <cisstVector/vctDynamicMatrixTypes.h>
#include <string>


class svlImageProcessingInternals
{
protected:
    // Protected default constructor:
    //   Class can only be instantiated by derived class
    svlImageProcessingInternals();
    
private:
    // Private copy constructor:
    //   No one else will be able to call it
    svlImageProcessingInternals(const svlImageProcessingInternals& internals);
    
public:
    // Virtual destructor:
    //   Will force the compiler to create the virtual function table
    virtual ~svlImageProcessingInternals();
};


namespace svlImageProcessingHelper
{
    /////////////////
    // Convolution //
    /////////////////

    void ConvolutionRGB(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal, bool absres);
    void ConvolutionRGBA(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal, bool absres);
    void ConvolutionMono8(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal, bool absres);
    void ConvolutionMono16(unsigned short* input, unsigned short* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal, bool absres);
    void ConvolutionMono32(unsigned int* input, unsigned int* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal, bool absres);

    void ConvolutionRGB(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicMatrix<int> & kernel, bool absres);
    void ConvolutionRGBA(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicMatrix<int> & kernel, bool absres);
    void ConvolutionMono8(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicMatrix<int> & kernel, bool absres);
    void ConvolutionMono16(unsigned short* input, unsigned short* output, const int width, const int height, vctDynamicMatrix<int> & kernel, bool absres);
    void ConvolutionMono32(unsigned int* input, unsigned int* output, const int width, const int height, vctDynamicMatrix<int> & kernel, bool absres);

    //////////////
    // Resizing //
    //////////////

    void ResampleMono8(unsigned char* src, const unsigned int srcwidth, const unsigned int srcheight,
                       unsigned char* dst, const unsigned int dstwidth, const unsigned int dstheight);
    void ResampleAndInterpolateHMono8(unsigned char* src, const unsigned int srcwidth,
                                      unsigned char* dst, const unsigned int dstwidth,
                                      const unsigned int height);
    void ResampleAndInterpolateVMono8(unsigned char* src, const unsigned int srcheight,
                                      unsigned char* dst, const unsigned int dstheight,
                                      const unsigned int width);
    void ResampleRGB24(unsigned char* src, const unsigned int srcwidth, const unsigned int srcheight,
                       unsigned char* dst, const unsigned int dstwidth, const unsigned int dstheight);
    void ResampleAndInterpolateHRGB24(unsigned char* src, const unsigned int srcwidth,
                                      unsigned char* dst, const unsigned int dstwidth,
                                      const unsigned int height);
    void ResampleAndInterpolateVRGB24(unsigned char* src, const unsigned int srcheight,
                                      unsigned char* dst, const unsigned int dstheight,
                                      const unsigned int width);

    ///////////////////
    // Deinterlacing //
    ///////////////////

    void DeinterlaceBlending(unsigned char* buffer, const unsigned int width, const unsigned int height);
    void DeinterlaceDiscarding(unsigned char* buffer, const unsigned int width, const unsigned int height);
    void DeinterlaceAdaptiveBlending(unsigned char* buffer, const unsigned int width, const unsigned int height);
    void DeinterlaceAdaptiveDiscarding(unsigned char* buffer, const unsigned int width, const unsigned int height);

    ///////////////////
    // Rectification //
    ///////////////////

    class RectificationInternals : public svlImageProcessingInternals
    {
    public:
        RectificationInternals();
        virtual ~RectificationInternals();

        bool Generate(unsigned int width, unsigned int height,const svlSampleCameraGeometry & geometry, unsigned int cam_id = SVL_LEFT);
        bool Load(const std::string &filepath, int exponentlen = 3);
        bool SetFromCameraCalibration(unsigned int height,unsigned int width,vct3x3 R,vct2 f, vct2 c, vctFixedSizeVector<double,7> k, double alpha, unsigned int videoch=0);
        void TransposeLUTArray2(unsigned int* index, unsigned int size, unsigned int width, unsigned int height);

        unsigned int Width;
        unsigned int Height;
        unsigned int* idxDest;
        int idxDestSize;
        unsigned int* idxSrc1;
        int idxSrc1Size;
        unsigned int* idxSrc2;
        int idxSrc2Size;
        unsigned int* idxSrc3;
        int idxSrc3Size;
        unsigned int* idxSrc4;
        int idxSrc4Size;
        unsigned char* blendSrc1;
        int blendSrc1Size;
        unsigned char* blendSrc2;
        int blendSrc2Size;
        unsigned char* blendSrc3;
        int blendSrc3Size;
        unsigned char* blendSrc4;
        int blendSrc4Size;

    protected:
        int LoadLine(std::ifstream &file, double* dblbuf, char* chbuf, unsigned int size, int explen);
        void TransposeLUTArray(unsigned int* index, unsigned int size, unsigned int width, unsigned int height);
        void Release();
    };

    //////////////
    // Exposure //
    //////////////

    class ExposureInternals : public svlImageProcessingInternals
    {
    public:
        ExposureInternals();

        void SetBrightness(double brightness);
        double GetBrightness();
        void SetContrast(double contrast);
        double GetContrast();
        void SetGamma(double gamma);
        double GetGamma();

        void CalculateCurve();

        vctFixedSizeVector<unsigned char, 255> Curve;

    private:
        double Brightness;
        double Contrast;
        double Gamma;
        bool Modified;
    };

    //////////////////
    // BlobDetector //
    //////////////////

    class BlobDetectorInternals : public svlImageProcessingInternals
    {
    public:
        BlobDetectorInternals();

        void SetFilterArea();
        void SetFilterCompactness();

        unsigned int CalculateLabels(const svlSampleImageMono8* image,
                                     svlSampleImageMono32* labels);
        unsigned int CalculateLabels(const svlSampleImageMono8Stereo* image,
                                     svlSampleImageMono32Stereo* labels,
                                     const unsigned int videoch);
        bool GetBlobs(const svlSampleImageMono8* image,
                      const svlSampleImageMono32* labels,
                      svlSampleBlobs* blobs,
                      unsigned int min_area,
                      unsigned int max_area,
                      double min_compactness,
                      double max_compactness);
        bool GetBlobs(const svlSampleImageMono8Stereo* image,
                      const svlSampleImageMono32Stereo* labels,
                      svlSampleBlobs* blobs,
                      const unsigned int videoch,
                      unsigned int min_area,
                      unsigned int max_area,
                      double min_compactness,
                      double max_compactness);

    protected:
        unsigned int  BlobCount;
        vctDynamicMatrix<int> ContourBuffer;

        unsigned int CalculateLabelsInternal(svlSampleImage* image,
                                             svlSampleImage* labels,
                                             const unsigned int videoch);
        bool GetBlobsInternal(svlSampleImage* image,
                              svlSampleImage* labels,
                              svlSampleBlobs* blobs,
                              const unsigned int videoch,
                              const unsigned int min_area,
                              const unsigned int max_area,
                              const double min_compactness,
                              const double max_compactness);
    };
};

#endif // _svlImageProcessingHelper_h

