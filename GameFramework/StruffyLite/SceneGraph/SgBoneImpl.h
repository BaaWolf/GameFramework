////////////////////////////////////////////////////////////////////////////////
// SgBoneDX11.h

#pragma once
#include "SgBone.h"
#include "BaFileData.h"
#include "ErrorLog.h"

// Class Declaration
class SgBoneDX11 : public SgBone
{
public:

	void							FixPointers( BtU8* pMemory );
	const MtMatrix4&				InverseBindPose() const;

private:

	friend class SgNodeDX11;
	friend class SgNodeWinDX;
	friend class SgNodeWinGL;
	friend class SgAnimatorWinGL;

	SgNode						   *m_pNode;
	BaSgBoneFileData*				m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// FixPointers

inline void SgBoneDX11::FixPointers( BtU8* pMemory )
{
	// Set the file data
	m_pFileData = (BaSgBoneFileData*) pMemory;
}

////////////////////////////////////////////////////////////////////////////////
// InverseBindPose

inline const MtMatrix4& SgBoneDX11::InverseBindPose() const
{
	return m_pFileData->m_m4InverseBindPose;
}
