/* Fake accelerometer data generator */
/* Jiva N. Bagale */
/* jnbagale@gmail.com  */
/* 2010 */



#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "fake.h"
#include "comms.h"
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
	  fake_obj->frequency_counter = g_rand_int_range(freq_rand,15,50);
	  g_free(freq_rand);
	}
      }
      else if(fake_obj->frequency_counter <= fake_obj->samplesize) {    
	process_data(fake_obj);
	fake_obj->message_counter++;
      }

      fake_obj->frequency_counter--;
      /* 100 ms delay to send controlled messages per cycle */
      g_usleep(100000);
    }
  return TRUE;
}

void process_data(fakeObject *fake_obj)
{
 
    gchar *message = g_strdup_printf("%s", "seven");
    g_print("Message number %d sent to the network\n",fake_obj->message_counter);
    write_message(fake_obj, message);
    g_free(message); 

}
