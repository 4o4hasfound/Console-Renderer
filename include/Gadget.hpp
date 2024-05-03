#ifndef GADGET_INCLUDED
#define GADGET_INCLUDED

#include "Drawable.hpp"
#include "CallBack.hpp"

class WindowBase;
class Drawable;

class Gadget: public Drawable {
public:
	Gadget() = default;

	void addCallback(Callback* callback) { m_callback = callback; }

	virtual void update(WindowBase* window) = 0;

	bool alive = true;
protected:
	virtual void draw(WindowBase* window) = 0;

	Callback* m_callback = nullptr;
	bool m_dragged = false;
};

#endif