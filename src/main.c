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
#include <glib/gthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fake.h"
#include "comms.h"
#include "fakeaccel.h"
#include "fake_spread.h"
#include "config.h"

int main ( int argc, char *argv[] )
{
  GError *error;
  GMainLoop *mainloop;
  GOptionContext *context;
  gchar *type = DEFAULT_FREQUENCY_TYPE;
  gint frequency = DEFAULT_FREQUENCY_VALUE;
  gint samplesize = DEFAULT_SAMPLE_SIZE;
  gchar *sampletype = DEFAULT_SAMPLE_TYPE;
  gchar *group = DEFAULT_GROUP;
  gchar *host = DEFAULT_HOST;
  gint port = DEFAULT_PORT;
  gint client = DEFAULT_CLIENT_NO;
  guint count;

  fakeObject *fake_obj = NULL;
  
  GOptionEntry entries [] =
  {
    { "type", 't', 0, G_OPTION_ARG_STRING, &type, "Fake data frequency type: fixed or random ", NULL},
    { "frequency", 'f', 0, G_OPTION_ARG_INT, &frequency, "Fake data generation frequency", NULL},
    { "samplesize", 'n', 0, G_OPTION_ARG_INT, &samplesize, "Number of samples per cycle to be sent", NULL},
    { "sampletype", 's', 0, G_OPTION_ARG_STRING, &sampletype, "Samples type: fixed or random", NULL},
    { "client", 'c', 0, G_OPTION_ARG_INT, &client, "Number of fakeshake clients", NULL },
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

  /* fake_obj-> host, type, group and sampletype need to be freed later */
  fake_obj->host = g_strdup(host);
  fake_obj->type = g_strdup(type);
  fake_obj->group = g_strdup(group);
  fake_obj->sampletype = g_strdup(sampletype);

  fake_obj->port = port;
  fake_obj->client = client;
  fake_obj->frequency = frequency;
  fake_obj->samplesize = samplesize;
  fake_obj->frequency_counter = frequency;
  
  for(count = 0; count < fake_obj->client; count++)
    {
      connect_spread(fake_obj, count);
      /* join_spread(fake_obj, count); */
    }

  mainloop = g_main_loop_new(NULL, FALSE);
  if (mainloop == NULL) {
    g_printerr("Couldn't create GMainLoop\n");
    exit(EXIT_FAILURE);
  }
 
  /* if( g_thread_create( (GThreadFunc) get_network_info, (gpointer) fake_obj, FALSE, &error) == NULL) { */
  /* 	g_printerr("option parsing failed 1: %s\n", error->message); */
  /* 	exit (EXIT_FAILURE); */
  /* } */

  if( g_thread_create( (GThreadFunc) generate_accelerometer_data, (gpointer) fake_obj, FALSE, &error) == NULL ) {
    g_printerr("option parsing failed 2: %s\n", error->message);
    exit (EXIT_FAILURE);
  }

  g_main_loop_run(mainloop);

  return EXIT_FAILURE;
}
      
