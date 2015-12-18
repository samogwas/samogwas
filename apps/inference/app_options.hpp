/****************************************************************************************
 * File: app_options.hpp
 * Description:
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/05/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_APP_OPTIONS_HPP
#define SAMOGWAS_APP_OPTIONS_HPP

#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name
#include "utils/option_printer.hpp"
#include "utils/custom_option_desc.hpp"

namespace po = boost::program_options;


struct Options {
  /* Input */
  std::string inputDataFile;
  std::string inputImputedDataFile;
  std::string inputLabelFile;
  std::string inputPheno;
  std::string bayesVertices;
  std::string cndDataFile;
  std::string bayesDist;
  std::string outputDir;
};



//////////////////////////////////////////////////////////////////////////////
inline Options get_program_options(int argc, char** argv) {
  Options result;
  std::string appName = boost::filesystem::basename(argv[0]);
  po::options_description optDesc("Options");

  try  {
    /** Define and parse the program options
     */
    optDesc.add_options()
        ("help,h", "Print help messages")
        ("in_dat,i", po::value<std::string>(&result.inputDataFile)->required(), "Input Data File (output of FLTM's learning program)")
        ("in_lab,l", po::value<std::string>(&result.inputLabelFile)->required(), "Input Label File (output of FLTM's learning program)")
        ("in_bayes_vertex,v", po::value<std::string>(&result.bayesVertices)->required(), "Input Bayes File (output of FLTM's learning program)")
        ("in_bayes_dist,d", po::value<std::string>(&result.bayesDist)->required(), "Input Dist File (output of FLTM's learning program)")
        ("outDir,o", po::value<std::string>(&result.outputDir)->default_value("./out"), "Output Dir. Default: ./out")
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


/****************************************************************************************/
#endif // SAMOGWAS_APP_OPTIONS_HPP
