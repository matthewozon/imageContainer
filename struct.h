#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <vector>


#define DT_NONE                    0
#define DTI_UNKNOWN                0
#define DT_BINARY                  1
#define DT_UNSIGNED_CHAR          2
#define DT_SIGNED_SHORT           4
#define DT_SIGNED_INT              8
#define DT_FLOAT                   16
#define DT_COMPLEX                 32
#define DT_DOUBLE                  64
#define DT_RGB                     128
#define DT_ALL                     255


#define DICOM_ERROR 0
#define DICOM_BOOL 1
#define DICOM_CHAR 2
#define DICOM_UCHAR 4
#define DICOM_SHORT 8
#define DICOM_USHORT 16
#define DICOM_LONG 32
#define DICOM_ULONG 64
#define DICOM_FLOAT 128
#define DICOM_DOUBLE 256



struct diffInfo
{
    double g[3]; //unit gradient direction expressed in patient basis
    double echoTime; //echo time
    double repTime;  //repetition time
    double b; //b-value
    double X_slice[3]; //first vector of patient basis expressed in machine basis
    double Y_slice[3]; //second vector...
    double Z_slice[3]; //third  vector...
    double voxelSize[3]; //size of voxels (X dim, Ydim, Z dim)
};



#endif // STRUCT_H_INCLUDED
