#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#include "fake.h"
#include "publisher.h"
#include "config.h"

fakeObject *publish_forwarder(fakeObject *fake_obj, gint count)
{
  gchar *forwarder_address =  g_strdup_printf("tcp://%s:%d",fake_obj->host, fake_obj->pub_obj.pubport);
  /* Prepare our context and publisher */
  fake_obj->pub_obj.pub_context[count] = zmq_init (1);
  fake_obj->pub_obj.publisher[count] = zmq_socket (fake_obj->pub_obj.pub_context[count], ZMQ_PUB);
  zmq_connect (fake_obj->pub_obj.publisher[count], forwarder_address);
  g_print("Now sending data to forwarder %s\n",forwarder_address);
  g_free(forwarder_address);
  return fake_obj;
}

static gint z_send(void *socket ,gchar *string)
{
  int rc;
  zmq_msg_t message;
  zmq_msg_init_size (&message, strlen (string));
  memcpy (zmq_msg_data (&message), string, strlen (string));
  rc = zmq_send (socket, &message, 0);
  zmq_msg_close (&message);
  return (rc);
}

void send_data(fakeObject *fake_obj, gchar *message, gint count)
{
  
    // Send message to all subscribers of group: world
    gchar *update;
    gchar *user_hash_fix;
    user_hash_fix = g_strdup_printf("%s%d",fake_obj->user_hash, count);
    //sprintf (update,"%s %s %s", message, fake_obj->user_hash, fake_obj->group_hash);
    update = g_strdup_printf("%s %s %s",fake_obj->group_hash, user_hash_fix, message);
    z_send (fake_obj->pub_obj.publisher[count], update); 
    //g_print("Sent %s of size %d bytes\n",update, strlen(update));
    g_free(update);
    g_free(user_hash_fix);
}

