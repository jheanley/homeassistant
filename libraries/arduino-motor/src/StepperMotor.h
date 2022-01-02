#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include "common.h"

class StepperMotor
{
public:
    enum class Direction
    {
        Clockwise,
        AntiClockwise
    };

    enum class State
    {
        Idle,
        Running
    };

public:
    StepperMotor();

    void setPins( int directionPin, int stepPin, int enablePin, int sleepPin = -1, int resetPin = -1, int m0Pin = -1, int m1Pin = -1, int m2Pin = -1 );

    Direction direction() const { return m_direction; }
    void setDirection( Direction direction );

    void setMaxSpeed( int stepsPerSecond );

    void move( uint32_t steps, Direction direction );

    void step();

    bool run();

    void stop();

    //void calculateSpeed();

    /// The distance from the current position to the target position.
    /// \return the distance from the current position to the target position
    /// in steps. Positive is clockwise from the current position.
    //long distanceToGo();

    int run( float elapsed );

private:
    int m_pinDirection = -1;
    int m_pinStep = -1;
    int m_pinEnable = -1;
    int m_pinSleep = -1;
    int m_pinReset = -1;
    int m_pinM0 = -1;
    int m_pinM1 = -1;
    int m_pinM2 = -1;
    int m_acceleration = 1;
    float m_speed = 0.0f;
    int m_maxStepsPerSecond = 100;

    unsigned int m_minPulseWidth = 100;
    uint32_t m_stepsToMove = 0;
    uint32_t m_currentPosition = 0;
    Direction m_direction;
};

#endif // STEPPERMOTOR_H
