/* 
 * File:   Conference.h
 * Author: Harishchandra Patidar
 *
 * Created on 18 August, 2015, 1:00 PM
 */

#ifndef CONFERENCE_H
#define	CONFERENCE_H

#include<iostream>
using namespace std;

#include "Track.h"

class Conference {
private:
    // The array of tracks.
    Track *tracks;

    // The number of parallel tracks.
    int parallelTracks;

    // The number of sessions in a track.
    int sessionsInTrack;

    // The number of papers in a session.
    int papersInSession;

    // score of the conference
    double score;

public:
    Conference();
    ~Conference();
    
    /**
     * Constructor for Conference.
     * 
     * @param parallelTracks is the number of parallel tracks.
     * @param sessionsInTrack is the number of sessions in a track.
     * @param papersInSession is the number of papers in a session.
     */
    Conference(int parallelTracks, int sessionsInTrack, int papersInSession);

    /**
     * Copy Constructor
     */
    Conference(const Conference& anotherConference);

    /**
     * Assignment Operator
     */
    Conference & operator=(const Conference &anotherConference);

    /**
     * Less than Operator
     */
    bool operator < (const Conference &rhs);
    
    
    /**
     * Initialize the tracks.
     * @param parallelTracks is the number of parallel tracks.
     * @param sessionsInTrack is the number of sessions in a track.
     * @param papersInSession is the number of papers in a session.
     */
    void initTracks(int parallelTracks, int sessionsInTrack, int papersInSession);
    
    
    /**
     * returns the number of parallel tracks.
     * @return the number of parallel tracks.
     */
    int getParallelTracks();
    
    
    /**
     * returns the number of sessions in a track.
     * @return the number of sessions in a track.
     */
    int getSessionsInTrack();
    
    
    /**
     * returns the number of papers in a session.
     * @return the number of papers in a session.
     */
    int getPapersInSession();

    /**
     * returns the score of the conference
     * @return the score of the conference
     */
    double getScore();

    /**
     * increase the score of conference
     * @param deltaScore value to be added to score
     */
    void increaseScore(double deltaScore);
    
    
    /**
     * Gets the track at the specified index.
     * @param trackIndex the index of the specified track.
     * @return the track
     */
    Track* getTrack(int trackIndex);

    /**
     * Gets the session specified by indexes
     * @param trackIndex the index of the track
     * @param sessionIndex the index of the specified session
     * @return the session
     */
    Session* getSession(int trackIndex, int sessionIndex);

    /**
     * Gets the paper specified by indexes
     * @param trackIndex the index of the track
     * @param sessionIndex the index of the session
     * @param paperIndex the index of the specified paper
     * @return the paper
     */
    int getPaper(int trackIndex, int sessionIndex, int paperIndex);
    
    
    /**
     * Sets the paper in the specified slot to the given paper id.
     * @param trackIndex the index of the track
     * @param sessionIndex the index of the session
     * @param paperIndex the index of the paper
     * @param paperId the id of the paper.
     */
    void setPaper(int trackIndex, int sessionIndex, int paperIndex, int paperId);
    
    /**
     * Prints the conference
     */
    void printConference();
};

#endif	/* CONFERENCE_H */

