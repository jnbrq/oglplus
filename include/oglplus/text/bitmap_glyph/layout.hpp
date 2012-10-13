/**
 *  @file oglplus/text/bitmap_glyph/layout.hpp
 *  @brief Bitmap-font-based text rendering, layout class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_TEXT_BITMAP_GLYPH_LAYOUT_HPP
#define OGLPLUS_TEXT_BITMAP_GLYPH_LAYOUT_HPP

#include <oglplus/config.hpp>
#include <oglplus/buffer.hpp>
#include <oglplus/text/unicode.hpp>
#include <oglplus/text/bitmap_glyph/fwd.hpp>
#include <oglplus/text/bitmap_glyph/font.hpp>
#include <oglplus/text/bitmap_glyph/layout_storage.hpp>

#include <vector>
#include <algorithm>

namespace oglplus {
namespace text {

class BitmapGlyphLayout
{
private:
	// reference to the parent rendering system
	BitmapGlyphRendering& _parent;

	// the font used by this layout
	BitmapGlyphFont _font;

	// layout data used by a renderer
	BitmapGlyphLayoutData _data;

	// the list of font pages that this layout references
	std::vector<GLint> _pages;

	friend class BitmapGlyphRenderer;

	// sanity check
	bool _is_ok(void) const
	{
		return	(_data._offset >= 0) &&
			(_data._capacity > 0) &&
			(_data._storage);
	}
public:
	BitmapGlyphLayout(
		BitmapGlyphRendering& parent,
		const BitmapGlyphFont& font,
		GLsizei max_len
	): _parent(parent)
	 , _font(font)
	 , _data(max_len)
	{
		BitmapGlyphAllocateLayoutData(_parent, _data);
		assert(_is_ok());
	}

	BitmapGlyphLayout(const BitmapGlyphLayout& other)
	 : _parent(other._parent)
	 , _font(other._font)
	 , _data(other._data)
	 , _pages(other._pages)
	{
		_data._offset = -1;
		BitmapGlyphAllocateLayoutData(_parent, _data);
		assert(_is_ok());
	}

	BitmapGlyphLayout(BitmapGlyphLayout&& tmp)
	 : _parent(tmp._parent)
	 , _font(std::move(tmp._font))
	 , _data(std::move(tmp._data))
	 , _pages(std::move(tmp._pages))
	{
		tmp._data._offset = -1;
		assert(_is_ok());
	}

	~BitmapGlyphLayout(void)
	{
		if(_data._offset >= 0)
			BitmapGlyphDeallocateLayoutData(_parent, _data);
	}

	GLsizei Capacity(void) const
	{
		return _data._capacity;
	}

	void Set(const CodePoint* cps, GLsizei size)
	{
		assert(_is_ok());
		assert(size <= Capacity());
		BitmapGlyphInitializeLayoutData(_parent,_data,_font, cps, size);

		// update the list of referenced font pages
		_pages.clear();
		for(GLsizei cp=0; cp!=size; ++cp)
		{
			GLint page = BitmapGlyphPageOfCP(_parent, cps[cp]);
			auto i = _pages.begin(), e = _pages.end();
			if(std::find(i, e, page) == e)
				_pages.push_back(page);
		}
	}

	void Set(const std::vector<CodePoint>& cps)
	{
		Set(cps.data(), GLsizei(cps.size()));
	}

	void Set(const StrLit& lit)
	{
		Set(UTF8ToCodePoints(lit.c_str(), lit.size()));
	}

	void Set(const String& str)
	{
		Set(UTF8ToCodePoints(str.c_str(), str.size()));
	}
};

} // namespace text
} // namespace oglplus

#endif // include guard