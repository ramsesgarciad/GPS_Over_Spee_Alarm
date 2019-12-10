
#ifndef _SERVER_OF_PAGE_h_
#define _SERVER_OF_PAGE_h_
#include "Arduino.h"



  void server_pages();
  String processor(const String& var);

String configuredPage(unsigned long speed_limited , String name_config, String pass_config );



#endif
