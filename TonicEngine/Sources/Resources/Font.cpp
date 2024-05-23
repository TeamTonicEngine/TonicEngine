#include "pch.hpp"

#include "Resources\Font.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

Resources::FontPtr Resources::Font::s_p_defaultFont;
Resources::ShaderPtr Resources::Font::s_p_textShader;

void Resources::Font::Destroy()
{
	ResourceUnload();
	//IMPORTANT: this line prevents memory leaks: Case read but never loaded so never freed
	if (bRead_ && !bLoaded_)
	{
		ENGINE.RDR->UnloadResource(shared_from_this());
	}
}

void Resources::Font::ReadFile(const fs::path _path)
{
	if (FT_Init_FreeType(&p_ft_))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	if (FT_New_Face(p_ft_, _path.string().c_str(), 0, &p_face_))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	FT_Set_Pixel_Sizes(p_face_, 0, fontSize_);
	bRead_ = true;
}

void Resources::Font::LoadFile()
{
	if (bRead_)
	{
		ENGINE.RDR->LoadResource(shared_from_this());
		if (bLoaded_)
		{
			DEBUG_SUCCESS("Font %s loaded #%i", name.c_str(), ID);
			//stbi_image_free(data_);
			bRead_ = false;
		}
		else
			DEBUG_WARNING("Loading failed for Font %s", name.c_str());
	}
	else
		DEBUG_WARNING("File not Read, cannot be loaded: %s", name.c_str());

	if (!s_p_defaultFont)
		s_p_defaultFont = shared_from_this();
}

void Resources::Font::Use()
{
	if (this)
	{
		if (bLoaded_)
			ENGINE.RDR->UseResource(shared_from_this());
		else
		{
			LoadFile();
			if (bLoaded_)
				ENGINE.RDR->UseResource(shared_from_this());
		}
	}
	else
		ENGINE.RDR->UseResource(s_p_defaultFont);
}
//Avoid to send a Mat4 buffer twice
void Resources::Font::UseWithoutSafety()
{
	if (this)
	{
		if (bLoaded_)
			ENGINE.RDR->UseResourceWithoutSafety(shared_from_this());
		else
		{
			LoadFile();
			if (bLoaded_)
				ENGINE.RDR->UseResourceWithoutSafety(shared_from_this());
		}
	}
	else
		ENGINE.RDR->UseResourceWithoutSafety(s_p_defaultFont);
}

void Resources::Font::ResourceUnload()
{
	FT_Done_Face(p_face_);
	FT_Done_FreeType(p_ft_);
	bLoaded_ = false;
	ID = -1;
	characters_.clear();
}

int Resources::Font::LoadGlyph(unsigned char _c)
{
	// load character glyph
	if (FT_Error errorCode = FT_Load_Char(p_face_, (FT_ULong)_c, FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		return (int)errorCode;
	}
	return 0;
}

void Resources::Font::StoreCharacter(const unsigned char _c, unsigned int _texture)
{
	Resources::Font::Character character = {
	_texture,
	u32_2
	{
		p_face_->glyph->bitmap.width, p_face_->glyph->bitmap.rows
	},
	u32_2
	{
		(u32)p_face_->glyph->bitmap_left,
		(u32)p_face_->glyph->bitmap_top
	},
	(u32)p_face_->glyph->advance.x
	};
	characters_.insert(std::pair<char, Resources::Font::Character>(_c, character));
}

const unsigned int Resources::Font::GetWidth() const { return p_face_->glyph->bitmap.width; }

const unsigned int Resources::Font::GetRows() const { return p_face_->glyph->bitmap.rows; }

const unsigned char* Resources::Font::GetBuffer() const { return p_face_->glyph->bitmap.buffer; }

void Resources::Font::InitShader(Resources::ShaderPtr _textShader) { s_p_textShader = _textShader; }

void Resources::Font::ChangeSize(int _pixelHorizontal, int _pixelVertical)
{
	if (!bRead_ && bLoaded_)
	{
		ENGINE.RDR->UnloadResource(shared_from_this());
		bLoaded_ = false;
		bRead_ = true;
	}

	fontSize_ = _pixelVertical;
	FT_Set_Pixel_Sizes(p_face_, _pixelHorizontal, _pixelVertical);
	LoadFile();
}