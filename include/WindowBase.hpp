#ifndef WINDOW_BASE_INCLUDED
#define WINDOW_BASE_INCLUDED

#define NOMINMAX
#include<Windows.h>
#include<iostream>
#include<unordered_map>
#include<chrono>
#include<string>

#include "Debug/Time.hpp"
#include "Math/Vector.hpp"
#include "Gadget.hpp"
#include "Drawable.hpp"
#include "CallBack.hpp"

enum BGCOLOUR {
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum FGCOLOUR {
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007, // Thanks MS :-/
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F
};

enum PIXEL_TYPE {
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

class Drawable;
class Gadget;
class KeyState;

class WindowBase {
public:
	WindowBase() = default;
	virtual ~WindowBase() = default;

	virtual void setPixel(uint32_t x, uint32_t y, const ivec3& color) {}
	virtual void setPixel(uint32_t x, uint32_t y, CHAR_INFO char_info) {}
	virtual void fill(const ivec3& color = ivec3(0, 0, 0)) {}
	void draw(Drawable* object);

	void addGadget(Gadget* gadget);

	virtual void render() = 0;
	void updateGadgets();
	void renderGadgets();

	void setWindowPos(const ivec2& position) { m_windowPos = position; }
	const ivec2& getWindowPos() const { return m_windowPos; }

	void Kill() { m_alive = false; }
	bool Alive() const { return m_alive; }

	void setCallback(Callback* callback) { m_callback = callback; }

protected:
	HANDLE m_stdout_handle, m_stdin_handle;
	HWND m_console;
	HDC m_dc;

	ivec2 m_windowPos;

	CHAR_INFO* m_colorbuffer;
	uint32_t m_buffersize;

	uint32_t m_width, m_height;

	std::vector<Gadget* > m_gadgets;

	bool m_alive = true;

	Callback* m_callback = nullptr;

	void setupWindow(uint32_t width, uint32_t height, uint32_t characterSize = 3);
};

#endif