#ifndef FAKE_SPREAD_H_
#define FAKE_SPREAD_H_

#include "fake.h"
#include "config.h"

fakeObject *connect_spread (fakeObject *fake_obj,  gint mbox_counter);
fakeObject *join_spread(fakeObject *fake_obj,  gint mbox_counter);

#endif /*FAKE_SPREAD_H_*/
