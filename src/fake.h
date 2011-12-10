#ifndef FAKE_H_
#define FAKE_H_

#include <sp.h>
#include <glib.h>

#include "config.h"

typedef struct {
  gint client;
  gchar *type;
  gchar *group;
  gchar *host;
  gchar *group_hash;
  gchar *user_hash;
  gint port;
  gchar *group_name;
  gchar *sampletype;
  gint frequency_counter;
  gint message_counter;
  gint frequency;
  gint samplesize;

  /*ZeroMQ publisher object */
  struct {
    void *pub_context[2560];
    void *publisher[2560];
    gint pubport;
  } pub_obj;

}fakeObject;

fakeObject *make_fake_object(void);
void free_fake_object(fakeObject *fake_obj);

#endif /*FAKE_H_*/
