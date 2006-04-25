#ifndef __BASE_FORCE_H__
#define __BASE_FORCE_H__

class Vector3d;

//base class for forces in system
class Force
{
public:
    virtual Vector3d getForceAt( Vector3d &pos ) = 0;
};

#endif //__BASE_FORCE_H__