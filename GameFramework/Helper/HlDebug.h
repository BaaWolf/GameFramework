////////////////////////////////////////////////////////////////////////////////
// HlDebug.h

#pragma once
#include "BtArray.h"
#include "BtTypes.h"
#include "RsColour.h"
#include "MtVector3.h"

const BtU32 HlDebugNode	   = 0;
const BtU32 HlDebugInteger = 1;
const BtU32 HlDebugFloat   = 2;
const BtU32 HlDebugFPS     = 3;
const BtU32 HlDebugVector  = 4;
const BtU32 HlDebugBool    = 5;
const BtU32 HlDebugSave	   = 6;
const BtU32 HlDebugEnum    = 7;
const BtU32 HlDebugColour  = 8;

const BtU32 HLDebugStandardIndex = 0;
const BtU32 HLDebugKnots = 1;

class RsMaterial;
class BaArchive;

struct HlItem
{
	HlItem()
	{
		m_isReadOnly = BtTrue;
		m_isCollapsed = BtFalse;
		m_unitsSystem = HLDebugStandardIndex;
	}

	BtU32							m_type;
	BtU32							m_unitsSystem;
	BtChar							m_name[32];
	BtChar						   *m_pEnumString;
	BtFloat						   *m_pFloat;
	MtVector3					   *m_pVector;
	RsColour					   *m_pColour;
	BtBool						   *m_pBool;
	BtS32						   *m_pInteger;
	BtU32						   *m_pEnum;
	BtU32							m_minInt;
	BtU32							m_maxInt;
	BtFloat							m_minFloat;
	BtFloat							m_maxFloat;
	BtU32							m_diffInt;
	HlItem						   *m_pParent;
	BtBool							m_isReadOnly;
	BtBool							m_isCollapsed;
};

// Class definition
class HlDebug
{
public:

	// Public functions
	static void						Setup( BaArchive *pArchive );
	static void						Reset();

	static HlItem*					AddFPS( HlItem *pParent );
	static HlItem*					AddNode( HlItem *pParent, BtChar *name );
	static HlItem*					AddFloat( HlItem *pParent, BtChar *name, BtFloat *value, BtBool isReadOnly, BtU32 unitsSystem = HLDebugStandardIndex, BtFloat minFloat = 0.0f, BtFloat maxFloat = 1.0f );
	static HlItem*					AddEnum( HlItem *pParent, BtChar *name, BtChar *enumString, BtU32 *value, BtBool isReadOnly, BtU32 range);
	static HlItem*					AddInteger(HlItem *pParent, BtChar *name, BtS32 *value, BtBool isReadOnly, BtS32 minInt, BtS32 maxInt, BtS32 diffInt);
	static HlItem*					AddUInt(HlItem *pParent, BtChar *name, BtU32 *value, BtBool isReadOnly, BtU32 minInt, BtU32 maxInt, BtU32 diffInt);
	static HlItem*					AddVector(HlItem *pParent, BtChar *name, MtVector3 *value, BtBool isReadOnly);
	static HlItem*					AddColour( HlItem *pParent, BtChar *name, RsColour *value, BtBool isReadOnly);
	static HlItem*					AddBool( HlItem *pParent, BtChar *name, BtBool *value, BtBool isReadOnly);
	static HlItem*					AddItem( HlItem &item );

	static void						Update();
	static void						Render();
	static BtU32					GetIndex();
	static HlItem				   *GetItem( BtChar *name );

	static BtBool					m_unlocked;

private:
	static void						Load();
	static void						Save();
	static BtU32					GetNumParents( HlItem *pItem );
	static BtBool					IsHidden( HlItem &item );
	static void						Increment();
	static void						Decrement();

	static BtArray<HlItem, 1024>	m_items;
	static BtBool					m_isLoaded;
};
