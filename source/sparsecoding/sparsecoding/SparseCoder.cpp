#include "dct.h"
#include "sparsecoder.h"

#include "../core/Logger.h"
#include "../graphics/Visualizer.h"


#include <iomanip>
#include <iostream>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
SparseCoder::SparseCoder()     
{}


////////////////////////////////////////////////////////////////////////////////
//  orthogonal matching pursuit with cholseky factorization
//  implementation of 
//
void SparseCoder::encodeL0(
    const cv::Mat& X, 
    cv::Mat& Z,
    const int sparsity,
    const float maxError)
{
    Logger log("SparseCoder::encode");

    const int satom = D_.rows;
    const int natoms = D_.cols;
    const int nsamples = X.cols;
    
    Z = cv::Mat::zeros(natoms, nsamples, CV_32F);
    // cv::Mat DTD = D_.t() * D_;

    //  encode each sample
    for (int sample = 0; sample < nsamples; sample++)
    {
        if (sample % 100 == 0) {
            log << std::setprecision(4) << 100.0f * (sample + 1.0f) / nsamples << endl;
        }
        
        //  initialize
        std::vector<int> supp;
        std::set<int> supps;
        float error = std::numeric_limits<float>::infinity();
        
        cv::Mat z;
        cv::Mat x = X.col(sample);
        cv::Mat r = x.clone();
        cv::Mat D = D_.clone();
        // cv::Mat alpha = D.t() * x;

        // cv::Mat L = cv::Mat::ones(1, 1, CV_32F);
        cv::Mat DiT = cv::Mat::zeros(0, satom, CV_32F);

        //  repeat while 
        //      - error too large 
        //      - unused atoms 
        //      - too sparse
        while (supp.size() < natoms) 
        {
            //  find best representing atom
            cv::Mat rTD = r.t() * D;
            
            int maxIdx = -1;
            float maxVal = -std::numeric_limits<float>::infinity();
            
            for (int atom = 0; atom < natoms; atom++)
            {
                if (supps.find(atom) == supps.end())
                {
                    const float val = rTD.at<float>(atom);
                
                    if (val > maxVal) {
                        maxVal = val;
                        maxIdx = atom;
                    }
                }
            }
            
            cv::Mat d = D.col(maxIdx).t();
            DiT.push_back(d);

            // //  update factorization
            // if (supp.size() > 0)
            // {
            //     cv::Mat w;
            //     cv::solve(L, DiT * D.col(k), w);
                
            //     cv::vconcat(L, w.t(), L);
            //     cv::hconcat(L, cv::Mat::zeros(L.rows, 1, L.type()), L);

            //     L.at<L.rows - 1, L.cols - 1> = sqrtf(1.0f - w.dot(w)); 
            // }

            //  update index set
            supp.push_back(maxIdx);
            supps.insert(maxIdx);

            // //  find optimal coefficient
            // cv::Mat gamma;
            // cv::solve(L * L.t(), )

            // z = (DiT.inv(cv::DECOMP_SVD)).t() * x;
            z = (DiT * DiT.t()).inv() * DiT * x;
            r = x - DiT.t() * z;

            error = r.dot(r);

            if (sparsity >= 0 && supp.size() >= sparsity) {
                break;
            }

            if (sparsity < 0 && error <= maxError) {
                break;
            }
        }

        for (int i = 0; i < supp.size(); i++) {
            Z.at<float>(supp[i], sample) = z.at<float>(i);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
//  matching pursuit
//  implementation of 
//      "Elad M. - Sparse and Redundant Representations", p. 37
void SparseCoder::encode(
    const cv::Mat& X, 
    cv::Mat& Z,
    const int sparsity,
    const float maxError)
{
    Logger log("SparseCoder::encode");

    const int natoms = D_.cols;
    Z = cv::Mat::zeros(D_.cols, X.cols, CV_32F);

    //  precompute inner products
    cv::Mat DD;
    cv::multiply(D_, D_, DD);

    cv::Mat dd;
    cv::reduce(DD, dd, 0, CV_REDUCE_SUM);

    //  encode each sample
    for (int i = 0; i < X.cols; i++)
    {
        if (X.cols > 1000) {
            if (i % (X.cols / 1000) == 0) {
                log << std::setprecision(3) << 100.0f * (i + 1.0f) / X.cols << endl;
            }
        }
        
        //  initialize
        float error = 1;
        int iter = 0;

        cv::Mat z = Z.col(i);
        cv::Mat r = cv::Mat::zeros(1, X.rows, CV_32F);
        cv::Mat rD = cv::Mat::zeros(1, D_.cols, CV_32F);
        
        X.col(i).copyTo(r);

        //  set of used (thus unavailable) atoms
        std::set<int> supp;

        //  repeat while 
        //      - error too large 
        //      - unused atoms 
        //      - too sparse
        while (supp.size() < natoms) 
        {
            //  sweep
            rD = r.t() * D_;
            
            float minEps = std::numeric_limits<float>::infinity();
            float minZ = -1;
            int minId = -1;

            const float rr = r.dot(r);

            for (int j = 0; j < natoms; j++) 
            {
                if (supp.find(j) == supp.end())
                {
                    const float rd = rD.at<float>(0, j);
                    const float z_j = rd / dd.at<float>(0, j);
                    const float eps_j = rr - rd * z_j;

                    if (eps_j < minEps)
                    {
                        minId = j;
                        minZ = z_j;
                        minEps = eps_j;
                    }
                }
            }

            //  update support
            supp.insert(minId);

            //  update provisional solution
            z.row(minId) = minZ;

            //  update residual
            r = X.col(i) - D_ * z;

            //  stopping rule
            error = r.dot(r);
            iter++; 

            if (sparsity >= 0 && iter >= sparsity) {
                break;
            }

            if (sparsity < 0 && error <= maxError) {
                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void SparseCoder::decode(
    cv::Mat& X,
    const cv::Mat& Z) 
{
    X = D_ * Z;
}


////////////////////////////////////////////////////////////////////////////////
//  K-SVD dictionary learning
//  implementation of
//      "M. Aharon, M. Elad, A. Bruckstein - K-SVD: An Algorithm for Designing 
//      Overcomplete Dictionaries for Sparse Representation"
void SparseCoder::fit(
    const cv::Mat& X,
    const int sparsity,
    const float maxError,
    const int k)
{
    Logger log("SparseCoder::fit");

    //  initialize dictionary if necessary
    if (D_.empty()) {
        initialize(X, 255, DICTIONARY_DCT);
    }

    //  repeat for given number of iterations
    for (int i = 0; i < k; i++)
    {
        log << "iteration " << i + 1 << " / " << k << endl;

        //  sparse coding stage
        cv::Mat Z;
        encode(X, Z, sparsity, maxError);

        //  codebook update stage
        for (int atom = 0; atom < D_.cols; atom++)
        {
            //  find active samples using this atom
            cv::Mat Xk = cv::Mat(0, X.rows, X.type());
            cv::Mat Zk = cv::Mat(0, Z.rows, Z.type());

            for (int sample = 0; sample < X.cols; sample++)
            {
                if (Z.at<float>(atom, sample) != 0) 
                {
                    cv::Mat x = X.col(sample).t();
                    cv::Mat z = Z.col(sample).t();

                    Xk.push_back(x);
                    Zk.push_back(z);
                }
            }

            //  update if atom is used at all
            if (Xk.rows > 0)
            {
                //  compute representation error matrix
                Zk.col(atom).setTo(0);
                cv::Mat Ek = Xk.t() - D_ * Zk.t();


                //  find optimal decomposition
                cv::Mat U, S, Vt;
                cv::SVD::compute(Ek, S, U, Vt);

                U.col(0).copyTo(D_.col(atom));
                Z.row(atom) = Vt.row(0) * S.at<float>(0, 0);
            }
        }

        //  visualize update
        showDictionary();
    }
}


////////////////////////////////////////////////////////////////////////////////
void SparseCoder::initialize(
    const cv::Mat& X,
    const int size,
    const int mode) 
{
    if (mode == DICTIONARY_DCT) {
        dctBasis2d(X.rows, size, D_);
    }

    A_ = cv::Mat::zeros(D_.cols, D_.cols, D_.type());
    B_ = cv::Mat::zeros(D_.rows, D_.cols, D_.type());
}


// ////////////////////////////////////////////////////////////////////////////////
void SparseCoder::showDictionary() const
{
    cv::Mat img;
    dictToImage(D_, img);
    
    cv::resize(img, img, cv::Size(800, 800));
    Visualizer::showImage(img);
}


////////////////////////////////////////////////////////////////////////////////
void SparseCoder::fitOnline(
    const cv::Mat& X,
    const int sparsity,
    const float maxError)
{
    Logger log("SparseCoder::fitOnline");

    
    //  update dictionary in an online approach, sample by sample
    for (int sample = 0; sample < X.cols; sample++)
    {
        if (X.cols > 1000) {
            if (sample % (X.cols / 1000) == 0) {
                log << std::setprecision(3) << 100.0f * (sample + 1.0f) / X.cols << endl;
            }
        }

        //  get and encode current sample
        cv::Mat z;
        const cv::Mat& x = X.col(sample);

        Logger::setActive(false);
        encode(x, z, sparsity, maxError);
        Logger::setActive(true);

        //  update history
        A_ += z * z.t();
        B_ += x * z.t();

        //  update dictionary
        for (int atom = 0; atom < D_.cols; atom++) 
        {
            if (A_.at<float>(atom, atom) != 0) 
            {
                D_.col(atom) = 
                    (B_.col(atom) - D_ * A_.col(atom)) 
                    / A_.at<float>(atom, atom)
                    + D_.col(atom);
            }
        }

        //  normalize dictionary
        for (int atom = 0; atom < D_.cols; atom++) {
            D_.col(atom) /= cv::norm(D_.col(atom));
        }
 
        //  visualize results       
        if (sample % 100 == 0) {
            showDictionary();
        }
    }
}