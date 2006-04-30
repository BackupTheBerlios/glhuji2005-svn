#ifndef __MIDPOINT_SOLVER_H__
#define __MIDPOINT_SOLVER_H__

class NumericalSolver;

class MidPointSolver : public NumericalSolver
{
public:
    MidPointSolver();
    ~MidPointSolver();

    void step( double h );

protected:
    Vector3d *mAccel;
    Vector3d *mTmpPos;
    Vector3d *mTmpV;
};

#endif //__MIDPOINT_SOLVER_H__