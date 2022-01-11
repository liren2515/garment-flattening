#pragma once

#include <Eigen/Core>
#include <igl/boundary_loop.h>

#include "param/bary_optimizer.h"
#include "param/param_utils.h"
#include "param/auto_select.h"

class ClothParam {
public:

    // ----- MAIN FUNCTIONS ----- //

    // Initialize and allocate memory
    ClothParam(const Eigen::MatrixXd& V_3d, const Eigen::MatrixXi& F,
               double max_stretch = 0.05,
               const std::vector<std::vector<std::pair<int, int>>>& dart_duplicates = {},
               const std::vector<int>& dart_tips = {});

    // Tries to parameterize. Returns whether max_stretch 
    // was satisfied within max number of iterations.
    bool paramAttempt(int max_iter = 10);

    // ----- OPTIONAL ----- //

    // Tries to align V axis in parameterization with
    // direction v1 -> v2 
    void setAlignmentVertexPair(int v1_id, int v2_id);



    // Seams
    // TODO decide: do it in order of patch tracing, or do it in the end?

    // TODO
    bool checkSelfIntersect() const;

    // ----- MISC. ----- //

    void printStretchStats() const;

    Eigen::MatrixXd getV2d(){return V_2d_;}

private:
    // Set during init
    const Eigen::MatrixXd V_3d_;
    const Eigen::MatrixXi F_;
    const double max_stretch_;

    Eigen::VectorXi bnd_;
    Eigen::MatrixXd V_2d_;
    BaryOptimizer bo_;

    Eigen::VectorXd stretch_u_, stretch_v_;

    bool constraintSatisfied(){
        if (stretch_u_.rows() < 1) return false;
        return stretch_u_.minCoeff() > -max_stretch_ 
            && stretch_u_.maxCoeff() < max_stretch_ 
            && stretch_v_.minCoeff() > -max_stretch_ 
            && stretch_v_.maxCoeff() < max_stretch_;
    };

    // Sets pairs of vertex ids which should be symmetrical in a given dart.
    // first indexing: dart id
    // second indexing: pair id in dart    
    // pair: two vertex ids, which should be symmetrical w.r.t. dart
    void setDartPairs(const std::vector<std::vector<std::pair<int, int>>>& dart_duplicates,
                      const std::vector<int>& dart_tips);
};