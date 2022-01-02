#ifndef BLIND_H
#define BLIND_H

#include "NodeEngineObject.h"
#include "NodeEngineMqttListener.h"

#include "StepperMotor.h"

#define M1_STEP 23
#define M1_DIR 22
#define M1_EN 21

class Blind : public NodeEngineObject, public NodeEngineMqttListener
{
    enum class State
    {
        Idle,
        OpeningBlind,
        ClosingBlind,
        SetApperture
    };

    struct PositionInfo
    {
        uint32_t m_position = 0;
        uint32_t m_tilt = 0;

        int getPositionState( float ratio )
        {
            return m_position * ratio;
        }

        int getStepsToPosition( const PositionInfo& toPosition, const PositionInfo& maxPosition )
        {
            if( toPosition.m_position > m_position )
            {

            }
        }
    };

public:
    Blind();

    void setName( const std::string& name ) { m_name = name; }

    StepperMotor& stepperMotor() { return m_motor; }

    void setPosition( int position );

    void setTilt( int tilt );

    virtual std::list<std::string> getTopics() const override;

    virtual bool nextStateMessage( std::string& topic, std::string& ) override;

    virtual void processMessage(const std::string& topic, const std::string& payload) override;

private:
    void movePosition( int steps );

    void moveTilt( int steps );

    virtual void update( float elapsed ) override;

private:
    std::string m_name;
    State m_state = State::Idle;
    StepperMotor::Direction m_openDirection = StepperMotor::Direction::Clockwise;
    StepperMotor::Direction m_closeDirection = StepperMotor::Direction::AntiClockwise;
    uint32_t    m_endPosition = 0;       ///< total numbers of steps needed to go from fully open to fully closed
    uint32_t    m_tiltSteps = 0;
    PositionInfo m_maxPosition;
    PositionInfo m_currentPosition;
    PositionInfo m_requestedPosition;
    bool        m_positionChanged = true;
    bool        m_tiltChanged = true;
    float       m_updateTimer = 0;
    uint32_t    m_stateUpdateInterval = 1.0f; ///< interval in seconds
    bool        m_sendStates = true;

    StepperMotor m_motor;
};

#endif // BLIND_H
