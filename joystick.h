#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "mbed.h"

//Netid-it233, mr2265
class Joystick
{
public:
    /** Over here we create a Joystick object connected to the given pins
     addiitonally we dynamically allocates AnalogIn for input potentiometers, 
     InterruptIn for the joystick buton and the debounce Timeout
    
     x_axis_pin connected to the Joystick's x potentiometer output
     y_axis_pin connected to the Joystick's y potentiometer output
     button_pin connected to the Joystick's button
    
    */
    Joystick(PinName x_axis_pin, PinName y_axis_pin, PinName button_pin);

    /** Destroys the Joystick object
     Clears the AnalogIn's, InterruptIn and Timeout from memory
    */
    ~Joystick(); //Destructor

    /** Initalises the Joystick
     Sets up the InterruptIn Mode ISR
     Initalizes the offset vairables and ISR flags
     And then Samples the AnalogIn's 5 times and takes an average to get the offset
    */
    void init();

    /** responsible for getting the value of the x potentiometer
     and will then take 5 readings from the potentiometer 
     and calculates the average measurement, accounting for joystick offset
     and then Caps the average between 0 and 1
     
     It then the average value of the x potentiometer
    */
    float GetXValue();

    /** responsible for getting the value of the y potentiometer
     and will then take 5 readings from the potentiometer 
     Then calculates the average measurement, accounting for joystick offset
    Caps the average between 0 and 1
 
     It then the average value of the y potentiometer
    */
    float GetYValue();//In the end we did not end up using this function as the motion
                        //was controlled by X-Axis value of Joystick

    /** Reads the state of the button flag
    * It then returns the button flag
    */
    int get_button_flag();

    /** This will Set the button flag
   
    */
    void set_button_flag(bool value);

private:
    void button_isr();
    void button_fall_isr();
    void button_debounce_isr();
private:
    //Pin inputs
    AnalogIn* x_axis_;
    AnalogIn* y_axis_;
    InterruptIn* button_;

    //Ticker to prevent joystick button to bounce
    Timeout* button_debounce_;

    //Stores X and Y offsets
    float x_offset_;
    float y_offset_;

    //Stores interrupt flags
    volatile bool g_button_flag_;
    volatile bool g_button_debounce_flag_;
};

#endif