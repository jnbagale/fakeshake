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
#include <string.h>
#include <stdlib.h>

#include "fake.h"
#include "config.h"


fakeObject *connect_spread(fakeObject *fake_obj,gint mbox_counter)
{
  uuid_t buf;
  gint ret;
  gchar id[36];
  gchar *user_hash;
  gchar *spread_server;
  gchar *group_hash;
  gchar *group_name_fix;

  uuid_generate_random(buf);
  uuid_unparse(buf, id);
 // generate a hash of a unique id
  user_hash = g_compute_checksum_for_string(G_CHECKSUM_MD5, id, strlen(id));
  // format needed by spread
  spread_server = g_strdup_printf("%d@%s", fake_obj->port, fake_obj->host);
  // returns an id for this connection in private_group and set mbox
  ret = SP_connect(spread_server, user_hash, 0, 1, &(fake_obj->mbox[mbox_counter]), fake_obj->private_group);

  g_free(spread_server);
  g_free(user_hash);

  if(ret == ACCEPT_SESSION) {
    g_print("Successfully connected to %d@%s as %s\n",fake_obj->port, fake_obj->host, fake_obj->private_group);
  }
  else {
    SP_error(ret);
    /* wait 30 seconds and try to reconnect to spread */
    g_print("Trying to reconnect to spread in 30 seconds...\n");
    g_usleep(30000000);
    connect_spread(fake_obj, mbox_counter);
  }
 
  // generate a hash of the group name
  group_hash = g_compute_checksum_for_string(G_CHECKSUM_MD5, fake_obj->group, strlen(fake_obj->group));
  // 32 len strings don't seem to work so this is a fix
  group_name_fix = g_strndup(group_hash, 31);
  g_free(group_hash);

  // will need to be freed
  fake_obj->group_name = group_name_fix;

  return fake_obj;
} 

fakeObject *disconnect_spread(fakeObject *fake_obj, gint mbox_counter)
{
  gint ret;

 ret = SP_disconnect( fake_obj->mbox[mbox_counter]);
 
 if(ret == 0) {
   g_print("Successfully disconnected from %d@%s\n", fake_obj->port, fake_obj->host);
 }
 else {
   SP_error(ret);
 }

 return fake_obj;
}

fakeObject *join_spread(fakeObject *fake_obj, gint mbox_counter)
{
  gint ret;

  ret = SP_join( fake_obj->mbox[mbox_counter], fake_obj->group_name );

  if(ret == 0) {
    g_print("Successfully joined group: %s\n",fake_obj->group_name);
  }
  else {
    SP_error(ret);
  }

  return fake_obj;
}

fakeObject *leave_spread(fakeObject *fake_obj, gint mbox_counter)
{
  gint ret;

  ret = SP_leave( fake_obj->mbox[mbox_counter], fake_obj->group_name );
 
  if(ret == 0) {
    g_print("Successfully left group: %s\n",fake_obj->group_name);
  }
  else {
    SP_error(ret);
  }

  return fake_obj;
}
