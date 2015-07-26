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
#include <time.h>
#ifdef _OPENMP
    #include <omp.h>
#endif

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
  // double fltm_alpha;
  // double fltm_beta;
  // int fltm_maxCard;
  samogwas::FLTM_Params fltm_params;
  // int matrixType;
  // int outType;
  // // int seed;
  // int nbrConsidered;
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
        // ("mat_type,m", po::value<int>(&result.matrixType)->default_value(0), "Matrix type (0): rows: variables, (1): rows: individuals. Default: 0")
        ("in_lab,l", po::value<std::string>(&result.inputLabelFile)->required(), "Input Label File")
        ("in_card,N", po::value<int>(&result.fltm_params.cardinality)->default_value(3), "Input cardinality. Default: 3")

        ("out,o", po::value<std::string>(&result.outputDir)->default_value("./out"), "Output Dir. Default: ./out")
        // ("outtype,t", po::value<int>(&result.outType)->default_value(2), "Output Type (0): Distri, (1): Tulip, (2): Both. Default: 2")
        // ("random seed,R", po::value<unsigned>(&result.fltm_params.seed)->default_value(time(NULL)), "Random seed. Default: time based")
        // #ifdef _OPENMP
        // ("jobs_number,j", po::value<int>(&result.fltm_params.jobsNumber)->default_value(omp_get_max_threads()), "Number of jobs. Default: all available threads")
        // #endif

        // ("clustConf,c", po::value<std::string>(&result.clustConf)->default_value("../example/inputs/clustering.cfg"), "Clust Config File. Default: ../example/inputs/clustering.cfg")
        ("max_dist,x", po::value<unsigned>(&result.fltm_params.maxDist)->default_value(50000), "Max Dist. Default: 50000 bp")

        // ///////////////////////////////////////////////////////////////////////////
        // ("f_alpha,a", po::value<double>(&result.fltm_alpha)->default_value(0.5), "FLTM alpha. Default 0.5")
        // ("f_beta,b", po::value<double>(&result.fltm_beta)->default_value(2), "FLTM beta. Default 2")
        // ("f_maxCard,X", po::value<int>(&result.fltm_maxCard)->default_value(10), "FLTM maxCard. Default 10")

        // ("f_nbr_restarts,r", po::value<int>(&result.fltm_params.nbrRestarts)->default_value(10), "FLTM nbr restarts. Default: 10")
        // ("f_nbr_steps,s", po::value<int>(&result.fltm_params.nbrSteps)->default_value(7), "FLTM nbr steps. Default 7") 

        // ("f_thres_info,i", po::value<double>(&result.fltm_params.latentVarQualityThres)->default_value(0), "FLTM thres info.Default: 0")
        // ("f_thres_em,e", po::value<double>(&result.fltm_params.emThres)->default_value(0.00001), "FLTM thres EM. Default: 0.00001")


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
