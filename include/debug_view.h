#ifndef _DEBUG_VIEW_H_
#define _DEBUG_VIEW_H_

void debug_view_init(void);
void debug_push_int(const char *label, const int value);
void debug_view_values_draw(void);
void debug_view_terminate(void);

#endif /* _DEBUG_VIEW_H_ */
