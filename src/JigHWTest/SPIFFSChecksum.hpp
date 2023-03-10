#ifndef BATCONTROLLER_FIRMWARE_SPIFFSCHECKSUM_H
#define BATCONTROLLER_FIRMWARE_SPIFFSCHECKSUM_H

#include <Arduino.h>

struct {
	String name;
	uint32_t sum;
} static const SPIFFSChecksums[] PROGMEM = {
		{ "/SimpleProg/Frontlight.bin",      8885 },
		{ "/SimpleProg/newFocused.bin",      2399 },
		{ "/SimpleProg/footer_1.bin",        7657 },
		{ "/SimpleProg/Drive.bin",           2742 },
		{ "/SimpleProg/footer_2.bin",        5423 },
		{ "/SimpleProg/Delay.bin",           5776 },
		{ "/SimpleProg/Sound.bin",           4309 },
		{ "/SimpleProg/ActionPickBg.bin",    138349 },
		{ "/SimpleProg/new.bin",             2501 },
		{ "/SimpleProg/Underlight.bin",      7627 },
		{ "/SimpleProg/Frame.bin",           7461 },
		{ "/SimpleProg/Backlight.bin",       7406 },
		{ "/LowBattery/CLowBat.bin",         410514 },
		{ "/LowBattery/VLowBat.bin",         336196 },
		{ "/LowBattery/LowBat.bin",          69996 },
		{ "/DriveDir/Arrow0.raw",            5181 },
		{ "/DriveDir/Arrow1.raw",            3498 },
		{ "/DriveMode/Line.bin",             5313 },
		{ "/DriveMode/Idle.bin",             2403 },
		{ "/DriveMode/Ball.bin",             5905 },
		{ "/DriveMode/Manual.bin",           3661 },
		{ "/DriveMode/SimpleProg.bin",       5871 },
		{ "/DriveMode/Dance.bin",            5075 },
		{ "/DriveMode/Marker.bin",           6258 },
		{ "/Battery/7.bin",                  2816 },
		{ "/Battery/Controller.bin",         3081 },
		{ "/Battery/5.bin",                  2687 },
		{ "/Battery/6.bin",                  2688 },
		{ "/Battery/2.bin",                  2673 },
		{ "/Battery/1.bin",                  2657 },
		{ "/Battery/4.bin",                  2685 },
		{ "/Battery/Batmobile.bin",          2332 },
		{ "/Battery/0.bin",                  2625 },
		{ "/Battery/3.bin",                  2681 },
		{ "/Fire/frame0.bin",                35848 },
		{ "/Fire/frame2.bin",                29819 },
		{ "/Fire/frame5.bin",                29819 },
		{ "/Fire/frame4.bin",                33101 },
		{ "/Fire/frame7.bin",                33101 },
		{ "/Fire/frame10.bin",               33101 },
		{ "/Fire/frame11.bin",               29819 },
		{ "/Fire/frame9.bin",                29919 },
		{ "/Fire/frame8.bin",                29819 },
		{ "/Fire/frame6.bin",                35848 },
		{ "/Fire/frame3.bin",                29919 },
		{ "/Fire/frame1.bin",                33101 },
		{ "/Pairing/WiFiConnecting.gif",     52124 },
		{ "/Pairing/WiFiOff.bin",            8341 },
		{ "/Signal/2.bin",                   2376 },
		{ "/Signal/1.bin",                   2465 },
		{ "/Signal/0.bin",                   1122 },
		{ "/Signal/3.bin",                   1611 },
		{ "/DriveScreen/Boost_segment.bin",  566 },
		{ "/DriveScreen/BallHue.bin",        6578 },
		{ "/DriveScreen/Boost_empty.bin",    15059 },
		{ "/DriveScreen/MarkerRotate.bin",   7042 },
		{ "/DriveScreen/LineStatusOff.bin",  1712 },
		{ "/DriveScreen/SpeedText.bin",      11853 },
		{ "/DriveScreen/ModalBg.bin",        111438 },
		{ "/DriveScreen/MarkerBackward.bin", 7086 },
		{ "/DriveScreen/Board.bin",          419996 },
		{ "/DriveScreen/Boost_full.bin",     27513 },
		{ "/DriveScreen/BallHueText.bin",    25877 },
		{ "/DriveScreen/SpeedBar.bin",       13735 },
		{ "/DriveScreen/BallHuePicker.bin",  33687 },
		{ "/DriveScreen/SpeedBarFill.bin",   434 },
		{ "/DriveScreen/MarkerForward.bin",  7086 },
		{ "/DriveScreen/BallHueBar.bin",     165866 },
		{ "/DriveScreen/LineStatusOn.bin",   1750 },
		{ "/DriveScreen/MarkerBurnout.bin",  7682 },
		{ "/Intro/14.bin",                   3129884 },
		{ "/Intro/23.bin",                   2522292 },
		{ "/Intro/9.bin",                    3218088 },
		{ "/Intro/18.bin",                   3403338 },
		{ "/Intro/7.bin",                    3220743 },
		{ "/Intro/10.bin",                   3184980 },
		{ "/Intro/13.bin",                   3157106 },
		{ "/Intro/24.bin",                   2310435 },
		{ "/Intro/27.bin",                   150 },
		{ "/Intro/5.bin",                    3245080 },
		{ "/Intro/11.bin",                   3168581 },
		{ "/Intro/17.bin",                   3425135 },
		{ "/Intro/19.bin",                   3474448 },
		{ "/Intro/25.bin",                   2255700 },
		{ "/Intro/20.bin",                   3475809 },
		{ "/Intro/6.bin",                    3237588 },
		{ "/Intro/8.bin",                    3222292 },
		{ "/Intro/2.bin",                    3285535 },
		{ "/Intro/1.bin",                    3306917 },
		{ "/Intro/21.bin",                   3253244 },
		{ "/Intro/4.bin",                    3246651 },
		{ "/Intro/22.bin",                   2854794 },
		{ "/Intro/26.bin",                   2087113 },
		{ "/Intro/12.bin",                   3140012 },
		{ "/Intro/16.bin",                   3472294 },
		{ "/Intro/0.bin",                    3310705 },
		{ "/Intro/3.bin",                    3270554 },
		{ "/Intro/15.bin",                   3184564 },
		{ "/bg.bin",                         3410322 },
		{ "/Menu/Label/Line.bin",            45338 },
		{ "/Menu/Label/Ball.bin",            45537 },
		{ "/Menu/Label/Manual.bin",          51894 },
		{ "/Menu/Label/SimpleProg.bin",      63363 },
		{ "/Menu/Label/Dance.bin",           43815 },
		{ "/Menu/Label/Marker.bin",          57182 },
		{ "/Menu/Label/Settings.bin",        33871 },
		{ "/Menu/Big/Ball.gif",              321693 },
		{ "/Menu/Big/Marker.gif",            1033020 },
		{ "/Menu/Big/Manual.gif",            1401293 },
		{ "/Menu/Big/SimpleProg.gif",        498868 },
		{ "/Menu/Big/Settings.gif",          594852 },
		{ "/Menu/Big/Dance.gif",             628614 },
		{ "/Menu/Big/Line.gif",              85728 },
		{ "/Menu/Small/SimpleProg_b.bin",    7248 },
		{ "/Menu/Small/Ball_b.bin",          5678 },
		{ "/Menu/Small/Dance_b.bin",         2843 },
		{ "/Menu/Small/Manual_b.bin",        6306 },
		{ "/Menu/Small/Line_r.bin",          7007 },
		{ "/Menu/Small/Dance_r.bin",         2040 },
		{ "/Menu/Small/Marker_b.bin",        8180 },
		{ "/Menu/Small/SimpleProg_r.bin",    8586 },
		{ "/Menu/Small/Ball_r.bin",          4875 },
		{ "/Menu/Small/Settings_b.bin",      4218 },
		{ "/Menu/Small/Line_b.bin",          8325 },
		{ "/Menu/Small/Manual_r.bin",        6789 },
		{ "/Menu/Small/Settings_r.bin",      3189 },
		{ "/Menu/Small/Marker_r.bin",        7475 },
};

#endif //BATCONTROLLER_FIRMWARE_SPIFFSCHECKSUM_H
