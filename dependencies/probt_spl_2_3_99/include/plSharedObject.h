/*=============================================================================
 * Product        : ProBT 
 * File           : plSharedObject.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Jan 30 14:59:23 2005
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef PL_SHARED_OBJECT_H
#define PL_SHARED_OBJECT_H

#include "plConfig.h"


//#define PL_COUNT_SHARED_OBJECTS



class plRefCounter;

class plSharedObject
{
private:
  plSharedObject(const plSharedObject&o);
  plSharedObject &operator=(const plSharedObject&o);
public:
  plSharedObject();
	
  virtual ~plSharedObject();
	
  void addRef();
  
  void remRef();
  
#ifdef PL_COUNT_SHARED_OBJECTS
  // Returns the number of alive shared objects
  static unsigned int get_n_shared_objects()
  {
    return nobjects_;
  }

#endif
	
protected:
  plRefCounter *nreferences_;

 
#ifdef PL_COUNT_SHARED_OBJECTS
  static unsigned int nobjects_;
#endif
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
