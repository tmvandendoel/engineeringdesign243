//This file contains the neccesary functions for movement etc...
//FILE NOT READY TO COMPILE!


void move(int direction, int speed)
{
    /* int direction: integer between -100 and 100 indicating the
     * direction to be taken. 0 means straight forward.
 
     * int speed: integer between 0 and 100 indicating how fast the ROD
     * should be moving. 100 represents the fasted speed possible in the
     * GIVEN direction! Due to wheel position, the fastest speed might
     * differ in different directions.

     * NOTE: this function uses trigonometric calculations. These might
     * slow down the reaction speed of the Arduino or consume too much
     * memory. In both cases a lookuptable will be created.
     */
        
    int wheelLeft = 0;
    int wheelRight = 0;
    int wheelBack = 0;
    // at the end, these vars contain the power sent to each motor

    float cons = pi / 6.0;
    float dir = direction / 100 * pi;
    // convert to radians
        
    wheelLeft = (int) 100*sin(dir - cons);
    wheelRight = (int) 100*sin(dir + cons);
    wheelBack = (int) 100*sin(dir);
    //calculate relative powers
    

    int largest = wheelLeft;
    if(wheelRight > largest)
    {
      largest = wheelRight;
    }
    if(wheelBack > largest)
    {
      largest = wheelBack;
    }
    //calculate largest value of the three wheels

    wheelRight = (int) wheelRight / largest * 100;
    wheelLeft = (int) wheelLeft / largest * 100;
    wheelBack = (int) wheelBack / largest * 100;
    //make the largest power become 100, scale the others appropriately
}
