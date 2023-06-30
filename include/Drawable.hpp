#ifndef DRAWABLE_INCLUDED
#define DRAWABLE_INCLUDED

#include<vector>
#include "Window.hpp"
#include "Math/Vector.hpp"

class WindowBase;

class Drawable {
	friend class WindowBase;
public:
	Drawable() = default;
	virtual ~Drawable() = default;
protected:
	virtual void draw(WindowBase* window) const = 0;
};


#endif