Introduction
============

The aim of this small library is to help you with usual operation in medical imaging community, for instance, loading dicom images and compute the simplest operations such as addition, subtraction, multiplication... Some methods are multithread using pthread (POSIX). As you may have understood, this library is written in C++ (the multithreading class is written so that it looks like C++).
Code

The library includes several classes:

*  C_ReadDicomDiffInfo
*  C_readDicomDiffData
*  C_ReadAnalyze75
*  rawData
*  C_kmean2D
*  C_kmean3D
*  C_sort_files
*  C_getDicomFromDirectory
*  C_thread
