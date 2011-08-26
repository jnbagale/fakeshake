
#include <glib.h>
#include <stdlib.h>
#include "fake.h"
#include "config.h"


fakeObject *make_fake_object(void)
{
  fakeObject *fake_obj;
  if ((fake_obj = (fakeObject *)g_malloc(sizeof(fakeObject))) == NULL){
      g_printerr("failed to malloc fakeObject!");
      exit(EXIT_FAILURE);
      }
  fake_obj->message_counter = 0;
  return fake_obj;
}

void free_fake_object(fakeObject *fake_obj)
{
  g_free(fake_obj->type);
  g_free(fake_obj->sampletype);
  g_free(fake_obj);
}

