#include "pebble.h"

static Window *window;

static TextLayer *heartninja_layer;
static char score[16];

// Key values for HeartKeyMessage Dictionary

enum HeartKeyMessage {
  STATUS_KEY = 0,
  MESSAGE_KEY,
  SCORE_NINJA_KEY
};

// When a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
  Tuple *tuple;
  
  tuple = dict_find(received, STATUS_KEY);
  if(tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
  }
  
  tuple = dict_find(received, MESSAGE_KEY);
  if(tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
  }

  tuple = dict_find(received, SCORE_NINJA_KEY); 
  if(tuple) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Score Ninja: %s", tuple->value->cstring);
    text_layer_set_text(heartninja_layer, tuple->value->cstring);
  } 
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Drop Message: %i", reason);
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Failed Message: %i", reason);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
  snprintf(score, sizeof(score), "--");
  heartninja_layer = text_layer_create(GRect(0, 100, 144, 68));
  text_layer_set_text_color(heartninja_layer, GColorWhite);
  text_layer_set_background_color(heartninja_layer, GColorClear);
  text_layer_set_font(heartninja_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(heartninja_layer, GTextAlignmentCenter);
  text_layer_set_text(heartninja_layer, score);

  layer_add_child(window_layer, text_layer_get_layer(heartninja_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(heartninja_layer);
}

static void init() {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_fullscreen(window, true);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  // Register HeartKeyMessage handlers
  app_message_register_inbox_received(in_received_handler); 
  app_message_register_inbox_dropped(in_dropped_handler); 
  app_message_register_outbox_failed(out_failed_handler);
    
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit() {
  app_message_deregister_callbacks();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}