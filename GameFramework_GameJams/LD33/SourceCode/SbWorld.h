////////////////////////////////////////////////////////////////////////////////
// SbWorld.h

// Include guard
#pragma once
#include "BtBase.h"

const BtS32 SeaSize  = 256;
const BtS32 HalfSeaSize = SeaSize / 2;
const BtS32 QuarterSeaSize = SeaSize / 4;
const BtS32 SeaDepth = 64;				//						20000 * 5556 - Jules what were you thinking?
const BtFloat SeaLevel = 0;
const BtFloat EyeHeight = 1.0f;
const BtS32 LostDepth = SeaDepth / 3;

const BtS32 ShipSortOrder = 10;
const BtS32 ParticleFXSortOrder = 12;
const BtS32 SeaSortOrder = 13;
