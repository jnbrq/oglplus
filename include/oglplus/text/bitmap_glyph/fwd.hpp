/**
 *  @file oglplus/text/bitmap_glyph/fwd.hpp
 *  @brief Bitmap-font-based text rendering, forward declarations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_TEXT_BITMAP_GLYPH_FWD_HPP
#define OGLPLUS_TEXT_BITMAP_GLYPH_FWD_HPP

#include <oglplus/config.hpp>
#include <oglplus/text/unicode.hpp>
#include <string>

namespace oglplus {
namespace text {

// Forward declaration of the bitmap glyph rendering utility
class BitmapGlyphRendering;

// Forward declaration of the renderer
class BitmapGlyphRenderer;

// Forward declaration of the font
class BitmapGlyphFont;

// Forward declaration of layout storage
class BitmapGlyphStaticLayoutStorage;

// Forward declaration of layout data
struct BitmapGlyphLayoutData;

// Returns the number of glyphs per a font page
unsigned BitmapGlyphGlyphsPerPage(const BitmapGlyphRendering&);

// Returns the number of font pages per a unicode plane
unsigned BitmapGlyphPagesPerPlane(const BitmapGlyphRendering&);

// Returns the number of planes supported by the bitmap font rendering utility
// One plane consists of PagesPerPlane pages of GlyphsPerPage glyphs
unsigned BitmapGlyphPlaneCount(const BitmapGlyphRendering&);

std::string BitmapGlyphFontPagePath(
	const BitmapGlyphRendering& that,
	const std::string& font_name,
	GLint font_page
);

void BitmapGlyphAllocateLayoutData(
	BitmapGlyphRendering& parent,
	BitmapGlyphLayoutData& layout_data
);
void BitmapGlyphInitializeLayoutData(
	BitmapGlyphRendering& parent,
	BitmapGlyphLayoutData& layout_data,
	BitmapGlyphFont& font,
	const CodePoint* cps,
	GLsizei length
);
void BitmapGlyphDeallocateLayoutData(
	BitmapGlyphRendering& parent,
	BitmapGlyphLayoutData& layout_data
);

// Returns the page of the font containing the glyph for the specified codepoint
inline GLint BitmapGlyphPageOfCP(
	const BitmapGlyphRendering& parent,
	CodePoint code_point
)
{
	assert(BitmapGlyphGlyphsPerPage(parent) != 0);
	assert(BitmapGlyphGlyphsPerPage(parent) % 4 == 0);
	return code_point / BitmapGlyphGlyphsPerPage(parent);
}

inline GLint BitmapGlyphCellOfCP(
	const BitmapGlyphRendering& parent,
	CodePoint code_point
)
{
	assert(BitmapGlyphGlyphsPerPage(parent) != 0);
	assert(BitmapGlyphGlyphsPerPage(parent) % 4 == 0);
	return code_point % BitmapGlyphGlyphsPerPage(parent);
}

inline std::string BitmapGlyphPageName(
	const BitmapGlyphRendering& parent,
	CodePoint code_point
)
{
	const char hexdigit[16] = {
		'0','1','2','3','4',
		'5','6','7','8','9',
		'A','B','C','D','E','F'
	};
	GLint page_head =
		BitmapGlyphPageOfCP(parent, code_point)*
		BitmapGlyphGlyphsPerPage(parent);
	const char result[6] = {
		hexdigit[(page_head >> 20) & 0x0F],
		hexdigit[(page_head >> 16) & 0x0F],
		hexdigit[(page_head >> 12) & 0x0F],
		hexdigit[(page_head >>  8) & 0x0F],
		hexdigit[(page_head >>  4) & 0x0F],
		hexdigit[(page_head >>  0) & 0x0F]
	};
	return std::string(result, 6);
}

} // namespace text
} // namespace oglplus

#endif // include guard
