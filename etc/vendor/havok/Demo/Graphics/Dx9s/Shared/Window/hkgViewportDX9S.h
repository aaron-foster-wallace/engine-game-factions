/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VIEWPORT_DX9S_H
#define HK_GRAPHICS_VIEWPORT_DX9S_H

#include <Graphics/Common/Window/hkgViewport.h>

class hkgViewportDX9S : public hkgViewport
{
public:

	static hkgViewport* createViewportDX9S(hkgWindow* owner)
	{
		return new hkgViewportDX9S(owner);
	}

	virtual void setAsCurrent(hkgDisplayContext* context);

protected:

	inline hkgViewportDX9S(hkgWindow* owner);
	virtual ~hkgViewportDX9S() {}

};

#include <Graphics/Dx9s/Shared/Window/hkgViewportDX9S.inl>

#endif //HK_GRAPHICS_VIEWPORT_DX9S_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/