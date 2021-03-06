/** 
 *  @file PWMmotors.h
 *  @brief Motors interface.
 *  @author Madzivire Chikwanda
 *  @date 22 April 2015
 *
 *  Methods for controling motors. The TIMER module has to be initialized by calling
 *  init() , defined in init.h, before methods defined in this file can be called.
 *
 *  The left motor is interfaced with PORT B, PWM channel 4, & left wheel encoder is
 *  interfaced to Pulse Accummulator A (PACN32).
 *  The right motor is interfaced with PORT A, PWM channel 5, & left wheel encoder is
 *  interfaced to Pulse Accummulator B (PACN10).
 *  
 */



#define nextSquare  28      // #13 of steps to next cell 28
#define _90DEG      6       // # of steps for 90 degree turn
#define _180DEG     12      // # of steps for U turn
#define SpdAdjCnst  1
#define TurnSpd     25
#define MedSpd      28
#define MaxPWMval   200

unsigned short rightWheelCount, leftWheelCount;


/**
 *  @brief Reset pulse accummulators & PWM count registers
 *  @param void
 *  @return void
 */
void resetCounters (void)
{
   PACN10=0;
   PACN32=0;
   PWMCNT4=0;
   PWMCNT5=0; 
}

/** 
 *  @brief Set motors direction to forward.
 *  @param void
 *  @return void
 */
void fwdDir (void)
{
   PORTA=0;
   PORTB=0;
}

/** 
 *  @brief Set motors direction to reverse.
 *  @param void
 *  @return void 
 */
void reverseDir (void)
{
   PORTA=1;
   PORTB=0x80;
}

/** 
 *  @brief Set motors direction to left.
 *  @param void
 *  @return void
 */
void leftDir (void)
{
   PORTA=0;
   PORTB=0x80;  
}

/** 
 *  @brief Set motors direction to right.
 *  @param void
 *  @return void 
 */
void rightDir (void)
{
   PORTA=1;
   PORTB=0;  
}

/** 
 *  @brief Set motors speed to turning speed.
 *  @param void
 *  @return void
 * 
 */
void turnSpeed (void)
{
   PWMDTY4=TurnSpd;                      
   PWMDTY5=TurnSpd;  
}

/** 
 *  @brief Set motors speed to medium.
 *  @param void
 *  @return void
 * 
 */
void medSpeed (void)
{
   PWMDTY5=MedSpd;
   PWMDTY4=MedSpd;  
}

/** 
 *  @brief Set motors speed to idle.
 *  @param void
 *  @return void
 * 
 */
void idleSpeed (void)
{
   PWMDTY4=5;
   PWMDTY5=5;  
}

/** 
 *  @brief Increase right motor speed & decrease left motor speed.
 *  @param void
 *  @return void
 * 
 */
void adjLeft (void)
{
   if(PWMDTY4 < (MaxPWMval - SpdAdjCnst))
      PWMDTY4 = PWMDTY4 + SpdAdjCnst;

   if(PWMDTY5 > SpdAdjCnst)
      PWMDTY5 = PWMDTY5 - SpdAdjCnst;
}

/** 
 *  @brief Increase left motor speed & decrease right motor speed.
 *  @param void
 *  @return void
 * 
 */
void adjRight (void)
{
   if(PWMDTY4 > SpdAdjCnst)
      PWMDTY4 = PWMDTY4 - SpdAdjCnst;

   if(PWMDTY5 < (MaxPWMval - SpdAdjCnst))
      PWMDTY5 = PWMDTY5 + SpdAdjCnst;
}


void showRegisters (void)
{
   printf("pwmDTY5==%i  pwmDTY5==%i\n\r",(unsigned int)PWMDTY5,(unsigned int)PWMDTY4);
   printf("pacn32=%i  pacn10=%i\n\r\n\r",(unsigned int)PACN32,(unsigned int)PACN10);  
}
 
/**
 *  @brief Start foward motion at medium speed on both motors.
 *  @param void
 *  @return void
 */
void fwd (void)
{
   fwdDir();
   medSpeed();
}

/**
 *  @brief Stop back-up motion on both motors.
 *  @param void
 *  @return void
 */
void stopBckp (void)
{
   fwdDir();
   idleSpeed();
}

/**
 *  @brief Stop foward motion on both motors
 *  @param void
 *  @return void
 */
void stopFwd (void)
{
   PORTA=1;
   PORTB=0x80;
   PWMDTY4=5;
   PWMDTY5=5;
}

/** 
 *  @brief Turn right slightly.
 *  @param void
 *  @return void
 *
 *  Left alignment sensor or right wall sensor must be reading OFF.
 * 
 */
void slightRight (void)
{
  stopFwd();
  rightDir();
  turnSpeed();
  
  do {} while( !sensorOn( readLeftAlign() ) || !sensorOn( readRightWall() ));
  leftDir();
  idleSpeed();
  fwd();
}

/** 
 *  @brief Turn left slightly.
 *  @param void
 *  @return void
 * 
 *  Right alignment sensor or left wall sensor must be reading OFF.
 * 
 */
void slightLeft (void)
{
  stopFwd();
  leftDir();
  turnSpeed();
  
  do {} while( !sensorOn( readRightAlign() ) || !sensorOn( readLeftWall( ) )  );
  rightDir();
  idleSpeed();
  fwd();
}

/**
 *  @brief Starts back-up motion on both motors.
 *  @param void
 *  @param void
 */
void bckp (void)
{
   reverseDir();
   PWMDTY4=80;
   PWMDTY5=80;
}

/** 
 *  @brief Uses left or right wall to align mouse movement.
 *  @param void
 *  @return void
 *
 *  This method is supposed to keep the mouse parallel with a wall on either the
 *  left or right side.
 * 
 */
void followWall (void)
{
   
   if(!sensorOn( readRightAlign() ) && sensorOn( readRightWall() ) )
   {
      slightLeft();
      delay(_4mSec);
      return;
   }
   
   if(sensorOn( readRightAlign() ) && !sensorOn( readRightWall() ) )
   {
      slightRight();
      delay(_4mSec);
      return;
   }
      
   if(!sensorOn( readLeftAlign() ) && sensorOn( readLeftWall() ) )
   {
      slightRight();
      delay(_4mSec);
      return;
   }
   
   if(sensorOn( readLeftAlign() ) && !sensorOn( readLeftWall() ) )
   {
      slightLeft();
      delay(_4mSec);
      return;
   }
     
}


/**
 *  @brief Move foward to next cell
 *  @param void
 *  @return void
 */
void nextCellGo (void)
{
   resetCounters();
   fwd();
   while((PACN10 < nextSquare) || (PACN32 < nextSquare))
   {
      followWall();
   }
   stopFwd();

   // update mouse coordinates
   nextCell();
}


/**
 *  @brief 90 degree right turn
 *  @param void
 *  @return void
 */
void rightTurn (void)
{   
   resetCounters();   
   rightDir();   
   turnSpeed();

   while((PACN10 < _90DEG) || (PACN32 < _90DEG))
   {
   }

   leftDir();
   idleSpeed();
   resetCounters();
}

/**
 *  @brief 90 degree left turn
 *  @param void
 *  @return void
 */
void leftTurn (void)
{
   resetCounters();   
   leftDir();   
   turnSpeed();

   while((PACN10 < _90DEG) || (PACN32 < _90DEG))
   {
   }

   rightDir();
   idleSpeed();
   resetCounters();
}

/**
 *  @brief Turn around
 *  @param void
 *  @return void
 */
void turn180 (void)
{
   resetCounters();   
   rightDir();   
   turnSpeed();

   while((PACN10 < _180DEG) || (PACN32 < _180DEG))
   {
   }

   leftDir();
   idleSpeed();
   resetCounters();
}

void testWheelCntrs (void) 
{
   short count;
   count = 5;
   PWMDTY4=50;
   PWMDTY5=50;
   resetCounters();
   while(PACN10 < nextSquare)
   {

      showRegisters();
   }
   stopFwd();
}
