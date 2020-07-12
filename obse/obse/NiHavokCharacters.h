#pragma once

/**** havok character control classes
 *	bhkCharacterProxy
 *		bhkCharacterListenerArrow - no NiRTTI
 *		bhkCharacterListenerSpell - no NiRTTI
 *		bhkCharacterController - no NiRTTI
 */

#include "obse/NiHavok.h"
#include "obse/HavokCollision.h"
#include "obse/HavokCharacters.h"

// 68+
class bhkCharacterListener : public hkCharacterProxyListener
{
public:
	bhkCharacterListener();
	~bhkCharacterListener();

	UInt32				flags004;						// 004 if either bit 8 or 9 set, no fall damage
	UInt32				unk008[(0x024 - 0x008) >> 2];	// 008
	UInt32				groundSurfaceMaterial;			// 24 one of bhkShape::kMaterial_XXX
	UInt32				unk028[(0x068 - 0x028) >> 2];	// 28
};

STATIC_ASSERT(sizeof(bhkCharacterListener) == 0x68);