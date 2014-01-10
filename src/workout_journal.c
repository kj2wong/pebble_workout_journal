//Pebble App: Workout Journal
//	The purpose of this app is to act like a gym journal to keep track of progressions
//Author: Kevin J Wong

//NOTE: This app currently has the following functionality (still in progress)
//	-implemented the window that will count your reps

#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static int reps = 0;

//Function: convert_reps_to_string
//Purpose: helper function to convert number of reps (int) to a string
static char* convert_reps_to_string(int i) {
	static char buf[32];
  	snprintf(buf, 32, "Reps: %u", reps);
	return buf;
}

//Function: select_click_handler
//Purpose: handler for the middle button; resets reps to 0
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	reps = 0;
	char* reps_str = convert_reps_to_string(reps);
	text_layer_set_text(text_layer, reps_str);
}

//Function: up_click_handler
//Purpose: handler for the up button; increase reps by 1
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	reps += 1;
	char* reps_str = convert_reps_to_string(reps);
	text_layer_set_text(text_layer, reps_str);
}

//Function: down_click_handler
//Purpose: handler for the down button; decrease reps by 1
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (reps > 0)
		reps -= 1;
	char* reps_str = convert_reps_to_string(reps);
  	text_layer_set_text(text_layer, reps_str);
}

//Function: click_config_provider
//Purpose: subscribe to all the button handlers
static void click_config_provider(void *context) {
  	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

//Function: window_load
//Purpose: handler for loading the window initially
static void window_load(Window *window) {
  	Layer *window_layer = window_get_root_layer(window);
  	GRect bounds = layer_get_bounds(window_layer);

  	text_layer = text_layer_create((GRect) { .origin = { 0, 65 }, .size = { bounds.size.w, 40 } });

	char* reps_str = convert_reps_to_string(reps);
  	text_layer_set_text(text_layer, reps_str);

  	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  	layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

//Function: window_unload
//Purpose: unloads the window by destroying its components
static void window_unload(Window *window) {
  	text_layer_destroy(text_layer);
}

//Function: init
//Purpose: initializes the windows and window stack
static void init(void) {
  	window = window_create();
  	window_set_click_config_provider(window, click_config_provider);
  	window_set_window_handlers(window, (WindowHandlers) {
    	.load = window_load,
    	.unload = window_unload,
  	});
  	const bool animated = true;
  	window_stack_push(window, animated);
}

//Function: deinit
//Purpose: deinitializes the app by destroying windows
static void deinit(void) {
  	window_destroy(window);
}

//Function: main
//Purpose: entry point in the program
int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
