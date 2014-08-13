#include "C_ReadDicomDiffInfo.h"

C_ReadDicomDiffInfo::C_ReadDicomDiffInfo(string fileName)
{
    //set the file name for all reader attribute
    reader.SetFileName( fileName.c_str() );
    mImageReader.SetFileName( fileName.c_str() );
    //mPixmapReader.SetFileName( fileName.c_str() );
    //cout << "nb rows " << mPixmapReader.GetPixmap().GetRows() << " nb columns " << mPixmapReader.GetPixmap().GetColumns() << endl; //OK

    if(!reader.Read() || !mImageReader.Read()) //fill the readers with image information
    {
        cerr << "Failed to read: " << fileName << endl;
    }
    else
    {
        //fill containers
        ds = reader.GetFile().GetDataSet();
        mFile = reader.GetFile();
        mImage = mImageReader.GetImage();

        ///get image dimension
        //unsigned int ndim = mImage.GetNumberOfDimensions(); //OK
        unsigned int *dims = (unsigned int*) mImage.GetDimensions(); //OK do not delocated
        //cout << "nb of dim: " << ndim << endl;
        if(dims==NULL)
        {
            cout << "dims not available" << endl;
        }
    }

    //ctor
}

C_ReadDicomDiffInfo::~C_ReadDicomDiffInfo()
{
    //mFile.~File();
    //mImage.~Image();
    mImageReader.~ImageReader();
    reader.~Reader();
    //dtor
}


string C_ReadDicomDiffInfo::getSequenceVariant()
{
    gdcm::Attribute<0x18,0x21> at4;
    at4.SetFromDataElement( ds.GetDataElement( at4.GetTag() ) );
    //cout << at4.GetValue() << endl;
    return at4.GetValue();
}
bool C_ReadDicomDiffInfo::signedPixelRepresentation()
{
    gdcm::Attribute<0x28,0x103> at4;
    at4.SetFromDataElement( ds.GetDataElement( at4.GetTag() ) );
    //cout << "pixel representation " << at4.GetValue() << endl; //0 for unsigned, 1 for signed
    return (at4.GetValue()==1);
}

unsigned short C_ReadDicomDiffInfo::getPixelRepresentation()
{
    ///get pixel encoding information
    if(gdcm::PixelFormat::UINT8 == mImage.GetPixelFormat()) //OK
    {
        //cout << "UNSIGNED CHAR" << endl;
        return DICOM_UCHAR;
    }
    else if(gdcm::PixelFormat::UINT16 == mImage.GetPixelFormat()) //OK
    {
        //cout << "UNSIGNED SHORT" << endl;
        return DICOM_USHORT;
    }
    else if(gdcm::PixelFormat::UINT32 == mImage.GetPixelFormat())
    {
        //cout << "UNSIGNED LONG" << endl;
        return DICOM_ULONG;
    }
    else if(gdcm::PixelFormat::INT8 == mImage.GetPixelFormat())
    {
        //cout << "CHAR" << endl;
        return DICOM_CHAR;
    }
    else if(gdcm::PixelFormat::INT16 == mImage.GetPixelFormat())
    {
        //cout << "SHORT" << endl;
        return DICOM_SHORT;
    }
    else if(gdcm::PixelFormat::INT32 == mImage.GetPixelFormat())
    {
        //cout << "LONG" << endl;
        return DICOM_LONG;
    }
    else if(gdcm::PixelFormat::SINGLEBIT == mImage.GetPixelFormat())
    {
        //cout << "BOOL" << endl;
        return DICOM_BOOL;
    }
    else if(gdcm::PixelFormat::FLOAT32 == mImage.GetPixelFormat())
    {
        //cout << "FLOAT" << endl;
        return DICOM_FLOAT;
    }
    else if(gdcm::PixelFormat::FLOAT64 == mImage.GetPixelFormat())
    {
        //cout << "DOUBLE" << endl;
        return DICOM_DOUBLE;
    }
    else if(gdcm::PixelFormat::UNKNOWN == mImage.GetPixelFormat())
    {
        //cout << "UNKNOWN" << endl;
        return DICOM_ERROR;
    }
    else
    {
        //gdcm::PixelFormat::FLOAT16;
        //gdcm::PixelFormat::INT12; //compressed mode?
        //gdcm::PixelFormat::UINT12; //compressed mode?
        //cout << "DEFAULT ERROR VALUE" << endl;
        return DICOM_ERROR;
    }
}





double** C_ReadDicomDiffInfo::getChangeBasisMatrix()
{
    gdcm::Attribute<0x20,0x37> at4;
    at4.SetFromDataElement( ds.GetDataElement( at4.GetTag() ) );

    double** PMachineToSlice = new double*[3];
    if(PMachineToSlice==NULL)
    {
        return NULL;
    }
    PMachineToSlice[0] = new double[3];
    PMachineToSlice[1] = new double[3];
    PMachineToSlice[2] = new double[3];
    if( (PMachineToSlice[0]==NULL) | (PMachineToSlice[1]==NULL) | (PMachineToSlice[2]==NULL) )
    {
        if(PMachineToSlice[0]!=NULL) delete PMachineToSlice[0];
        if(PMachineToSlice[1]!=NULL) delete PMachineToSlice[1];
        if(PMachineToSlice[2]!=NULL) delete PMachineToSlice[2];
        return NULL;
    }

    try
    {
        ///first row is the first basis vector of patient basis written in machine coordinates
        PMachineToSlice[0][0] = at4.GetValue(0); PMachineToSlice[0][1] = at4.GetValue(1); PMachineToSlice[0][2] = at4.GetValue(2);
        ///second row is the second basis vector of patient basis written in machine coordinates
        PMachineToSlice[1][0] = at4.GetValue(3); PMachineToSlice[1][1] = at4.GetValue(4); PMachineToSlice[1][2] = at4.GetValue(5);

        ///third row is the cross product of the to first rows (last vector must be orthogonal to the 2 first)
        PMachineToSlice[2][0] = PMachineToSlice[0][1]*PMachineToSlice[1][2] - PMachineToSlice[1][1]*PMachineToSlice[0][2];
        PMachineToSlice[2][1] = PMachineToSlice[0][2]*PMachineToSlice[1][0] - PMachineToSlice[0][0]*PMachineToSlice[1][2];
        PMachineToSlice[2][2] = PMachineToSlice[0][0]*PMachineToSlice[1][1] - PMachineToSlice[1][0]*PMachineToSlice[0][1];
    }
    catch(...)///there is probably some errors to catch, but I don't know which ones
    {
        delete PMachineToSlice[0];
        delete PMachineToSlice[1];
        delete PMachineToSlice[2];
        delete PMachineToSlice;
        PMachineToSlice = NULL;
    }

    return PMachineToSlice;

}






double* C_ReadDicomDiffInfo::getGradientDirection()
{
    gdcm::Tag tt(0x0019,0x100E);
    if(!ds.FindDataElement( tt ))
    {
        cout << "element not found" << endl;
        return NULL;
    }
    const gdcm::ByteValue* mbv = ds.GetDataElement(tt).GetByteValue();
    char* buf = new char[mbv->GetLength()];
    mbv->GetBuffer(buf, mbv->GetLength());
    return (double*) buf;
}


double* C_ReadDicomDiffInfo::getGradientDirectionSliceBasis()
{
    double* Gmachine = this->getGradientDirection();
    if(Gmachine==NULL) return NULL;
    double** P = this->getChangeBasisMatrix();
    if(P==NULL)
    {
        delete Gmachine;
        return NULL;
    }

    double* Gslice = new double[3];
    if(Gslice==NULL)
    {
        delete P[0];
        delete P[1];
        delete P[2];
        delete P;
        delete Gmachine;
        return NULL;
    }

    //mat prod
    Gslice[0] = P[0][0]*Gmachine[0] + P[0][1]*Gmachine[1] + P[0][2]*Gmachine[2];
    Gslice[1] = P[1][0]*Gmachine[0] + P[1][1]*Gmachine[1] + P[1][2]*Gmachine[2];
    Gslice[2] = P[2][0]*Gmachine[0] + P[2][1]*Gmachine[1] + P[2][2]*Gmachine[2];

    delete Gmachine;
    return Gslice;
}





double C_ReadDicomDiffInfo::getBValue()
{
    gdcm::Tag tt(0x0019,0x100c);
    if(!ds.FindDataElement( tt ))
    {
        cout << "element not found" << endl;
        return -1.0;
    }
    const gdcm::ByteValue* mbv = ds.GetDataElement(tt).GetByteValue();
    char* buf = new char[mbv->GetLength()];
    mbv->GetBuffer(buf, mbv->GetLength());
    double mb = atof(buf);
    delete buf;
    return mb;
}
double C_ReadDicomDiffInfo::getEchoTime()
{
    gdcm::Attribute<0x18,0x81> at3;
    at3.SetFromDataElement( ds.GetDataElement( at3.GetTag() ) );
    return at3.GetValue();
}
double C_ReadDicomDiffInfo::getRepetitionTime()
{
    gdcm::Attribute<0x18,0x80> at3;
    at3.SetFromDataElement( ds.GetDataElement( at3.GetTag() ) );
    return at3.GetValue();
}
unsigned short C_ReadDicomDiffInfo::getBitsAllocated()
{
    gdcm::Attribute<0x28,0x100> at3;
    at3.SetFromDataElement( ds.GetDataElement( at3.GetTag() ) );
    return at3.GetValue();
}
unsigned long C_ReadDicomDiffInfo::getNumberOfRows()
{
    gdcm::Attribute<0x28,0x10> at3;
    at3.SetFromDataElement( ds.GetDataElement( at3.GetTag() ) );
    return at3.GetValue();
}
unsigned long C_ReadDicomDiffInfo::getNumberOfColumns()
{
    gdcm::Attribute<0x28,0x11> at3;
    at3.SetFromDataElement( ds.GetDataElement( at3.GetTag() ) );
    return at3.GetValue();
}
double C_ReadDicomDiffInfo::getPixelDimX()
{
    try
    {
        gdcm::Attribute<0x28,0x30> at5;
        if(!ds.IsEmpty())
        {
            if(!ds.GetDataElement(at5.GetTag()).IsEmpty())
            {
                at5.SetFromDataElement( ds.GetDataElement( at5.GetTag() ) );
            }
            else
            {
                return -1.0;
            }
        }
        else
        {
            return -1.0;
        }
        return at5.GetValue(0);
    }
    catch(...)
    {
        cout << "bad pixDimX" << endl;
        return -1.0;
    }

}
double C_ReadDicomDiffInfo::getPixelDimY()
{
    try
    {
        gdcm::Attribute<0x28,0x30> at5;
        if(!ds.IsEmpty())
        {
            if(!ds.GetDataElement(at5.GetTag()).IsEmpty())
            {
                at5.SetFromDataElement( ds.GetDataElement( at5.GetTag() ) );
            }
            else
            {
                return -1.0;
            }
        }
        else
        {
            return -1.0;
        }
        return at5.GetValue(1);
    }
    catch(...)
    {
        cout << "bad pixDimX" << endl;
        return -1.0;
    }

}
double C_ReadDicomDiffInfo::getPixelDimZ()
{

    try
    {
        gdcm::Attribute<0x18,0x88> at5;
        if(!ds.IsEmpty())
        {
            if(!ds.GetDataElement(at5.GetTag()).IsEmpty())
            {
                at5.SetFromDataElement( ds.GetDataElement( at5.GetTag() ) );
            }
            else
            {
                return -1.0;
            }
        }
        else
        {
            return -1.0;
        }
        return at5.GetValue();
    }
    catch(...)
    {
        cout << "bad pixDimX" << endl;
        return -1.0;
    }

}
double* C_ReadDicomDiffInfo::getPixelDim()
{
    double* DIM = new double[3];
    if(DIM==NULL) return NULL;
    DIM[0] = this->getPixelDimX();
    DIM[1] = this->getPixelDimY();
    DIM[2] = this->getPixelDimZ();
    return DIM;
}
unsigned long C_ReadDicomDiffInfo::getAcquisitionMatrixNumberOfRows()
{
    gdcm::Attribute<0x18,0x1310> at5;
    at5.SetFromDataElement( ds.GetDataElement( at5.GetTag() ) );
    return at5.GetValue(1);
}
unsigned long C_ReadDicomDiffInfo::getAcquisitionMatrixNumberOfColumns()
{
    gdcm::Attribute<0x18,0x1310> at5;
    at5.SetFromDataElement( ds.GetDataElement( at5.GetTag() ) );
    return at5.GetValue(2);
}



unsigned short C_ReadDicomDiffInfo::getNumberOfSlice()
{
    gdcm::Tag tt(0x0019,0x100a);
    if(!ds.FindDataElement( tt ))
    {
        cout << "element not found" << endl;
        return -1;
    }
    const gdcm::ByteValue* mbv = ds.GetDataElement(tt).GetByteValue();
    return *((unsigned short*) mbv->GetPointer());

}

string C_ReadDicomDiffInfo::getPhotometricInterpretation()
{
    //mImage.GetPhotometricInterpretation() == gdcm::PhotometricInterpretation::RGB
    gdcm::Attribute<0x28,0x4> at5;
    at5.SetFromDataElement( ds.GetDataElement( at5.GetTag() ) );
    return at5.GetValue();
}


