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

  samogwas::FLTM_Params fltm_params;


  std::string clustering_file;
  std::string ref_clustering;
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
        ("in_dat,d", po::value<std::string>(&result.inputDataFile), "Input Data File")
        ("in_lab,l", po::value<std::string>(&result.inputLabelFile), "Input Label File")
        ("in_card,N", po::value<int>(&result.fltm_params.cardinality), "Input cardinality. Default: 3")

        ("out,o", po::value<std::string>(&result.outputDir)->default_value("./out"), "Output Dir. Default: ./out")
        ("max_dist,x", po::value<unsigned>(&result.fltm_params.maxDist)->default_value(50000), "Max Dist. Default: 50000 bp")

        // ///////////////////////////////////////////////////////////////////////////
        ("ref_clustering,r", po::value<std::string>(&result.ref_clustering), "ref_clustering")
        ("clustering_file,c", po::value<std::string>(&result.clustering_file), "clusterings")
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
  catch(std::exception& e)
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
