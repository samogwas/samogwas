/// \file plDatabaseInitError.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plDatabaseInitError.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Sep 27 12:22:46 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plDatabaseInitError_h
#define plDatabaseInitError_h

/** Database initialization errors */
enum  plDatabaseInitError {PL_INITIALIZATION_ERROR, /*!< Initialization error. */
			   PL_CONNECTION_ERROR,  /*!< Connection error. */
			   PL_QUERY_ERROR, /*!< Query error. */
			   PL_NO_ERROR /*!< No error. */
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
