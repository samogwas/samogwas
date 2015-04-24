/*=============================================================================
 * Product        : ProBT 
 * File           : plScores.h
 * Author         : David Raulo
 * Creation       : Sun May 31 21:50:03 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

/// \file plScores.h
/// Defines the score classes to be used with class plStructureLearner.

#ifndef PL_SCORES_H_
#define PL_SCORES_H_

#include <plConfig.h>
#include <plDLLExport.h>
#include <plSymbol.h>
#include <plJointDistribution.h>
#include <plDataDescriptor.h>

//=============================================================================
/// Base-class for scores.
/// \sa class plStructureLearner.
class PL_DLL_API plScore
{
 public:
  plScore() {}
};

//=============================================================================
/// Base-class for decomposable node scores.
/// \sa class plStructureLearner.
class PL_DLL_API plNodeScore: public plScore
{
 public:

  plNodeScore()
    : plScore()
    {}
  
  /// Compute the node's contribution to the score.
  virtual plFloat operator()(const plSymbol& node,
                             const plVariablesConjunction& parents) const = 0;

  /// Compute the score of the whole joint distribution.
  plFloat operator()(const plJointDistribution& jd) const;

  virtual ~plNodeScore() {}
};

//-----------------------------------------------------------------------------
/// Base-class template for node scores operating on datasets. This provides
/// storage for the data descriptor.
class PL_DLL_API plNodeScore_t: public plNodeScore
{
 public:
  /// Construct a node score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plNodeScore_t(plDataDescriptor& data_desc)
    : plNodeScore()
    , data_desc_(data_desc)
  {}

 protected:
  plDataDescriptor& data_desc_;
};

//-----------------------------------------------------------------------------
/// Akaike's Information Criterion.
class PL_DLL_API plNodeScoreAIC: public plNodeScore_t
{
 public:
  /// Construct a node score object using Akaike's Information
  /// Criterion, for evaluating a model's performance with respect to a
  /// dataset.
  /// \param data_desc the data descriptor
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plNodeScoreAIC(plDataDescriptor& data_desc);

  plFloat operator()(const plSymbol& node,
                     const plVariablesConjunction& parents) const PL_OVERRIDE;

  using plNodeScore_t::operator();
};

//-----------------------------------------------------------------------------
/// Bayesian Information Criterion.
class PL_DLL_API plNodeScoreBIC: public plNodeScore_t
{
 public:
  /// Construct a node score object using the Bayesian Information
  /// Criterion, for evaluating a model's performance with respect to a
  /// dataset.
  /// \param data_desc the data descriptor
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plNodeScoreBIC(plDataDescriptor& data_desc);

  plFloat operator()(const plSymbol& node,
                     const plVariablesConjunction& parents) const PL_OVERRIDE;

  using plNodeScore_t::operator();
};

//-----------------------------------------------------------------------------
/// Bayesian Dirichlet Equivalent Uniform.
class PL_DLL_API plNodeScoreBDeu: public plNodeScore_t
{
 public:
  /// Construct a node score object using the Bayesian Dirichlet
  /// Equivalent Uniform criterion, for evaluating a model's performance with
  /// respect to a dataset.
  /// \param data_desc the data descriptor
  /// \param Nprime parameter used for defining the bayesian prior.
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  plNodeScoreBDeu(plDataDescriptor& data_desc,
                  plFloat Nprime);

  plFloat operator()(const plSymbol& node,
                     const plVariablesConjunction& parents) const PL_OVERRIDE;

  using plNodeScore_t::operator();

 protected:
  const plFloat Nprime_;
};

//-----------------------------------------------------------------------------
/// Minimum Description Length.
class PL_DLL_API plNodeScoreMDL: public plNodeScore_t
{
 public:
  /// Construct a node score object using the Minimum Description
  /// Length, for evaluating a model's performance with respect to a dataset.
  /// \param data_desc the data descriptor
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plNodeScoreMDL(plDataDescriptor& data_desc);

  plFloat operator()(const plSymbol& node,
                     const plVariablesConjunction& parents) const PL_OVERRIDE;

  using plNodeScore_t::operator();

 protected:
  unsigned int max_card_;
};

//=============================================================================
/// Base-class for decomposable edge scores.
/// \sa class plStructureLearner.
class PL_DLL_API plEdgeScore  : public  plScore
{
 public:
  /// Default constructor
  plEdgeScore()
    : plScore()
    , conditioning_variables_()
    {}

  /// Constructor using conditioning variables
  explicit plEdgeScore(const plVariablesConjunction &conditioning_variables)
    : plScore()
    , conditioning_variables_(conditioning_variables)
  {}
  
  /// Compute the edge's contribution to the score.
  /// If the score is conditioning consructed using conditioning variables,
  /// this equivalente to operator(A, B, conditioning_variables)
  virtual plFloat operator()(const plSymbol& A, const plSymbol& B) const = 0;
  
  /// Compute the Conditional score of variables A and B knowing
  /// the variable C.
  virtual plFloat operator()(const plSymbol& A, const plSymbol& B, 
                             const plVariablesConjunction& C) const = 0;

  /// Compute the score of the whole joint distribution.
  plFloat operator()(const plJointDistribution& jd) const;

  /// Destructor
  virtual ~plEdgeScore() {}

 protected:
  plVariablesConjunction conditioning_variables_;

 private:
  plEdgeScore(const plEdgeScore&);
  plEdgeScore& operator=(const plEdgeScore&);
};

//-----------------------------------------------------------------------------
/// Mutual Information.
class PL_DLL_API plEdgeScoreMI: public plEdgeScore
{
 public:
  /// Construct a Mutual information edge score object operating on a given
  /// dataset.
  /// \param data_desc the data descriptor ;
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plEdgeScoreMI(plDataDescriptor& data_desc)
    : plEdgeScore()
    , data_desc_(data_desc)
  {}

  /// Constructor using conditioning variables
  plEdgeScoreMI(plDataDescriptor& data_desc,
                const plVariablesConjunction &conditioning_variables)
    : plEdgeScore(conditioning_variables)
    , data_desc_(data_desc)
  {}

  plFloat operator()(const plSymbol& A, const plSymbol& B) const PL_OVERRIDE;

  /// Compute the Conditional Mutual Information of variables A and B knowing
  /// the variable C.
  plFloat operator()(const plSymbol& A, const plSymbol& B, 
                     const plVariablesConjunction& C) const PL_OVERRIDE;
  
  using plEdgeScore::operator();

 protected:
  plDataDescriptor& data_desc_;
};

//-----------------------------------------------------------------------------
/// Normalized metric variant of the Mutual Information.
/// D(X,Y) = I(X,Y) / H(X,Y)
class PL_DLL_API plEdgeScoreMIDistance: public plEdgeScore
{
 public:
  /// Construct a mutual information distance edge score object operating on a given
  /// dataset.
  /// \param data_desc the data descriptor ;
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plEdgeScoreMIDistance(plDataDescriptor& data_desc)
    : plEdgeScore()
    , data_desc_(data_desc)
  {}

  /// Constructor using conditioning variables
  plEdgeScoreMIDistance(plDataDescriptor& data_desc,
                        const plVariablesConjunction &conditioning_variables)
    : plEdgeScore(conditioning_variables)
    , data_desc_(data_desc)
  {}

  plFloat operator()(const plSymbol& A, const plSymbol& B) const PL_OVERRIDE;

  /// Compute the Conditional Mutual Information of variables A and B knowing
  /// the variable C.
  plFloat operator()(const plSymbol& A, const plSymbol& B, 
                     const plVariablesConjunction& C) const PL_OVERRIDE;
  
  using plEdgeScore::operator();

 protected:
  plDataDescriptor& data_desc_;
};

//-----------------------------------------------------------------------------
/// Entropy.
class PL_DLL_API plEdgeScoreEntropy: public plEdgeScore
{
 public:
  /// Construct an Entropy edge score object operating on a given
  /// dataset.
  /// \param data_desc the data descriptor ;
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plEdgeScoreEntropy(plDataDescriptor& data_desc)
    : plEdgeScore()
    , data_desc_(data_desc)
  {}
  
  /// Constructor using conditioning variables
  plEdgeScoreEntropy(plDataDescriptor& data_desc,
                     const plVariablesConjunction &conditioning_variables)
    : plEdgeScore(conditioning_variables)
    , data_desc_(data_desc)
  {}

  /// Compute the Joint Entropy of variables A and B.
  plFloat operator()(const plSymbol& A, const plSymbol& B) const PL_OVERRIDE;

  /// Compute the Conditional Entropy of variables A and B knowing
  /// the variable C.
  plFloat operator()(const plSymbol& A, const plSymbol& B, 
                     const plVariablesConjunction& C) const PL_OVERRIDE;
  
  using plEdgeScore::operator();

 protected:
  plDataDescriptor& data_desc_;
};

//-----------------------------------------------------------------------------
// Constraint object to assert that a type T is derived from B.
// The check occurs at compilation-time, and no code is generated.
template< class T, class B > struct Derived_from
{
  static void constraints(T* p) { B* pb = p; (void) pb; }
  Derived_from() { void(*p)(T*) = constraints; (void) p; }
};

/// Base-class template for edge-scores constructed from a node score.
template <typename NodeScore>
class plEdgeScoreFromNodeScore: public plEdgeScore
                              , private Derived_from<NodeScore, plNodeScore>
{
public:
  plFloat operator()(const plSymbol& A, const plSymbol& B) const PL_OVERRIDE
  {
    // std::cout << __PRETTY_FUNCTION__ << ": " 
    //           << nscore_(B, A^conditioning_variables_) << " - " << nscore_(B, conditioning_variables_) << " = "
    //           << nscore_(B, A^conditioning_variables_) - nscore_(B, conditioning_variables_)
    //           << " "
    //           << nscore_(A, B^conditioning_variables_) << " - " << nscore_(A, conditioning_variables_) << " = "
    //           << nscore_(A, B^conditioning_variables_) - nscore_(A, conditioning_variables_)
    //           << std::endl;
    /// \todo Check this:  nscore_(B, A^conditioning_variables_) - nscore_(B, conditioning_variables_) should be the same 
    /// as nscore_(A, B^conditioning_variables_) - nscore_(A, conditioning_variables_)
    /// BUT it is NOT the case when using incomplete data
    //return nscore_(B, A^conditioning_variables_) - nscore_(B, conditioning_variables_);
    return nscore_(A, B^conditioning_variables_) - nscore_(A, conditioning_variables_);
  }

  plFloat operator()(const plSymbol& A, const plSymbol& B,
                     const plVariablesConjunction& C) const PL_OVERRIDE
  {
    //return nscore_(B, A^C) - nscore_(B, C);
    return nscore_(A, B^C) - nscore_(A, C);
  }

  using plEdgeScore::operator();

protected:
  explicit plEdgeScoreFromNodeScore(const NodeScore& nscore) 
  : plEdgeScore()
  , nscore_(nscore)
  {}

  plEdgeScoreFromNodeScore(const NodeScore& nscore, const plVariablesConjunction &conditioning_variables) 
    : plEdgeScore(conditioning_variables)
    , nscore_(nscore)
  {}

  NodeScore nscore_;
};

//-----------------------------------------------------------------------------
/// Akaike's Information Criterion used as an edge score.
/// \sa plNodeScoreAIC, plEdgeScoreFromNodeScore.
class plEdgeScoreAIC
  : public plEdgeScoreFromNodeScore<plNodeScoreAIC >
{
  typedef plNodeScoreAIC NodeScore;
  typedef plEdgeScoreFromNodeScore<NodeScore> Base;
public:
  /// Construct an AIC edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plEdgeScoreAIC(plDataDescriptor& data_desc)
    : Base(NodeScore(data_desc)) {}

  /// Construct an AIC edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// \param conditioning_variables the variables condioning the score
  /// If this parameter is not empty, the score is assumed to be conditional
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  plEdgeScoreAIC(plDataDescriptor& data_desc, const plVariablesConjunction &conditioning_variables)
    : Base(NodeScore(data_desc), conditioning_variables) {}
};

//-----------------------------------------------------------------------------
/// Bayesian Information Criterian used as an edge score.
/// \sa plNodeScoreBIC, plEdgeScoreFromNodeScore.
class plEdgeScoreBIC
  : public plEdgeScoreFromNodeScore<plNodeScoreBIC >
{
  typedef plNodeScoreBIC NodeScore;
  typedef plEdgeScoreFromNodeScore<NodeScore> Base;
public:
  /// Construct a BIC edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plEdgeScoreBIC(plDataDescriptor& data_desc)
    : Base(NodeScore(data_desc)) {}

  /// Construct a BIC edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// \param conditioning_variables the variables condioning the score
  /// If this parameter is not empty, the score is assumed to be conditional
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  plEdgeScoreBIC(plDataDescriptor& data_desc, const plVariablesConjunction &conditioning_variables)
    : Base(NodeScore(data_desc), conditioning_variables) {}
};

//-----------------------------------------------------------------------------
/// Bayesian Dirichlet Equivalent Uniform used as an edge score.
/// \sa plNodeScoreBDeu, plEdgeScoreFromNodeScore.
class plEdgeScoreBDeu
  : public plEdgeScoreFromNodeScore<plNodeScoreBDeu >
{
  typedef plNodeScoreBDeu NodeScore;
  typedef plEdgeScoreFromNodeScore<NodeScore> Base;
public:
  /// Construct a BDeu edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// \param Nprime parameter used for defining the bayesian prior.
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  plEdgeScoreBDeu(plDataDescriptor& data_desc,
                  plFloat Nprime)
    : Base(NodeScore(data_desc, Nprime)) {}

  /// Construct a BDeu edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// \param Nprime parameter used for defining the bayesian prior.
  /// \param conditioning_variables the variables condioning the score
  /// If this parameter is not empty, the score is assumed to be conditional
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  plEdgeScoreBDeu(plDataDescriptor& data_desc,
                  plFloat Nprime, const plVariablesConjunction &conditioning_variables)
    : Base(NodeScore(data_desc, Nprime), conditioning_variables) {}
};

//-----------------------------------------------------------------------------
/// Minimum Description Length used as an edge score.
/// \sa plNodeScoreMDL, plEdgeScoreFromNodeScore.
class plEdgeScoreMDL
  : public plEdgeScoreFromNodeScore<plNodeScoreMDL >
{
  typedef plNodeScoreMDL NodeScore;
  typedef plEdgeScoreFromNodeScore<NodeScore> Base;
public:
  /// Construct an MDL edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  explicit plEdgeScoreMDL(plDataDescriptor& data_desc)
    : Base(NodeScore(data_desc)) {}

  /// Construct an MDL edge score object operating on a given dataset.
  /// \param data_desc the data descriptor ;
  /// \param conditioning_variables the variables condioning the score
  /// If this parameter is not empty, the score is assumed to be conditional
  /// 
  /// Note that the object keeps a reference to the dataset, so don't destroy 
  /// it during the lifetime of the score object.
  plEdgeScoreMDL(plDataDescriptor& data_desc, const plVariablesConjunction &conditioning_variables)
    : Base(NodeScore(data_desc), conditioning_variables) {}
};

//=============================================================================
#endif //PL_SCORES_H_

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
