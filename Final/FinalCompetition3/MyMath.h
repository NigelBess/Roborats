#ifndef MyMath_H
#define MyMath_H
class MyMath
{
  public:
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
  static float saturate(float var, float maxValue)
  {
    return clamp(var,-abs(maxValue),abs(maxValue));
  }
};
#endif
