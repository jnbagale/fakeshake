
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
  return fake_obj;
}

void free_fake_object(fakeObject *fake_obj)
{
  g_free(fake_obj->frequency);
  g_free(fake_obj->size);
  g_free(fake_obj);

}

