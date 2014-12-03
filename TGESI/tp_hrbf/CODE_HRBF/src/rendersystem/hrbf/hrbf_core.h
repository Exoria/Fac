/// @file hrbf_core.hpp
/// @author Gael Guennebaud - gael.guennebaud@inria.fr

#ifndef HRBF_CORE_HPP__
#define HRBF_CORE_HPP__

#include <Eigen/LU>
#include <Eigen/Cholesky>
#include <vector>
#include <iostream>

/// @brief fitting surface on a cloud point and evaluating the implicit surface
/// @tparam _Scalar : a base type float double...
/// @tparam _Dim : dimension of the ambient space
/// @tparam Rbf : the radial basis function used
/// @note see hrbf_setup.hpp for some examples of hrbf
template<typename _Scalar, int _Dim, typename Rbf>
class HRBF_fit
{
public:
    typedef _Scalar Scalar;
    enum { Dim = _Dim };

    typedef Eigen::Matrix<Scalar,Dim,Dim>                       MatrixDD;
    typedef Eigen::Matrix<Scalar,Dim,1>                         Vector;
    typedef Eigen::Matrix<Scalar,Dim,Eigen::Dynamic>            MatrixDX;
    typedef Eigen::Matrix<Scalar,Eigen::Dynamic,Eigen::Dynamic> MatrixXX;
    typedef Eigen::Matrix<Scalar,Eigen::Dynamic,1>              VectorX;

    HRBF_fit() {}

    // --------------------------------------------------------------------------

    void hermite_fit(const std::vector<Vector>& points,
                     const std::vector<Vector>& normals)
    {
        // A COMPLETER --------------------
    }

    // -------------------------------------------------------------------------

    /// evaluate potential at position 'x'
    Scalar eval(const Vector& x) const
    {
        Scalar ret = 0;
        // A COMPLETER --------------------
        return ret;
    }

    // -------------------------------------------------------------------------

    /// evaluate gradient at position 'x'
    Vector grad(const Vector& x) const
    {
        Vector grad = Vector::Zero();
        // A COMPLETER --------------------
        return grad;
    }

    // --------------------------------------------------------------------------

    /// Each column represents p_i:  VectorX pi = _node_centers.col(i);
    MatrixDX  _node_centers;
    VectorX   _alphas;
    /// Each column represents beta_i:  VectorX bi = _betas.col(i);
    MatrixDX  _betas;

}; // END HermiteRbfReconstruction Class =======================================

#endif // HRBF_CORE_HPP__

