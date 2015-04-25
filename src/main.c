#include <pebble.h>
#define TAP_NOT_DATA true

Window *myWindow;
TextLayer *repCountText, setCountText;
short unsigned int repMax = 5;
short unsigned int setMax = 5;
short unsigned int reps = 0;
short unsigned int sets = 0;
short unsigned int timer = 0;
char repBuffer[20];
char setBuffer[20];
char timerBuffer[20];
bool movedDown = false;
bool movedUp = false;

static void tap_handler(AccelAxisType axis, int32_t direction){
  snprintf(setBuffer, sizeof(setBuffer), "%d\n Set:", sets);
  text_layer_set_text(setCountText, setBuffer);
  switch(axis){
    case ACCEL_AXIS_Z:
      if(direction > 0){ //move up towards head
        movedUp = true; 
      }
    else { //move down towards feet
      if(movedUp){
        reps++;
        snprintf(repBuffer, sizeof(repBuffer), "%d\n Reps", reps);
        text_layer_set_text(repCountText, repBuffer);
        movedUp = false;
      }
    }
    break;
    case ACCEL_AXIS_Y:
    break;
    case ACCEL_AXIS_X:
    break;
  }
}

void window_load(Window *myWindow){
  repCountText = text_layer_create(GRect(0, 0, 144, 168));
  //setCountText
  text_layer_set_background_color(repCountText, GColorClear);
  text_layer_set_text_color(repCountText, GColorBlack);
  text_layer_set_font(repCountText, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(repCountText, GTextAlignmentCenter);

  layer_add_child(window_get_root_layer(myWindow), text_layer_get_layer(repCountText));
  text_layer_set_text(repCountText, "Start");
  
  
}

void window_unload(Window *myWindow){
  text_layer_destroy(repCountText);
 
}

void init(){
  myWindow = window_create();
  window_set_window_handlers(myWindow, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(myWindow, true);
  
  if (TAP_NOT_DATA)
    accel_tap_service_subscribe(tap_handler); 
  accel_service_set_sampling_rate(ACCEL_SAMPLING_50HZ);
}

void deinit(){
   window_destroy(myWindow);
  
  if(TAP_NOT_DATA)
    accel_tap_service_unsubscribe();
}
  
int main(void){
  init();
  app_event_loop();
  deinit();
}
