#ifndef NODEENGINEOBJECT_H
#define NODEENGINEOBJECT_H

class NodeEngineObject
{
public:
    NodeEngineObject();

    virtual void update( float elapsed ) = 0;
};

#endif // NODEENGINEOBJECT_H
