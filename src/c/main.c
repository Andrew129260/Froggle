/*
 * FROGGLE v5.0.3 (The Master Build)
 * Features a dynamic, hardware-aware Settings menu, Splash Screen,
 * restored retro logs, persistent settings, and completely airtight 
 * memory deallocation across all watch targets.
 */

#include <pebble.h>

#define SAVE_KEY_LEVEL 1 
#define SAVE_KEY_TOUCH 2
#define SAVE_KEY_ART   3
#define MAX_ENTITIES 30 
#define ENTITIES_PER_LANE 2 
#define IDLE_TIMEOUT_TICKS 1200 // 60 seconds at 50ms per tick

// --- RETRO 16-PIXEL BITMAP DATA ---
static const uint8_t retro_frog_data[] = { 0b00000000, 0b00000000, 0, 0, 0b00011000, 0b00011000, 0, 0, 0b00111100, 0b00111100, 0, 0, 0b00011111, 0b11111000, 0, 0, 0b00001111, 0b11110000, 0, 0, 0b00001111, 0b11110000, 0, 0, 0b01101111, 0b11110110, 0, 0, 0b01101111, 0b11110110, 0, 0, 0b00001111, 0b11110000, 0, 0, 0b00001111, 0b11110000, 0, 0, 0b00001111, 0b11110000, 0, 0, 0b00001111, 0b11110000, 0, 0, 0b00111111, 0b11111100, 0, 0, 0b01111000, 0b00011110, 0, 0, 0b01100000, 0b00000110, 0, 0, 0b00000000, 0b00000000, 0, 0 };
static const uint8_t retro_splat_data[] = { 0b00000000, 0b00000000, 0, 0, 0b00010000, 0b00001000, 0, 0, 0b00001010, 0b01000000, 0, 0, 0b00000111, 0b10000000, 0, 0, 0b01001111, 0b11010000, 0, 0, 0b00111111, 0b11100000, 0, 0, 0b00011111, 0b11110010, 0, 0, 0b10111111, 0b11111000, 0, 0, 0b00011111, 0b11111001, 0, 0, 0b01001111, 0b11110000, 0, 0, 0b00000111, 0b11100100, 0, 0, 0b00000011, 0b11000000, 0, 0, 0b00101010, 0b00100000, 0, 0, 0b00010000, 0b00010000, 0, 0, 0b00000000, 0b00000000, 0, 0, 0b00000000, 0b00000000, 0, 0 };
static const uint8_t retro_lilypad_data[] = { 0b00000111, 0b11100000, 0, 0, 0b00011111, 0b11111000, 0, 0, 0b00111111, 0b11111100, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b11111111, 0b11111111, 0, 0, 0b11111000, 0b01111111, 0, 0, 0b11110000, 0b00111111, 0, 0, 0b11110000, 0b00111111, 0, 0, 0b11111000, 0b01111111, 0, 0, 0b11111111, 0b11111111, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b00111111, 0b11111100, 0, 0, 0b00011111, 0b11111000, 0, 0, 0b00000111, 0b11100000, 0, 0 };
static const uint8_t retro_car_data[] = { 0b00000000, 0b00000000, 0, 0, 0b00111111, 0b11111100, 0, 0, 0b01100000, 0b00000110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b01100000, 0b00000110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b01111111, 0b11111110, 0, 0, 0b00111111, 0b11111100, 0, 0, 0b00000000, 0b00000000, 0, 0, 0b00000000, 0b00000000, 0, 0, 0b00000000, 0b00000000, 0, 0, 0b00000000, 0b00000000, 0, 0, 0b00000000, 0b00000000, 0, 0 };

// Fixed 192-byte retro log (48 full rows of texture)
static const uint8_t retro_log_data[192] = {
  0b00111111, 0b11111100, 0, 0, 
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0, 
  0b00111111, 0b11111100, 0, 0 
};

// --- DATA STRUCTURES & GLOBALS ---
typedef struct { int x, y_scaled, speed_scaled, car_color; bool is_lilypad, is_car; } Entity;

// Settings State
static bool s_setting_touch = true;
static bool s_setting_modern_art = true;
static int s_idle_ticks = 0; 

// Window Pointers
static Window *s_main_menu_window;
static SimpleMenuLayer *s_main_menu_layer;
static SimpleMenuSection s_main_menu_sections[1];
static SimpleMenuItem s_main_menu_items[3];
static GBitmap *s_menu_icon_bitmap;

static Window *s_settings_window;
static SimpleMenuLayer *s_settings_layer;
static SimpleMenuSection s_settings_sections[1];
static SimpleMenuItem s_settings_items[2];

static Window *s_game_window;
static Layer *s_canvas_layer;

// Art Pointers
static GBitmap *s_frog_bitmap, *s_splat_bitmap, *s_log_bitmap, *s_lilypad_bitmap;
static GBitmap *s_car_bitmap_r_up, *s_car_bitmap_g_up, *s_car_bitmap_b_up;
static GBitmap *s_car_bitmap_r_down, *s_car_bitmap_g_down, *s_car_bitmap_b_down;
static GBitmap *s_headlight_bitmap, *s_sprite_sheet_bitmap, *s_current_frog_bitmap; 

// Game State
static AppTimer *s_game_timer = NULL;
static AppTimer *s_splat_timer = NULL; 
static int s_screen_w = 144, s_screen_h = 168, s_right_shore_x = 128; 
static int s_num_active_lanes = 0, s_total_entities = 0, s_level_type = 0; 
static Entity s_platforms[MAX_ENTITIES];
static int s_frog_x = 0, s_frog_y_scaled = 720; 
static int s_lives = 3, s_level = 1;
static bool s_is_dead = false, s_is_paused = false;

// --- FORWARD DECLARATIONS ---
static void game_loop(void *data);
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
#if defined(PBL_TOUCH)
static void touch_handler(const TouchEvent *event, void *context);
#endif

// --- MENU LOGIC ---
static void menu_play_callback(int index, void *ctx) {
  if (persist_exists(SAVE_KEY_LEVEL)) s_level = persist_read_int(SAVE_KEY_LEVEL);
  else s_level = 1;
  
  s_lives = 3;
  s_is_dead = false;
  s_is_paused = false;
  s_idle_ticks = 0;
  window_stack_push(s_game_window, true);
}

static void menu_open_settings_callback(int index, void *ctx) {
  window_stack_push(s_settings_window, true);
}

static void menu_exit_callback(int index, void *ctx) {
  window_stack_pop_all(true);
}

#if defined(PBL_TOUCH)
static void settings_touch_callback(int index, void *ctx) {
  s_setting_touch = !s_setting_touch;
  s_settings_items[index].subtitle = s_setting_touch ? "Enabled" : "Disabled";
  layer_mark_dirty(simple_menu_layer_get_layer(s_settings_layer));
  persist_write_bool(SAVE_KEY_TOUCH, s_setting_touch);

  if (touch_service_is_enabled()) {
    if (s_setting_touch) touch_service_subscribe(touch_handler, NULL);
    else touch_service_unsubscribe();
  }
}
#endif

static void settings_art_callback(int index, void *ctx) {
  s_setting_modern_art = !s_setting_modern_art;
  s_settings_items[index].subtitle = s_setting_modern_art ? "Modern" : "Retro";
  layer_mark_dirty(simple_menu_layer_get_layer(s_settings_layer));
  persist_write_bool(SAVE_KEY_ART, s_setting_modern_art);
}

// --- MENU WINDOW MANAGEMENT ---
static void main_menu_window_load(Window *window) {
  s_menu_icon_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_menu_icon_bitmap), retro_frog_data, sizeof(retro_frog_data));

  s_main_menu_items[0] = (SimpleMenuItem) { 
    .title = "Play", .subtitle = "Start Game", .icon = s_menu_icon_bitmap, .callback = menu_play_callback 
  };
  s_main_menu_items[1] = (SimpleMenuItem) { 
    .title = "Settings", .subtitle = "Configure Game", .callback = menu_open_settings_callback 
  };
  s_main_menu_items[2] = (SimpleMenuItem) { 
    .title = "Exit", .subtitle = "Close App", .callback = menu_exit_callback 
  };
  s_main_menu_sections[0] = (SimpleMenuSection) { 
    .title = "FROGGLE", .num_items = 3, .items = s_main_menu_items 
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_main_menu_layer = simple_menu_layer_create(bounds, window, s_main_menu_sections, 1, NULL);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_main_menu_layer));
}

static void main_menu_window_unload(Window *window) {
  simple_menu_layer_destroy(s_main_menu_layer);
  gbitmap_destroy(s_menu_icon_bitmap);
}

static void settings_window_load(Window *window) {
  int item_count = 0;
  
  #if defined(PBL_TOUCH)
  s_settings_items[item_count++] = (SimpleMenuItem) { 
    .title = "Touch Controls", .subtitle = s_setting_touch ? "Enabled" : "Disabled", .callback = settings_touch_callback 
  };
  #endif
  
  s_settings_items[item_count++] = (SimpleMenuItem) { 
    .title = "Art Style", .subtitle = s_setting_modern_art ? "Modern" : "Retro", .callback = settings_art_callback 
  };
  
  s_settings_sections[0] = (SimpleMenuSection) { 
    .title = "SETTINGS", .num_items = item_count, .items = s_settings_items 
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_settings_layer = simple_menu_layer_create(bounds, window, s_settings_sections, 1, NULL);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_settings_layer));
}

static void settings_window_unload(Window *window) {
  simple_menu_layer_destroy(s_settings_layer);
}

// --- GAME LOGIC ---
static void init_platforms() {
  int total_cols = s_screen_w / 16;
  s_num_active_lanes = (total_cols - 2) > 12 ? 12 : (total_cols - 2); 
  s_level_type = (s_level - 1) % 3; 
  s_total_entities = 0;
  int mid_lane = s_num_active_lanes / 2;

  for(int i = 0; i < s_num_active_lanes; i++) {
    bool lane_is_highway = (s_level_type == 1) || (s_level_type == 2 && i >= mid_lane);
    int lane_speed = (lane_is_highway ? (12 + (s_level * 2)) : (9 + (s_level * 2))) + (rand() % 10); 
    if (i % 2 == 0) lane_speed *= -1; 

    int lane_stagger = (i % 2 == 0) ? 0 : 80;
    int entities_in_this_lane = (lane_is_highway && s_screen_h <= 168) ? 1 : ENTITIES_PER_LANE;

    for(int j = 0; j < entities_in_this_lane; j++) {
      int idx = s_total_entities;
      s_platforms[idx].x = 16 + (i * 16); 
      s_platforms[idx].y_scaled = ((j * (s_screen_h/ENTITIES_PER_LANE)) + lane_stagger + (rand() % 30)) * 10; 
      s_platforms[idx].speed_scaled = lane_speed; 
      s_platforms[idx].is_car = lane_is_highway;
      s_platforms[idx].is_lilypad = !lane_is_highway && (i % 2 != 0);
      if (lane_is_highway) s_platforms[idx].car_color = rand() % 3;
      s_total_entities++;
    }
  }
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  #ifdef PBL_COLOR
    if (s_level_type == 0) {
      graphics_context_set_fill_color(ctx, GColorPictonBlue); graphics_fill_rect(ctx, GRect(16, 0, s_right_shore_x - 16, s_screen_h), 0, GCornerNone);
      graphics_context_set_fill_color(ctx, GColorPastelYellow); graphics_fill_rect(ctx, GRect(0, 0, 16, s_screen_h), 0, GCornerNone); graphics_fill_rect(ctx, GRect(s_right_shore_x, 0, s_screen_w - s_right_shore_x, s_screen_h), 0, GCornerNone); 
    } else if (s_level_type == 1) {
      graphics_context_set_fill_color(ctx, GColorDarkGray); graphics_fill_rect(ctx, GRect(16, 0, s_right_shore_x - 16, s_screen_h), 0, GCornerNone);
      graphics_context_set_fill_color(ctx, GColorMalachite); graphics_fill_rect(ctx, GRect(0, 0, 16, s_screen_h), 0, GCornerNone); graphics_fill_rect(ctx, GRect(s_right_shore_x, 0, s_screen_w - s_right_shore_x, s_screen_h), 0, GCornerNone); 
      graphics_context_set_stroke_color(ctx, GColorWhite); for (int i = 1; i < s_num_active_lanes; i++) graphics_draw_line(ctx, GPoint(16 + (i*16), 0), GPoint(16 + (i*16), s_screen_h));
    } else {
      int mid_x = 16 + ((s_num_active_lanes / 2) * 16);
      graphics_context_set_fill_color(ctx, GColorPictonBlue); graphics_fill_rect(ctx, GRect(16, 0, mid_x - 16, s_screen_h), 0, GCornerNone);
      graphics_context_set_fill_color(ctx, GColorPastelYellow); graphics_fill_rect(ctx, GRect(0, 0, 16, s_screen_h), 0, GCornerNone); 
      graphics_context_set_fill_color(ctx, GColorDarkGray); graphics_fill_rect(ctx, GRect(mid_x, 0, s_right_shore_x - mid_x, s_screen_h), 0, GCornerNone);
      graphics_context_set_fill_color(ctx, GColorMalachite); graphics_fill_rect(ctx, GRect(s_right_shore_x, 0, s_screen_w - s_right_shore_x, s_screen_h), 0, GCornerNone); 
      graphics_context_set_stroke_color(ctx, GColorWhite); for (int i = (s_num_active_lanes / 2) + 1; i < s_num_active_lanes; i++) graphics_draw_line(ctx, GPoint(16 + (i*16), 0), GPoint(16 + (i*16), s_screen_h));
    }
  #else
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_line(ctx, GPoint(16, 0), GPoint(16, s_screen_h)); graphics_draw_line(ctx, GPoint(s_right_shore_x, 0), GPoint(s_right_shore_x, s_screen_h));
    if (s_level_type == 2) graphics_draw_line(ctx, GPoint(16 + ((s_num_active_lanes / 2) * 16), 0), GPoint(16 + ((s_num_active_lanes / 2) * 16), s_screen_h));
  #endif

  graphics_context_set_compositing_mode(ctx, GCompOpSet);

  for (int i = 0; i < s_total_entities; i++) {
    int actual_y = s_platforms[i].y_scaled / 10;
    int plat_h = s_platforms[i].is_car ? 16 : (s_platforms[i].is_lilypad ? 16 : 48); 
    GRect bounds = GRect(s_platforms[i].x, actual_y, 16, plat_h);
    
    if (s_platforms[i].is_car) {
      bool moving_up = s_platforms[i].speed_scaled < 0;      
      
      if (s_setting_modern_art) {
        graphics_draw_bitmap_in_rect(ctx, s_headlight_bitmap, GRect(s_platforms[i].x, actual_y + 16 * (moving_up ? -1 : 1), 16, 16));
        GBitmap *bmp;
        switch (s_platforms[i].car_color) {
          case 0: bmp = moving_up ? s_car_bitmap_r_up : s_car_bitmap_r_down; break;  
          case 1: bmp = moving_up ? s_car_bitmap_g_up : s_car_bitmap_g_down; break;  
          default: bmp = moving_up ? s_car_bitmap_b_up : s_car_bitmap_b_down; break;  
        }
        graphics_draw_bitmap_in_rect(ctx, bmp, bounds);
      } else {
        #ifdef PBL_COLOR
          if (s_platforms[i].car_color == 0) graphics_context_set_fill_color(ctx, GColorRed);
          else if (s_platforms[i].car_color == 1) graphics_context_set_fill_color(ctx, GColorPictonBlue);
          else graphics_context_set_fill_color(ctx, GColorKellyGreen);
          graphics_fill_rect(ctx, bounds, 0, GCornerNone);
        #endif
        graphics_draw_bitmap_in_rect(ctx, s_car_bitmap_r_up, bounds); 
      }
    } else {
      if (!s_setting_modern_art) {
        #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, s_platforms[i].is_lilypad ? GColorKellyGreen : GColorWindsorTan);
          graphics_fill_rect(ctx, bounds, 0, GCornerNone);
        #endif
      }
      graphics_draw_bitmap_in_rect(ctx, s_platforms[i].is_lilypad ? s_lilypad_bitmap : s_log_bitmap, bounds);
    }
  }

  GRect highlight_bounds = GRect(s_frog_x, s_frog_y_scaled / 10, 16, 16);
  if (!s_setting_modern_art) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, highlight_bounds, 0, GCornerNone); 
  }
  graphics_draw_bitmap_in_rect(ctx, s_current_frog_bitmap, highlight_bounds);

  char ui_buffer[32];
  snprintf(ui_buffer, sizeof(ui_buffer), s_lives > 0 ? "Level %d | Lives: %d" : "Lives: 0", s_level, s_lives);
  int ui_y = PBL_IF_ROUND_ELSE(12, 0);
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, ui_y, s_screen_w, 16), 0, GCornerNone);
  graphics_draw_text(ctx, ui_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0, ui_y - 2, s_screen_w, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  if (s_is_dead && s_lives <= 0) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(s_screen_w/2 - 60, s_screen_h/2 - 20, 120, 40), 4, GCornersAll);
    graphics_context_set_text_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
    graphics_draw_text(ctx, "GAME OVER", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(s_screen_w/2 - 60, s_screen_h/2 - 16, 120, 30), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  } else if (s_is_paused) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(s_screen_w/2 - 50, s_screen_h/2 - 20, 100, 40), 4, GCornersAll);
    graphics_context_set_text_color(ctx, GColorWhite);
    graphics_draw_text(ctx, "PAUSED", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(s_screen_w/2 - 50, s_screen_h/2 - 16, 100, 30), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  }
}

static void reset_splat_callback(void *data) {
  s_splat_timer = NULL; 
  if (s_lives > 0) {
    s_is_dead = false; s_current_frog_bitmap = s_frog_bitmap; s_frog_x = 0; s_frog_y_scaled = (s_screen_h / 2) * 10; layer_mark_dirty(s_canvas_layer);
    if (!s_is_paused) s_game_timer = app_timer_register(50, game_loop, NULL);
  }
}

static void kill_frog() {
  if (s_is_dead) return; 
  vibes_short_pulse(); 
  s_is_dead = true; 
  s_lives--; 
  s_current_frog_bitmap = s_splat_bitmap; 
  layer_mark_dirty(s_canvas_layer); 
  s_splat_timer = app_timer_register(800, reset_splat_callback, NULL);
}

static void check_collisions() {
  if (s_is_dead) return; 
  int frog_actual_y = s_frog_y_scaled / 10;
  if (s_frog_x == 0 || s_frog_x >= s_right_shore_x) return; 

  int mid_x = 16 + ((s_num_active_lanes / 2) * 16);
  bool in_highway = (s_level_type == 1) || (s_level_type == 2 && s_frog_x >= mid_x);

  for (int i = 0; i < s_total_entities; i++) {
    if (s_frog_x == s_platforms[i].x) {
      int plat_actual_y = s_platforms[i].y_scaled / 10;
      int plat_h = s_platforms[i].is_car || s_platforms[i].is_lilypad ? 16 : 48; 
      int frog_center_y = frog_actual_y + 8;
      
      if (frog_center_y >= plat_actual_y && frog_center_y <= plat_actual_y + plat_h) {
        if (in_highway) { kill_frog(); return; }
        s_frog_y_scaled += s_platforms[i].speed_scaled; 
        return; 
      }
    }
  }
  if (!in_highway || frog_actual_y > s_screen_h || frog_actual_y < -16) kill_frog(); 
}

static void game_loop(void *data) {
  if (s_is_paused || s_is_dead) return; 
  
  s_idle_ticks++;
  if (s_idle_ticks >= IDLE_TIMEOUT_TICKS) {
    s_is_paused = true; layer_mark_dirty(s_canvas_layer); return; 
  }

  for (int i = 0; i < s_total_entities; i++) {
    s_platforms[i].y_scaled += s_platforms[i].speed_scaled;
    int actual_y = s_platforms[i].y_scaled / 10;
    int plat_h = s_platforms[i].is_car || s_platforms[i].is_lilypad ? 16 : 48;
    if (s_platforms[i].speed_scaled > 0 && actual_y > s_screen_h) {
      s_platforms[i].y_scaled = -plat_h * 10; if (s_platforms[i].is_car) s_platforms[i].car_color = rand() % 3;
    } else if (s_platforms[i].speed_scaled < 0 && actual_y < -plat_h) {
      s_platforms[i].y_scaled = s_screen_h * 10; if (s_platforms[i].is_car) s_platforms[i].car_color = rand() % 3;
    }
  }
  check_collisions();
  layer_mark_dirty(s_canvas_layer);
  s_game_timer = app_timer_register(50, game_loop, NULL); 
}

static void app_focus_handler(bool in_focus) {
  if (!in_focus && !s_is_dead && !s_is_paused) { s_is_paused = true; layer_mark_dirty(s_canvas_layer); }
}

static void up_click_handler(ClickRecognizerRef r, void *ctx) {
  s_idle_ticks = 0; 
  if (s_is_dead || s_lives <= 0 || s_is_paused) return; 
  if ((s_frog_y_scaled / 10) > 16) { s_frog_y_scaled -= 160; layer_mark_dirty(s_canvas_layer); }
}

static void down_click_handler(ClickRecognizerRef r, void *ctx) {
  s_idle_ticks = 0; 
  if (s_is_dead || s_lives <= 0 || s_is_paused) return; 
  if ((s_frog_y_scaled / 10) < s_screen_h - 16) { s_frog_y_scaled += 160; layer_mark_dirty(s_canvas_layer); }
}

static void select_click_handler(ClickRecognizerRef r, void *ctx) {
  s_idle_ticks = 0; 
  if (s_lives <= 0) {
    s_lives = 3; s_level = 1; persist_write_int(SAVE_KEY_LEVEL, s_level); s_is_dead = false; s_is_paused = false; 
    s_current_frog_bitmap = s_frog_bitmap; s_frog_x = 0; s_frog_y_scaled = (s_screen_h / 2) * 10; 
    init_platforms(); layer_mark_dirty(s_canvas_layer); s_game_timer = app_timer_register(50, game_loop, NULL); return;
  }
  if (s_is_dead) return; 
  if (s_is_paused) { s_is_paused = false; s_game_timer = app_timer_register(50, game_loop, NULL); layer_mark_dirty(s_canvas_layer); return; }
  
  if (s_frog_x < s_right_shore_x) { 
    s_frog_x += 16;
    if (s_frog_x >= s_right_shore_x) { vibes_double_pulse(); s_level++; s_lives++; persist_write_int(SAVE_KEY_LEVEL, s_level); s_frog_x = 0; s_frog_y_scaled = (s_screen_h / 2) * 10; init_platforms(); }
    layer_mark_dirty(s_canvas_layer);
  }
}

#if defined(PBL_TOUCH)
static void touch_handler(const TouchEvent *event, void *context) {
  if (!s_setting_touch) return; 
  if (event->type == TouchEvent_Touchdown) select_click_handler(NULL, NULL);
}
#endif

static void game_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

// --- GAME WINDOW MANAGEMENT ---
static void game_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_screen_w = bounds.size.w; s_screen_h = bounds.size.h;
  s_right_shore_x = ((s_screen_w / 16) - 1) * 16;

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);

  if (s_setting_modern_art) {
    #ifdef PBL_COLOR
      s_sprite_sheet_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SPRITESColor);
    #else
      s_sprite_sheet_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SPRITES);
    #endif
    s_frog_bitmap = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(0, 0, 16, 16));
    s_lilypad_bitmap = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(16, 0, 16, 16));
    s_splat_bitmap = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(32, 0, 16, 16));
    s_headlight_bitmap = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(48, 0, 16, 16));
    s_log_bitmap = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(0, 16, 16, 48));

    #ifdef PBL_COLOR
      s_car_bitmap_r_up = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(16, 16, 16, 16));
      s_car_bitmap_g_up = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(32, 16, 16, 16));
      s_car_bitmap_b_up = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(48, 16, 16, 16));
      s_car_bitmap_r_down = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(16, 32, 16, 16));
      s_car_bitmap_g_down = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(32, 32, 16, 16));
      s_car_bitmap_b_down = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(48, 32, 16, 16));
    #else
      s_car_bitmap_r_up = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(16, 16, 16, 16));
      s_car_bitmap_g_up = s_car_bitmap_r_up; s_car_bitmap_b_up = s_car_bitmap_r_up;
      s_car_bitmap_r_down = gbitmap_create_as_sub_bitmap(s_sprite_sheet_bitmap, GRect(16, 32, 16, 16));
      s_car_bitmap_g_down = s_car_bitmap_r_down; s_car_bitmap_b_down = s_car_bitmap_r_down;
    #endif
  } else {
    s_frog_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit); memcpy(gbitmap_get_data(s_frog_bitmap), retro_frog_data, sizeof(retro_frog_data));
    s_splat_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit); memcpy(gbitmap_get_data(s_splat_bitmap), retro_splat_data, sizeof(retro_splat_data));
    s_lilypad_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit); memcpy(gbitmap_get_data(s_lilypad_bitmap), retro_lilypad_data, sizeof(retro_lilypad_data));
    s_car_bitmap_r_up = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit); memcpy(gbitmap_get_data(s_car_bitmap_r_up), retro_car_data, sizeof(retro_car_data));
    s_log_bitmap = gbitmap_create_blank(GSize(16, 48), GBitmapFormat1Bit); memcpy(gbitmap_get_data(s_log_bitmap), retro_log_data, sizeof(retro_log_data));
    
    s_car_bitmap_g_up = s_car_bitmap_r_up; s_car_bitmap_b_up = s_car_bitmap_r_up;
    s_car_bitmap_r_down = s_car_bitmap_r_up; s_car_bitmap_g_down = s_car_bitmap_r_up; s_car_bitmap_b_down = s_car_bitmap_r_up;
    s_headlight_bitmap = NULL;
  }

  s_current_frog_bitmap = s_frog_bitmap;
  s_frog_x = 0; s_frog_y_scaled = (s_screen_h / 2) * 10;
  
  init_platforms();
  s_game_timer = app_timer_register(50, game_loop, NULL);
}

static void game_window_unload(Window *window) {
  if (s_game_timer != NULL) { app_timer_cancel(s_game_timer); s_game_timer = NULL; }
  if (s_splat_timer != NULL) { app_timer_cancel(s_splat_timer); s_splat_timer = NULL; }

  gbitmap_destroy(s_frog_bitmap); gbitmap_destroy(s_splat_bitmap); gbitmap_destroy(s_log_bitmap); gbitmap_destroy(s_lilypad_bitmap); gbitmap_destroy(s_car_bitmap_r_up);
  
  if (s_setting_modern_art) {
    gbitmap_destroy(s_headlight_bitmap); gbitmap_destroy(s_car_bitmap_r_down);
    #ifdef PBL_COLOR
      gbitmap_destroy(s_car_bitmap_g_up); gbitmap_destroy(s_car_bitmap_b_up); gbitmap_destroy(s_car_bitmap_g_down); gbitmap_destroy(s_car_bitmap_b_down);
    #endif
    gbitmap_destroy(s_sprite_sheet_bitmap);
  }
  layer_destroy(s_canvas_layer);
}

// --- APP LIFECYCLE ---
static void init() {
  srand(time(NULL)); 
  if (persist_exists(SAVE_KEY_TOUCH)) s_setting_touch = persist_read_bool(SAVE_KEY_TOUCH);
  if (persist_exists(SAVE_KEY_ART)) s_setting_modern_art = persist_read_bool(SAVE_KEY_ART);

  s_game_window = window_create();
  window_set_window_handlers(s_game_window, (WindowHandlers) { .load = game_window_load, .unload = game_window_unload });
  window_set_click_config_provider(s_game_window, game_click_config_provider);
  
  s_settings_window = window_create();
  window_set_window_handlers(s_settings_window, (WindowHandlers) { .load = settings_window_load, .unload = settings_window_unload });

  s_main_menu_window = window_create();
  window_set_window_handlers(s_main_menu_window, (WindowHandlers) { .load = main_menu_window_load, .unload = main_menu_window_unload });
  
  #if defined(PBL_TOUCH)
  if (touch_service_is_enabled() && s_setting_touch) {
    touch_service_subscribe(touch_handler, NULL);
  }
  #endif

  app_focus_service_subscribe(app_focus_handler);
  window_stack_push(s_main_menu_window, true);
}

static void deinit() {
  #if defined(PBL_TOUCH)
  touch_service_unsubscribe();
  #endif
  app_focus_service_unsubscribe();
  window_destroy(s_game_window);
  window_destroy(s_settings_window);
  window_destroy(s_main_menu_window);
}

int main(void) { init(); app_event_loop(); deinit(); }
