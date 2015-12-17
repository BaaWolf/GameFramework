////////////////////////////////////////////////////////////////////////////////
// ImageLibrary.cpp

// Includes
#include "BaArchive.h"
#include "ImageLibrary.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsSprite.h"
#include "ErrorLog.h"
#include "BtMemory.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "BtBase.h"

using namespace cv;

////////////////////////////////////////////////////////////////////////////////
// CreateImage

BtU64 ImageLibrary::CreateImage( BtU32 width, BtU32 height )
{
	Mat *pImage = new Mat( Size( width, height ), CV_8UC3 );
	return (BtU64)pImage;
}

////////////////////////////////////////////////////////////////////////////////
// OpenImage

BtU64 ImageLibrary::OpenImage( const BtChar *filename )
{
	Mat *pImage = new Mat();
	*pImage = imread( filename, CV_LOAD_IMAGE_ANYCOLOR );   // Read the file
	return (BtU64)pImage;
}

////////////////////////////////////////////////////////////////////////////////
// GetWidth

BtU32 ImageLibrary::GetWidth( void *source )
{
	Mat* pSource = (Mat*)source;
	return pSource->cols;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumChannels

BtU32 ImageLibrary::GetNumChannels( void *source )
{
	Mat* pSource = (Mat*)source;
	BtU32 channels = pSource->channels();
	return channels;
}

////////////////////////////////////////////////////////////////////////////////
// GetHeight

BtU32 ImageLibrary::GetHeight( void *source )
{
	Mat* pSource = (Mat*)source;
	return pSource->rows;
}

////////////////////////////////////////////////////////////////////////////////
// GetPixels

BtU8 *ImageLibrary::GetPixels(void *source)
{
	Mat* pSource = (Mat*)source;
	return pSource->data;
}

////////////////////////////////////////////////////////////////////////////////
// FlipX

void ImageLibrary::FlipX(void *source)
{
	Mat* pSource = (Mat*)source;
	Mat dest;
	cv::flip( *pSource, dest, 0 );
	*pSource = dest;
}

////////////////////////////////////////////////////////////////////////////////
// FlipY

void ImageLibrary::FlipY(void *source)
{
	Mat* pSource = (Mat*)source;
	Mat dest;
	cv::flip(*pSource, dest, 1 );
	*pSource = dest;
}

////////////////////////////////////////////////////////////////////////////////
// Resize

void ImageLibrary::Resize( void *source, void *dest )
{
	Mat* pSource = (Mat*)source;
	Mat* pDest   = (Mat*)dest;
	resize( *pSource, *pDest, pDest->size(), 0, 0, INTER_LINEAR );
}

////////////////////////////////////////////////////////////////////////////////
// Copy

void ImageLibrary::Copy( void *source, BtU32 x, BtU32 y, BtU32 width, BtU32 height, void *dest)
{
	Mat* pSource = (Mat*)source;
	Mat* pDest   = (Mat*)dest;

	BtAssert( pDest->rows == width );
	BtAssert( pDest->cols == height );

	// Copy a rectangle part of it
	Mat m  = *pSource;
	*pDest = m( Rect( x, y, width, height ) );
}

////////////////////////////////////////////////////////////////////////////////
// Save

void ImageLibrary::Save( void *handle, BtChar *filename )
{
	Mat *pImage = (Mat*)handle;
	imwrite( filename, *pImage );
}

////////////////////////////////////////////////////////////////////////////////
// Close

void ImageLibrary::Close( void *handle )
{
	Mat* pSource = (Mat*)handle;
	pSource->release();
	delete pSource;
}
