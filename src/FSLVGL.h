#ifndef BATCONTROLLER_FIRMWARE_FSLVGL_H
#define BATCONTROLLER_FIRMWARE_FSLVGL_H

#include <FS.h>
#include <lvgl.h>
#include <unordered_map>
#include <string>

class FSLVGL {
public:
	FSLVGL(fs::FS& filesystem, char letter);
	fs::FS& getFS();

	static void loadCache();
	static void unloadCache();

	template<std::size_t SIZE>
	static void addCache(const std::array<const char*, SIZE>& paths){
		for(auto path: paths){
			addCache(path);
		}
	}

	static void addCache(const char* path);

	template<std::size_t SIZE>
	static void removeCache(const std::array<const char*, SIZE>& paths){
		for(auto path: paths){
			removeCache(path);
		}
	}

	static void removeCache(const char* path);

	static void loadSpecialCache(const char* path);
	static void unloadSpecialCache();

	static void loadSimple();
	static void unloadSimple();

	static void loadFleha();
	static void unloadFleha();

private:
	static const char* cached[];
	static bool cacheLoaded;
	static std::unordered_map<std::string, fs::File*> cache;
	static fs::File* specialCache;

	static const char* cachedSimple[];
	static bool simpleLoaded;

	static bool flehaLoaded;

	lv_fs_drv_t drv;                   /*Needs to be static or global*/
	fs::FS filesys;


	static bool ready_cb(struct _lv_fs_drv_t* drv);
	static void* open_cb(struct _lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode);
	static lv_fs_res_t close_cb(struct _lv_fs_drv_t* drv, void* file_p);
	static lv_fs_res_t read_cb(struct _lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br);
	static lv_fs_res_t write_cb(struct _lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw);
	static lv_fs_res_t seek_cb(struct _lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence);
	static lv_fs_res_t tell_cb(struct _lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p);
	static void* dir_open_cb(struct _lv_fs_drv_t* drv, const char* path);
	static lv_fs_res_t dir_read_cb(struct _lv_fs_drv_t* drv, void* rddir_p, char* fn);
	static lv_fs_res_t dir_close_cb(struct _lv_fs_drv_t* drv, void* rddir_p);
};


#endif //BATCONTROLLER_FIRMWARE_FSLVGL_H
