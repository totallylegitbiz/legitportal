#ifndef INPUT_STATE
struct InputState
{
  int BUTTON_VALUE[2];
  int PADDLE_VALUE[2];
  int MS_PASSED;
  int LAST_LOOP;
  int MS_ELAPSED;
};
#define INPUT_STATE
#endif