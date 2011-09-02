/* Copyright (C) 2009-2011 Jiva Nath Bagale */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

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
  g_free(fake_obj->group);
  g_free(fake_obj->host); 
  g_free(fake_obj);
}

