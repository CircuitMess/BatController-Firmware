#include <SPIFFS.h>
#include "FSLVGL.h"
#include <FS/RamFile.h>

const char* FSLVGL::cached[] = {
		"/Menu/Small/Ball_b.bin",
		"/Menu/Small/Ball_r.bin",
		"/Menu/Small/SimpleProg_b.bin",
		"/Menu/Small/SimpleProg_r.bin",
		"/Menu/Small/Dance_b.bin",
		"/Menu/Small/Dance_r.bin",
		"/Menu/Small/Manual_b.bin",
		"/Menu/Small/Manual_r.bin",
		"/Menu/Small/Marker_b.bin",
		"/Menu/Small/Marker_r.bin",
		"/Menu/Small/Settings_b.bin",
		"/Menu/Small/Settings_r.bin",
		"/Menu/Label/Ball.bin",
		"/Menu/Label/SimpleProg.bin",
		"/Menu/Label/Dance.bin",
		"/Menu/Label/Manual.bin",
		"/Menu/Label/Marker.bin",
		"/Menu/Label/Settings.bin",
		// "/DriveScreen/ModalBg.bin",
		"/DriveScreen/BallHue.bin",
		"/DriveScreen/MarkerBackward.bin",
		"/DriveScreen/MarkerForward.bin",
		"/DriveScreen/MarkerRotate.bin",
		"/DriveScreen/MarkerBurnout.bin",
		"/DriveScreen/BallHuePicker.bin",
		"/DriveScreen/BallHueBar.bin",
		"/DriveScreen/SpeedBar.bin",
		"/DriveScreen/SpeedBarFill.bin",
		"/DriveScreen/Boost_full.bin",
		"/DriveScreen/Boost_empty.bin",
		"/DriveScreen/Boost_segment.bin",
		"/Signal/0.bin",
		"/Signal/1.bin",
		"/Signal/2.bin",
		"/Signal/3.bin",
		"/Battery/0.bin",
		"/Battery/1.bin",
		"/Battery/2.bin",
		"/Battery/3.bin",
		"/Battery/4.bin",
		"/Battery/5.bin",
		"/Battery/6.bin",
		"/Battery/7.bin",
		"/Battery/Batmobile.bin",
		"/Battery/Controller.bin",
		"/DriveMode/Ball.bin",
		"/DriveMode/Idle.bin",
		"/DriveMode/Line.bin",
		"/DriveMode/Marker.bin",
		"/DriveMode/Manual.bin",
		"/DriveMode/SimpleProg.bin"
};

const char* FSLVGL::cachedSimple[] = {
		"/SimpleProg/Backlight.bin",
		"/SimpleProg/Delay.bin",
		"/SimpleProg/Drive.bin",
		"/SimpleProg/footer_1.bin",
		"/SimpleProg/footer_2.bin",
		"/SimpleProg/Frontlight.bin",
		"/SimpleProg/new.bin",
		"/SimpleProg/newFocused.bin",
		"/SimpleProg/Sound.bin",
		"/SimpleProg/Underlight.bin"
};

std::unordered_map<std::string, fs::File*> FSLVGL::cache;
fs::File* FSLVGL::specialCache = nullptr;
bool FSLVGL::cacheLoaded = false;
bool FSLVGL::simpleLoaded = false;

FSLVGL::FSLVGL(fs::FS& filesystem, char letter) : filesys(filesystem){
	size_t cacheCount = (sizeof(cached) / sizeof(cached[0]));
	size_t simpleCount = (sizeof(cachedSimple) / sizeof(cachedSimple[0]));
	cache.reserve((cacheCount + simpleCount) * 2);

	lv_fs_drv_init(&drv);                     /*Basic initialization*/

	drv.letter = letter;                         /*An uppercase letter to identify the drive */
	drv.ready_cb = ready_cb;               /*Callback to tell if the drive is ready to use */
	drv.open_cb = open_cb;                 /*Callback to open a file */
	drv.close_cb = close_cb;               /*Callback to close a file */
	drv.read_cb = read_cb;                 /*Callback to read a file */
	drv.write_cb = write_cb;               /*Callback to write a file */
	drv.seek_cb = seek_cb;                 /*Callback to seek in a file (Move cursor) */
	drv.tell_cb = tell_cb;                 /*Callback to tell the cursor position  */

	drv.dir_open_cb = dir_open_cb;         /*Callback to open directory to read its content */
	drv.dir_read_cb = dir_read_cb;         /*Callback to read a directory's content */
	drv.dir_close_cb = dir_close_cb;       /*Callback to close a directory */

	drv.user_data = this;             /*Any custom data if required*/

	lv_fs_drv_register(&drv);                 /*Finally register the drive*/
}

void FSLVGL::loadCache(){
	if(cacheLoaded) return;
	cacheLoaded = true;
	for(const char* path : cached){
		File file = SPIFFS.open(path);
		if(!file) continue;

		auto pair = cache.find(path);
		if(pair == cache.end()){
			File* ram = new fs::File();
			*ram = RamFile::open(file);

			cache.insert(std::make_pair(path, ram));
		}else{
			*pair->second = RamFile::open(file);
		}
	}
}

void FSLVGL::unloadCache(){
	if(!cacheLoaded) return;
	cacheLoaded = false;

	for(auto& pair : cache){
		pair.second->close();
		*pair.second = File();
	}
}

bool FSLVGL::ready_cb(struct _lv_fs_drv_t* drv){
	return true;
}

void* FSLVGL::open_cb(struct _lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode){
	auto entry = cache.find(path);
	if(entry != cache.end()){
		File* file = entry->second;
		file->seek(0);
		return file;
	}
	if(specialCache != nullptr){
		if(strcmp(specialCache->name(), path) == 0){
			specialCache->seek(0);
			return specialCache;
		}
	}

	const char* fsMode;
	switch(mode){
		case LV_FS_MODE_WR:
			fsMode = FILE_WRITE;
			break;
		default:
			fsMode = FILE_READ;
	}

	return (void*) new fs::File(static_cast<FSLVGL*>(drv->user_data)->getFS().open(path, fsMode));
}

fs::FS& FSLVGL::getFS(){
	return filesys;
}

lv_fs_res_t FSLVGL::close_cb(struct _lv_fs_drv_t* drv, void* file_p){
	if(cache.count(static_cast<File*>(file_p)->name())) return 0;
	if(specialCache == static_cast<File*>(file_p)) return 0;

	static_cast<fs::File*>(file_p)->close();
	delete static_cast<fs::File*>(file_p);
	return 0;
}

lv_fs_res_t FSLVGL::read_cb(struct _lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br){
	if(!static_cast<File*>(file_p)->operator bool()) return LV_FS_RES_NOT_EX;
	*br = static_cast<fs::File*>(file_p)->read((uint8_t*) buf, btr);
	return 0;
}

lv_fs_res_t FSLVGL::write_cb(struct _lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw){
	if(!static_cast<File*>(file_p)->operator bool()) return LV_FS_RES_NOT_EX;
	*bw = static_cast<fs::File*>(file_p)->write((uint8_t*) buf, btw);
	return 0;
}

lv_fs_res_t FSLVGL::seek_cb(struct _lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence){
	if(!static_cast<File*>(file_p)->operator bool()) return LV_FS_RES_NOT_EX;

	SeekMode mode;
	switch(whence){
		case LV_FS_SEEK_SET:
			mode = fs::SeekSet;
			break;
		case LV_FS_SEEK_CUR:
			mode = fs::SeekCur;
			break;
		case LV_FS_SEEK_END:
			mode = fs::SeekEnd;
			break;
	}
	if(!static_cast<fs::File*>(file_p)->seek(pos, mode)){
		return LV_FS_RES_INV_PARAM;
	}
	return 0;

}

lv_fs_res_t FSLVGL::tell_cb(struct _lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p){
	*pos_p = static_cast<fs::File*>(file_p)->position();
	if(*pos_p == (uint32_t) -1) return LV_FS_RES_UNKNOWN;
	return 0;
}

void* FSLVGL::dir_open_cb(struct _lv_fs_drv_t* drv, const char* path){
	return (void*) new fs::File(static_cast<FSLVGL*>(drv->user_data)->getFS().open(path));
}

lv_fs_res_t FSLVGL::dir_read_cb(struct _lv_fs_drv_t* drv, void* rddir_p, char* fn){
	if(!static_cast<fs::File*>(rddir_p)->isDirectory()) return LV_FS_RES_NOT_EX;
	fs::File temp = static_cast<fs::File*>(rddir_p)->openNextFile();
	if(!temp) return LV_FS_RES_NOT_EX;
	strncpy(fn, temp.name(), 256);
	return 0;
}

lv_fs_res_t FSLVGL::dir_close_cb(struct _lv_fs_drv_t* drv, void* rddir_p){
	static_cast<fs::File*>(rddir_p)->close();
	delete static_cast<fs::File*>(rddir_p);
	return 0;
}

void FSLVGL::loadSpecialCache(const char* path){
	unloadSpecialCache();
	File file = SPIFFS.open(path);
	if(!file) return;

	specialCache = new fs::File();
	*specialCache = RamFile::open(file);
	file.close();
}

void FSLVGL::unloadSpecialCache(){
	delete specialCache;
	specialCache = nullptr;
}

void FSLVGL::addCache(const char* path){
	File file = SPIFFS.open(path);
	if(!file) return;

	auto pair = cache.find(path);
	if(pair == cache.end()){
		lv_img_cache_invalidate_src((std::string("S:") + path).c_str());
		File* ram = new fs::File();
		*ram = RamFile::open(file);

		cache.insert(std::make_pair(path, ram));
	}
}

void FSLVGL::removeCache(const char* path){
	auto pair = cache.find(path);
	if(pair == cache.end()) return;

	lv_img_cache_invalidate_src((std::string("S:") + path).c_str());
	delete pair->second;
	cache.erase(pair);
}

void FSLVGL::loadSimple(){
	if(simpleLoaded) return;
	simpleLoaded = true;

	for(const auto& path : cachedSimple){
		addCache(path);
	}
}

void FSLVGL::unloadSimple(){
	if(!simpleLoaded) return;
	simpleLoaded = false;

	for(const auto& path : cachedSimple){
		removeCache(path);
	}
}
