#include "Blind.h"

Blind::Blind()
{

}

void Blind::setPosition(int position)
{
    Serial.print("Blind::setPosition: ");
    Serial.println(position);
    m_requestedPosition.m_position = (m_endPosition * position) / 100;
    Serial.print("Blind::setPosition: ");
    Serial.println(m_requestedPosition.m_position);

    //int totalSteps
}

std::list<std::string> Blind::getTopics() const
{
    std::list<std::string> topics;
}

bool Blind::nextStateMessage(std::string& topic, std::string& messagePayload)
{
    if( m_sendStates )
    {
        if( m_positionChanged )
        {
            messagePayload = std::to_string(m_currentPosition.m_position);
            topic = m_name;
            topic.append("/state");
            m_positionChanged = false;
            return true;
        }
        if( m_tiltChanged )
        {
            messagePayload = std::to_string(m_currentPosition.m_tilt);
            topic = m_name;
            topic.append("/tilt-state");
            m_tiltChanged = false;
        }
    }
    return false;
}

void Blind::processMessage(const std::string& topic, const std::string& payload)
{
    if( topic.find("config") < topic.size() )
    {
        // some easy ways to update configuration for motor using mqtt from home assistant
        Serial.print("Received config state: ");
        Serial.println(topic.c_str());
        if( topic.find("setPosition") < topic.size() )
        {
            int value = atoi(payload.c_str());
            Serial.print("Set position to: ");
            Serial.println(value);
            m_currentPosition.m_position = m_requestedPosition.m_position = value;
            m_positionChanged = true;
        }
        else if( topic.find("setTilt") < topic.size() )
        {
            int value = atoi(payload.c_str());
            Serial.print("Set tilt to: ");
            Serial.println(value);
            m_currentPosition.m_tilt = m_requestedPosition.m_tilt = value;
            m_tiltChanged = true;
        }
        else if( topic.find("toggleDirection") < topic.size() )
        {
            Serial.println("Toggle opening direction for motor");
            if( m_openDirection == StepperMotor::Direction::Clockwise )
            {
                m_openDirection = StepperMotor::Direction::AntiClockwise;
                m_closeDirection = StepperMotor::Direction::Clockwise;
            }
            else
            {
                m_openDirection = StepperMotor::Direction::Clockwise;
                m_closeDirection = StepperMotor::Direction::AntiClockwise;
            }
        }
        else if( topic.find("movePosition") < topic.size() )
        {
            int value = atoi(payload.c_str());
            Serial.print("Moving position by steps: ");
            Serial.println(value);
            movePosition(value);
        }
        else if( topic.find("moveTilt") < topic.size() )
        {
            int value = atoi(payload.c_str());
            Serial.print("Moving tilt by steps: ");
            Serial.println(value);
            moveTilt(value);
        }
        else if( topic.find("setEndPosition") < topic.size() )
        {
            Serial.println("Setting current position as end position");
            m_endPosition = m_currentPosition.m_position;
        }
        else if( topic.find("setEndTilt") < topic.size() )
        {
            Serial.println("Setting current tilt as end tilt");
            m_tiltSteps = m_currentPosition.m_tilt;
        }
    }
    else
    {
        if( topic.find("set_position") < topic.size() )
        {
            int value = atoi(payload.c_str());
        }
    }
}

void Blind::movePosition(int steps)
{
    StepperMotor::Direction direction(m_openDirection);
    if(steps < 0)
    {
        direction = m_closeDirection;
    }
    m_motor.move(fabs(steps), direction);
}

void Blind::moveTilt(int steps)
{
    m_requestedPosition.m_tilt = fmax(0, m_requestedPosition.m_tilt + steps);
    uint32_t stepsMove(m_requestedPosition.m_tilt - m_currentPosition.m_tilt);
    StepperMotor::Direction direction(m_openDirection);
    if(stepsMove < 0)
    {
        direction = m_closeDirection;
    }
    m_motor.move(fabs(stepsMove), direction);
}

void Blind::update(float elapsed)
{
    int steps = m_motor.run(elapsed);
    if( steps )
    {
        // motor is moving
        if( m_openDirection == m_motor.direction() )
        {

        }
    }
    else
    {
        // motor is not moving
        m_state = State::Idle;
    }

    // triger state updates
    if( !m_sendStates
            && (m_positionChanged
                || m_tiltChanged) )
    {
        m_updateTimer += elapsed;
        if( m_updateTimer > m_stateUpdateInterval )
        {
            m_sendStates = true;
            m_updateTimer = 0.0f;
        }
    }
}
