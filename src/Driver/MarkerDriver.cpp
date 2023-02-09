#include "MarkerDriver.h"

MarkerDriver::MarkerDriver(lv_obj_t* container) : Driver(DriveMode::Marker), autoControls(container), markerElement(container, MarkerAction::None),
												  batsElement(container){
}

MarkerDriver::~MarkerDriver(){
	stop();
}

void MarkerDriver::onFrame(const DriveInfo& frame, Color* pixels){
	auto markerInfo = frame.toMarker();
	if(markerInfo == nullptr) return;

	auto action = markerInfo->action;

	markerElement.setAction(action);
	autoControls.setDirection(frame.motors);

	if(markerInfo->markers.empty()) return;

	if(markerInfo->markers[0].id == 100){
		batsElement.startAnim();
	}

	static const auto drawLine = [&pixels](int x1, int y1, int x2, int y2){
		static const auto drawPixel = [&pixels](int x, int y){
			int i = y * 160 + x;
			pixels[i] = C_RGB(255, 0, 0);
		};

		int dx = abs (x2 - x1), sx = x1 < x2 ? 1 : -1;
		int dy = -abs (y2 - y1), sy = y1 < y2 ? 1 : -1;
		int err = dx + dy, e2; /* error value e_xy */

		for(;;){  /* loop */
			drawPixel(x1, y1);

			if(x1 == x2 && y1 == y2) break;

			e2 = 2 * err;

			if(e2 >= dy){
				err += dy;
				x1 += sx;
			}

			if(e2 <= dx){
				err += dx;
				y1 += sy;
			}
		}
	};

	struct DrawInfo {
		int x, y;
		uint16_t id;
	};
	std::vector<DrawInfo> infos;
	infos.reserve(markerInfo->markers.size());

	for(const auto& marker: markerInfo->markers){
		DrawInfo info = { .x = 0, .y = 0, .id = marker.id };

		for(int i = 0; i < 4; i++){
			drawLine(marker.projected[i].x, marker.projected[i].y, marker.projected[(i + 1) % 4].x, marker.projected[(i + 1) % 4].y);
			info.x += marker.projected[i].x;
			info.y += marker.projected[i].y;
		}

		info.x /= 4;
		info.y /= 4;
		infos.push_back(info);
	}

	lv_obj_t* canvas = lv_canvas_create(nullptr);
	lv_canvas_set_buffer(canvas, pixels, 160, 120, LV_IMG_CF_TRUE_COLOR);

	lv_draw_label_dsc_t draw;
	lv_draw_label_dsc_init(&draw);
	draw.color = lv_color_make(255, 0, 0);
	draw.opa = LV_OPA_COVER;
	draw.font = &lv_font_unscii_8;
	draw.align = LV_TEXT_ALIGN_CENTER;
	draw.ofs_x = -42 / 2;
	draw.ofs_y = -4;

	for(const auto& info: infos){
		char text[12];
		sprintf(text, "%d", info.id);
		lv_canvas_draw_text(canvas, info.x, info.y, 42, &draw, text);
	}

	lv_obj_del(canvas);
}

void MarkerDriver::onStart(){
	autoControls.start();
}

void MarkerDriver::onStop(){
	autoControls.stop();
}
