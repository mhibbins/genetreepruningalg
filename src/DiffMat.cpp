#include "DiffMat.hpp"

class eigen_multiplier
{
public:
    std::vector<Eigen::MatrixXd> doit(const std::vector<Eigen::MatrixXcd>& matrices, const Eigen::MatrixXcd& transpose);
};

DiffMat::DiffMat(int Npts) {
    // Npts is the number of points in which the interval is discretized
    Eigen::MatrixXd A(Npts, Npts);
    A.setZero();
    for (int i = 0; i < Npts - 1; ++i) {
        A(i, i) = -2;
        A(i, i + 1) = 1;
        A(i + 1, i) = 1;
    }
    A(0, 0) = -1;
    A(Npts - 1, Npts - 1) = -1;

    Diff = A;
    Eigen::EigenSolver<Eigen::MatrixXd> es(Diff);
    passage = es.eigenvectors();
    eig = Diff.eigenvalues();

    multiplier = new eigen_multiplier();
}

std::vector<Eigen::MatrixXd> ConvProp_bounds_batched(std::vector<double> vt, double cCoeff, const DiffMat& dMat, std::vector<boundaries> vbounds) {
    // Calculate the transition density (dMat.diff to the power of cCoeff * t * (n-1)^2 / (b-a)^2
    // using eigenvectors to speed up the calculation
    size_t count = vt.size();
    std::vector<Eigen::MatrixXcd> vTemp(count);
    int Npts = dMat.Diff.cols();

    for (size_t k = 0; k < count; ++k)
    {
        double tau = pow((vbounds[k].second - vbounds[k].first) / (Npts - 1), 2);
        std::vector<double> expD(Npts);
        for (int i = 0; i < Npts; ++i)
        {
            expD[i] = exp(cCoeff * (vt[k] / tau) * dMat.eig[i].real());
        }
        Eigen::MatrixXcd temp(Npts, Npts);
        for (int i = 0; i < Npts; ++i)
            for (int j = 0; j < Npts; ++j)
                temp(i, j) = dMat.passage(i, j) * expD[j];
        vTemp[k] = temp;
    }
    Eigen::MatrixXcd transpose = dMat.passage.transpose();

    return dMat.multiplier->doit(vTemp, transpose);
}

DiffMat* p_diffmat = nullptr;
const DiffMat& DiffMat::instance() {

    if (!p_diffmat) 
        p_diffmat = new DiffMat(100);

    return *p_diffmat;
}

Eigen::MatrixXd ConvProp_bounds(double t, double cCoeff, const DiffMat& dMat, boundaries bounds) {
    return ConvProp_bounds_batched(std::vector<double>({ t }), cCoeff, dMat, std::vector<boundaries>({ bounds }))[0];
}

Eigen::VectorXd VectorPos_bounds(double x, int Npts, boundaries bounds) {
    Eigen::VectorXd X = Eigen::VectorXd::Zero(Npts);
    if (x == bounds.second)
    {
        X[Npts - 1] = 1;
    }
    else
    {
        double nx = (Npts - 1) * (x - bounds.first) / double(bounds.second - bounds.first);
        int ix = floor(nx);
        double ux = nx - ix;
        X[ix + 1] = ux;
        X[ix] = 1 - ux;
    }
    return X.unaryExpr([Npts, bounds](double x) {return x * (Npts - 1) / double(bounds.second - bounds.first); });
}

std::vector<Eigen::MatrixXd> eigen_multiplier::doit(const std::vector<Eigen::MatrixXcd>& matrices, const Eigen::MatrixXcd& transpose)
{
    int Npts = transpose.rows();
    Eigen::MatrixXcd a(Npts, Npts);

    size_t count = matrices.size();
    std::vector<Eigen::MatrixXd> vResult(count);
    for (size_t k = 0; k < count; ++k)
    {
        Eigen::MatrixXcd a = matrices[k] * transpose;
#ifdef _WIN32
        vResult[k] = MatrixXd(Npts, Npts);
        for (int i = 0; i < Npts; ++i)
            for (int j = 0; j < Npts; ++j)
                vResult[k](i, j) = max(a(i, j).real(), 0.0);
#else
        vResult[k] = a.unaryExpr([](std::complex<double> x) {return std::max(x.real(), 0.0); });
#endif
    }
    return vResult;
}
