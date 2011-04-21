// Fake accelerometer data generator

#include<glib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#include "config.h"
#include "fakeshake.h"
#include "fake.h"

#define BUFLEN 32
// To store shaking frequency
static gint count;

gboolean generate_accelerometer_data(fakeObject *fake_obj)
{
  GRand *x_rand, *y_rand, *freq_rand;
  gchar *val1, *val2, *val;	
  gint xvalue, yvalue, duration = 500, range, fd, ret;
   
  x_rand = g_rand_new();
  y_rand = g_rand_new();
  freq_rand = g_rand_new();
  count =count - 1;

  if(count >0 && count <=duration)
    {
      if( strcmp(fake_obj->size,"high") ==0)
       range = 100;
      else if ( strcmp(fake_obj->size,"medium") ==0)
	range =50;
      else if (strcmp(fake_obj->size,"low") ==0)
	range = 20;
      else 
	range = 50;
    }
  else 
    {
      range = 5;
    }

  if(count ==0)
    {
      if(strcmp(fake_obj->frequency,"fixed")==0)
	{
	  count = fake_obj->value;
	}
      else
	{
	  count = g_rand_int_range(freq_rand,1000,5000);
	  //g_print("%s %d %s \n",fake_obj->frequency,fake_obj->value,fake_obj->size);
	  g_print("New Quake Frequency :%d\n",count);
	}
    }
  // Generating random numbers   
  xvalue = g_rand_int_range(x_rand,0,range);
  yvalue = g_rand_int_range(y_rand,0,range);
  val1 = g_strdup_printf ("%d",xvalue);
  val2 = g_strdup_printf ("%d",yvalue);
  val =  g_strdup_printf("(%s,%s)",val1,val2);
 
  fd = open(FAKE_POSITION_FILE,O_WRONLY);
  if( fd < 0){
    g_printerr("failed to open the file %s!\n",FAKE_POSITION_FILE);
    exit(EXIT_FAILURE);
  }

  ret = write(fd, val, BUFLEN);
  if( ret < 0){
    g_printerr("failed to write data !\n");
    exit(EXIT_FAILURE);
  }

  close(fd);
  g_print("%s\n",val);
  g_free(val1);
  g_free(val2);	
  g_free(val);
  g_free(x_rand);
  g_free(y_rand);
  g_free(freq_rand);
  
  return TRUE;
}


int main ( int argc, char *argv[] )
{
  GOptionContext *context;
  GError *error;
  GMainLoop *mainloop;
  gchar *freqtype, *size;
  gint freqvalue;
  fakeObject *fake_obj = NULL;
  
  GOptionEntry entries [] =
  {
    { "type", 'f', 0, G_OPTION_ARG_STRING, &freqtype, "Quake Frequency type: Fixed or Random ", NULL},
    { "value", 'v', 0, G_OPTION_ARG_INT, &freqvalue, "Quake Frequency value", NULL},
    { "size", 's', 0, G_OPTION_ARG_STRING, &size, "Quake size: Low, Medium or High", NULL}, 
    { NULL}
  };

 context = g_option_context_new ("- the fake shake generator");
  g_option_context_add_main_entries (context, entries, PACKAGE_NAME);
  
  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_printerr("option parsing failed: %s\n", error->message);
    exit (EXIT_FAILURE);
  }
  
  fake_obj = make_fake_object();
  fake_obj->frequency = freqtype;
  fake_obj->value = freqvalue;
  fake_obj->size = size;
  count = freqvalue;

  mainloop = g_main_loop_new(NULL, FALSE);
  if (mainloop == NULL) {
    g_printerr("Couldn't create GMainLoop\n");
    exit(EXIT_FAILURE);
  }

  g_timeout_add(FAKE_GEN_FREQ, (GSourceFunc)generate_accelerometer_data,(gpointer)fake_obj);
 
  g_main_loop_run(mainloop);

  return EXIT_FAILURE;
}
      
