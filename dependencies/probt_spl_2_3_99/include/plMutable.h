/*=============================================================================
 * Product        : ProBT 
 * File           : plMutable.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Sep 28 16:27:50 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plMutable_h
#define plMutable_h

#include "plConfig.h"

#include <plDLLExport.h>
#include <plSPL.h>
#include <list>
#include <algorithm>
#include <cassert>

class kplComputableObject;

SPL_DECL2T(plMutable);

/** Template class to make computable objects mutable.
    This template is instantiated for plComputableObject, plDistribution, 
    and plCndDistribution classes. These explicit instantiations are defined 
    as  plMutableComputableObject, plMutableDistribution, and 
    plMutableCndDistribution types respectively. It's recommended to use these 
    typedefs instead of using the template directly.
    
    Mutability is the ability for a given computable object to
    change its shape dynamically.  This property is especially
    useful for defining dynamic probabilistic models such as
    Bayesian filters.
    
    When a mutable computable object is mutated, all non-compiled
    computable objects referencing it will receive a notification with
    this mutation. These non-compiled computable objects are:
    
    - Expressions obtained using the plJointDistribution::ask() method or using
    the plCndDistribution::instantiate() method on an expression.

    - Product expressions constructed using plComputableObject::plComputableObject(const plComputableObjectList &)
    constructor.
    
    - Joint distribution objects (plJointDistribution).
    
    ATTENTION: In some situations, mutability affects inference performances
    because it makes impossible caching intermediate calculation
    using mutable computable objects.
	
    \sa plComputableObject::replace 
*/
template <typename T>
class PL_DLL_API plMutable : public T
{
  SPL_ACCESS2T(plMutable);

 protected:
  std::list<kplComputableObject*> masks_;

 private:
  void finalize_assignment();
  void init(const T& init_object);
  bool is_initialized()const;
  template <typename masktype,typename mutmasktype,typename builtintype>
    void process_it(const T &new_object);
  
 public:
  /** Default constructor */
  plMutable();
  
  /** Construction and initialization */
  explicit plMutable(const T &init_object);

  /** Copy constructor */
  plMutable(const plMutable<T> &other);

  /** Destructor */
  ~plMutable();

  /** Assignment operator.
   */
  plMutable<T> &operator=(const plMutable<T> &new_object);
  
  /** Mutate the object to the new value @em new_object.
   */
  void mutate(const T& new_object);

  /** Assignment operator.
      Equivalent to calling @em mutate
  */
  plMutable<T> &operator=(const T &new_object);
  
 protected:

  inline void add_mask(kplComputableObject* mask)
  {
    assert( std::find(masks_.begin(), masks_.end(), mask) == masks_.end() );
    if(mask) masks_.push_back(mask);
  }
  
  inline void remove_mask(kplComputableObject* mask)
  {
    masks_.remove(mask);
  }  

  template <typename T1, typename T2, typename T3> 
    friend class kplMutableMask;
};



#endif


// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
