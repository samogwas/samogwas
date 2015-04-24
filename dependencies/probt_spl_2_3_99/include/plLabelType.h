/*=============================================================================
 * Product        : ProBT 
 * File           : plLabelType.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Jul 10 16:14:59 2009
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plLabelType_h
#define plLabelType_h

#include <plConfig.h>
#include <plType.h>

#include <string>

/**
   The \em plLabelType class is used to create label types
   taking there values in a set of textual labels.
   e.g. {"TRUE", "FALSE"}, {"BLUE", "RED", "YELLOW"}
*/
class PL_DLL_API plLabelType: public plDiscreteType
{
public:
  /** Default constructor, needed by the serialization code. */
  plLabelType();

  /**
     Creates a label type taking its values in the set \em vals.
  */
  explicit plLabelType(const std::vector<std::string> &vals);

  /** Destructor of the type */
  virtual ~plLabelType();

  /** Promote from a plType. This makes sense because all the information
      is actually in parent class plType. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plLabelType(const plType&);

  /** Get the set of values */
  const std::vector<std::string> &get_values()const;

  /** Get the index corresponding to a given label if exists.
      throws an exception otherwise.
  */
  unsigned int label_to_index(const std::string &label)const;

};














#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
