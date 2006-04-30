#ifndef __MIDPOINT_SOLVER_H__
#define __MIDPOINT_SOLVER_H__

class NumericalSolver;

class MidPointSolver : public NumericalSolver
{
protected:
    void step( double h );
};

#endif //__MIDPOINT_SOLVER_H__