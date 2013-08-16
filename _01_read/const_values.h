// ADDRESSES
typedef unsigned char byte;

const byte dev_addr_0 = 0x1D;
const byte MCTL = 0x16;

// SIGNALS
typedef enum { STANDBY = 0, MEASURE = 1, LEVEL_DETECT = 3, PULSE_DETECT = 2 } MCTL_mode_t;
typedef enum { _8G, _4G, _2G } MCTL_sensitivity_t;
typedef union {
  struct {
    MCTL_mode_t mode : 2;
    MCTL_sensitivity_t glvl : 2;
    byte  ston : 1; // self-test mode on
    byte  drpd : 1; // disable sending data-ready status to INT1 pin
  } v;
  byte b;
} MCTL_value_t;

typedef struct {
  int x : 10;
  int y : 10;
  int z : 10;
} acceleration_3d_t;
