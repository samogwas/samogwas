/// \file plDrawingLanguage.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plDrawingLanguage.h
 * Author         : Kamel Mekhnacha
 * Creation       : January 21 2011
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

#ifndef plDrawingLanguage_h
#define plDrawingLanguage_h

/* Doxygen bug: a reference like "plJointDistribution::draw_graph()" creates a broken link.
   That is why for now we say "%plJointDistribution::draw_graph()" instead.
 */

/** The drawing language to be used for generation the graph 
    (used in %plJointDistribution::draw_graph()).
*/ 
enum plDrawingLanguage {PL_XFIG_GRAPH, /*!< Xfig. */
			PL_DOT_GRAPH /*!< Graphviz Dot. */
};


#endif // plDrawingLanguage_h


// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
