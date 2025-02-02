#ifndef DISPATCH_KEYS_H
#define DISPATCH_KEYS_H

typedef struct 
{
    void (*on_pressed)(void);
    void (*on_short_pressed)(void);
    void (*on_long_pressed)(void);
}key_handle;

typedef struct
{
    unsigned int pressed:1;
    unsigned int shortPress:1;
    unsigned int longPress:1;
}key_state;

void dispatch_keys(key_handle *key);


#endif /* DISPATCH_KEYS_H */
