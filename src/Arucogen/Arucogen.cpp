
#include <vector>
#include "Arucogen.h"
#include "ArucoDictionary.h"


Arucogen::Arucogen(Sprite* baseSprite): baseSprite(baseSprite){

}

Arucogen::~Arucogen(){
}

void Arucogen::generateMarker(uint8_t index){
	uint8_t* bytes = (uint8_t*) DICTIONARY[index];
	//uint8_t* bytes = (uint8_t*)DICTIONARY[index];
	uint bitsCount = width*height;
	std::vector<char> bits;

	// Parse marker's bytes
	for (uint8_t byte : bytes) {
		int start = bitsCount - bits.size();
		for (int i = std::min(7, start - 1); i >= 0; i--) {
			bits.push_back((byte >> i) & 1);
		}
	}

//	Serial.println();
//
//	int size = 0;
//	for(int i = 0; i < 16; i++){
//		int start = bitsCount - size;
//		for(int j = std::min(7, start- 1); j >= 0; j++ ){
//			Serial.printf("%d :\n",(bytes[i] >> j) & 1);
//			bits[size] = (bytes[i] >> j) & 1;
//			size++;
//		}
//		Serial.println();
//	}

//	const char bits[] = {1,0,0,0,0,
//				   1,0,0,0,0,
//				   1,0,0,0,0,
//				   1,0,0,0,0,
//				   0,1,1,1,0,};
//
//	baseSprite->clear(TFT_BLUE);
//	baseSprite->push();

	//Tiles
//	Serial.println();
//	for(int i = 0; i < height; i++){
//		for(int j = 0; j < width; j++){
//			int white = bits[i*height + j];
//			Serial.printf(" %d ", white);
//			if(white == 0)	continue;
//
//			baseSprite->fillRect(((j+1)*pixelPerTile), ((i+1)*pixelPerTile), pixelPerTile, pixelPerTile, TFT_WHITE);
////			Serial.printf("\nTILE[%d][%d]: x = %d, y= %d, w = %d, h = %d",i, j, (j+1)*pixelPerTile, (i+1)*pixelPerTile,pixelPerTile,pixelPerTile);
//		}
//		Serial.println();
//	}

}
