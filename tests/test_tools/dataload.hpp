/****************************************************************************************
 * File: dataload.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 31/03/2015

 ***************************************************************************************/
#ifndef NOVO_DATALOAD_HPP
#define NOVO_DATALOAD_HPP


#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <memory>

#include "utils/csv_parser.hpp"
#include "utils/matrix_utils.hpp"

namespace samogwas_test
{

typedef std::vector<double> RealVec;
// typedef std::vector<int> DataVec;
typedef std::shared_ptr<RealVec> RealVecPtr;
// typedef std::shared_ptr<DataVec> DataVecPtr;

typedef std::vector<VecPtr> PtrMatrix;
typedef std::vector<RealVecPtr> RealMatrix;

typedef std::shared_ptr<PtrMatrix> PtrMatrixPtr;
typedef std::shared_ptr<RealMatrix> RealMatrixPtr;


inline RealMatrixPtr loadRealDataTable( const std::string& infile,
                                        const char& sep = ',',
                                        const char& quote = '"' ) {

  RealMatrixPtr dt(new RealMatrix());
  std::ifstream matrixFile(infile.c_str());
  if (!matrixFile) {
    printf("file data %s not existing\n", infile.c_str());
    exit(-1);
  }

  dt->reserve(10000);

  utility::CSVIterator<double> matrixLine(matrixFile);
  
  for( ; matrixLine != utility::CSVIterator<double>(); ++matrixLine ) {         
    //std::vector<double> row(matrixLine->size(), 0);
    auto row = std::make_shared<RealVec>(matrixLine->size(), 0);
    for (unsigned i = 0; i < matrixLine->size(); ++i) {
      (*row)[i] = matrixLine->at(i);
    }
    dt->push_back(row);    
  }

  dt->resize(dt->size());
  size_t ncols = dt->empty() ? 0 : (*dt)[0]->size();

  return dt;
}


inline PtrMatrixPtr loadDataTable( const std::string& infile,
                                const char& sep = ',',
                                const char& quote = '"' ) {

  PtrMatrixPtr dt(new PtrMatrix());
  std::ifstream matrixFile(infile.c_str());
  if (!matrixFile) {
    printf("file data %s not existing\n", infile.c_str());
    exit(-1);
  }
  dt->reserve(200000);

  utility::CSVIterator<int> matrixLine(matrixFile);
  
  for( ; matrixLine != utility::CSVIterator<int>(); ++matrixLine ) {         
    //std::vector<int> row(matrixLine->size(), 0);
    auto row = std::make_shared<Vec>(matrixLine->size(), 0);

    for (unsigned i = 0; i < matrixLine->size(); ++i) {
(*row)[i] = matrixLine->at(i);
    }
    dt->push_back(row);    
  }

  dt->resize(dt->size());
  size_t ncols = dt->empty() ? 0 : (*dt)[0]->size();

  return dt;
}


} // namespace novo ends here. 

/****************************************************************************************/
#endif // NOVO_DATALOAD_HPP
