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
#include <glib/gthread.h>
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
  gchar *type = DEFAULT_FREQUENCY_TYPE;
  gint frequency = DEFAULT_FREQUENCY_VALUE;
  gint samplesize = DEFAULT_SAMPLE_SIZE;
  gchar *sampletype = DEFAULT_SAMPLE_TYPE;
  gchar *spread_server;
  fakeObject *fake_obj = NULL;
  
  GOptionEntry entries [] =
  {
    { "type", 't', 0, G_OPTION_ARG_STRING, &type, "Fake data frequency type: fixed or random ", NULL},
    { "frequency", 'f', 0, G_OPTION_ARG_INT, &frequency, "Fake data generation frequency", NULL},
    { "samplesize", 'n', 0, G_OPTION_ARG_INT, &samplesize, "Number of samples per cycle to be sent", NULL},
    { "sampletype", 's', 0, G_OPTION_ARG_STRING, &sampletype, "Samples type: fixed or random", NULL},
    { "group", 'g', 0, G_OPTION_ARG_STRING, &group, "Spread group", NULL },
    { "host", 'h', 0, G_OPTION_ARG_STRING, &host, "Spread host", NULL },
    { "port", 'p', 0, G_OPTION_ARG_INT, &port, "Spread port", "N" },
    { NULL}
  };

 context = g_option_context_new ("- the fake shake data generator");
  g_option_context_add_main_entries (context, entries, PACKAGE_NAME);
  
  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_printerr("option parsing failed: %s\n", error->message);
    exit (EXIT_FAILURE);
  }

  g_thread_init(NULL);

  fake_obj = make_fake_object();

  fake_obj->type = g_strdup(type);
  fake_obj->frequency = frequency;
  fake_obj->samplesize = samplesize;
  fake_obj->sampletype = g_strdup(sampletype);
  fake_obj->frequency_counter = frequency;
  
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

  if( g_thread_create( (GThreadFunc) get_network_info, (gpointer) fake_obj, FALSE, &error) == NULL) {
      g_printerr("option parsing failed1: %s\n", error->message);
  exit (EXIT_FAILURE);
}
 
 if( g_thread_create( (GThreadFunc) generate_accelerometer_data, (gpointer) fake_obj, FALSE, &error) == NULL ) {
      g_printerr("option parsing failed 2: %s\n", error->message);
  exit (EXIT_FAILURE);
}

  g_main_loop_run(mainloop);

  return EXIT_FAILURE;
}
      
