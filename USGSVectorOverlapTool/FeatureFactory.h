/**
 * @file FeatureFactory.h
 * $Id: FeatureFactory.h,v 1.1 2004/12/10 18:54:30 ahartman Exp $
 */

#ifndef VO_FEATUREFACTORY_H
#define VO_FEATUREFACTORY_H

class Feature;

class FeatureFactory
{
    public:
        FeatureFactory();

        /**
         * This takes a well-known binary stream and converts it to a feature.
         * @param wkbStream A pointer to geometry represented in the
         * well-known binary format.
         * @param streamSize The size of the array that wkbStream points to.
         * @return A pointer to a newly allocated feature constructed from
         * wkbStream or NULL if the operation failed.
         */
        Feature* createFeature(unsigned char* wkbStream, size_t streamSize);
};

#endif

