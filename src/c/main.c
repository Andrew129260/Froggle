#include <pebble.h>

#define SAVE_KEY_LEVEL 1 // The memory slot we will use for the Autosave

// --- BITMAP DATA ---

static const uint8_t frog_bitmap_data[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b01111110, 0b00000000, 0b00000000, 
  0b00000001, 0b11111110, 0b00000000, 0b00000000, 
  0b00000011, 0b10011111, 0b10000000, 0b00000000, 
  0b00000011, 0b10011111, 0b11100000, 0b00000000, 
  0b00001111, 0b11111111, 0b11110000, 0b00000000, 
  0b00001111, 0b11111111, 0b11110000, 0b00000000, 
  0b00001111, 0b11111111, 0b11110000, 0b00000000, 
  0b00011111, 0b11111111, 0b11110000, 0b00000000, 
  0b00011111, 0b11111111, 0b11110000, 0b00000000, 
  0b00011111, 0b11111111, 0b11110000, 0b00000000, 
  0b00001111, 0b00011110, 0b00110000, 0b00000000, 
  0b00001100, 0b00011110, 0b00011000, 0b00000000, 
  0b00011000, 0b00011100, 0b00001100, 0b00000000, 
  0b00111100, 0b00011110, 0b00001111, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000  
};

static const uint8_t splat_bitmap_data[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00010001, 0b00000000, 0b00000000, 
  0b00000010, 0b00001010, 0b00001000, 0b00000000, 
  0b00000100, 0b00000100, 0b00010000, 0b00000000, 
  0b00000010, 0b01111110, 0b01000000, 0b00000000, 
  0b00000001, 0b11111111, 0b10000000, 0b00000000, 
  0b00000000, 0b11111111, 0b00000000, 0b00000000, 
  0b00001111, 0b11111111, 0b11110000, 0b00000000, 
  0b00001111, 0b11111111, 0b11110000, 0b00000000, 
  0b00011111, 0b11111111, 0b11111000, 0b00000000, 
  0b00111111, 0b11111111, 0b11111100, 0b00000000, 
  0b00011111, 0b11111111, 0b11111000, 0b00000000, 
  0b00001111, 0b11111111, 0b11110000, 0b00000000, 
  0b00001111, 0b11111111, 0b11110000, 0b00000000, 
  0b00000000, 0b11111111, 0b00000000, 0b00000000, 
  0b00000001, 0b11111111, 0b10000000, 0b00000000, 
  0b00000010, 0b01111110, 0b01000000, 0b00000000, 
  0b00000100, 0b00000100, 0b00010000, 0b00000000, 
  0b00000010, 0b00001010, 0b00001000, 0b00000000, 
  0b00000000, 0b00010001, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000  
};

static const uint8_t car_bitmap_data[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b11000011, 0b00000000, 0b00000000, 
  0b00000001, 0b11111111, 0b10000000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00011011, 0b11111111, 0b11011000, 0b00000000, 
  0b00011011, 0b00000000, 0b11011000, 0b00000000, 
  0b00011011, 0b11111111, 0b11011000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00011011, 0b00000000, 0b11011000, 0b00000000, 
  0b00011011, 0b11111111, 0b11011000, 0b00000000, 
  0b00011011, 0b11111111, 0b11011000, 0b00000000, 
  0b00000011, 0b11111111, 0b11000000, 0b00000000, 
  0b00000001, 0b11111111, 0b10000000, 0b00000000, 
  0b00000000, 0b11000011, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000  
};

// --- GLOBAL VARIABLES ---

static Window *s_main_window;
static Layer *s_canvas_layer;

static GBitmap *s_frog_bitmap;
static GBitmap *s_splat_bitmap;
static GBitmap *s_car_bitmap;
static GBitmap *s_current_frog_bitmap; // Tracks which state the frog is in

static AppTimer *s_game_timer;

// Game State
static int s_frog_x = 0;
static int s_frog_y = 72;
static int s_car_x = 72;
static int s_car_y = 168;

static int s_lives = 3;
static int s_level = 1;
static bool s_is_dead = false;

// --- DRAWING LOOP ---

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // 1. Draw Background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  // 2. Setup Transparencies
  graphics_context_set_compositing_mode(ctx, GCompOpSet);

  // 3. Draw the Car
  GRect car_bounds = GRect(s_car_x, s_car_y, 24, 24);
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_fill_rect(ctx, car_bounds, 0, GCornerNone);
  #endif
  graphics_draw_bitmap_in_rect(ctx, s_car_bitmap, car_bounds);

  // 4. Draw the Frog (or Splat)
  GRect frog_bounds = GRect(s_frog_x, s_frog_y, 24, 24);
  graphics_draw_bitmap_in_rect(ctx, s_current_frog_bitmap, frog_bounds);

  // 5. Draw UI (Level and Lives)
  char ui_buffer[32];
  if (s_lives > 0) {
    snprintf(ui_buffer, sizeof(ui_buffer), "Level %d  |  Lives: %d", s_level, s_lives);
  } else {
    snprintf(ui_buffer, sizeof(ui_buffer), "GAME OVER (Select to Retry)");
  }
  
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_draw_text(ctx, ui_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0, 0, 144, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

// --- GAME LOGIC ---

static void reset_splat_callback(void *data) {
  // After the splat animation plays, reset the player if they have lives left
  if (s_lives > 0) {
    s_is_dead = false;
    s_current_frog_bitmap = s_frog_bitmap;
    s_frog_x = 0;
    s_frog_y = 72;
    layer_mark_dirty(s_canvas_layer);
  }
}

static void check_collision() {
  if (s_is_dead) return; // Don't crash again while already dead

  if (s_frog_x < s_car_x + 24 && s_frog_x + 24 > s_car_x &&
      s_frog_y < s_car_y + 24 && s_frog_y + 24 > s_car_y) {
      
      vibes_short_pulse();
      s_is_dead = true;
      s_lives--;
      s_current_frog_bitmap = s_splat_bitmap; // Switch graphic to splat
      
      // Keep the splat on screen for 800 milliseconds before resetting
      app_timer_register(800, reset_splat_callback, NULL);
  }
}

static void game_loop(void *data) {
  // The car moves faster on higher levels!
  int car_speed = 3 + (s_level / 2);
  s_car_y -= car_speed; 
  
  if (s_car_y < -24) {
    s_car_y = 168; 
  }

  check_collision();
  layer_mark_dirty(s_canvas_layer);
  s_game_timer = app_timer_register(33, game_loop, NULL);
}

// --- BUTTON INPUTS ---

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_is_dead || s_lives <= 0) return; // Lock controls on death
  
  if (s_frog_y > 0) {
    s_frog_y -= 24;
    check_collision();
    layer_mark_dirty(s_canvas_layer);
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_is_dead || s_lives <= 0) return; 
  
  if (s_frog_y < 144) { 
    s_frog_y += 24;
    check_collision();
    layer_mark_dirty(s_canvas_layer);
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Restart Game from Game Over screen
  if (s_lives <= 0) {
    s_lives = 3;
    s_level = 1;
    persist_write_int(SAVE_KEY_LEVEL, s_level); // Reset Save Data
    s_is_dead = false;
    s_current_frog_bitmap = s_frog_bitmap;
    s_frog_x = 0;
    s_frog_y = 72;
    layer_mark_dirty(s_canvas_layer);
    return;
  }

  if (s_is_dead) return; 
  
  // Normal Jump
  if (s_frog_x < 120) { 
    s_frog_x += 24;
    check_collision();
    
    // Check Win Condition (Crossed the road)
    if (s_frog_x >= 120) {
      vibes_double_pulse(); // Happy victory buzz
      s_level++;
      
      // AUTOSAVE THE LEVEL
      persist_write_int(SAVE_KEY_LEVEL, s_level);
      
      // Reset position for the next level
      s_frog_x = 0;
      s_frog_y = 72;
    }
    
    layer_mark_dirty(s_canvas_layer);
  }
}

static void click_config_provider(void *context) {
  // window_raw_click_subscribe arguments: (Button ID, Down Handler, Up Handler, Context)
  // By placing your handlers in the "Down Handler" slot, movement happens instantly on press.
  
  window_raw_click_subscribe(BUTTON_ID_UP, up_click_handler, NULL, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_click_handler, NULL, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_click_handler, NULL, NULL);
}

// --- WINDOW MANAGEMENT ---

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  s_canvas_layer = layer_create(layer_get_bounds(window_layer));
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);

  // Initialize Bitmaps
  s_frog_bitmap = gbitmap_create_blank(GSize(24, 24), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_frog_bitmap), frog_bitmap_data, sizeof(frog_bitmap_data));

  s_splat_bitmap = gbitmap_create_blank(GSize(24, 24), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_splat_bitmap), splat_bitmap_data, sizeof(splat_bitmap_data));

  s_car_bitmap = gbitmap_create_blank(GSize(24, 24), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_car_bitmap), car_bitmap_data, sizeof(car_bitmap_data));

  // Set the starting graphic
  s_current_frog_bitmap = s_frog_bitmap;
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_frog_bitmap);
  gbitmap_destroy(s_splat_bitmap);
  gbitmap_destroy(s_car_bitmap);
  layer_destroy(s_canvas_layer);
}

// --- APP LIFECYCLE ---

static void init() {
  // LOAD AUTOSAVE DATA
  if (persist_exists(SAVE_KEY_LEVEL)) {
    s_level = persist_read_int(SAVE_KEY_LEVEL);
  }

  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_click_config_provider(s_main_window, click_config_provider);
  s_game_timer = app_timer_register(33, game_loop, NULL);
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}