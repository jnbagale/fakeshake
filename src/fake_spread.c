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
 
  if( ret < 0 ) 
  {
    SP_error( ret );
    exit(0);
  }
  g_free(spread_server);
  g_free(user_hash);
 
  // generate a hash of the group name
  group_hash = g_compute_checksum_for_string(G_CHECKSUM_MD5, fake_obj->group, strlen(fake_obj->group));
  // 32 len strings don't seem to work so this is a fix
  group_name_fix = g_strndup(group_hash, 31);
  g_free(group_hash);

  // will need to be freed
  fake_obj->group_name = group_name_fix;

  return fake_obj;
} 

fakeObject *join_spread(fakeObject *fake_obj, gint mbox_counter)
{
  
  SP_join( fake_obj->mbox[mbox_counter], fake_obj->group_name );

  return fake_obj;
}
