#ifndef hidef.h
#define hidef.h      /* common defines and macros */
#endif

#ifndef derivative.h
#define derivative.h /* derivative-specific definitions */
#endif

#ifndef main_asm.h
#define main_asm.h  /* interface to the assembly module */
#endif

#ifndef maze.h
#define maze.h
#endif

unsigned short readFrontSensor(void)
{
   return  ATD1DR0L;
}

unsigned short readRightWheel(void)
{
   return  ATD1DR4L;
}

unsigned short readRightWall(void)
{
   return  ATD1DR3L;
}

unsigned short readLeftWheel(void)
{
   return  ATD1DR2L;
}

unsigned short readLeftWall(void){
   return  ATD1DR1L;
}

void showSensors(void)
{
   printf("FRONT SENSOR==%X\n\r",ATD1DR0L);
   printf("RIGHT WHEEL SENSOR==%X   RIGHT WALL SENSOR==%X\n\r",ATD1DR4L,ATD1DR3L);
   printf("LEFT WHEEL SENSOR==%X    LEFT WALL SENSOR==%X\n\r",ATD1DR2L,ATD1DR1L);
}                                                       
