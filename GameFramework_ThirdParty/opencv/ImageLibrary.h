////////////////////////////////////////////////////////////////////////////////
// ImageLibrary.h

// Include guard
#pragma once
#include "BtTypes.h"

// Class definition
class ImageLibrary
{
public:

	// Public functions
	static BtU64						   OpenImage( const BtChar *filename );
	static BtU64						   CreateImage( BtU32 width, BtU32 height );
	static BtU32						   GetWidth( void *source );
	static BtU32						   GetHeight( void *source );
	static BtU32						   GetNumChannels( void *source );
	static void							   FlipX( void *source );
	static void							   FlipY( void *source );
	static BtU8*						   GetPixels( void *source );
	static void							   Resize( void *source, void *dest );
	static void							   Close( void *source );
	static void							   Save( void *source, BtChar *filename );
	static void							   Copy( void *source, BtU32 x, BtU32 y, BtU32 width, BtU32 height, void *dest );

	// Accessors

private:

	// Private functions

	// Private members
};
