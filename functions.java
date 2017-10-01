//This file contains the neccesary functions for movement etc...
//FILE NOT READY TO COMPILE!

public class functions
{
    /*This class is used to make NetBeans stop nagging - when copying to
     *Arduino IDE, only copy the functions.
     */
    
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
        
        float dir = direction / 100 * pi;
        // convert to radians
        
        
        
    }
}
