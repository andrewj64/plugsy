#include "serial.h"

// Commands enum
typedef enum cmds {
  eQ,
  eSettings,
  eGetP,
  eSetLighting,
  eSetVLimit,
  eSetHLimit,
  eSetVSpeed,
  eSetHSpeed,
  eSetTracking,
  eRunD,
  eRunU,
  eMoveH,
  eMoveV,
  eEnd,
  eSetZero,
  eMoveS,
  eReset,
  eZeroV,
  eUnknown  
} cmd;

cmd hashit(char* command)
{
	if(command == "?") return eQ;
  if(command == ".") return eSettings;
  if(command == "getP") return eGetP;
  if(command == "setLighting") return eSetLighting;
  if(command == "setVLimit") return eSetVLimit;
  if(command == "setHLimit") return eSetHLimit;
  if(command == "setVSpeed") return eSetVSpeed; 
  if(command == "setHSpeed") return eSetHSpeed; 
  if(command == "setTracking") return eSetTracking;
  if(command == "runD") return eRunD;
  if(command == "runU") return eRunU;
  if(command == "moveH") return eMoveH;
  if(command == "moveV") return eMoveV;
  if(command == "end") return eEnd;
  if(command ==  "setZero") return eSetZero;
  if(command ==  "zeroV") return eZeroV;  // eZeroV and reset do the same thing right now
  if(command ==  "reset") return eReset;
  if(command == "moveS") return eMoveS;
  return eUnknown; // command not found
}