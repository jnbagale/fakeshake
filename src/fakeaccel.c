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
#include <string.h>

#include "fake.h"
#include "publisher.h"
#include "fakeaccel.h"
#include "config.h"


gboolean generate_accelerometer_data(fakeObject *fake_obj)
{
  GRand *sample_rand;
  GRand *freq_rand;

  while(1)
    {
      if(fake_obj->frequency_counter ==0) {
	/* Random number of samples per cycle generation */
	if(strcmp (fake_obj->sampletype,"random") == 0 ) {
	  sample_rand = g_rand_new();
	  fake_obj->samplesize  = g_rand_int_range(sample_rand,1,10);
	  g_free(sample_rand);
	}
      
	if(strcmp(fake_obj->type,"fixed")==0) {
	  fake_obj->frequency_counter = fake_obj->frequency;
	}
	else  {
	  /*Random fake data frequency generation*/
	  freq_rand = g_rand_new();
	  fake_obj->frequency_counter = g_rand_int_range(freq_rand,150,300);
	  g_free(freq_rand);
	}
      }
      else if(fake_obj->frequency_counter <= fake_obj->samplesize) {    
	process_data(fake_obj);
      }

      fake_obj->frequency_counter--;
      /* (10000) = 10 ms delay to send controlled messages per cycle */
      g_usleep(10000);
    }
  return TRUE;
}

void process_data(fakeObject *fake_obj)
{
  gint count;
  for(count = 0; count < fake_obj->client ; count ++)
    {
      gchar *message = g_strdup_printf("%s", "a");
      send_data(fake_obj, message, count);
      g_free(message); 
    }
}
