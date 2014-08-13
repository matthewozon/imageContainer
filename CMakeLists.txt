cmake_minimum_required(VERSION 2.8)

#Configuration du projet
project(imageContainer)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

#Inclusion de Boost
include_directories(. /usr/include/gdcm) #make sure this path exists on your machin
link_directories(.) # /usr/lib64)

#Configuration de l'exécutable
#file(
#     	GLOB
#	source_files
#        *.h *.cpp
#)
add_library(
        imageContainer
	SHARED

	#Name should be explicite enough. If you cannot understand or if there is any mistake, feel free to send me a mail matthew.ozon@gmail.com
	C_sort_files.cpp
	C_getDicomFromDirectory.cpp
	C_kmean3D.cpp
	C_kmean2D.cpp
	C_thread.cpp
	C_readDicomDiffData.cpp
	C_ReadAnalyze75.cpp
	C_ReadDicomDiffInfo.cpp
)

#Configuration de l'édition de liens
target_link_libraries(
        imageContainer
        #multithread from POSIX: if you don't have the right system, feel to not use it (meaning that you should comment it AND modify the code)
	pthread
	#the following are for dicom handling: read meta-data and data. If you are not using Dicom, comment them AND remove dicom related lines
        gdcmCommon
        gdcmMSFF
        gdcmIOD
        gdcmDSED
        gdcmDICT
        gdcmjpeg8
        gdcmjpeg16
        gdcmjpeg12
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O3")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()
