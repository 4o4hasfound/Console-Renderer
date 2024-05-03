#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED

#define NOMINMAX
#include<Windows.h>
#include<iostream>
#include<unordered_map>
#include<chrono>
#include<string>
#include <locale>
#include <codecvt>

#include "Debug/Time.hpp"
#include "Math/Vector.hpp"
#include "WindowBase.hpp"
#include "Gadget.hpp"
#include "Drawable.hpp"
#include "CallBack.hpp"


class Window : public WindowBase {
public:
	Window() = default;
	Window(uint32_t width, uint32_t height, uint32_t characterSize=3 /*Best performance*/);
	~Window() = default;
	
	void setTitle(std::wstring title);
	void setTitle(std::string title);

	virtual void setPixel(uint32_t x, uint32_t y, const ivec3& color) override final;
	virtual void setPixel(uint32_t x, uint32_t y, CHAR_INFO char_info) override final;
	virtual void fill(const ivec3& color = ivec3(0, 0, 0)) override final;

	virtual void render() override;
private:
	void SetUpRGBMap();

	static const uint32_t ColorMapStep = 10;

	// FG, BG code to RPG value
	static std::unordered_map<int8_t, uint64_t> ToRGB;

	// RGB value to CHAR_INFO
	static std::unordered_map<int32_t, CHAR_INFO> ToCharInfo;
};

#endif