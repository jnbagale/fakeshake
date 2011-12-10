INCLUDES = -Wall $(GLIB_CFLAGS) $(GTHREAD_CFLAGS) $(UUID_CFLAGS) $(GSL_CFLAGS) $(LIBZMQ_CFLAGS)

bin_PROGRAMS = fakeshake

fakeshake_SOURCES =  main.c fake.c fake.h fakeaccel.c fakeaccel.h  publisher.c publisher.h config.h

fakeshake_LDADD = $(GLIB_LIBS) $(GTHREAD_LIBS) $(UUID_LIBS) $(GSL_LIBS) $(LIBZMQ_LIBS)

fakeshake_DATA =  

fakeshakedir =