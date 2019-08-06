#ifndef SideMover_H
#define SideMover_H

//the side mover deals with all of the motion control of the robot. It is a state machine defining the algorithm for positioning the robot
//under the cheese, and getting it back to the wall to place the cheese

#include "SideMover.h"
#include "DCMotor.h"
#include "LineSensor.h"

#define estimatedWallFollowDistanceToCheese  1.32//meters
#define estimatedReturnDistance  1.2//meters
#define distanceAfterLine 0.18//meters
#define cheeseTurnAmount 160.0//degrees
#define finalTurnAmount 66.0//degrees
#define distanceToWallAfterTurn 0.6//meters

//the left side of the board is a little bit darker than the right. We account for this with out line sensor thresholds
#define leftLineSenseThreshold 600
#define rightLineSenseThreshold 500

//left and right range sensors were not identical. Taarget distances for wall following were adjusted appropriately
#define rightTargetDistance 33.0//cm
#define leftTargetDistance 29.0//cm


#define returnTargetDistance 40.0//meters
#define backUpDistance 0.55//cm
#define maxBackUpTime 4000//ms
#define  backUpFromWallDistance 0.13//meters
#define  driveToWallTime 1400//ms
#define  maxUnstuckTime 1000//ms


#define right 1
#define left -1

    
class SideMover : public GameObject
{
  protected :
  
    //used for anything that needs to store millis()  values
    unsigned long int timer = 0;
   
    //side to follow
    int side = right;

    //references to hardware control objects
    DCMotor* rightMotor;
    DCMotor* leftMotor;
    RangeSensor* rightRangeSensor;
    RangeSensor* leftRangeSensor;
    FullDistanceController* distControl;
    WallFollower* wallFollower;
    LineSensor* rightLineSensor;
    LineSensor* leftLineSensor;

    //state for state machine
    enum State
    {
      firstWallFollow,
      turningBack,
      returnFollow,
      turningToWall,
      waiting,
      goingSetDistance,
      gettingUnstuck,
      drivingToWall,
      backingUpFromCheese,
      backingUpFromWall,
    };
    State state = waiting;
      
  public:
  
  SideMover(DCMotor* rightM, DCMotor* leftM, RangeSensor* rrs, RangeSensor* lrs, FullDistanceController* dc, WallFollower* wf,LineSensor* rls,LineSensor* lls)
  {
    rightMotor = rightM;
    leftMotor = leftM;
    distControl = dc;
    wallFollower = wf;
    rightRangeSensor = rrs;
    leftRangeSensor = lrs;
    rightLineSensor = rls;
    leftLineSensor = lls;
  }

  
  void Update(int dt) override
  {
    switch (state)
    {
      case firstWallFollow:
        if ((*distControl).hasArrived() && ((*leftLineSensor).isOverLine() || (*rightLineSensor).isOverLine()))//have we gone far enough, and then reached a line?
        {
          go(distanceAfterLine);//go until we expect to be under the cheese
          state = goingSetDistance;
        }
         break;
     case goingSetDistance:
      if((*distControl).hasArrived())//have we gone far enough?
      {
        //should be under the cheese now. We have arrived, so just wait for new commands
          halt();
          state = waiting;
      }
      break;
      case backingUpFromCheese:
     if((*distControl).hasArrived() || (millis()- timer)>maxBackUpTime)//have we either gone far enough, or tried long enough?
     {
      //we have now backed up enough. Turn around
      turn(cheeseTurnAmount,left);
      state = turningBack;
     }
      break;
     case turningBack:
        if ((*distControl).hasArrived())//have we turned all the way around?
        {
          side = -side;//follow the opposite wall side
          (*wallFollower).setDistance(returnTargetDistance);// set approprate distance at which to follow wall
          followWallForDistance(side,estimatedReturnDistance);//follow the wall until we expect to be back home
          state = returnFollow;
        }
        break;
     case returnFollow:
       if((*distControl).hasArrived())//are we back at the start?
       {
         turn(finalTurnAmount,side);//turn to face the wall
         state = turningToWall;
       }
       break;
     case turningToWall:
        if((*distControl).hasArrived())//have we turned to face the wall?
         {
            go(distanceToWallAfterTurn);//drive forward into wall
           timer = millis();//start a timer
            state = drivingToWall;
         }
         break;
     case drivingToWall:
        if((*distControl).hasArrived() || (millis()-timer)>driveToWallTime)//have we traveled far enough or tried long enough?
         {
            go(-backUpFromWallDistance);//back up from the wall to the cheese gets placed appropriately
            state = backingUpFromWall;
         }
         break;
     case backingUpFromWall:
     if((*distControl).hasArrived())//have we backed up enough?
     {
          //we have now gotter to a place where the cheese can be dropped onto the wall 
            halt();
            state = waiting;
     }
      break;
    }
  }
  void setSide(int newSide)
  {

    //sets approprate variables depending on which side we are following the wall on
    side = newSide;
    if (side == 0)
    {
      side = right;
    }

    //get approprate line sensor sensitivity and range sensor distance
    short int sense;
    float dist;
    if (side == left)
    {
      sense = leftLineSenseThreshold;
      dist = leftTargetDistance;
    } else
    {
      sense = rightLineSenseThreshold;
      dist = rightTargetDistance;
    }
    (*rightLineSensor).setSensitivity(sense);
    (*leftLineSensor).setSensitivity(sense);
    (*wallFollower).setDistance(dist);
  }



  
  void FollowWall(int direction)
  { 
    //makes the robot follow the wall
    (*wallFollower).enabled = true;//turn on wall follower
    (*wallFollower).allowFullVelocityControl(true);//give wall follower control of both average speed and difference in motor speed
    (*distControl).setEnabled(true);//turn on the distance controllers (to keep track of distance traveled)
    (*distControl).applyControl(false);//dont give the distance controllers control of the motors

    //pick the appropriate range sensor
    RangeSensor* sense = rightRangeSensor;
    if (direction<0)  
    {
      sense = leftRangeSensor;
    }

    //follow the wall using appropriate sensor
    (*wallFollower).setSensor(sense);
    (*wallFollower).setDirection(direction);
  }
  
  void go(float distance)
  {
    //makes robot travel forward by the specified distance in meters
    //backward if distance is negative
    
     setDelta(0);// makes sure motors move at the same speed

     //turn on distance controllers
     (*distControl).setEnabled(true);
     (*distControl).applyControl(true);

     //turn off the wall follower
    (*wallFollower).enabled = false;

    //tell the distance controllers to go the defined distance
    (*distControl).go(distance);
  }
  void turn(float degrees,int direction)
  {
    //turns robot by the specified number of degrees in the specified direction (1 or -1)
    //shouldve just used the sign of degrees, in hindsight
    halt();//reset motor speeds
    
    //turn on distance controllers, and give them control of motors
    (*distControl).setEnabled(true);
    (*distControl).applyControl(true);

    //move the motors the desired amount
    (*distControl).turn(degrees,direction);
  }
  void turn(float degrees)
  {
    //by default turn toward the side we are on
    turn(degrees,side);
  }

  void followWallForDistance(int direction,float distance)
  {
    //follows wall for specified distanc
    (*distControl).go(distance);
    FollowWall(direction);
  }
  void goToCheese()
  {
    //starts path from start light to golden cheese
    setSide(side);
    state = firstWallFollow;
    followWallForDistance(side,estimatedWallFollowDistanceToCheese);
  }
   void returnFromCheese()
  {
    //starts path from golden cheese back home
    state = backingUpFromCheese;
    timer = millis();
    go(-backUpDistance);
    
  }
  void halt()
  {
    //stops all motion

    // off objects that control motors
    (*wallFollower).enabled = false;
    (*distControl).setEnabled(false);

    //set motor speeds to zero
    setVel(0);
    setDelta(0);
  }
   void setVel(int vel)
  {
    //sets nominal velocity of both motors with infinite acceleration
    (*rightMotor).setVel(vel);
    (*leftMotor).setVel(vel);
  }
  void setDelta(int delta)
  {
    //sets the delta in speed of the motors
    (*rightMotor).setDelta(delta/2);
    (*leftMotor).setDelta(-delta/2);
  }
  bool isWaiting()
  {
    //are we at the end of a path of travel?
    return (state==waiting);
  }
};
#endif
