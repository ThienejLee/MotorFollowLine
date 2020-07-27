#ifndef kmotor_h
#define kmotor_H
#include <Arduino.h>
class kmotor
{
  public:
   kmotor(bool msg);
  void cauhinh();
  void tien(int a,int b);
  void run(int a,int b);
  void stop();
  private:
  bool _msg;
};
#endif
