/// \file plPlotData.h
/*=======================================================================
 * Product        : ProBT 
 * File           : plPlotData.h
 * Author         : ahuactzin
 * Creation       : Thu Oct 27 16:05:38 2005
 *
 *=======================================================================
 *                       (c) Copyright 2005, Probayes SAS,
 *                              all rights reserved
 *=======================================================================
 *
 *------------------------- Description ---------------------------------
 *   
 *   
 *-----------------------------------------------------------------------
*/
#ifndef plPlotData_h
#define plPlotData_h

#include "plConfig.h"

// Doxygen creates a broken link for plDistribution::plot(). Prevent the link.
/** Gnuplot output format using the %plDistribution::plot() function.  These Enum
    values correspond to gnuplot terminal types to be used in 'set terminal'
 */
enum plPlotType {
  PL_DEFAULT_PLOT, /*!< Screen display. */
  PL_EPS_COLOR_PLOT, /*!< COLOR EPS. */
  PL_EPS_PLOT, /*!< EPS. */
  PL_FIG_PLOT, /*!< FIG. */
  PL_JPEG_PLOT, /*!< JPEG. */
  PL_LATEX_PLOT, /*!< LATEX. */
  PL_PDF_COLOR_PLOT,  /*!< COLOR PDF. */
  PL_PDF_PLOT, /*!< PDF. */
  PL_PNG_PLOT, /*!< PNG. */
  PL_POSTSCRIPT_COLOR_PLOT, /*!< COLOR POSTSCRIPT. */
  PL_POSTSCRIPT_PLOT /*!< POSTSCRIPT. */
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
