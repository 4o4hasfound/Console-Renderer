#ifndef ASCII_WINDOW_INCLUDED
#define ASCII_WINDOW_INCLUDED

#define NOMINMAX
#include<Windows.h>
#include<iostream>
#include<unordered_map>
#include<chrono>
#include<string>
#include<locale>
#include<codecvt>
#include<algorithm>
#include<functional>

#include "Debug/Time.hpp"
#include "Math/Vector.hpp"
#include "WindowBase.hpp"
#include "Gadget.hpp"
#include "Drawable.hpp"
#include "CallBack.hpp"
#include "KeyState.hpp"
#include "Font.hpp"

class AsciiWindow : public WindowBase {
public:
	AsciiWindow() = default;
	AsciiWindow(uint32_t width, uint32_t height, uint32_t characterSize = 3, std::string ascii = " !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
	~AsciiWindow() = default;

	void setTitle(std::wstring title);
	void setTitle(std::string title);

	virtual void setPixel(uint32_t x, uint32_t y, const ivec3& color) override final;
	virtual void setPixel(uint32_t x, uint32_t y, CHAR_INFO char_info) override final;
	virtual void fill(const ivec3& color = ivec3(0, 0, 0)) override final;

	virtual void render() override;

private:
	std::string m_ascii;
	char m_pallet[256];

	void sortAscii();
	CHAR_INFO RGBToAscii(const ivec3& color);
};

#endif