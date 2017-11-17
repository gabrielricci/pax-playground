#ifndef _GLITCHECK_H_
#define _GLITCHECK_H_

typedef void *GlitchCheck_t;
typedef void (*GlitchCheck_cb_t)(void *);

/* GlitCheck_cb_t call back function */
void GlitcheckPedTrigger(void *data);

/* create glitch check */
GlitchCheck_t GlitchCheckCreate(int steps, int timeout, GlitchCheck_cb_t callback, void *data);

/* update glitch check */
void GlitchCheckUpdate(GlitchCheck_t *ctx);

/* finish glitch check*/
void GlitchCheckFinish(GlitchCheck_t *ctx);

#endif /* _GLITCHECK_H_ */
