/// \file plGraph.h
/*=======================================================================
 * Product        : ProBT 
 * File           : plGraph.h
 * Author         : Juan-Manuel Ahuactzin & Emmanuel Mazer
 * Creation       : Tue May 24 11:05:23 2005
 *
 *=======================================================================
 *(c) Copyright 2000-2004, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=======================================================================
 *
 *------------------------- Description ---------------------------------
 *   
 *   
 *-----------------------------------------------------------------------
*/


#ifndef PL_GRAPH_H
#define PL_GRAPH_H

#include "plConfig.h"

#include <set>
#include <vector>
#include <string>
#include <list>
#include <plMath.h>
#include <plObject.h>

/************************************************************************
 *                           Classes Definitions
 ***********************************************************************/

/// Defines the unit to be used among Microsoft Office units or Xfig units
enum plDrawingProgram {PL_OFFICE, /*!< Use Microsoft Office units */
                       PL_XFIG /*!< Use Xfig units */
};

/// Indicates which parameter will be provided among (i) font size, (ii) the
/// node size, or (iii) the paper size. For example, if font size is porovided,
/// then the node size and paper size will be computed automatically.
enum plGraphDesignType {PL_FONT_SIZE,  /*!< Font size is provided */
                        PL_NODE_SIZE,  /*!< Node size is provided */
                        PL_PAPER_SIZE  /*!< Paper size is provided */
};


// plGraphNode represents a node of a plGraph 
struct plGraphNode {
  int x;                 // x coordinate of the node in Xfig units.
  int y;                 // y coordinate of the node in Xfig units.
  unsigned int level;    // Level of the node. Root nodes have level 0.
  std::vector <std::string> vars;  // Variables names on the node.
  std::set <unsigned int> parent;      // Set of parents ids of the node.
  std::vector <unsigned int> son;      // Vector of sons ids of the node.
  std::string distribution_label;  // Contains the distribution label of the node
                              // e.g. P(S | K) where S is the set of variables
                              // of the node and K is the set of parents of the 
                              // node.
};

//=============================================================================
SPL_DECL1(plGraph);

/** 
    The \em plGraph class is used to obtain a a graphical representation of a
    joint distribution.  An instance of this class can be passed as an output
    parameter to the method plJointDistribution::draw_graph(). This function
    will return a graph equivalent to the description used to define the joint
    distribution.
*/
class PL_DLL_API plGraph :public plObject
{
protected:
  SPL_ACCESS1(plGraph);

  unsigned int figure_border_width_;   // Figure border width
  unsigned int figure_border_height_; // Figure border height
  unsigned int x_min_limit_;          // Min x coordinate for nodes
  unsigned int y_min_limit_;          // Min y coordinates for nodes 
  unsigned int x_max_limit_;          // Max x coordinate for nodes
  unsigned int y_max_limit_;          // Max y coordinates for nodes 

  plGraphDesignType designe_type_;   // Indicates from which criteria must be
				    // dawn the graph
				    
  plDrawingProgram drawing_program_; // Indicates what kind of program will draw
				    // the graph
				    
				    
  unsigned int units_by_point_; //Units by font pooint
  bool fixed_orientation_;     // Obligates to use a fixed orientation of the page
  bool portrait_;             // Paper orientation portrait
                             // (portrait=true) or landscape
                             // (portrait=false).
  unsigned int node_width_;    // Width of a node in Xfig units
  unsigned int node_height_;  // Node height in Xfig units
  unsigned int font_size_;    // Font size for labels
  unsigned int width_in_nodes_; // Width of the network in nodes

  // A vector containing the list of nodes at each of the levels
  std::vector <std::list <unsigned int> > hierarchy_;

  // The vector of nodes
  std::vector <plGraphNode> node_;

public:

  /** Empty constructor. This constructor defines a default figure border with
      a width and hight corresponding to a A4 paper where the orientation is not
      fixed (portrait or landscape). */
  plGraph();

  /** Constructs the graph by using the specified window borders sizes. The
      size of a node and the font size are computed automatically. The variable
      \em fix_oriented indicates if the dimentions (width and height) can be
      inversed to best fit the figure.*/
  plGraph(unsigned int width, unsigned int height,
          plGraphDesignType the_type = PL_PAPER_SIZE,
          plDrawingProgram the_software = PL_OFFICE,
          bool fix_oriented=true);

  /* Construct the graph by using the specific node size. The resulting figure
     borders will be computed as a function of the node size. The font size is
     then computed from the node size.*/
  plGraph(unsigned int the_size, 
          plGraphDesignType the_type = PL_FONT_SIZE,
          plDrawingProgram the_software = PL_OFFICE);

  /** Indicates if the graph must be drawn in portrait orientation for the
      given border dimensions width and height */
  bool is_in_portrait() const;             

  /** Returns the recomended node width to be used if the graph to be drawn */
  unsigned int get_node_width() const;

  /** Returns the recomended node height to be used if the graph should be
      drawn */    
  unsigned int get_node_height() const;  

  /** Returns the recomended font size to be used for the node labels*/
  unsigned int get_font_size() const;    

  /** Returns the width in number of nodes of the tree representing graph */
  unsigned int get_width_in_nodes() const;

  /** Gets border width in point units of the graph */
  unsigned int get_border_width() const; 

  /** Returns border height in point units of the graph */
  unsigned int get_border_height() const;

  /** Indicates if the graph has been set with a fixed orientation when it was
      constructed. A fixed orientation indicates that width and height can not
      be inversed to better fit the resulting figure representing the
      graph. For example when using a A4 page it can be inversed (set in
      landscape) if the figure fits better in that way. A fixed_orientation
      equals to true will not allow to re-orient the given dimensions.*/
  bool is_fixed_orientation() const;

  /** Returns the number of nodes */
  unsigned int get_number_of_nodes() const;
  
  /** Returns the string labeling node i. That is the set of variables on the
      node*/
  std::string get_label(unsigned int i) const;


  /** Returns the string labeling the distribution of the node, "P(S | K)" for
      example, where S is the set of variables of the node and K is the set of
      parent variables of the node. */
  std::string  get_distribution_label(unsigned int i) const; 

  
  /** Returns the x coordinate of the node i */
  unsigned int get_x_coordinate (unsigned int i) const;
  
  /** Returns the y coordinate of node i */
  unsigned int get_y_coordinate (unsigned int i) const;
  
  /** Returns the number of sons of node i */
  unsigned int get_number_of_sons (unsigned int i) const;
    
  /** Returns the index of jth son of node i */
  unsigned int get_son(unsigned int i, unsigned int j) const;

 protected:
  void Output(std::ostream &os)const;

};

#endif //PL_GRAPH_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
