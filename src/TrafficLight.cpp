#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}


void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
    
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    
    std::srand(std::time(nullptr));
    int duration_ms = 4000 + std::rand()%2001;
    std::chrono::time_point<std::chrono::high_resolution_clock> last = std::chrono::high_resolution_clock::now(); 
    while (true)
    {
        //Check if it is time to change the light
        auto elapsed_time = std::chrono::high_resolution_clock::now() - last; 
        if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() >= duration_ms){
            
            //lock resource so light can be fliiped to opposit state safely
            std::unique_lock<std::mutex> lock(_mutex);

            //Toggle light 
            switch (_currentPhase)
            {
            case TrafficLightPhase::red:
                _currentPhase = TrafficLightPhase::green;
                //message to queue ? or after switch case ie send for every change or only when back to green?
                //_condition.notify_one();
                break;
            case TrafficLightPhase::green:
                _currentPhase = TrafficLightPhase::red;
                //message to queue ? or after switch case ie send for every change or only when back to green?
                //_condition.notify_one();
                break;
            }

            //unlock now light is toggled
            lock.unlock();

            // restart timer and pull new random timer duration
            duration_ms = 4000 + std::rand()%2001;
            last = std::chrono::high_resolution_clock::now();
        }
        //sleep as to not needlessly max out processor
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
}

