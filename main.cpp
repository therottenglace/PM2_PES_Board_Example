#include <mbed.h> //Library von mbed -> Mbed OS Steht zur Verfügung 
 
#include "PM2_Drivers.h" //header File wird inkludiert, beinhaltet Klassen 
 
# define M_PI 3.14159265358979323846 // number pi, an example in case you need it
 
bool do_execute_main_task = false; // this variable will be toggled via the user button (blue button) and decides whether to execute the main task or not
bool do_reset_all_once = false;    // this variable is used to reset certain variables and objects and shows how you can run a code segment only once
 
// objects for user button (blue button) handling on nucleo board
DebounceIn user_button(PC_13);  //PC_13 steht für Pineingang, dieser ist bei uns blauer Knopf/ create InterruptIn interface object to evaluate user button falling and rising edge (no blocking code in ISR)
void user_button_pressed_fcn(); //Funktion, wird lediglich deklariert/ custom functions which get executed when user button gets pressed, definition below
 
/*floar_ir_sensor_compensation (float _ir_distance_mV) {
    static const float = //hier a aus matlab 
    static const float = //hier b aus matlab 
}//das hier muss noch geschrieben werden anhand matlab - sinn der Funktion ist, die mV in Distanz in cm auszugeben 
 
// alles oberhalb ist global / main runs as an own thread*/
int main()
{
    // attach button fall function to user button object, button has a pull-up resistor
    user_button.fall(&user_button_pressed_fcn); //Objekt userButton wird genommen und Funktion fall wird ausgeführt 
 
    // while loop gets executed every main_task_period_ms milliseconds (simple aproach to repeatedly execute main)
    const int main_task_period_ms = 50; // Programm läuft zyklisch alle 50 ms ab / define main task period time in ms e.g. 50 ms -> main task runs 20 times per second
    Timer main_task_timer;              // create Timer object which we use to run the main task every main_task_period_ms
 
    // led on nucleo board
    DigitalOut user_led(LED1);       // LED 1 in Board wird angesprochen / create DigitalOut object to command user led
 
    // additional led
    DigitalOut additional_led(PB_9); // create DigitalOut object to command extra led (you need to add an aditional resistor, e.g. 220...500 Ohm)
 
    // mechanical button
    DigitalIn mechanical_button(PC_5); // create DigitalIn object to evaluate extra mechanical button, you need to specify the mode for proper usage, see below
    mechanical_button.mode(PullUp);    // set pullup mode: sets pullup between pin and 3.3 V, so that there is a defined potential
 
    // Sharp GP2Y0A41SK0F, 4-40 cm IR Sensor
    float ir_distance_mV = 0.0f; // define variable to store measurement
    AnalogIn ir_Analog_in (PC_2);// create AnalogIn object to read in infrared distance sensor, 0...3.3V are mapped to 0...1
 
    main_task_timer.start(); //jedes mal wenn Programm abläuft, wird Timer gestartet, sodass das Programm wieder beginnt 
    
    // this loop will run forever
    while (true) {
 
        main_task_timer.reset();
 
        if (do_execute_main_task) {
 
            if (mechanical_button.read()) {
            ir_distance_mV=1.0e3f* ir_Analog_in.read () *3.3f; //Weil der kontroller den Volt auf 1 skaliert wir ändern dies auf 3.3 V
                // read analog input
            }
 
            // visual feedback that the main task is executed, setting this once would actually be enough
            additional_led = 1;
 
        } else {
 
            if (do_reset_all_once) {
                do_reset_all_once = false;
 
                ir_distance_mV = 0.0f;
 
                additional_led = 0;
            }            
        }
 
        // toggling the user led
        user_led = !user_led;
 
        // do only output via serial what's really necessary, this makes your code slow
        printf("IR sensor (mV): %3.3f\r\n", ir_distance_mV);
 
        // read timer and make the main thread sleep for the remaining time span (non blocking)
        int main_task_elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(main_task_timer.elapsed_time()).count(); //Timer auslesen 
        thread_sleep_for(main_task_period_ms - main_task_elapsed_time_ms);
    }
}
 
void user_button_pressed_fcn()
{
    // do_execute_main_task if the button was pressed
    do_execute_main_task = !do_execute_main_task;
    if (do_execute_main_task) do_reset_all_once = true;
}//vorher deklarierte UserButton Funktion wird definiert




