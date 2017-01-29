
#include "matrix.h"


Matrix::Matrix() : Adafruit_GFX(NUMBER_OF_DRIVER_DEVICES*8, 8)
{
	_shutdown = 0;
}

Matrix::~Matrix()
{

}


void Matrix::init()
{
  // call the max696x initialisation function
  _max6960.init((GPC_PIXEL_INTENSITY_2BPP | GPC_RIPPLESYNC_ENABLED |
    GPC_DISPLAY_MONO | GPC_ENABLE | GPC_PIXEL_NORMAL),   // Global Panel Config register setting
    0x80,                           // Panel Intensity
    PIS_FULL ,                    // Pixel Intensity Scale
    NUMBER_OF_DRIVER_DEVICES,    // Number of driver devices
    NUMBER_OF_DRIVERS_PER_ROW,   // Number of drivers per row
    PCR_RATE);                   // Frame Rate, calculated from the max696x_conf.h settings

  // a variable to keep track of when the display is enabled or shut down
  _shutdown = 0;
}

void Matrix::drawPixel(int16_t x, int16_t y, uint16_t color)
{
}

void Matrix::clear()
{
	_max6960.command_wr(GLOBAL_CLEAR_PLANES, (GCP_REDP0 | GCP_REDP1 | GCP_REDP2 | GCP_REDP3), GLOBAL);
}

void Matrix::registerDump()
{
    _max6960.reg_dump();
}

void Matrix::sleep(bool shutdown)
{
	byte data = _max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);

	if (shutdown)
	{
		_max6960.command_wr(GLOBAL_PANEL_CONFIG, data & ~(GPC_ENABLE), GLOBAL);
		_shutdown = 1;
	}
	else
	{
		_max6960.command_wr(GLOBAL_PANEL_CONFIG, data | GPC_ENABLE, GLOBAL);
		_shutdown = 0;
	}
}

void Matrix::reset()
{
    // reset the display ICs
    _max6960.display_reset();
}
