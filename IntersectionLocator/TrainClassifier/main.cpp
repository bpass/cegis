/**
 * @file main.cpp
 *
 * $Id: main.cpp,v 1.9 2005/08/22 18:21:20 ahartman Exp $
 */

// This file is a quick program to train a classifier and save the
// training data.  This is just for my convenience, so that I don't
// have to deal with this later on.
// Ryan Stelzleni
// 10/25/04


#include <iostream>
#include <string>
#include "../Classifier.h"

void outputClassifiedImage( std::string ortho, std::string outputFile,
                            Classifier &C );
void outputClassifiedImageAsGeoTiff( const std::string& ortho, 
                                     const std::string& outputFile,
                                     Classifier &C );

int main()
{
    using std::cerr;
    using std::cout;

    const std::string rd("D:/Data/foristell_final/training/images/reprojected/roads/c2_r7.tif");
    const std::string nonrd("D:/Data/foristell_final/training/images/reprojected/nonroads/c2_r7.tif");
    const std::string outfile("D:/Data/foristell_final/training/reprojectedOutput.dat");
    const std::string testOrtho("D:/Data/foristell_final/orthoimages/reprojected/c2_r7.tif");
    const std::string testOutput("D:/Data/foristell_final/training/output/reprojected/c2_r7.tif");

    cerr << "Running\n";

    GDALAllRegister();
    GDALDataset *pRd, *pNonRd;

    pRd = static_cast<GDALDataset *>( GDALOpen( rd.c_str(), GA_ReadOnly ) );
    if( pRd == NULL )
    {
        cerr << "Failed to open rd file\n";
        return -1;
    }
    pNonRd = static_cast<GDALDataset *>( GDALOpen( nonrd.c_str(), GA_ReadOnly ) );
    if( pNonRd == NULL )
    {
        cerr, "Failed to open nonrd file\n";
        return -1;
    }

    // Comment or uncomment lines as you see fit
    Classifier C;
    //C.inputText( outfile.c_str() );
    //for( int i=0; i<5; ++i )
    //   C.addRdTraining( pRd );
    C.addRdTraining(pRd);
    C.addNonRdTraining( pNonRd );
    C.outputText( outfile.c_str() );

//    outputClassifiedImage( testOrtho, testOutput, C );
//    outputClassifiedImageAsGeoTiff( testOrtho, testOutput, C );

    return 0;
}


// take the image named ortho, classify it with C and put the result
// in a generic binary file called outputFile.  For instructions on
// viewing the output, see the doxygen documentation for the main
// project, on the page called Preparing for new data.
void outputClassifiedImage( std::string ortho, std::string outputFile,
                            Classifier &C )
{
    using std::cerr;
    using std::cout;

    GDALDataset *image;
    image = static_cast<GDALDataset *>( GDALOpen( ortho.c_str(), GA_ReadOnly ) );
    if( image == NULL )
    {
        cerr << "Failed to open image file for test run\n";
        return;
    }

    InMemRaster rasta( image );
    rasta.convertToHSV();
    rasta.classify( C );
    rasta.dumpToBinary( outputFile.c_str() );

    return;
}

// take the image named ortho, classify it with C and put the result
// in a GeoTiff file called outputFile.  View the output like you would
// any other Tiff file.
void outputClassifiedImageAsGeoTiff( const std::string& ortho, 
                                     const std::string& outputFile,
                                     Classifier &C )
{
    using std::cerr;
    const unsigned char isRoadValue = 255;
    const unsigned char isNonRoadValue = 0;

    GDALDataset *image;
    image = static_cast<GDALDataset *>( GDALOpen( ortho.c_str(), GA_ReadOnly ) );
    if( image == NULL )
    {
        cerr << "Failed to open image file for test run\n";
        return;
    }

    cerr << "Getting GeoTiff driver\n";
    GDALDriver* tiffDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
    if( tiffDriver == NULL)
    {
        cerr << "Failed to get GeoTiff driver\n";
        return;
    }

    cerr << "Creating a new output image\n";
    GDALDataset* outputImage = 
        tiffDriver->Create( outputFile.c_str(), 
                            image->GetRasterXSize(), image->GetRasterYSize(),
                            1, GDT_Byte, NULL );
    if( outputImage == NULL )
    {
        cerr << "Failed to create a new output image\n";
        return;
    }

    cerr << "Getting the projection of the input image\n";
    const std::string imageProjection = image->GetProjectionRef();
    if( imageProjection != "" )
    {
        cerr << "Setting the projection of the output image to "
             << imageProjection << "\n";
        const CPLErr setProjectionError = 
            outputImage->SetProjection( imageProjection.c_str() );

        if( setProjectionError == CE_Failure )
        {
            cerr << "An error occurred when setting the projection\n";
        }
    }
    else
    {
        cerr << "Input image had no projection information\n";
    }

    cerr << "Getting the GeoTransform of the input image\n";
    double imageGeoTransform[6];
    const CPLErr getGeoTransformError =
        image->GetGeoTransform( imageGeoTransform );
    if( getGeoTransformError == CE_Failure )
    {
        cerr << "Failed getting the geo transform of the input image\n";
    }

    cerr << "Setting the GeoTransform of the output image\n";
    const CPLErr setGeoTransformError =
        outputImage->SetGeoTransform( imageGeoTransform );
    if( setGeoTransformError == CE_Failure )
    {
        cerr << "Failed setting the geo transform of the output image\n";
    }

    cerr << "Copying the input image to an InMemRaster\n";
    InMemRaster rasta( image );
    cerr << "Changing the InMemRaster to HSV\n";
    rasta.convertToHSV();
    cerr << "Classifying the InMemRaster\n";
    rasta.classify( C );

    GDALRasterBand* band = outputImage->GetRasterBand( 1 );
    for(int j = 0; j < rasta.getYSize(); ++j)
    {
//        if( (j+1) % 100 == 0 )
//        {
//            cerr << "Writing line " << j+1 << " of the output image\n";
//        }
        for(int i = 0; i < rasta.getXSize(); ++i)
        {
            if( rasta.isRoad( i, j ) )
            {
                band->RasterIO( GF_Write, 
                                i, j,
                                1, 1,
                                &(const_cast<unsigned char&>(isRoadValue)),
                                1, 1,
                                GDT_Byte,
                                0, 0);
            }
            else
            {
                band->RasterIO( GF_Write, 
                                i, j,
                                1, 1,
                                &(const_cast<unsigned char&>(isNonRoadValue)),
                                1, 1,
                                GDT_Byte,
                                0, 0);
            }
        }
    }
    
    cerr << "Closing output image\n";
    delete outputImage; outputImage = NULL;

//    rasta.dumpToBinary( outputFile.c_str() );

    return;
}

