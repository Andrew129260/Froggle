#include <pebble.h>

// --- BITMAP DATA ---

// 24x24 pixel frog, padded to 32-bits per row
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

// 24x24 pixel Top-Down Car, padded to 32-bits per row
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
static GBitmap *s_car_bitmap;

// Added for tracking locations
static int s_frog_x = 0;
static int s_frog_y = 72; // Start vertically centered
static int s_car_x = 72;  // Start in the middle lane
static int s_car_y = 168; // Start just off the bottom of the screen

static AppTimer *s_game_timer;

// --- DRAWING LOOP ---

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // 1. Draw the background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  // 2. Set compositing mode so black pixels in our array are transparent
  graphics_context_set_compositing_mode(ctx, GCompOpSet);

  // 3. Draw the Car using dynamic variables
  GRect car_bounds = GRect(s_car_x, s_car_y, 24, 24);
  
  #ifdef PBL_COLOR
    // Paint a red square under the car first for color watches
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_fill_rect(ctx, car_bounds, 0, GCornerNone);
  #endif
  
  graphics_draw_bitmap_in_rect(ctx, s_car_bitmap, car_bounds);

  // 4. Draw the Frog using dynamic variables
  GRect frog_bounds = GRect(s_frog_x, s_frog_y, 24, 24);
  graphics_draw_bitmap_in_rect(ctx, s_frog_bitmap, frog_bounds);
} 

// --- GAME LOOP & ANIMATION ---

static void game_loop(void *data) {
  // Move the car "up" the highway
  s_car_y -= 3; 
  
  // If the car drives off the top of the screen, reset it at the bottom
  if (s_car_y < -24) {
    s_car_y = 168; 
  }

  // Tell the watch to redraw the screen
  layer_mark_dirty(s_canvas_layer);

  // Re-register the timer to run again in 33 milliseconds (~30 FPS)
  s_game_timer = app_timer_register(33, game_loop, NULL);
}

// --- BUTTON INPUTS ---

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Move "Left" (Up the watch face) by exactly one 24px grid space
  if (s_frog_y > 0) {
    s_frog_y -= 24;
    layer_mark_dirty(s_canvas_layer);
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Move "Right" (Down the watch face) by exactly one 24px grid space
  if (s_frog_y < 144) { // 168 total height - 24 sprite size
    s_frog_y += 24;
    layer_mark_dirty(s_canvas_layer);
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Jump Forward (Right across the watch face)
  if (s_frog_x < 120) { // 144 total width - 24 sprite size
    s_frog_x += 24;
    layer_mark_dirty(s_canvas_layer);
  }
}

static void click_config_provider(void *context) {
  // Tell the watch which functions to run when buttons are pressed
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

// --- WINDOW MANAGEMENT ---

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);

  s_frog_bitmap = gbitmap_create_blank(GSize(24, 24), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_frog_bitmap), frog_bitmap_data, sizeof(frog_bitmap_data));

  s_car_bitmap = gbitmap_create_blank(GSize(24, 24), GBitmapFormat1Bit);
  memcpy(gbitmap_get_data(s_car_bitmap), car_bitmap_data, sizeof(car_bitmap_data));
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_frog_bitmap);
  gbitmap_destroy(s_car_bitmap);
  layer_destroy(s_canvas_layer);
}

// --- APP LIFECYCLE ---

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Subscribe to physical button clicks
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  // Start the 30FPS game loop
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