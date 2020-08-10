#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"

#include <string.h>

#define STATE_STRING_LENGTH 5

class Light
{
  enum State
  {
    On,
    Off
  };
  
public:
  Light();
  virtual ~Light();
  
  State state() const { return m_state; }
  void setState( State state );
  void setState( const std::string& state );

  std::string getStateString() const { return m_stateStr; }
  
private:
  State m_state;
  std::string m_stateStr;
  char m_stateString[STATE_STRING_LENGTH];
};

#endif
