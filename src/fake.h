#ifndef FAKE_H_
#define FAKE_H_

#include <glib.h>

typedef struct {
  gchar *frequency;
  gint value;
  gchar *size;
}fakeObject;

fakeObject *make_fake_object(void);
void free_fake_object(fakeObject *fake_obj);

#endif
