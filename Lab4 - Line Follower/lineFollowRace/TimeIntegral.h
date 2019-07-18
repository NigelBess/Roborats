#ifndef TimeIntegral_H
#define TimeIntegral_H
//this class stores error values in an array
class TimeIntegral
{
  private:
  float *values;//error values
  int index = 0;//current index in array
  int size;//size of array
  public:
  TimeIntegral(int quantity)
  {
    values = new float[quantity];//allocate enough memory for the quantity of values
    size = quantity;//keep track of size of array
    index = 0;//start and index zero
    reset();//fill the array with zeros
  }
  void add(float value)
  {
    //stores another value in the array
    values[index] = value;//fill in the next value
    index ++;//move index to the next spot
    index %= size;//if index overflowed, cycle back around the array
  }
  float calculate()
  {
    //sums all values in array
    float outVar = 0;//initalize output
    for(int i = 0; i < size; i++)
    {
      //add all values in the array.
      outVar += values[i];
    }
    return outVar;
  }
  
  void reset()
  {
    //set all values to zero.
    for(int i = 0; i < size; i++)
    {
      values[i] = 0.0;
    }
  }
};


#endif
