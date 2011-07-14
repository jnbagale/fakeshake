#ifndef FAKE_H_
#define FAKE_H_

#include <sp.h>
#include <glib.h>

#include "config.h"

typedef struct {
  mailbox mbox;
  gchar *size;
  gchar *freq_type; 
  gchar *group_name;
  gchar private_group[MAX_GROUP_NAME];
  gint freq_value;
  gint count;
  gdouble x_values[SAMPLE_SIZE];
  gdouble y_values[SAMPLE_SIZE];
}fakeObject;


fakeObject *make_fake_object(void);
void free_fake_object(fakeObject *fake_obj);

#endif
