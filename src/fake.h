#ifndef FAKE_H_
#define FAKE_H_

#include <sp.h>
#include <glib.h>

#include "config.h"

typedef struct {
  mailbox mbox[2560];
  gint client;
  gchar *type;
  gchar *group;
  gchar *host;
  gint port;
  gchar *group_name;
  gchar private_group[MAX_GROUP_NAME];
  gchar *sampletype;
  gint frequency_counter;
  gint message_counter;
  gint frequency;
  gint samplesize;
}fakeObject;


fakeObject *make_fake_object(void);
void free_fake_object(fakeObject *fake_obj);

#endif /*FAKE_H_*/
