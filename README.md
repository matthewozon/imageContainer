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


and it depends on the library gdcm which you can easily install on your favorite system by typing in the terminal (you must be superuser su):

  ` yum install gdcm2-devel`

ou

`  apt-get install libgdcm2-dev`

You can find the documentation at link.

If the multithreading does not suit you, don't worry, you can quite easily modify the code so that it won't bother you anymore.

Loading data
============

The project allow you to load data that are saved in two formats: DICOM and analyse7.5. In the library, it is dedicated to diffusion MRI data, but it works even though you don't have diffusion data.

DICOM
=====

The two classes handling DICOM images are C_ReadDicomDiffInfo and C_readDicomDiffData, the first dealing with meta-data and the second with the raw data. C_readDicomDiffData inherite from C_ReadDicomDiffInfo so that you only need to instantiate a data reader object to access both meta-data and data.

* meta-data The method implemented in C_ReadDicomDiffInfo are dedicated to diffusion MRI, so you can find most of the caracteristics to caracterise dMRI images (only for Siemens now, but it's not too complicated to retrive private field from other manufacturer using gdcm). For instance, you can access the voxel size, the data dimension, the gradient direction, the b-value... and so on and so forth.

*  data C_readDicomDiffData is the class to use in order to load the data. You can use all methods from C_ReadDicomDiffInfo, and you also have C_readDicomDiffData::getRawData() and C_readDicomDiffData::getUnmosaicData(ushort N) that load data into a container (rawData class). getRawData returns a container that has the exact same size as the dicom, getUnmosaicData returns a container which has one dimension more than the dicom image because it reconstruct a stack with N slices.

A piece of code for dicom loading is available here I tried it on several fedora machine, it works, but I can not guaranty that it would be the same for all OSs.


Analyse7.5 (or related format)
==============================

Coming soon... or not: explanations of methods link


Data container
==============

rawData is a template class that aims to make data manipulation in arrays easy... as much as possible. Because it is a template class, it can handle all type of data usual data. It is limited to 4 dimensions data because most images are in 1 to 4 dimensions... but if you need more dimensions, feel free to add some.

*  attributes:
  * numDim: number of dimension
  * DimX, DimY, DimZ and DimT: the number of elements of each dimension
  * DICOM_TYPE: the type of data
  * raw1D, raw2D, raw3D and raw4D: the actual data (not that only one pointer is not null)
  * pixDimX, pixDimY, pixDimZ and pixDimT: the physical size of an image element.

Extra operation on data
=======================

Coming soon: explanations of methods


File helper
===========

Coming soon: explanations of methods


Multithread class: how can I use it in my code?
===============================================

Coming soon: explanations of methods
====================================
