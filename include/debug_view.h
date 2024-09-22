#ifndef _DEBUG_VIEW_H_
#define _DEBUG_VIEW_H_

enum { DEBUG_VALUE_INT, DEBUG_VALUE_FLOAT, DEBUG_VALUE_COUNT };

void debug_view_init(void);
void debug_view_push(const char *label, const void *value, const int type);
void debug_view_update(const int toggle_is_pressed, const float dt);
void debug_view_draw(void);
void debug_view_terminate(void);

#endif /* _DEBUG_VIEW_H_ */
