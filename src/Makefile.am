INCLUDES = -Wall $(GLIB_CFLAGS) $(UUID_CFLAGS) $(GSL_CFLAGS)

bin_PROGRAMS = fakeshake

fakeshake_SOURCES =  main.c fakeaccel.c fakeaccel.h fake.c fake.h comms.c comms.h config.h

fakeshake_LDADD = $(GLIB_LIBS) $(UUID_LIBS) $(GSL_LIBS)

fakeshake_DATA =  fakeposition.txt 

fakeshakedir =