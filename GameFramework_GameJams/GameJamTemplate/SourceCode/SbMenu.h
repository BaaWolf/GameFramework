////////////////////////////////////////////////////////////////////////////////
// SbMenu.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtSphere.h"
#include "MtVector3.h"
#include "RsCamera.h"
#include "HlView.h"

class BaArchive;
class RsMaterial;

// Class definition
class SbMenu : public HlView
{
public:

	// Public functions
	void							Init();
	void							Setup();
	void							Update();
	void							Render();

	// Accessors

private:

	// Private functions

	// Private members
};
