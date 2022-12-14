#include <core/lv_disp.h>
#include <misc/lv_gc.h>
#include <extra/widgets/msgbox/lv_msgbox.h>
#include "BatTheme.h"

#define MODE_DARK 0

#define LIGHT_COLOR_TEXT       lv_palette_darken(LV_PALETTE_GREY, 4)
#define DARK_COLOR_TEXT        lv_palette_lighten(LV_PALETTE_GREY, 5)

#define OUTLINE_WIDTH           lv_disp_dpx(theme.disp, 3)
#define TRANSITION_TIME         80

#define COLOR_WHITE   lv_color_white()
#define COLOR_LIGHT   lv_palette_lighten(LV_PALETTE_GREY, 3)
#define COLOR_MID     lv_palette_lighten(LV_PALETTE_GREY, 1)
#define COLOR_DARK    lv_palette_main(LV_PALETTE_GREY)
#define COLOR_DIM     lv_palette_darken(LV_PALETTE_GREY, 2)
#define PAD_DEF       LV_DPX(5)


#define RADIUS_DEFAULT (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 12) : lv_disp_dpx(theme.disp, 8))

#define PAD_DEF     (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 24) : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 20) : lv_disp_dpx(theme.disp, 16))
#define PAD_SMALL   (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 14) : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 12) : lv_disp_dpx(theme.disp, 10))
#define PAD_TINY   (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 8) : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 6) : lv_disp_dpx(theme.disp, 2))

typedef struct {
	lv_style_t scr;
	lv_style_t scrollbar;
	lv_style_t scrollbar_scrolled;
	lv_style_t card;
	lv_style_t btn;
    lv_style_t light;

	/*Utility*/
	lv_style_t bg_color_primary;
	lv_style_t bg_color_primary_muted;
	lv_style_t bg_color_secondary;
	lv_style_t bg_color_secondary_muted;
	lv_style_t bg_color_grey;
	lv_style_t bg_color_white;
	lv_style_t pressed;
	lv_style_t disabled;
	lv_style_t pad_zero;
	lv_style_t pad_tiny;
	lv_style_t pad_small;
	lv_style_t pad_normal;
	lv_style_t pad_gap;
	lv_style_t line_space_large;
	lv_style_t text_align_center;
	lv_style_t outline_primary;
	lv_style_t outline_secondary;
	lv_style_t circle;
	lv_style_t no_radius;
	lv_style_t clip_corner;
	lv_style_t grow;
	lv_style_t transition_delayed;
	lv_style_t transition_normal;
	lv_style_t anim;
	lv_style_t anim_fast;
	/*Parts*/
	lv_style_t knob;
	lv_style_t indic;

	lv_style_t arc_indic;
	lv_style_t arc_indic_primary;

	lv_style_t chart_series, chart_indic, chart_ticks, chart_bg;

	lv_style_t dropdown_list;

	lv_style_t cb_marker, cb_marker_checked;

	lv_style_t switch_knob;

	lv_style_t line;

	lv_style_t table_cell;

	lv_style_t meter_marker, meter_indic;

	lv_style_t ta_cursor, ta_placeholder;

	lv_style_t calendar_btnm_bg, calendar_btnm_day, calendar_header;

	lv_style_t colorwheel_main;

	lv_style_t msgbox_bg, msgbox_btn_bg, msgbox_backdrop_bg;

	lv_style_t keyboard_btn_bg;

	lv_style_t list_bg, list_btn, list_item_grow, list_label;

	lv_style_t tab_bg_focus, tab_btn;

	lv_style_t led;

    lv_style_t text_area;
} my_theme_styles_t;

typedef struct {
	lv_theme_t base;
	uint8_t light: 1;
} my_theme_t;

typedef enum {
	DISP_SMALL = 3,
	DISP_MEDIUM = 2,
	DISP_LARGE = 1,
} disp_size_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void theme_apply(lv_theme_t* th, lv_obj_t* obj);

static void style_init_reset(lv_style_t* style);

/**********************
 *  STATIC VARIABLES
 **********************/
static my_theme_styles_t* styles;
static lv_theme_t theme;
static disp_size_t disp_size;
static bool inited;
static lv_color_t color_scr;
static lv_color_t color_text;
static lv_color_t color_card;
static lv_color_t color_grey;


/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/
static lv_color_t grey_filter_cb(const lv_color_filter_dsc_t* f, lv_color_t color, lv_opa_t opa){
	LV_UNUSED(f);
	if(theme.flags & MODE_DARK) return lv_color_mix(lv_palette_darken(LV_PALETTE_GREY, 2), color, opa);
	else return lv_color_mix(lv_palette_lighten(LV_PALETTE_GREY, 2), color, opa);
}


static lv_color_t dark_color_filter_cb(const lv_color_filter_dsc_t * f, lv_color_t c, lv_opa_t opa)
{
    LV_UNUSED(f);
    return lv_color_darken(c, opa);
}


static void style_init(void){
	static const lv_style_prop_t trans_props[] = {
			LV_STYLE_BG_OPA, LV_STYLE_BG_COLOR,
			LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT,
			LV_STYLE_TRANSLATE_Y, LV_STYLE_TRANSLATE_X,
			LV_STYLE_TRANSFORM_ZOOM, LV_STYLE_TRANSFORM_ANGLE,
			LV_STYLE_COLOR_FILTER_OPA, LV_STYLE_COLOR_FILTER_DSC,

	};

	static lv_color_filter_dsc_t grey_filter;
	lv_color_filter_dsc_init(&grey_filter, grey_filter_cb);

	static lv_style_transition_dsc_t trans_normal;
	lv_style_transition_dsc_init(&trans_normal, trans_props, lv_anim_path_linear, TRANSITION_TIME, 0, NULL);

	color_text = theme.flags & MODE_DARK ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT;

	style_init_reset(&styles->scrollbar);
	lv_style_set_bg_color(&styles->scrollbar, (theme.flags & MODE_DARK) ? lv_palette_darken(LV_PALETTE_GREY, 2) : lv_palette_main(LV_PALETTE_GREY));

	lv_style_set_radius(&styles->scrollbar, LV_RADIUS_CIRCLE);
	lv_style_set_pad_right(&styles->scrollbar, lv_disp_dpx(theme.disp, 7));
	lv_style_set_pad_top(&styles->scrollbar, lv_disp_dpx(theme.disp, 7));
	lv_style_set_size(&styles->scrollbar, lv_disp_dpx(theme.disp, 5));
	lv_style_set_bg_opa(&styles->scrollbar, LV_OPA_40);

	style_init_reset(&styles->card);
	lv_style_set_bg_opa(&styles->card, LV_OPA_COVER);
	lv_style_set_bg_color(&styles->card, color_card);
	lv_style_set_border_color(&styles->card, color_grey);
	lv_style_set_text_color(&styles->card, color_text);
	lv_style_set_line_width(&styles->card, lv_disp_dpx(theme.disp, 1));

	style_init_reset(&styles->ta_cursor);
	lv_style_set_border_color(&styles->ta_cursor, color_text);
	lv_style_set_border_width(&styles->ta_cursor, lv_disp_dpx(theme.disp, 1));
	lv_style_set_pad_left(&styles->ta_cursor, -lv_disp_dpx(theme.disp, 1));
	lv_style_set_translate_y(&styles->ta_cursor, 1);
	lv_style_set_border_side(&styles->ta_cursor, LV_BORDER_SIDE_LEFT);
	lv_style_set_anim_time(&styles->ta_cursor, 500);

	style_init_reset(&styles->bg_color_primary_muted);
	lv_style_set_bg_color(&styles->bg_color_primary_muted, theme.color_primary);
	lv_style_set_text_color(&styles->bg_color_primary_muted, theme.color_primary);
	lv_style_set_bg_opa(&styles->bg_color_primary_muted, LV_OPA_20);

	style_init_reset(&styles->circle);
	lv_style_set_radius(&styles->circle, LV_RADIUS_CIRCLE);

	style_init_reset(&styles->knob);
	lv_style_set_bg_color(&styles->knob, theme.color_primary);
	lv_style_set_bg_opa(&styles->knob, LV_OPA_COVER);
	lv_style_set_pad_all(&styles->knob, lv_disp_dpx(theme.disp, 6));
	lv_style_set_radius(&styles->knob, LV_RADIUS_CIRCLE);

	style_init_reset(&styles->ta_placeholder);
	lv_style_set_text_color(&styles->ta_placeholder, (theme.flags & MODE_DARK) ? lv_palette_darken(LV_PALETTE_GREY,
																								   2) : lv_palette_lighten(LV_PALETTE_GREY, 1));
	style_init_reset(&styles->outline_primary);
	lv_style_set_outline_color(&styles->outline_primary, theme.color_primary);
	lv_style_set_outline_width(&styles->outline_primary, OUTLINE_WIDTH);
	lv_style_set_outline_pad(&styles->outline_primary, OUTLINE_WIDTH);
	lv_style_set_outline_opa(&styles->outline_primary, LV_OPA_50);

	style_init_reset(&styles->outline_secondary);
	lv_style_set_outline_color(&styles->outline_secondary, theme.color_secondary);
	lv_style_set_outline_width(&styles->outline_secondary, OUTLINE_WIDTH);
	lv_style_set_outline_opa(&styles->outline_secondary, LV_OPA_50);

    style_init_reset(&styles->keyboard_btn_bg);
    lv_style_set_shadow_width(&styles->keyboard_btn_bg, 0);
    lv_style_set_radius(&styles->keyboard_btn_bg, disp_size == DISP_SMALL ? RADIUS_DEFAULT / 2 : RADIUS_DEFAULT);

    style_init_reset(&styles->scr);
    lv_style_set_bg_opa(&styles->scr, LV_OPA_COVER);
    lv_style_set_bg_color(&styles->scr, color_scr);
    lv_style_set_text_color(&styles->scr, color_text);
    lv_style_set_pad_row(&styles->scr, PAD_SMALL);
    lv_style_set_pad_column(&styles->scr, PAD_SMALL);

    style_init_reset(&styles->pad_tiny);
    lv_style_set_pad_all(&styles->pad_tiny, PAD_TINY);
    lv_style_set_pad_row(&styles->pad_tiny, PAD_TINY);
    lv_style_set_pad_column(&styles->pad_tiny, PAD_TINY);

    style_init_reset(&styles->pad_small);
    lv_style_set_pad_all(&styles->pad_small, PAD_SMALL);
    lv_style_set_pad_gap(&styles->pad_small, PAD_SMALL);

    style_init_reset(&styles->btn);
    lv_style_set_radius(&styles->btn, (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp,
                                                                             16) : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 12) : lv_disp_dpx(theme.disp, 8)));
    lv_style_set_bg_opa(&styles->btn, LV_OPA_COVER);
    lv_style_set_bg_color(&styles->btn, color_grey);
    if(!(theme.flags & MODE_DARK)) {
        lv_style_set_shadow_color(&styles->btn, lv_palette_main(LV_PALETTE_GREY));
        lv_style_set_shadow_width(&styles->btn, LV_DPX(3));
        lv_style_set_shadow_opa(&styles->btn, LV_OPA_50);
        lv_style_set_shadow_ofs_y(&styles->btn, lv_disp_dpx(theme.disp, LV_DPX(4)));
    }
    lv_style_set_text_color(&styles->btn, color_text);
    lv_style_set_pad_hor(&styles->btn, PAD_DEF);
    lv_style_set_pad_ver(&styles->btn, PAD_SMALL);
    lv_style_set_pad_column(&styles->btn, lv_disp_dpx(theme.disp, 5));
    lv_style_set_pad_row(&styles->btn, lv_disp_dpx(theme.disp, 5));

    static lv_color_filter_dsc_t dark_filter;
    lv_color_filter_dsc_init(&dark_filter, dark_color_filter_cb);

    style_init_reset(&styles->pressed);
    lv_style_set_color_filter_dsc(&styles->pressed, &dark_filter);
    lv_style_set_color_filter_opa(&styles->pressed, 35);

    style_init_reset(&styles->bg_color_secondary_muted);
    lv_style_set_bg_color(&styles->bg_color_secondary_muted, theme.color_secondary);
    lv_style_set_text_color(&styles->bg_color_secondary_muted, theme.color_secondary);
    lv_style_set_bg_opa(&styles->bg_color_secondary_muted, LV_OPA_20);


	style_init_reset(&styles->bg_color_primary);
	lv_style_set_bg_color(&styles->bg_color_primary, theme.color_primary);
	lv_style_set_text_color(&styles->bg_color_primary, lv_color_white());
	lv_style_set_bg_opa(&styles->bg_color_primary, LV_OPA_COVER);

	style_init_reset(&styles->bg_color_grey);
	lv_style_set_bg_color(&styles->bg_color_grey, color_grey);
	lv_style_set_bg_opa(&styles->bg_color_grey, LV_OPA_COVER);
	lv_style_set_text_color(&styles->bg_color_grey, color_text);

	style_init_reset(&styles->anim_fast);
	lv_style_set_anim_time(&styles->anim_fast, 120);

	style_init_reset(&styles->disabled);
	lv_style_set_color_filter_dsc(&styles->disabled, &grey_filter);
	lv_style_set_color_filter_opa(&styles->disabled, LV_OPA_50);

	style_init_reset(&styles->bg_color_white);
	lv_style_set_bg_color(&styles->bg_color_white, color_card);
	lv_style_set_bg_opa(&styles->bg_color_white, LV_OPA_COVER);
	lv_style_set_text_color(&styles->bg_color_white, color_text);

	style_init_reset(&styles->switch_knob);
	lv_style_set_pad_all(&styles->switch_knob, -lv_disp_dpx(theme.disp, 4));
	lv_style_set_bg_color(&styles->switch_knob, lv_color_white());

	style_init_reset(&styles->transition_normal);
	lv_style_set_transition(&styles->transition_normal, &trans_normal); /*Go back to default state with delay*/

    style_init_reset(&styles->light);
    lv_style_set_bg_opa(&styles->light, LV_OPA_COVER);
    lv_style_set_bg_color(&styles->light, COLOR_LIGHT);
    lv_style_set_border_color(&styles->light, COLOR_MID);
    lv_style_set_border_width(&styles->light, 1);
    lv_style_set_pad_all(&styles->light, PAD_DEF);
    lv_style_set_pad_gap(&styles->light, PAD_DEF / 2);
    lv_style_set_line_width(&styles->light, LV_DPX(2));
    lv_style_set_line_color(&styles->light, COLOR_MID);
    lv_style_set_arc_width(&styles->light, LV_DPX(2));
    lv_style_set_arc_color(&styles->light, COLOR_MID);

//    static lv_color_filter_dsc_t dark_filter;
    lv_color_filter_dsc_init(&dark_filter, dark_color_filter_cb);

    style_init_reset(&styles->text_area);
    lv_style_set_border_color(&styles->text_area, lv_color_white());
    lv_style_set_border_width(&styles->text_area, 2);
    lv_style_set_border_side(&styles->text_area, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_LEFT );
    lv_style_set_border_opa(&styles->text_area, LV_OPA_80);
    lv_style_set_pad_all(&styles->text_area, 2);
    lv_style_set_pad_right(&styles->text_area,6);

}
/**********************
*   GLOBAL FUNCTIONS
**********************/

void BatThemeInit(lv_disp_t* disp){

	/*This trick is required only to avoid the garbage collection of
	 *styles' data if LVGL is used in a binding (e.g. Micropython)
	 *In a general case styles could be in simple `static lv_style_t my_style...` variables*/
	if(!inited){
		LV_GC_ROOT(_lv_theme_default_styles) = lv_mem_alloc(sizeof(my_theme_styles_t));
		styles = (my_theme_styles_t*) LV_GC_ROOT(_lv_theme_default_styles);
	}

	if(LV_HOR_RES <= 320) disp_size = DISP_SMALL;
	else if(LV_HOR_RES < 720) disp_size = DISP_MEDIUM;
	else disp_size = DISP_LARGE;

	theme.disp = disp;
	theme.apply_cb = theme_apply;

	style_init();

	inited = true;

	if(disp == NULL || lv_disp_get_theme(disp) == &theme) lv_obj_report_style_change(NULL);

	disp->theme = &theme;

//	lv_disp_set_bg_image(disp, "S/scanAruco.bin");
	lv_disp_set_bg_opa(disp, LV_OPA_COVER);
}

lv_theme_t * BatThemeGet(void)
{
	if(!inited) return NULL;

	return (lv_theme_t *)&theme;
}

bool BatThemeInited(void)
{
	return inited;
}
static void theme_apply(lv_theme_t * th, lv_obj_t * obj)
{
	LV_UNUSED(th);

	if(lv_obj_check_type(obj, &lv_textarea_class)) {
		lv_obj_add_style(obj, &styles->ta_cursor, LV_PART_CURSOR | LV_STATE_FOCUSED);
		lv_obj_add_style(obj, &styles->ta_placeholder, LV_PART_TEXTAREA_PLACEHOLDER);
        lv_obj_add_style(obj, &styles->text_area, 0);
    }

	if(lv_obj_check_type(obj, &lv_obj_class)){
		// lv_obj_add_style(obj, &styles->card, 0);
		lv_obj_add_style(obj, &styles->scrollbar, LV_PART_SCROLLBAR);
	}
	else if(lv_obj_check_type(obj, &lv_slider_class)){
		lv_obj_add_style(obj, &styles->bg_color_primary_muted, 0);
		lv_obj_add_style(obj, &styles->circle, 0);
		lv_obj_add_style(obj, &styles->outline_primary, LV_STATE_FOCUS_KEY);
		lv_obj_add_style(obj, &styles->outline_secondary, LV_STATE_EDITED);
		lv_obj_add_style(obj, &styles->bg_color_primary, LV_PART_INDICATOR);
		lv_obj_add_style(obj, &styles->circle, LV_PART_INDICATOR);
		lv_obj_add_style(obj, &styles->knob, LV_PART_KNOB);

	}else if(lv_obj_check_type(obj, &lv_switch_class)) {
		lv_obj_add_style(obj, &styles->bg_color_grey, 0);
		lv_obj_add_style(obj, &styles->circle, 0);
		lv_obj_add_style(obj, &styles->anim_fast, 0);
		lv_obj_add_style(obj, &styles->disabled, LV_STATE_DISABLED);
		lv_obj_add_style(obj, &styles->outline_primary, LV_STATE_FOCUS_KEY);
		lv_obj_add_style(obj, &styles->bg_color_primary, LV_PART_INDICATOR | LV_STATE_CHECKED);
		lv_obj_add_style(obj, &styles->circle, LV_PART_INDICATOR);
		lv_obj_add_style(obj, &styles->disabled, LV_PART_INDICATOR | LV_STATE_DISABLED);
		lv_obj_add_style(obj, &styles->knob, LV_PART_KNOB);
		lv_obj_add_style(obj, &styles->bg_color_white, LV_PART_KNOB);
		lv_obj_add_style(obj, &styles->switch_knob, LV_PART_KNOB);
		lv_obj_add_style(obj, &styles->disabled, LV_PART_KNOB | LV_STATE_DISABLED);

		lv_obj_add_style(obj, &styles->transition_normal, LV_PART_INDICATOR | LV_STATE_CHECKED);
		lv_obj_add_style(obj, &styles->transition_normal, LV_PART_INDICATOR);

    }else if(lv_obj_check_type(obj, &lv_keyboard_class)) {
        lv_obj_add_style(obj, &styles->light, 0);
        lv_obj_add_style(obj, &styles->light, LV_PART_ITEMS);
        lv_obj_add_style(obj, &styles->pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, disp_size == DISP_LARGE ? &styles->pad_small : &styles->pad_tiny, 0);
        lv_obj_add_style(obj, &styles->outline_primary, LV_STATE_FOCUS_KEY);
//        lv_obj_add_style(obj, &styles->outline_secondary, LV_STATE_EDITED);
//        lv_obj_add_style(obj, &styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
//        lv_obj_add_style(obj, &styles->bg_color_white, LV_PART_ITEMS);
//        lv_obj_add_style(obj, &styles->keyboard_btn_bg, LV_PART_ITEMS);
//        lv_obj_add_style(obj, &styles->bg_color_grey, LV_PART_ITEMS | LV_STATE_CHECKED);
//        lv_obj_add_style(obj, &styles->bg_color_primary_muted, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
//        lv_obj_add_style(obj, &styles->bg_color_secondary_muted, LV_PART_ITEMS | LV_STATE_EDITED);
    }

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init_reset(lv_style_t* style){
	if(inited) lv_style_reset(style);
	else lv_style_init(style);
}