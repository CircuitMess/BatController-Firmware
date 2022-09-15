
#ifndef BATCONTROLLER_FIRMWARE_ARUCOGEN_H
#define BATCONTROLLER_FIRMWARE_ARUCOGEN_H


#include <Display/Sprite.h>

class Arucogen {
public:
	Arucogen(Sprite* baseSprite);
	~Arucogen();
	void generateMarker(uint8_t index);
private:
	Sprite* baseSprite;

	const int width = 5;
	const int height = 5;
	const int pixelPerTile = 10;
};


#endif //BATCONTROLLER_FIRMWARE_ARUCOGEN_H
