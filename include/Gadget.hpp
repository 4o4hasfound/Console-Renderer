#ifndef GADGET_INCLUDED
#define GADGET_INCLUDED

#include "Window.hpp"
#include "CallBack.hpp"

class WindowBase;

class Gadget {
public:
	Gadget() = default;

	void addCallback(Callback* callback) { m_callback = callback; }

	virtual void update(WindowBase* window) = 0;
	virtual void render(WindowBase* window) = 0;

	bool alive = true;
protected:
	Callback* m_callback = new Callback();
	bool m_dragged = false;
};

#endif