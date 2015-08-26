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
#include <unistd.h>


#include "Conference.h"
#include "Track.h"
#include "Session.h"

using namespace std;

extern time_t starting_time;


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

    /**
     * Read in the number of parallel tracks, papers in session, sessions
     * in a track, and the similarity matrix from the specified filename.
     * @param filename is the name of the file containing the matrix.
     */
    void readInInputFile(string filename);

    /*
     * Organize the papers Initially
     */
    void initializeOrganization();


    /*
     * Greedily Organize the papers Initially
     */

    void initializeGreedyOrganization();

    /**
     * swap two papers in the conference
     * @param indexes specifying the papers to be swapped
     */
    void swapPapers(Conference &conference, int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2);


    /**
     * Swap two randomly chosen papers in the conference
     */
    void swapTwoRandomPapers();


    /**
     * Calculate the score on swapping two papers
     * @param conference conference containing papers
     * @param indexes specifying two papers
     * @return score if the two papers are swapped
     */
    double getScoreOnSwapping(Conference &conference, int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2);


    bool greedyStep();


    bool greedyStep2();


    /**
     * climbing step of Hill Climbing Algorithm
     * @return true if the climb is succesful otherwise false
     */
    bool climbStep();


    /**
     * Similarity score for all the papers in a session
     * @param conference corresponding conference for the session
     * @param trackIndex is the track index
     * @param sessionIndex is the session index
     */
    double getSimilarityScoreForSession(Conference &conference, int trackIndex, int sessionIndex);

    /**
     * Parallel score for two competing sessions
     * @param conference corresponding conference for the sessions
     * @param trackIndex1 is the track index for first session
     * @param sessionIndex1 is the session index for first session
     * @param trackIndex2 is the track index for second session
     * @param sessionIndex2 is the session index for second session
     */
    double getParallelScoreBetweenSessions(Conference &conference, int trackIndex1, int sessionIndex1, int trackIndex2, int sessionIndex2);

    /**
     * Score for the session with all other parallel sessions in the same time slot
     * @param conference corresponding conference for the session
     * @param trackIndex is the track index
     * @param sessionIndex is the session index
     */
    double getParallelScoreForSession(Conference &conference, int trackIndex, int sessionIndex);


    /**
     * Score the conference.
     * @param conference for which score is to be calculated
     * @return the score of the conference.
     */
    double scoreConference(Conference &conference);

public:
    SessionOrganizer();
    SessionOrganizer(string filename);
    
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
     * Print the current organization
     */
    void printSessionOrganiser();

    /**
     * returns the score for conference
     * @return score of the conference
     */
    double getConferenceScore();

};

#endif	/* SESSIONORGANIZER_H */

