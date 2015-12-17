////////////////////////////////////////////////////////////////////////////////
// SbTexture.h

// Include guard
#pragma once
#include "BtTypes.h"

class RsTexture;
class BaArchive;

enum GameState
{
	GameState_Start,
	GameState_Playing,
	GameState_Win,
	GameState_Lose,
};

// Class definition
class SbMenu
{
public:

	// Public functions
	void							Init();
	void							Setup(BaArchive *pArchive);
	void							Update();
	void							Render();

	// Accessors

private:

	// Private functions

	// Private members
	RsTexture					   *m_pStart;
	RsTexture					   *m_pWin;
	RsTexture					   *m_pLose;
};
