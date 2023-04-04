#ifndef BATCONTROLLER_FIRMWARE_DRIVER_H
#define BATCONTROLLER_FIRMWARE_DRIVER_H

#include <DriveInfo.h>
#include <Display/Color.h>
#include <LVObject.h>

class Driver{
public:
    virtual ~Driver();

    void start();
	void stop();

	virtual void onFrame(const DriveInfo& frame, Color* pixels);

	DriveMode getMode() const;

protected:
	Driver(DriveMode mode);

	virtual void onStart(){};
	virtual void onStop(){};

private:
	DriveMode mode;

};


#endif //BATCONTROLLER_FIRMWARE_DRIVER_H
