////////////////////////////////////////////////////////////////////////////////
// VideoCapture.cpp

// Includes
#include "BaArchive.h"
#include "VideoCapture.h"
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
#include "ApConfig.h"

using namespace cv;

// http://thefreecoder.wordpress.com/2012/09/11/opencv-c-video-capture/
Mat cameraFrame;
Mat cameraProcessed;
BtU8 VideoMemory[1920 * 1080 * 4];
VideoCapture *pStream = BtNull;

////////////////////////////////////////////////////////////////////////////////
// Start

BtU32 CVVideoCapture::Start()
{
    if( ApConfig::GetDevice() == ApDevice_WIN )
    {
		pStream = new VideoCapture(0);   //0 is the id of video device.0 if you have only one camera.

        if( !pStream->isOpened() )
        {
            ErrorLog::Fatal_Printf( "Could not open camera" );
        }

		BtBool success = pStream->read(cameraFrame);

		if(!success)
		{
			Stop();
		}

        cameraProcessed  = Mat( Size( 1920, 1080 ), CV_8UC4 );
    }
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Stop

void CVVideoCapture::Stop()
{
    if( ApConfig::GetDevice() == ApDevice_WIN )
    {
		if( pStream )
		{
			delete pStream;

			pStream = BtNull;
		}
    }
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CVVideoCapture::Update()
{
    if( ApConfig::GetDevice() == ApDevice_WIN )
    {
        if( pStream )
        {
            // Read a camera frame
            BtBool success = pStream->read(cameraFrame);

            if( success )
            {
                resize( cameraFrame, cameraProcessed, cameraProcessed.size(), 0, 0, INTER_LINEAR );
                
                // Convert from 3 bytes per channel to 4 bytes per channel
                cvtColor( cameraProcessed, cameraProcessed, CV_BGR2BGRA );
            }
            else
            {
                Stop();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// GetMemory

BtU8* CVVideoCapture::GetMemory()
{
    if( ApConfig::GetDevice() == ApDevice_WIN )
    {
        return cameraProcessed.data;
    }
    else
    {
        return VideoMemory;
    }
}

////////////////////////////////////////////////////////////////////////////////
// SetMemory

void CVVideoCapture::SetMemory( BtU8 *pBytes, BtU32 numBytes )
{
    memcpy( VideoMemory, pBytes, numBytes );
}

