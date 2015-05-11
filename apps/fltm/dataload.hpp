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

namespace samogwas
{

typedef std::vector<double> RealVec;
typedef std::shared_ptr<RealVec> RealVecPtr;

typedef std::vector<RealVecPtr> RealMatrix;
typedef std::vector<int> Vec;
typedef std::shared_ptr<Vec> VecPtr;
typedef std::vector<VecPtr> PtrMatrix;
typedef std::shared_ptr<PtrMatrix> PtrMatrixPtr;
typedef std::shared_ptr<RealMatrix> RealMatrixPtr;

typedef std::vector<std::string> LabelVec;
typedef std::shared_ptr<LabelVec> LabVecPtr;

typedef std::vector<int> PosVec;
typedef std::shared_ptr<std::vector<int>> PosVecPtr;
typedef std::vector<int> IndexVec;
typedef std::shared_ptr<std::vector<int>> IndexVecPtr;



inline void load_labels_positions( LabelVec& labels,
                                   IndexVec& ids,
                                   PosVec& positions,
                                   const std::string& infile )  {
  std::ifstream labPosFile(infile.c_str());
  if (!labPosFile) {
    printf("file lab-post %s not existing\n", infile.c_str());
    exit(-1);
  }
  printf("beginning loading label...\n");
  std::vector<std::string>().swap(labels); //lab2Pos.clear();
  std::vector<int>().swap(positions); //.clear();
  CSVIterator<std::string> labPosLine(labPosFile);// ++labPosLine;
  int id = 0;
  for( ; labPosLine != CSVIterator<std::string>(); ++labPosLine ) {
    std::string label =  (*labPosLine)[2];
    int position = boost::lexical_cast<int>( (*labPosLine)[3]);
    unsigned id = boost::lexical_cast<unsigned>( (*labPosLine)[1]);
    ids.push_back(id);
    labels.push_back(label);
    positions.push_back(position);
  }
  std::cout << "done loading. loaded " << labels.size() << " variables.\n";
}


inline RealMatrixPtr load_real_data_table( const std::string& infile,
                                           const char& sep = ',',
                                           const char& quote = '"' ) {
  RealMatrixPtr dt(new RealMatrix());
  std::ifstream matrixFile(infile.c_str());
  if (!matrixFile) {
    printf("file data %s not existing\n", infile.c_str());
    exit(-1);
  }

  dt->reserve(10000);

  samogwas::CSVIterator<double> matrixLine(matrixFile);
  
  for( ; matrixLine != samogwas::CSVIterator<double>(); ++matrixLine ) {         
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


inline PtrMatrixPtr load_data_table( const std::string& infile,
                                     const char& sep = ',',
                                     const char& quote = '"' ) {

  PtrMatrixPtr dt(new PtrMatrix());
  std::ifstream matrixFile(infile.c_str());
  if (!matrixFile) {
    printf("file data %s not existing\n", infile.c_str());
    exit(-1);
  }
  dt->reserve(200000);
  samogwas::CSVIterator<int> matrixLine(matrixFile);
  
  for( ; matrixLine != samogwas::CSVIterator<int>(); ++matrixLine ) {         
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

//////////////////////////////////////////////////////////////////


} // namespace novo ends here. 

/****************************************************************************************/
#endif // NOVO_DATALOAD_HPP
