/*
 * Copyright 2015, LAAS-CNRS
 * Author: Andrea Del Prete
 */

#ifndef ROBUST_EQUILIBRIUM_LIB_SOLVER_LP_ABSTRACT_HH
#define ROBUST_EQUILIBRIUM_LIB_SOLVER_LP_ABSTRACT_HH

#include <Eigen/Dense>
#include <robust-equilibrium-lib/config.hh>
#include <robust-equilibrium-lib/util.hh>

namespace robust_equilibrium
{

enum ROBUST_EQUILIBRIUM_DLLAPI SolverLP
{
#ifdef CLP_FOUND
  SOLVER_LP_CLP,
#endif
  SOLVER_LP_QPOASES
};

enum ROBUST_EQUILIBRIUM_DLLAPI LP_status
{
  LP_STATUS_UNKNOWN=-1, //before solve or if postSolve says not optimal
  LP_STATUS_OPTIMAL=0,
  LP_STATUS_INFEASIBLE=1,
  LP_STATUS_DUAL_INFEASIBLE=2,
  LP_STATUS_MAX_ITER_REACHED=3,
  LP_STATUS_ERROR=4
};

class ROBUST_EQUILIBRIUM_DLLAPI Solver_LP_abstract
{
public:

  Solver_LP_abstract(){}

  static Solver_LP_abstract* getNewSolver(SolverLP solverType);

  /** Solve the linear program
   *  minimize    c' x
   *  subject to  Alb <= A x <= Aub
   *              lb <= x <= ub
   */
  virtual LP_status solve(Cref_vectorX c, Cref_vectorX lb, Cref_vectorX ub,
                          Cref_matrixXX A, Cref_vectorX Alb, Cref_vectorX Aub,
                          Ref_vectorX sol) = 0;

  /**
   * @brief Solve the linear program described in the specified file.
   * @param filename Name of the file containing the LP description.
   * @param sol Output solution of the LP.
   * @return A flag describing the final status of the solver.
   */
  virtual LP_status solve(const char* filename, Ref_vectorX sol);

  /**
   * @brief Write the specified Linear Program to binary file.
   *  minimize    c' x
   *  subject to  Alb <= A x <= Aub
   *              lb <= x <= ub
   * @param filename
   * @param c
   * @param lb
   * @param ub
   * @param A
   * @param Alb
   * @param Aub
   * @return True if the operation succeeded, false otherwise.
   */
  virtual bool writeLpToFile(const char* filename,
                             Cref_vectorX c, Cref_vectorX lb, Cref_vectorX ub,
                             Cref_matrixXX A, Cref_vectorX Alb, Cref_vectorX Aub);

  /**
   * @brief Read the data describing a Linear Program from the specified binary file.
   * The vectors and matrices are resized inside the method.
   *  minimize    c' x
   *  subject to  Alb <= A x <= Aub
   *              lb <= x <= ub
   * @param filename
   * @param c
   * @param lb
   * @param ub
   * @param A
   * @param Alb
   * @param Aub
   * @return True if the operation succeeded, false otherwise.
   */
  virtual bool readLpFromFile(const char* filename,
                              VectorX &c, VectorX &lb, VectorX &ub,
                              MatrixXX &A, VectorX &Alb, VectorX &Aub);

  /** Get the status of the solver. */
  virtual LP_status getStatus() = 0;

  /** Get the objective value of the last solved problem. */
  virtual double getObjectiveValue() = 0;

  virtual void getDualSolution(Ref_vectorX res) = 0;

//  virtual void getDualColumnSolution(Ref_vectorX res) = 0;

  /** Get the current maximum number of iterations performed
   *  by the solver.
   */
  unsigned int getMaximumIterations();

  /** Set the current maximum number of iterations performed
   *  by the solver.
   */
  bool setMaximumIterations(unsigned int maxIter);

  /** Set the maximum time allowed to solve a problem. */
  bool setMaximumTime(double seconds);

};

} // end namespace robust_equilibrium

#endif //ROBUST_EQUILIBRIUM_LIB_SOLVER_LP_ABSTRACT_HH
