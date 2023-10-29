#include "input.h"
#include "common.h"
#include <SDL2/SDL_events.h>


// INPUT
void updateDeviceInputKey(DeviceInput *deviceInput, DeviceInputEvent event,
                          player_commands command) {
  if ((deviceInput->pressed_commands[command].key_started_down &&
       event.pressed_else_released) ||
      (!deviceInput->pressed_commands[command].key_started_down &&
       !event.pressed_else_released)) {
    deviceInput->pressed_commands[command].transition_count++;
  }

  deviceInput->pressed_commands[command].key_ended_down =
      event.pressed_else_released;
}

void updateDeviceInput(DeviceInput *deviceInput, DeviceInputEvent event) {
  switch (event.device_type) {
  case KEYBOARD: {
    switch (event.keyboard_code) {
    // case SDL_SCANCODE_L: {
    //   if (event.pressed_else_released) {
    //     if (GlobalPlatformState.IsRecording) {
    //       SDL_RWclose(GlobalPlatformState.RecordingInputFileHandle);
    //
    //       GlobalPlatformState.IsRecording = false;
    //       GlobalPlatformState.IsPlayingback = true;
    //
    //       GlobalPlatformState.PlaybackFileHandle =
    //           SDL_RWFromFile("loop_recording", "r");
    //
    //     } else if (GlobalPlatformState.IsPlayingback) {
    //       GlobalPlatformState.IsPlayingback = false;
    //       GlobalPlatformState.IsRecording = false;
    //
    //       for (button_state pressedCommand : deviceInput->PressedCommands) {
    //         pressedCommand.KeyStartedDown = false;
    //         pressedCommand.KeyEndedDown = false;
    //         pressedCommand.TransitionCount = 0;
    //         pressedCommand.HeldDownSinceLastEvent = false;
    //         pressedCommand.BufferedKeypressTime =
    //             std::chrono::time_point<std::chrono::high_resolution_clock>();
    //       }
    //     } else {
    //       GlobalPlatformState.IsRecording = true;
    //
    //       GlobalPlatformState.RecordingGameStateFileHandle =
    //           SDL_RWFromFile("game_state", "w+");
    //
    //       SDL_RWseek(GlobalPlatformState.RecordingGameStateFileHandle, 0,
    //                  RW_SEEK_SET);
    //       SDL_RWwrite(GlobalPlatformState.RecordingGameStateFileHandle,
    //                   GlobalPlatformState.persistantMemory, 8,
    //                   (GlobalPlatformState.persistantMemorySize +
    //                    GlobalPlatformState.temporaryMemorySize) /
    //                       8);
    //
    //       GlobalPlatformState.RecordingInputFileHandle =
    //           SDL_RWFromFile("loop_recording", "w+");
    //     }
    //   }
    //   break;
    // }
    // case SDL_SCANCODE_P: {
    //   if (event.pressed_else_released) {
    //     GlobalPlatformState.IsPaused = !GlobalPlatformState.IsPaused;
    //   }
    //   break;
    // }
    // case SDL_SCANCODE_U: {
    //   if (event.pressed_else_released) {
    //     fResetWorld();
    //   }
    //   break;
    // }
    case SDL_SCANCODE_W: {
      updateDeviceInputKey(deviceInput, event, MOVE_FORWARD);
      break;
    }
    case SDL_SCANCODE_S: {
      updateDeviceInputKey(deviceInput, event, MOVE_BACKWARD);
      break;
    }
    case SDL_SCANCODE_A: {
      updateDeviceInputKey(deviceInput, event, MOVE_LEFT);
      break;
    }
    case SDL_SCANCODE_D: {
      updateDeviceInputKey(deviceInput, event, MOVE_RIGHT);
      break;
    }

    case SDL_SCANCODE_1: {
      updateDeviceInputKey(deviceInput, event, DEBUG_1);
      break;
    }
    case SDL_SCANCODE_2: {
      updateDeviceInputKey(deviceInput, event, DEBUG_2);
      break;
    }
    case SDL_SCANCODE_3: {
      updateDeviceInputKey(deviceInput, event, DEBUG_3);
      break;
    }
    case SDL_SCANCODE_4: {
      updateDeviceInputKey(deviceInput, event, DEBUG_4);
      break;
    }
    case SDL_SCANCODE_5: {
      updateDeviceInputKey(deviceInput, event, DEBUG_5);
      break;
    }

    default: {
      // NOT HANDLED YET
      break;
    }
    }
    break;
  }
  case MOUSE: {
    switch (event.mouse_code) {
    case 0: {
      // MIDDLECLICK
      // updateDeviceInputKey(deviceInput, event, USE_ITEM);
      break;
    }
      // LEFT CLICK
    case 2: {
      break;
    }
      // RIGHT CLICK
    case 3: {
      // updateDeviceInputKey(deviceInput, event, USE_ITEM);
      break;
    }
    default: {
      // NOT HANDLED YET
      break;
    }

    break;
    }
  }
  }
}


void handleInput(DeviceInput *deviceInput) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYDOWN: {
      updateDeviceInput(
          deviceInput,
          DeviceInputEvent{.keyboard_code = event.key.keysym.scancode,
                             .pressed_else_released = true,
                             .device_type = KEYBOARD});
    } break;
    case SDL_KEYUP: {
      updateDeviceInput(deviceInput,
                        DeviceInputEvent{
                            .keyboard_code = event.key.keysym.scancode,
                            .pressed_else_released = false,
                            .device_type = KEYBOARD,
                        });
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      updateDeviceInput(deviceInput, DeviceInputEvent{
                                         .mouse_code = event.button.button,
                                         .pressed_else_released = true,
                                         .device_type = MOUSE,
                                     });
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      updateDeviceInput(deviceInput, DeviceInputEvent{
                                         .mouse_code = event.button.button,
                                         .pressed_else_released = false,
                                         .device_type = MOUSE,
                                     });
      break;
    }
    case SDL_MOUSEMOTION: {
      // make y zero on bottom instead of on top
      deviceInput->mouse_position =
          Vec2{(float)event.motion.x, (float)(IMAGE_HEIGHT - event.motion.y)};
      break;
    }
    case SDL_QUIT: {
      exit(1);
      break;
    }
    }
  }
}

