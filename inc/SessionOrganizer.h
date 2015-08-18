/* 
 * File:   SessionOrganizer.h
 * Author: Harishchandra Patidar
 *
 */

#ifndef SESSIONORGANIZER_H
#define	SESSIONORGANIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "Conference.h"
#include "Track.h"
#include "Session.h"

using namespace std;


/**
 * SessionOrganizer reads in a similarity matrix of papers, and organizes them
 * into sessions and tracks.
 * 
 * @author Harishchandra Patidar
 *
 */
class SessionOrganizer {
private:
    double **distanceMatrix;

    int parallelTracks ;
    int papersInSession ;
    int sessionsInTrack ;

    Conference *conference;

    double processingTimeInMinutes ;
    double tradeoffCoefficient ; // the tradeoff coefficient

    /*
     * Organize the papers Initially
     */
    void initialOrganization();


    /**
     * swap two papers in the conference
     * @param indexes specifying the papers to be swapped
     */
    void swapPapers(int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2);


    /**
     * Randomize the current conference
     * @param numberOfRandomizationSteps number of random swappings to be done
     */
    void randomizeConference(int numberOfRandomizationSteps);


    /**
     * climbing step of Hill Climbing Algorithm
     * @returns whether the climb is succesful
     */
    bool climbStep();

public:
    SessionOrganizer();
    SessionOrganizer(string filename);
    
    
    /**
     * Read in the number of parallel tracks, papers in session, sessions
     * in a track, and the similarity matrix from the specified filename.
     * @param filename is the name of the file containing the matrix.
     */
    void readInInputFile(string filename);
    

    /**
     * Get the distance matrix.
     * @return the distance matrix.
     */
    double** getDistanceMatrix();


    /**
     * Organize the papers according to some algorithm.
     */
    void organizePapers();
    

    /**
     * Score the organization.
     * @return the score.
     */
    double scoreOrganization();
    

    /**
     * Print the current organization
     */
    void printSessionOrganiser();

};

#endif	/* SESSIONORGANIZER_H */

