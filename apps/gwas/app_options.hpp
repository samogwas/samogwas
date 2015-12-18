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
  int chromosome;

  std::string bayesVertices;
  std::string bayesDist;
  std::string outputDir;

  char separator;
  int stat_test;
};


struct GWAS_Options: public Options {
  std::string thresholdFile;
  int permutations;
  std::string cndDataFile;
  std::string mappingFile;
};

//////////////////////////////////////////////////////////////////////////////
inline GWAS_Options get_gwas_program_options(int argc, char** argv) {
  GWAS_Options result;
  std::string appName = boost::filesystem::basename(argv[0]);
  po::options_description optDesc("Options");

  try  {
    /** Define and parse the program options
     */
    optDesc.add_options()
        ("help,h", "Print help messages")
        ("chr,c", po::value<int>(&result.chromosome)->default_value(0), "Chromosome. Default: 0")

        ("in_dat,i", po::value<std::string>(&result.inputDataFile)->required(), "Input Data File (output of FLTM program)")
        ("in_lab,l", po::value<std::string>(&result.inputLabelFile)->required(), "Input Label File (output of FLTM program)")
        ("mappingFile,m", po::value<std::string>(&result.mappingFile)->default_value("../example/inputs/snp_mapping.csv"), "SNP-RS Mapping File. Default: ../example/inputs/snp_mapping.csv")

        ("in_pheno,p", po::value<std::string>(&result.inputPheno)->default_value("../example/inputs/phenotype.csv"), "Input Pheno File. Default: ../example/inputs/phenotype.csv")

        ("in_bayes_vertex,v", po::value<std::string>(&result.bayesVertices)->required(), "Input Bayes Vertex File (output of the FLTM program)")
        ("in_bayes_dist,d", po::value<std::string>(&result.bayesDist)->required(), "Input Bayes Dist File (output of the FLTM program)")
        ("num_perms,n", po::value<int>(&result.permutations)->default_value(1000), "Nbr Permutations. Default: 1000")
        ("thresholdFile,r", po::value<std::string>(&result.thresholdFile)->default_value("../example/inputs/thresholds.txt"), "Thresholds. Default: ../example/inputs/thresholds.csv")

        ("test,t", po::value<int>(&result.stat_test)->default_value(0), "Statistic Test: 0 (chi-square) - Only 0 is supported for now")
        ("sep,s", po::value<char>(&result.separator)->default_value(','), "Output file deliminater. Default: ','")
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
  catch(std::exception& e)
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
