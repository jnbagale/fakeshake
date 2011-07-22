/*
 * The Spread Toolkit.
 *     
 * The contents of this file are subject to the Spread Open-Source
 * License, Version 1.0 (the ``License''); you may not use
 * this file except in compliance with the License.  You may obtain a
 * copy of the License at:
 *
 * http://www.spread.org/license/
 *
 * or in the file ``license.txt'' found in this distribution.
 *
 * Software distributed under the License is distributed on an AS IS basis, 
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License 
 * for the specific language governing rights and limitations under the 
 * License.
 *
 * The Creators of Spread are:
 *  Yair Amir, Michal Miskin-Amir, Jonathan Stanton, John Schultz.
 *
 *  Copyright (C) 1993-2009 Spread Concepts LLC <info@spreadconcepts.com>
 *
 *  All Rights Reserved.
 *
 * Major Contributor(s):
 * ---------------
 *    Ryan Caudy           rcaudy@gmail.com - contributions to process groups.
 *    Claudiu Danilov      claudiu@acm.org - scalable wide area support.
 *    Cristina Nita-Rotaru crisn@cs.purdue.edu - group communication security.
 *    Theo Schlossnagle    jesus@omniti.com - Perl, autoconf, old skiplist.
 *    Dan Schoenblum       dansch@cnds.jhu.edu - Java interface.
 *
 */



#include <sp.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "fake.h"
#include "comms.h"
#include "config.h"


gboolean get_network_info(fakeObject *fake_obj)
{
  if(SP_poll(fake_obj->mbox)>0)
    read_message(fake_obj);

  return TRUE;
}


gint read_message(fakeObject *fake_obj)
{

  static char mess[102400];
  char sender[MAX_GROUP_NAME];
  char target_groups[100][MAX_GROUP_NAME];
  int num_groups;
  membership_info memb_info;
  int service_type;
  short mess_type;
  int endian_mismatch;
  int i;
  int ret;

//  printf("\n============================\n");
  service_type = 0;
  ret = SP_receive(fake_obj->mbox, &service_type, sender, 100, &num_groups, target_groups, 
                    &mess_type, &endian_mismatch, sizeof(mess), mess);
  if( ret < 0 ) 
  {
    SP_error( ret );
    exit(0);
  }

  if( Is_regular_mess( service_type ) )
  {
    /* A regular message, sent by one of the processes */
    mess[ret] = 0;
//    if     ( Is_unreliable_mess( service_type ) ) printf("received UNRELIABLE ");
//    else if( Is_reliable_mess(   service_type ) ) printf("received RELIABLE ");
//    else if( Is_fifo_mess(       service_type ) ) printf("received FIFO ");
//    else if( Is_causal_mess(     service_type ) ) printf("received CAUSAL ");
//    else if( Is_agreed_mess(     service_type ) ) printf("received AGREED ");
//    else if( Is_safe_mess(       service_type ) ) printf("received SAFE ");
//    printf("message from %s of type %d (endian %d), to %d groups \n(%d bytes): %s\n",
//      sender, mess_type, endian_mismatch, num_groups, ret, mess );
      

  } else if( Is_membership_mess( service_type ) ){
    /* A membership notification */
                ret = SP_get_memb_info( mess, service_type, &memb_info );
                if (ret < 0) {
                        printf("BUG: membership message does not have valid body\n");
                        SP_error( ret );
                        exit( 1 );
                }
    if     ( Is_reg_memb_mess( service_type ) )
    {
      printf("received REGULAR membership ");
      if( Is_caused_join_mess( service_type ) ) printf("caused by JOIN of %s ",memb_info.changed_member);
      if( Is_caused_leave_mess( service_type ) ) printf("caused by LEAVE of %s",memb_info.changed_member);
      if( Is_caused_disconnect_mess( service_type ) ) {
	printf("caused by DISCONNECT of %s",memb_info.changed_member);
      }
      printf("for group %s with %d members:\n",
        sender, num_groups );
      for( i=0; i < num_groups; i++ )
        printf("\t%s\n", &target_groups[i][0] );
      printf("grp id is %d %d %d\n",memb_info.gid.id[0], memb_info.gid.id[1], memb_info.gid.id[2] );
    }else if( Is_transition_mess(   service_type ) ) {
      printf("received TRANSITIONAL membership for group %s\n", sender );
    }else if( Is_caused_leave_mess( service_type ) ){
      printf("received membership message that left group %s\n", sender );
    }else printf("received incorrect membership message of type %d\n", service_type );
  }else printf("received message of unknown message type %d with %d bytes\n", service_type, ret);
  return( service_type );
}


gint write_message(fakeObject *fake_obj, gchar *msg)
{
  gint ret;
  guint msg_len;
   
  msg_len = strlen(msg);
 
  ret = SP_multicast(fake_obj->mbox, UNRELIABLE_MESS, fake_obj->group_name, 1, msg_len, msg);
  
  return(ret);  
  
}
