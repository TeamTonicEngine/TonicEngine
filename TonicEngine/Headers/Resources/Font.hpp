#pragma once
#include "IResource.hpp"

#include "Core/Utils/u32_2.hpp"

struct FT_FaceRec_;
struct FT_LibraryRec_;

namespace Resources
{
	class Font;
	using FontPtr = std::shared_ptr<Font>; // FontPtr is used in font class

	class Font : public IResource, public std::enable_shared_from_this<Font>
	{
		/**********************************************
				VARIABLES BLOC
		**********************************************/
	public:
		struct Character
		{
			unsigned int textureID;  // ID handle of the glyph texture
			u32_2 size;       // Size of glyph
			u32_2 bearing;    // Offset from baseline to left/top of glyph
			u32 advance;    // Offset to advance to next glyph
		};

	private:
		std::map<char, Character> characters_;
		FT_FaceRec_* p_face_;
		FT_LibraryRec_* p_ft_;
		int fontSize_ = 48;
		unsigned int VAO_ = -1;
		unsigned int VBO_ = -1;

	public:
		static FontPtr s_p_defaultFont;
		static ShaderPtr s_p_textShader;

		friend class Core::Renderer::OpenGLWrapper;

		/*********************************************
				FUNCTIONS BLOC
		*********************************************/

		TONIC_ENGINE_API ~Font() = default;

		static void InitShader(Resources::ShaderPtr _textShader);

		TONIC_ENGINE_API void ChangeSize(int _pixelHorizontal, int _pixelVertical = 0);

		void TONIC_ENGINE_API Destroy() override;

		void TONIC_ENGINE_API ReadFile(const fs::path _path) override;
		void TONIC_ENGINE_API LoadFile() override;

		void TONIC_ENGINE_API Use() override;
		//Avoid to send a Mat4 buffer twice
		void TONIC_ENGINE_API UseWithoutSafety();
		void TONIC_ENGINE_API ResourceUnload() override;

		int TONIC_ENGINE_API LoadGlyph(unsigned char _c);

		void StoreCharacter(const unsigned char _c, unsigned int _texture);

		TONIC_ENGINE_API const int GetFontSize() const { return fontSize_; };
		const unsigned int GetWidth() const;
		const unsigned int GetRows() const;
		const unsigned char* GetBuffer() const;
	};
}