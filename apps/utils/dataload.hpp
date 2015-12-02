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
typedef std::vector<Vec> Matrix;
typedef std::shared_ptr<Matrix> MatrixPtr;

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
  return dt;
}

inline unsigned number_of_lines( const std::string& inf) {
  unsigned int numLines = 0;
  FILE *infile = fopen(inf.c_str(), "r");
  int ch;
  while (EOF != (ch=getc(infile)))
    if ('\n' == ch)
      ++numLines;
  fclose(infile);
  return numLines;
}

inline PtrMatrixPtr load_data_table( const std::string& infile,
                                     const bool transpose = false,
                                     const char& sep = ',',
                                     const char& quote = '"' ) {

  PtrMatrixPtr dt(new PtrMatrix());
  std::ifstream matrixFile(infile.c_str());
  if (!matrixFile) {
    printf("file data %s not existing\n", infile.c_str());
    exit(-1);
  }

  int nbrLines = number_of_lines( infile);
  if ( !transpose ) {
    dt->reserve(nbrLines);
    samogwas::CSVIterator<int> matrixLine(matrixFile);

    for( ; matrixLine != samogwas::CSVIterator<int>(); ++matrixLine ) {
      auto row = std::make_shared<Vec>(matrixLine->size(), 0);
      for (unsigned i = 0; i < matrixLine->size(); ++i) {
        (*row)[i] = matrixLine->at(i);
      }
      dt->push_back(row);
    }
    dt->resize(dt->size());
  } else {
    samogwas::CSVIterator<int> tmpLine(matrixFile);
    ++tmpLine;
    auto nbrColumns = tmpLine->size();
    matrixFile.clear();
    matrixFile.seekg(0, std::ios_base::beg);
    dt->reserve(nbrColumns);
    for (size_t i = 0; i < nbrColumns; ++i) { dt->push_back(std::make_shared<Vec>(nbrLines, 0)); }

    unsigned row = 0;
    samogwas::CSVIterator<int> matrixLine(matrixFile);
    for( ; matrixLine != samogwas::CSVIterator<int>(); ++matrixLine ) {
      for (unsigned col = 0; col < matrixLine->size(); ++col) {
        dt->at(col)->at(row) = matrixLine->at(col);
      }
      ++row;
    }
    dt->resize(dt->size());
  }
  return dt;
}


MatrixPtr transpose_data_matrix(const PtrMatrix& mat) {
  auto transposed_mat = std::make_shared<Matrix>();
  auto nbrCols = mat.at(0)->size();
  auto nbrRows = mat.size();
  transposed_mat->reserve(nbrCols);
  for (size_t i = 0; i < nbrCols; ++i) {
    transposed_mat->push_back(Vec(nbrRows, 0));
  }

  for (size_t row = 0; row < nbrRows; ++row) {
    for (size_t col = 0; col < nbrCols; ++col) {
      transposed_mat->at(col).at(row) = mat.at(row)->at(col);
    }
  }

  return transposed_mat;
}



//////////////////////////////////////////////////////////////////


} // namespace novo ends here. 

/****************************************************************************************/
#endif // NOVO_DATALOAD_HPP
