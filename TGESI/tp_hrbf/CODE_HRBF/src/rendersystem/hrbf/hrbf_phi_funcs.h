#ifndef HRBF_PHI_FUNCS_HPP_
#define HRBF_PHI_FUNCS_HPP_

/** @brief Radial basis functions definitions (function phi)
  Here you can add more radial basis function definitions.  
*/
template<typename Scalar>
struct Rbf_pow3
{
    static inline Scalar f  (const Scalar& x) { return x*x*x;             }

    static inline Scalar df (const Scalar& x) { return Scalar(3) * x * x; }

    static inline Scalar ddf(const Scalar& x) { return Scalar(6) * x;     }
};

#endif //HRBF_PHI_FUNCS_HPP_
