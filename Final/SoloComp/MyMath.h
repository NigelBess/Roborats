#ifndef MyMath_H
#define MyMath_H
//static class for math functions

class MyMath
{
  public:
  //gets sign of float as an integer
  static float sign(float var)
  {
    if (var == 0)
    {
      return 0;
    }
    if (var<0)
    {
      return -1;
    }
    return 1;
  }
  //clamps var between two values
  static float clamp(float var, float minVal, float maxVal)
  {
    if (var<minVal)
    {
      return minVal;
    }
    if (var>maxVal)
    {
      return maxVal;
    }
    return var;
  }

  //saturates var to a max absolute value
  static float saturate(float var, float maxValue)
  {
    return clamp(var,-abs(maxValue),abs(maxValue));
  }
};
#endif
