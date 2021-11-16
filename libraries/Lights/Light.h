#ifndef LIGHT_H
#define LIGHT_H

#include <csignal>

#include "common.h"

#define STATE_STRING_LENGTH 5

class Light
{
public:
    enum State
    {
        On,
        Off
    };

    enum StateFlag
    {
        Power = 0x01,
        Brightness = 0x02
    };

public:
    Light();
    virtual ~Light();

    virtual void init();

    std::string name() { return m_name; }
    void setName( const std::string& name ) { m_name = name; }

    bool stateChanged() const { return m_stateUpdateFlags != 0; }
    void clearStateFlags() { m_stateUpdateFlags = 0; }
    void setStateFlag( uint flag ) { m_stateUpdateFlags |= flag; }
    bool stateFlagSet( uint flag ) { return (m_stateUpdateFlags & flag) != 0; }

    State state() const { return m_state; }
    void setState( State state );
    void setState(const String &state );

    virtual int brightness() const;
    virtual void setBrightness(int brightness);

    bool updatePixels() const { return m_updatePixels; }
    void setUpdatePixels( bool update ) { m_updatePixels = update; }

    String getStateString() const { return m_stateStr; }

    virtual void update( float elapsed ) = 0;

    virtual void apply();

    virtual void showColor(const struct CRGB & data) = 0;

    virtual void processState(const StaticJsonDocument<1024>& jsonDoc);

    virtual void getState( StaticJsonDocument<1024>& jsonDoc );

private:
    std::string m_name;
    uint m_validStateFlags;
    uint m_stateUpdateFlags;
    int m_controlPin = -1;
    State m_state = Off;
    String m_stateStr;
    char m_stateString[STATE_STRING_LENGTH];
    int m_brightness = 255;
    bool m_updatePixels;
};

#endif
