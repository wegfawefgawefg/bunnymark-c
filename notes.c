const Uint8 *keys = SDL_GetKeyboardState(NULL);
if (keys[SDL_SCANCODE_Q]) {
    break;
}

Uint32 buttons; 
buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
if ((buttons & SDL_BUTTON_LMASK) != 0) {
    // SDL_RenderDrawPoint(renderer, mouse_x, mouse_y);
}