/* Fake accelerometer data generator */
/* Jiva N. Bagale */
/* jnbagale@gmail.com  */
/* 2010 */



#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <gsl/gsl_statistics.h>


#include "fake.h"
#include "comms.h"
#include "fakeaccel.h"
#include "config.h"


#define BUFLEN 32

gboolean is_shaking(fakeObject *fake_obj, gdouble *covariance)
{
  
  *covariance = gsl_stats_covariance(fake_obj->x_values, 1, fake_obj->y_values, 1, SAMPLE_SIZE);
  //g_print("covariance: %g\n", *covariance);
  if (*covariance > 1) {
    return TRUE;
  }
  else if(*covariance < -1){
    return TRUE;
  }
  else {
    return FALSE;   
  }
}


gboolean generate_accelerometer_data(fakeObject *fake_obj)
{
  GRand *x_rand;
  GRand *y_rand;
  GRand *freq_rand;
  gint range = 5;
  gint duration = 500;
  gint sample_count = 0;

  while(sample_count < SAMPLE_SIZE)
    {
      
      x_rand = g_rand_new();
      y_rand = g_rand_new();
      freq_rand = g_rand_new();
      fake_obj->count--;

      if(fake_obj->count >0 && fake_obj->count <=duration)
	{
	  if( strcmp(fake_obj->size,"high") ==0)
	    range = 100;
	  else if ( strcmp(fake_obj->size,"medium") ==0)
	    range =66;
	  else if (strcmp(fake_obj->size,"low") ==0)
	    range = 33;
	}
      else 
	{
	  range = 5;
	}

      if(fake_obj->count ==0)
	{
	  if(strcmp(fake_obj->freq_type,"fixed")==0)
	    {
	      fake_obj->count = fake_obj->freq_value;
	    }
	  else
	    {
	      fake_obj->count = g_rand_int_range(freq_rand,1000,5000);
	    }
	}

      /* Generating random numbers    */
      fake_obj->x_values[sample_count] = g_rand_int_range(x_rand,0,range);
      fake_obj->y_values[sample_count] = g_rand_int_range(y_rand,0,range);

      /*  write_to_file( fake_obj->x_values[sample_count],fake_obj->y_values[sample_count]); */
      /* g_print("x %f, y %f \n",fake_obj->x_values[sample_count],fake_obj->y_values[sample_count]); */
      g_free(x_rand);
      g_free(y_rand);
      g_free(freq_rand);
      sample_count++;

    }
  process_data(fake_obj);
  return TRUE;
}

void process_data(fakeObject *fake_obj)
{
  gdouble covariance = 0.0;

  if (is_shaking(fake_obj, &covariance)) {
    gchar *message = g_strdup_printf("%g", covariance);
    g_print("%s\n", message);
    write_message(fake_obj, message);
    g_free(message); 
  }  
}

void write_to_file(int x, int y)
{
  gchar *val1;
  gchar *val2;
  gchar *val;
  gint fd;
  gint ret;	

  val1 = g_strdup_printf ("%d",x);
  val2 = g_strdup_printf ("%d",y);
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
}
