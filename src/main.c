/* Copyright (C) 2009-2011 Jiva Nath Bagale */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

#include <sp.h>
#include <glib.h>
#include <uuid.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fake.h"
#include "comms.h"
#include "fakeaccel.h"
#include "config.h"

int main ( int argc, char *argv[] )
{
  GError *error;
  GMainLoop *mainloop;
  GOptionContext *context;
  uuid_t buf;
  gint ret;
  gchar id[36];
  gchar *user_hash;
  gchar *group_hash;
  gchar *group_name_fix;
  gchar *group = DEFAULT_GROUP;
  gchar *host = DEFAULT_HOST;
  gint port = DEFAULT_PORT;
  gchar *freqtype = DEFAULT_FREQ_TYPE;
  gint freqvalue = DEFAULT_FREQ_VALUE;
  gchar *size = DEFAULT_QUAKE_SIZE;
  gchar *spread_server;
  fakeObject *fake_obj = NULL;
  
  GOptionEntry entries [] =
  {
    { "type", 'f', 0, G_OPTION_ARG_STRING, &freqtype, "Quake Frequency type: Fixed or Random ", NULL},
    { "value", 'v', 0, G_OPTION_ARG_INT, &freqvalue, "Quake Frequency value", NULL},
    { "size", 's', 0, G_OPTION_ARG_STRING, &size, "Quake size: Low, Medium or High", NULL}, 
    { "group", 'g', 0, G_OPTION_ARG_STRING, &group, "Spread group", NULL },
    { "host", 'h', 0, G_OPTION_ARG_STRING, &host, "Spread host", NULL },
    { "port", 'p', 0, G_OPTION_ARG_INT, &port, "Spread port", "N" },
    { NULL}
  };

 context = g_option_context_new ("- the fake shake generator");
  g_option_context_add_main_entries (context, entries, PACKAGE_NAME);
  
  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_printerr("option parsing failed: %s\n", error->message);
    exit (EXIT_FAILURE);
  }
  
  fake_obj = make_fake_object();

  fake_obj->freq_type = g_strdup(freqtype);
  fake_obj->freq_value = freqvalue;
  fake_obj->size = g_strdup(size);
  fake_obj->count = freqvalue;
  
  uuid_generate_random(buf);
  uuid_unparse(buf, id);
 // generate a hash of a unique id
  user_hash = g_compute_checksum_for_string(G_CHECKSUM_MD5, id, strlen(id));
  // format needed by spread
  spread_server = g_strdup_printf("%d@%s", port, host);
  // returns an id for this connection in private_group and set mbox
  ret = SP_connect(spread_server, user_hash, 0, 1, &(fake_obj->mbox), fake_obj->private_group);
  if( ret < 0 ) 
  {
    SP_error( ret );
    exit(0);
  }
  g_free(spread_server);
  g_free(user_hash);
  
  // generate a hash of the group name
  group_hash = g_compute_checksum_for_string(G_CHECKSUM_MD5, group, strlen(group));
  // 32 len strings don't seem to work so this is a fix
  group_name_fix = g_strndup(group_hash, 31);
  g_free(group_hash);


  // will need to be freed
  fake_obj->group_name = group_name_fix;
  
  SP_join( fake_obj->mbox, fake_obj->group_name );

  mainloop = g_main_loop_new(NULL, FALSE);
  if (mainloop == NULL) {
    g_printerr("Couldn't create GMainLoop\n");
    exit(EXIT_FAILURE);
  }

  g_timeout_add(FAKE_GEN_FREQ, (GSourceFunc)generate_accelerometer_data,(gpointer)fake_obj);
  g_timeout_add(1000, (GSourceFunc)get_network_info,(gpointer)fake_obj);

  g_main_loop_run(mainloop);

  return EXIT_FAILURE;
}
      
