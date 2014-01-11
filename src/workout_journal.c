//Pebble App: Workout Journal
//	The purpose of this app is to act like a gym journal to keep track of progressions
//Author: Kevin J Wong

//NOTE: This app currently has the following functionality (still in progress)
//	-implemented the window that will count your reps
//	-implemented exercise menu to kick off the app, selecting one takes you to the rep counter

#include <pebble.h>

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 3

static Window *reps_window;
static Window *exercise_window;

static SimpleMenuLayer *exercise_menu;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem menu_items[NUM_MENU_ITEMS];

static TextLayer *reps_text;
static int reps = 0;

//Helper function to convert number of reps (int) to a string
static char* convert_reps_to_string(int i) {
	static char buf[32];
	snprintf(buf, 32, "Reps: %u", reps);
	return buf;
}


//------------------------------------------------
//	Handlers for button clicks on all windows
//------------------------------------------------

//Handler for when an exercise is selected in the menu; will show the rep counter window
static void exercise_menu_select(int index, void *ctx) {
	window_stack_push(reps_window, true);
}

//Used on the rep counter window
//When the select button is hit, it will save the rep number and return to exercise menu
static void reps_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	//TODO: implement saving mechanism
	window_stack_pop(true);
}

//Used on the rep counter window
//Up button increases reps by 1
static void reps_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	reps += 1;
	char* reps_str = convert_reps_to_string(reps);
	text_layer_set_text(reps_text, reps_str);
}

//Used on the rep counter window
//Up button increases reps by 1
static void reps_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (reps > 0)
		reps -= 1;
	char* reps_str = convert_reps_to_string(reps);
	text_layer_set_text(reps_text, reps_str);
}

//Subscribe to all the button handlers on the rep window
static void reps_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, reps_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, reps_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, reps_down_click_handler);
}


//------------------------------------------------
//	Load/unload functions for the windows
//------------------------------------------------

//Load function of the exercise menu window
static void exercise_window_load(Window *window) {
	//initialize all menu items and menu section
	menu_items[0] = (SimpleMenuItem){
		.title = "Squat",
		.callback = exercise_menu_select,
	};
	menu_items[1] = (SimpleMenuItem){
		.title = "Bench Press",
		.callback = exercise_menu_select,
	};
	menu_items[2] = (SimpleMenuItem){
		.title = "Deadlift",
		.callback = exercise_menu_select,
	};

	menu_sections[0] = (SimpleMenuSection){
		.title = "Exercises",
		.num_items = NUM_MENU_ITEMS,
		.items = menu_items,
	};

	Layer *window_layer = window_get_root_layer(exercise_window);
	GRect bounds = layer_get_frame(window_layer);

	//Initialize the menu layer
	exercise_menu = simple_menu_layer_create(bounds, exercise_window, menu_sections, NUM_MENU_SECTIONS, NULL);

	//Add the layer to the window
	layer_add_child(window_layer, simple_menu_layer_get_layer(exercise_menu));
}

// Unload function for the exercise menu window
void exercise_window_unload(Window *window) {
	simple_menu_layer_destroy(exercise_menu);
}

//Load function for the rep counter window
static void reps_window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	reps_text = text_layer_create((GRect) { .origin = { 0, 65 }, .size = { bounds.size.w, 40 } });

	//Set the initial reps to 0
	char* reps_str = convert_reps_to_string(reps);
	text_layer_set_text(reps_text, reps_str);

	text_layer_set_text_alignment(reps_text, GTextAlignmentCenter);
	text_layer_set_font(reps_text, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	layer_add_child(window_layer, text_layer_get_layer(reps_text));
}

//Unload function for the rep counter window
static void reps_window_unload(Window *window) {
	text_layer_destroy(reps_text);
}


//------------------------------------------------
//	Init/Deinit functions
//------------------------------------------------

//Purpose: initializes the windows and pushes the exercise menu onto the window stack
static void init(void) {
	//Inititalize the reps window
	reps_window = window_create();
	window_set_click_config_provider(reps_window, reps_click_config_provider);
	window_set_window_handlers(reps_window, (WindowHandlers) {
		.load = reps_window_load,
		.unload = reps_window_unload,
	});

	//Initialize the exercise menu
	exercise_window = window_create();
	window_set_window_handlers(exercise_window, (WindowHandlers) {
		.load = exercise_window_load,
		.unload = exercise_window_unload,
	});

	window_stack_push(exercise_window, true);
}

//Deinitializes the app by destroying windows
static void deinit(void) {
	window_destroy(reps_window);
	window_destroy(exercise_window);
}


//------------------------------------------------
//	MAIN
//------------------------------------------------

//Function: main
//Purpose: entry point in the program
int main(void) {
	init();

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", exercise_window);

	app_event_loop();
	deinit();
}
