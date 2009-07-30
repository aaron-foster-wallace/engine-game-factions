/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_FONT_DX9S_H
#define HK_GRAPHICS_FONT_DX9S_H

#include <Graphics/Common/Font/hkgFont.h>

class hkgFontDX9S : public hkgFont
{
public:

	static hkgFont* createFontDX9S()
	{
		return new hkgFontDX9S;
	}

	virtual void render(hkgDisplayContext* context, const char* text, float x, float y, float color[4] );
	
protected:

	LPDIRECT3DVERTEXBUFFER9			m_pVB;		// a large vb to hold our verts for the text so that we don't have to use the UP version
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;      // Vertex declaration for shaders
	
	hkgFontDX9S();
	virtual ~hkgFontDX9S();

};

#endif // HK_GRAPHICS_FONT_DX9S_H

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