#ifndef RangeSensor_H
#define RangeSensor_H
class RangeSensor: public GameObject
{
  protected:
    uint8_t pin;
    int calibrationData[12][2]= {{0,200},{94,70},{110,60},{126,50},{149,40},{188,30},{268,20},{305,17},{341,15},{414,12},{487,10},{638,7}};
    float interpolateFromCalibration(int reading)
  {
    //Interpolate analog reading to a distance, referencing calibration data table

    //n = number of data points in table
    int n = sizeof(calibrationData)/sizeof(calibrationData[0]);
    for (int i = 0;i<n;i++)
    {
      //calibration table is sorted in ascending order by reading (column 0)

      //if the reading value of the next data point is higher than the current reading
      //then we have found the data points to interpolate between
      if (calibrationData[i][0]>=reading)
      {
        if (i==0) return float(calibrationData[0][1]); 
        //solve for m and b in line defined by y = mx+b (y = distance, x = reading)
        float m = float((calibrationData[i][1]-calibrationData[i-1][1]))/float(calibrationData[i][0]-calibrationData[i-1][0]);
        float b = float(calibrationData[i][1])-float(calibrationData[i][0])*m;

        //solve for y (distance)
        return (m*float(reading)+b);
      }
    }
    //if we didn't find two values to interpolate between, just return the max value on the table
    return float(calibrationData[n][1]);
  }
  public:
  RangeSensor(int pinNum)
  {
    pin = pinNum;
  }
  void Awake() override
  {
    pinMode(pin,INPUT);
  }
  float getDistance()
  {
    //uses calibration table to interpolate from the analog signal
    float d = interpolateFromCalibration(analogRead(pin));
    return d;
  }
  
};
#endif
