#ifndef ENGINE_DEBUG_H_
#define ENGINE_DEBUG_H_

enum val_type {
	DV_INT,
	DV_FLOAT,
	DV_BOOL,
};

typedef struct {
	const char *name;
	void *val;
	enum val_type type;
} dvalue_t;

void debug_load(void);
void debug_add(const char *name, void *val, enum val_type type);
void debug_clear(void);
void debug_draw(void);

#endif /* ENGINE_DEBUG_H_ */
