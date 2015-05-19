/****************************************************************************************
 * File: options.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 09/07/2014

 ***************************************************************************************/
#ifndef FLTM_OPTIONS_HPP
#define FLTM_OPTIONS_HPP

#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name
#include "fltm/fltm.hpp"
#include "utils/option_printer.hpp"
#include "utils/custom_option_desc.hpp"
namespace po = boost::program_options;   

namespace samogwas
{

struct Options {
  std::string inputDataFile;
  std::string inputLabelFile;
  std::string outputDir;
  std::string clustConf;
  double fltm_alpha;
  double fltm_beta;
  int fltm_maxCard;
  samogwas::FLTM_Params fltm_params;

  int outType;


  int nbrConsidered;
};


inline Options get_program_options(int argc, char** argv) {
  Options result;
  std::string appName = boost::filesystem::basename(argv[0]);
  po::options_description optDesc("Options");

  try  {
    /** Define and parse the program options 
     */
    optDesc.add_options()
        ("help,h", "Print help messages")        
        ("in_dat,d", po::value<std::string>(&result.inputDataFile)->required(), "Input Data File")
        ("in_lab,l", po::value<std::string>(&result.inputLabelFile)->required(), "Input Label File")
        ("in_card,N", po::value<int>(&result.fltm_params.cardinality)->required(), "Input cardinality")
        
        ("out,o", po::value<std::string>(&result.outputDir)->required(), "Output Dir")
        ("outtype,t", po::value<int>(&result.outType)->required(), "Output Type (0): Distri (1) Tulip")

        ("clustConf,c", po::value<std::string >(&result.clustConf)->required(), "Clust Config File")
        ("max_dist,x", po::value<unsigned>(&result.fltm_params.maxDist)->required(), "Max Dist")

        ///////////////////////////////////////////////////////////////////////////
        ("f_alpha,a", po::value<double>(&result.fltm_alpha)->required(), "FLTM alpha")
        ("f_beta,b", po::value<double>(&result.fltm_beta)->required(), "FLTM beta")
        ("f_maxCard,X", po::value<int>(&result.fltm_maxCard)->required(), "FLTM maxCard")

        ("f_nbr_restarts,r", po::value<int>(&result.fltm_params.nbrRestarts)->required(), "FLTM nbr restarts")
        ("f_nbr_steps,s", po::value<int>(&result.fltm_params.nbrSteps)->required(), "FLTM nbr steps") 

        ("f_thres_info,i", po::value<double>(&result.fltm_params.latentVarQualityThres)->required(), "FLTM thres info")
        ("f_thres_em,e", po::value<double>(&result.fltm_params.emThres)->required(), "FLTM thres EM")      
        ;
    po::variables_map vm; 
    try { 
      po::store(po::command_line_parser(argc, argv).options(optDesc).run(), vm); // throws on error
      if (vm.count("help") ) {
        samogwas::OptionPrinter::printStandardAppDesc(appName,std::cout, optDesc, NULL);
        exit(1);
      }
      po::notify(vm);   	    

    } 
    catch(boost::program_options::required_option& e) /** missing arguments **/
    {
      samogwas::OptionPrinter::formatRequiredOptionError(e);
      std::cout << e.what() << std::endl << std::endl;
      samogwas::OptionPrinter::printStandardAppDesc( appName,std::cout,
                                                optDesc, NULL);

      exit(-1);
    }

  }
  catch(std::exception e)    
  {
    std::cout << "Unhandled Exception reached the top of main: "
              << e.what() << ", application will now exit" << std::endl;

    samogwas::OptionPrinter::printStandardAppDesc(appName, std::cout, optDesc, NULL);
    exit(-1);
  }

  return result;
}


} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_OPTIONS_HPP
