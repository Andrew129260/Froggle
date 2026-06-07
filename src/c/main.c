#include <pebble.h>

#define SAVE_KEY_LEVEL 1 
#define MAX_ENTITIES 30 
#define ENTITIES_PER_LANE 2 

// --- 16-PIXEL BITMAP DATA ---

static const uint8_t frog_bitmap_data[] = {
  0b00000000, 0b00000000, 0, 0,
  0b00011000, 0b00011000, 0, 0, 
  0b00111100, 0b00111100, 0, 0, 
  0b00011111, 0b11111000, 0, 0, 
  0b00001111, 0b11110000, 0, 0, 
  0b00001111, 0b11110000, 0, 0,
  0b01101111, 0b11110110, 0, 0, 
  0b01101111, 0b11110110, 0, 0,
  0b00001111, 0b11110000, 0, 0, 
  0b00001111, 0b11110000, 0, 0,
  0b00001111, 0b11110000, 0, 0,
  0b00001111, 0b11110000, 0, 0,
  0b00111111, 0b11111100, 0, 0, 
  0b01111000, 0b00011110, 0, 0, 
  0b01100000, 0b00000110, 0, 0, 
  0b00000000, 0b00000000, 0, 0
};

static const uint8_t splat_bitmap_data[] = {
  0b00000000, 0b00000000, 0, 0,
  0b00010000, 0b00001000, 0, 0,
  0b00001010, 0b01000000, 0, 0,
  0b00000111, 0b10000000, 0, 0,
  0b01001111, 0b11010000, 0, 0,
  0b00111111, 0b11100000, 0, 0,
  0b00011111, 0b11110010, 0, 0,
  0b10111111, 0b11111000, 0, 0,
  0b00011111, 0b11111001, 0, 0,
  0b01001111, 0b11110000, 0, 0,
  0b00000111, 0b11100100, 0, 0,
  0b00000011, 0b11000000, 0, 0,
  0b00101010, 0b00100000, 0, 0,
  0b00010000, 0b00010000, 0, 0,
  0b00000000, 0b00000000, 0, 0,
  0b00000000, 0b00000000, 0, 0
};

static const uint8_t lilypad_bitmap_data[] = {
  0b00000111, 0b11100000, 0, 0,
  0b00011111, 0b11111000, 0, 0,
  0b00111111, 0b11111100, 0, 0,
  0b01111111, 0b11111110, 0, 0,
  0b01111111, 0b11111110, 0, 0,
  0b11111111, 0b11111111, 0, 0,
  0b11111000, 0b01111111, 0, 0,
  0b11110000, 0b00111111, 0, 0,
  0b11110000, 0b00111111, 0, 0,
  0b11111000, 0b01111111, 0, 0,
  0b11111111, 0b11111111, 0, 0,
  0b01111111, 0b11111110, 0, 0,
  0b01111111, 0b11111110, 0, 0,
  0b00111111, 0b11111100, 0, 0,
  0b00011111, 0b11111000, 0, 0,
  0b00000111, 0b11100000, 0, 0
};

static const uint8_t car_bitmap_data[] = {
  0b00000000, 0b00000000, 0, 0,
  0b00111111, 0b11111100, 0, 0,
  0b01100000, 0b00000110, 0, 0, 
  0b01111111, 0b11111110, 0, 0, 
  0b01111111, 0b11111110, 0, 0, 
  0b01111111, 0b11111110, 0, 0, 
  0b01111111, 0b11111110, 0, 0, 
  0b01100000, 0b00000110, 0, 0, 
  0b01111111, 0b11111110, 0, 0, 
  0b01111111, 0b11111110, 0, 0, 
  0b00111111, 0b11111100, 0, 0, 
  0b00000000, 0b00000000, 0, 0,
  0b00000000, 0b00000000, 0, 0,
  0b00000000, 0b00000000, 0, 0,
  0b00000000, 0b00000000, 0, 0,
  0b00000000, 0b00000000, 0, 0
};

static const uint8_t log_bitmap_data[] = {
  0b00111111, 0b11111100, 0, 0, 0b01111111, 0b11111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b01111101, 0b10111110, 0, 0,
  0b01111011, 0b11011110, 0, 0, 0b01101111, 0b11110110, 0, 0,
  0b01111111, 0b11111110, 0, 0, 0b00111111, 0b11111100, 0, 0
};

// --- DATA STRUCTURES ---

typedef struct {
  int x;
  int y_scaled;      
  int speed_scaled;  
  bool is_lilypad; 
  bool is_car;
  int car_color; 
} Entity;

// --- GLOBAL VARIABLES ---

static Window *s_main_window;
static Layer *s_canvas_layer;

static GBitmap *s_frog_bitmap;
static GBitmap *s_splat_bitmap;
static GBitmap *s_log_bitmap;
static GBitmap *s_lilypad_bitmap;
static GBitmap *s_car_bitmap;
static GBitmap *s_current_frog_bitmap; 

static AppTimer *s_game_timer;

static int s_screen_w = 144;
static int s_screen_h = 168;
static int s_right_shore_x = 128; 
static int s_num_active_lanes = 0;
static int s_total_entities = 0;
static bool s_is_highway = false; 

static Entity s_platforms[MAX_ENTITIES];

static int s_frog_x = 0;
static int s_frog_y_scaled = 72 * 10; 
static int s_lives = 3;
static int s_level = 1;
static bool s_is_dead = false;

static bool s_is_paused = false;

// --- INIT LEVEL ---

static void init_platforms() {
  int total_cols = s_screen_w / 16;
  int max_lanes = total_cols - 2;
  s_num_active_lanes = max_lanes > 12 ? 12 : max_lanes; 
  
  s_is_highway = (s_level % 2 == 0); 
  s_total_entities = 0;

  for(int i = 0; i < s_num_active_lanes; i++) {
    int lane_speed;
    bool lane_is_car = s_is_highway;
    bool lane_is_lilypad = false;
    
    if (s_is_highway) {
      lane_speed = 15 + (s_level * 3) + (rand() % 15); 
    } else {
      lane_is_lilypad = (i % 2 != 0); 
      lane_speed = 9 + (s_level * 2) + (rand() % 10); 
    }

    if (i % 2 == 0) lane_speed *= -1; 

    int lane_stagger = (i % 2 == 0) ? 0 : 80;

    // DYNAMIC TRAFFIC THINNING FOR SMALL SCREENS
    int entities_in_this_lane = ENTITIES_PER_LANE;
    if (s_is_highway && s_screen_h <= 168) {
      // Alternates between 1 and 2 cars per lane to reduce cluster density 
      // and give players clear paths to jump through on classic watches.
      entities_in_this_lane = (i % 2 == 0) ? 1 : 2;
    }

    for(int j = 0; j < entities_in_this_lane; j++) {
      int idx = s_total_entities;
      s_platforms[idx].x = 16 + (i * 16); 
      s_platforms[idx].y_scaled = ((j * (s_screen_h/ENTITIES_PER_LANE)) + lane_stagger + (rand() % 30)) * 10; 
      
      s_platforms[idx].speed_scaled = lane_speed; 
      s_platforms[idx].is_car = lane_is_car;
      s_platforms[idx].is_lilypad = lane_is_lilypad;
      
      if (lane_is_car) {
        s_platforms[idx].car_color = rand() % 3;
      }
      
      s_total_entities++;
    }
  }
}

// --- DRAWING LOOP ---

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  #ifdef PBL_COLOR
    if (s_is_highway) {
      graphics_context_set_fill_color(ctx, GColorDarkGray); 
      graphics_fill_rect(ctx, GRect(16, 0, s_right_shore_x - 16, s_screen_h), 0, GCornerNone);
      
      graphics_context_set_fill_color(ctx, GColorMalachite); 
      graphics_fill_rect(ctx, GRect(0, 0, 16, s_screen_h), 0, GCornerNone); 
      graphics_fill_rect(ctx, GRect(s_right_shore_x, 0, s_screen_w - s_right_shore_x, s_screen_h), 0, GCornerNone); 
      
      graphics_context_set_stroke_color(ctx, GColorWhite);
      for (int i = 1; i < s_num_active_lanes; i++) {
         graphics_draw_line(ctx, GPoint(16 + (i*16), 0), GPoint(16 + (i*16), s_screen_h));
      }
    } else {
      graphics_context_set_fill_color(ctx, GColorPictonBlue);
      graphics_fill_rect(ctx, GRect(16, 0, s_right_shore_x - 16, s_screen_h), 0, GCornerNone);
      
      graphics_context_set_fill_color(ctx, GColorPastelYellow);
      graphics_fill_rect(ctx, GRect(0, 0, 16, s_screen_h), 0, GCornerNone); 
      graphics_fill_rect(ctx, GRect(s_right_shore_x, 0, s_screen_w - s_right_shore_x, s_screen_h), 0, GCornerNone); 
    }
  #else
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_line(ctx, GPoint(16, 0), GPoint(16, s_screen_h));
    graphics_draw_line(ctx, GPoint(s_right_shore_x, 0), GPoint(s_right_shore_x, s_screen_h));
  #endif

  graphics_context_set_compositing_mode(ctx, GCompOpSet);

  for (int i = 0; i < s_total_entities; i++) {
    int actual_y = s_platforms[i].y_scaled / 10;
    
    int plat_h = 16;
    if (!s_platforms[i].is_car && !s_platforms[i].is_lilypad) plat_h = 48; 
    
    GRect bounds = GRect(s_platforms[i].x, actual_y, 16, plat_h);
    
    #ifdef PBL_COLOR
      if (s_platforms[i].is_car) {
        if (s_platforms[i].car_color == 0) graphics_context_set_fill_color(ctx, GColorRed);
        else if (s_platforms[i].car_color == 1) graphics_context_set_fill_color(ctx, GColorPictonBlue);
        else graphics_context_set_fill_color(ctx, GColorKellyGreen);
      } else if (s_platforms[i].is_lilypad) {
        graphics_context_set_fill_color(ctx, GColorKellyGreen);
      } else {
        graphics_context_set_fill_color(ctx, GColorWindsorTan);
      }
      graphics_fill_rect(ctx, bounds, 0, GCornerNone);
    #endif

    GBitmap *bmp;
    if (s_platforms[i].is_car) {
      bmp = s_car_bitmap;
    } else {
      bmp = s_platforms[i].is_lilypad ? s_lilypad_bitmap : s_log_bitmap;
    }
    graphics_draw_bitmap_in_rect(ctx, bmp, bounds);
  }

  int frog_actual_y = s_frog_y_scaled / 10;
  
  GRect highlight_bounds = GRect(s_frog_x, frog_actual_y, 16, 16);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, highlight_bounds, 0, GCornerNone); 

  graphics_draw_bitmap_in_rect(ctx, s_current_frog_bitmap, highlight_bounds);

  char ui_buffer[32];
  if (s_lives > 0) {
    snprintf(ui_buffer, sizeof(ui_buffer), "Level %d | Lives: %d", s_level, s_lives);
  } else {
    snprintf(ui_buffer, sizeof(ui_buffer), "Lives: 0"); 
  }
  
  int ui_y = PBL_IF_ROUND_ELSE(12, 0);
  
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, ui_y, s_screen_w, 16), 0, GCornerNone);
  graphics_draw_text(ctx, ui_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0, ui_y - 2, s_screen_w, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  if (s_is_dead && s_lives <= 0) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(s_screen_w/2 - 60, s_screen_h/2 - 20, 120, 40), 4, GCornersAll);
    
    #ifdef PBL_COLOR
      graphics_context_set_text_color(ctx, GColorRed);
    #else
      graphics_context_set_text_color(ctx, GColorWhite);
    #endif
    
    graphics_draw_text(ctx, "GAME OVER", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(s_screen_w/2 - 60, s_screen_h/2 - 16, 120, 30), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  
  } else if (s_is_paused) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(s_screen_w/2 - 50, s_screen_h/2 - 20, 100, 40), 4, GCornersAll);
    graphics_context_set_text_color(ctx, GColorWhite);
    graphics_draw_text(ctx, "PAUSED", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(s_screen_w/2 - 50, s_screen_h/2 - 16, 100, 30), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  }
}

// --- GAME LOGIC ---

static void game_loop(void *data); 

static void reset_splat_callback(void *data) {
  if (s_lives > 0) {
    s_is_dead = false;
    s_current_frog_bitmap = s_frog_bitmap;
    s_frog_x = 0; 
    s_frog_y_scaled = (s_screen_h / 2) * 10; 
    layer_mark_dirty(s_canvas_layer);
    
    if (!s_is_paused) {
      s_game_timer = app_timer_register(50, game_loop, NULL);
    }
  }
}

static void kill_frog() {
  if (s_is_dead) return;
  vibes_short_pulse();
  s_is_dead = true;
  s_lives--;
  s_current_frog_bitmap = s_splat_bitmap; 
  layer_mark_dirty(s_canvas_layer); 
  app_timer_register(800, reset_splat_callback, NULL);
}

static void check_collisions() {
  if (s_is_dead) return; 

  int frog_actual_y = s_frog_y_scaled / 10;

  if (s_frog_x == 0 || s_frog_x >= s_right_shore_x) return; 

  if (s_is_highway) {
    for (int i = 0; i < s_total_entities; i++) {
      if (s_frog_x == s_platforms[i].x) {
        int plat_actual_y = s_platforms[i].y_scaled / 10;
        int plat_h = 16; 
        
        int frog_center_y = frog_actual_y + 8;
        if (frog_center_y >= plat_actual_y && frog_center_y <= plat_actual_y + plat_h) {
          kill_frog(); 
          return;
        }
      }
    }
  } else {
    bool safe_on_platform = false;
    for (int i = 0; i < s_total_entities; i++) {
      if (s_frog_x == s_platforms[i].x) {
        int plat_actual_y = s_platforms[i].y_scaled / 10;
        int plat_h = s_platforms[i].is_lilypad ? 16 : 48; 
        
        int frog_center_y = frog_actual_y + 8;
        if (frog_center_y >= plat_actual_y && frog_center_y <= plat_actual_y + plat_h) {
          safe_on_platform = true;
          s_frog_y_scaled += s_platforms[i].speed_scaled; 
          break;
        }
      }
    }

    if (!safe_on_platform) {
      kill_frog(); 
    } else if (frog_actual_y > s_screen_h || frog_actual_y < -16) {
      kill_frog(); 
    }
  }
}

static void game_loop(void *data) {
  if (s_is_paused || s_is_dead) return; 

  for (int i = 0; i < s_total_entities; i++) {
    s_platforms[i].y_scaled += s_platforms[i].speed_scaled;
    
    int actual_y = s_platforms[i].y_scaled / 10;
    
    int plat_h = 16;
    if (!s_platforms[i].is_car && !s_platforms[i].is_lilypad) plat_h = 48;

    if (s_platforms[i].speed_scaled > 0 && actual_y > s_screen_h) {
      s_platforms[i].y_scaled = -plat_h * 10; 
    } else if (s_platforms[i].speed_scaled < 0 && actual_y < -plat_h) {
      s_platforms[i].y_scaled = s_screen_h * 10; 
    }
  }

  check_collisions();
  layer_mark_dirty(s_canvas_layer);
  
  s_game_timer = app_timer_register(50, game_loop, NULL); 
}

// --- APP FOCUS & NOTIFICATIONS ---

static void app_focus_handler(bool in_focus) {
  if (!in_focus && !s_is_dead && !s_is_paused) {
    s_is_paused = true;
    layer_mark_dirty(s_canvas_layer); 
  }
}

// --- BUTTON & TAP INPUTS ---

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_is_dead || s_lives <= 0 || s_is_paused) return; 
  if ((s_frog_y_scaled / 10) > 16) { 
    s_frog_y_scaled -= 16 * 10; 
    layer_mark_dirty(s_canvas_layer);
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_is_dead || s_lives <= 0 || s_is_paused) return; 
  if ((s_frog_y_scaled / 10) < s_screen_h - 16) { 
    s_frog_y_scaled += 16 * 10;
    layer_mark_dirty(s_canvas_layer);
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_lives <= 0) {
    s_lives = 3;
    s_level = 1;
    persist_write_int(SAVE_KEY_LEVEL, s_level); 
    s_is_dead = false;
    s_is_paused = false;
    s_current_frog_bitmap = s_frog_bitmap;
    s_frog_x = 0;
    s_frog_y_scaled = (s_screen_h / 2) * 10;
    init_platforms();
    layer_mark_dirty(s_canvas_layer);
    
    s_game_timer = app_timer_register(50, game_loop, NULL);
    return;
  }

  if (s_is_dead) return; 

  if (s_is_paused) {
    s_is_paused = false;
    s_game_timer = app_timer_register(50, game_loop, NULL);
    layer_mark_dirty(s_canvas_layer);
    return; 
  }
  
  if (s_frog_x < s_right_shore_x) { 
    s_frog_x += 16;
    
    if (s_frog_x >= s_right_shore_x) {
      vibes_double_pulse(); 
      s_level++;
      s_lives++; 
      
      persist_write_int(SAVE_KEY_LEVEL, s_level); 
      
      s_frog_x = 0;
      s_frog_y_scaled = (s_screen_h / 2) * 10;
      init_platforms(); 
    }
    layer_mark_dirty(s_canvas_layer);
  }
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  select_click_handler(NULL, NULL);
}

static void click_config_provider(void *context) {
  window_raw_click_subscribe(BUTTON_ID_UP, up_click_handler, NULL, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_click_handler, NULL, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_click_handler, NULL, NULL);
}

// --- WINDOW MANAGEMENT ---

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
  GRect bounds = layer_get_bounds(window_layer);
  s_screen_w = bounds.size.w;
  s_screen_h = bounds.size.h;

  int total_cols = s_screen_w / 16;
  s_right_shore_x = (total_cols - 1) * 16;

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);

  s_frog_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_frog_bitmap), frog_bitmap_data, sizeof(frog_bitmap_data));

  s_splat_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_splat_bitmap), splat_bitmap_data, sizeof(splat_bitmap_data));

  s_lilypad_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_lilypad_bitmap), lilypad_bitmap_data, sizeof(lilypad_bitmap_data));
  
  s_car_bitmap = gbitmap_create_blank(GSize(16, 16), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_car_bitmap), car_bitmap_data, sizeof(car_bitmap_data));

  s_log_bitmap = gbitmap_create_blank(GSize(16, 48), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_log_bitmap), log_bitmap_data, sizeof(log_bitmap_data));

  s_current_frog_bitmap = s_frog_bitmap;
  s_frog_y_scaled = (s_screen_h / 2) * 10;
  
  init_platforms();
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_frog_bitmap);
  gbitmap_destroy(s_splat_bitmap);
  gbitmap_destroy(s_log_bitmap);
  gbitmap_destroy(s_lilypad_bitmap);
  gbitmap_destroy(s_car_bitmap);
  layer_destroy(s_canvas_layer);
}

// --- APP LIFECYCLE ---

static void init() {
  srand(time(NULL)); 

  if (persist_exists(SAVE_KEY_LEVEL)) {
    s_level = persist_read_int(SAVE_KEY_LEVEL);
  }

  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_click_config_provider(s_main_window, click_config_provider);
  
  accel_tap_service_subscribe(accel_tap_handler);
  app_focus_service_subscribe(app_focus_handler);

  s_game_timer = app_timer_register(50, game_loop, NULL);
  window_stack_push(s_main_window, true);
}

static void deinit() {
  accel_tap_service_unsubscribe();
  app_focus_service_unsubscribe();
  
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}