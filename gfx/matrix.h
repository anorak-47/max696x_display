#pragma once

#include "../max696x/max696x.h"
#include "gfx.h"

class Matrix : public Adafruit_GFX
{
public:
	Matrix();
    virtual ~Matrix();

	void init();

    // This MUST be defined by the subclass:
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

    void clear();
    void sleep(bool shutdown);

    void registerDump();
    void reset();

private:
	// a variable to keep track of the enable/disable state of the display
	byte _shutdown;

	// instantiate the MAX696x driver from the library.
	max696x _max6960;
};

