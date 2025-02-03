#include "dispatch_keys.h"
#include "encoder.h"
#include "page.h"

void dispatch_keys(key_handle *key)
{
    key_state keys_state[KEY_COUNT];

    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys_state[i].pressed = KeyEvent[i].KeyPressed;
        keys_state[i].shortPress = KeyEvent[i].shortPress;
        keys_state[i].longPress = KeyEvent[i].longPress;

        if (keys_state[i].pressed)
        {
            if(key[i].on_pressed != NULL)
                {key[i].on_pressed();} 
            KeyEvent[i].KeyPressed = false;
        }
        if (keys_state[i].shortPress)
        {
            if(key[i].on_short_pressed != NULL)
                {key[i].on_short_pressed();}
            KeyEvent[i].shortPress = false;
        }
        else if (keys_state[i].longPress)
        {
            if(key[i].on_long_pressed != NULL)
                {key[i].on_long_pressed();}
            KeyEvent[i].longPress = false;
        }
    }
}