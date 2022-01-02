#include "StepperMotor.h"

#include "common.h"

StepperMotor::StepperMotor()
{

}

void StepperMotor::setPins(int directionPin, int stepPin, int enablePin, int sleepPin, int resetPin, int m0Pin, int m1Pin, int m2Pin)
{
    m_pinDirection = directionPin;
    m_pinStep = stepPin;
    m_pinEnable = enablePin;
    m_pinSleep = sleepPin;
    m_pinReset = resetPin;
    m_pinM0 = m0Pin;
    m_pinM1 = m1Pin;
    m_pinM2 = m2Pin;

    pinMode(m_pinDirection, OUTPUT);
    pinMode(m_pinStep, OUTPUT);
    pinMode(m_pinEnable, OUTPUT);
    digitalWrite(m_pinEnable, HIGH);
}

void StepperMotor::setDirection(Direction direction)
{
    if( direction == Direction::Clockwise )
    {
        digitalWrite(m_pinDirection, HIGH);
    }
    else
    {
        digitalWrite(m_pinDirection, LOW);
    }
    m_direction = direction;
}

void StepperMotor::setMaxSpeed(int stepsPerSecond)
{
    m_maxStepsPerSecond = stepsPerSecond;
}

void StepperMotor::move(uint32_t steps, Direction direction)
{
    Serial.println("StepperMotor::move()");
    stop();
    setDirection(direction);
    m_currentPosition = 0;
    m_stepsToMove = steps;

    // enable the motor
    digitalWrite(m_pinEnable, LOW);
}

void StepperMotor::step()
{
    digitalWrite(m_pinStep, HIGH);
    delayMicroseconds(m_minPulseWidth);
    digitalWrite(m_pinStep, LOW);
}

void StepperMotor::stop()
{
    Serial.println("StepperMotor::stop()");
    m_stepsToMove = m_currentPosition;
    digitalWrite(m_pinEnable, HIGH);
}

//bool StepperMotor::run()
//{
//    // Dont do anything unless we actually have a step interval
//    if (!m_stepInterval)
//    return false;

//    unsigned long time = micros();
//    if (time - m_lastStepTime >= m_stepInterval)
//    {
//        ++m_currentPosition;
//        step(_currentPos);

//        _lastStepTime = time; // Caution: does not account for costs in step()

//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

int StepperMotor::run(float elapsed)
{
    int stepsLeft(m_stepsToMove - m_currentPosition);
    if( stepsLeft > 0 )
    {
        int steps( m_maxStepsPerSecond * m_speed * elapsed );
        uint32_t delayUs(fmax((1000000 / steps) - 1000, 3000));
        steps = fmax(steps, stepsLeft);
        int currentStep(steps);
        while( currentStep != 0 )
        {
            step();
            delayMicroseconds(delayUs);
            --currentStep;
        }
        m_currentPosition += steps;
        if( m_currentPosition >= m_stepsToMove )
        {
            stop();
        }
    }
    return 0;
}


//void StepperMotor::calculateSpeed()
//{
//    long distanceTo = distanceToGo(); // +ve is clockwise from curent location

//    long stepsToStop = (long)((m_speed * m_speed) / (2.0 * m_acceleration)); // Equation 16

//    if (distanceTo == 0 && stepsToStop <= 1)
//    {
//        // We are at the target and its time to stop
//        m_stepInterval = 0;
//        m_speed = 0.0;
//        _n = 0;
//        return;
//    }

//    if (distanceTo > 0)
//    {
//        // We are anticlockwise from the target
//        // Need to go clockwise from here, maybe decelerate now
//        if (_n > 0)
//        {
//            // Currently accelerating, need to decel now? Or maybe going the wrong way?
//            if ((stepsToStop >= distanceTo) || m_direction == Direction::Clockwise)
//            {
//                _n = -stepsToStop; // Start deceleration
//            }
//        }
//        else if (_n < 0)
//        {
//            // Currently decelerating, need to accel again?
//            if ((stepsToStop < distanceTo) && _direction == DIRECTION_CW)
//            {
//                _n = -_n; // Start accceleration
//            }
//        }
//    }

//    // Need to accelerate or decelerate
//    if (_n == 0)
//    {
//    // First step from stopped
//    _cn = _c0;
//    _direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
//    }
//    else
//    {
//    // Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
//    _cn = _cn - ((2.0 * _cn) / ((4.0 * _n) + 1)); // Equation 13
//    _cn = max(_cn, _cmin);
//    }
//    _n++;
//    _stepInterval = _cn;
//    _speed = 1000000.0 / _cn;
//    if (_direction == DIRECTION_CCW)
//    _speed = -_speed;

//#if 0
//    Serial.println(_speed);
//    Serial.println(_acceleration);
//    Serial.println(_cn);
//    Serial.println(_c0);
//    Serial.println(_n);
//    Serial.println(_stepInterval);
//    Serial.println(distanceTo);
//    Serial.println(stepsToStop);
//    Serial.println("-----");
//#endif
//}

//void StepperMotor::setOutputPins(uint8_t mask)
//{
//    uint8_t numpins = 4;
//    uint8_t i;
//    for (i = 0; i < numpins; i++)
//    {
//        digitalWrite(_pin[i], (mask & (1 << i)) ? (HIGH ^ _pinInverted[i]) : (LOW ^ _pinInverted[i]));
//    }
//}

//long StepperMotor::distanceToGo()
//{
//    return m_stepsToMove - m_currentPosition;
//}
