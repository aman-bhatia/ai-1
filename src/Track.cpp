/* 
 * File:   Track.cpp
 * Author: Harishchandra Patidar
 * 
 */

#include <stdlib.h>

#include "Track.h"

Track::Track() : sessions(NULL)
{
    this->sessionsInTrack = 0;
}

Track::~Track()
{
    delete [] sessions;
}

Track::Track(int sessionsInTrack)
{
    this->sessionsInTrack = sessionsInTrack;
    sessions = new Session[sessionsInTrack];
}

Track::Track(const Track &anotherTrack)
{
    this->sessionsInTrack = anotherTrack.sessionsInTrack;
    this->sessions = new Session[this->sessionsInTrack];

    for(int i=0; i<this->sessionsInTrack; i++){
        this->sessions[i] = anotherTrack.sessions[i];
    }
}

Track & Track::operator=(const Track &anotherTrack)
{
    if(this != &anotherTrack){
        delete this->sessions;

        this->sessionsInTrack = anotherTrack.sessionsInTrack;
        this->sessions = new Session[this->sessionsInTrack];

        for(int i=0; i<this->sessionsInTrack; i++){
            this->sessions[i] = anotherTrack.sessions[i];
        }
    }
    return *this;
}

void Track::setPaper(int sessionIndex, int paperIndex, int paperId)
{
    if (sessionIndex < this->sessionsInTrack)
    {
        sessions[sessionIndex].setPaper(paperIndex,paperId);
    }
    else
    {
        cout << "Index out of bound - Track::setPaper" << endl;
        exit ( 0 );
    }
}

int Track::getNumberOfSessions()
{
    return this->sessionsInTrack;
}

Session* Track::getSession(int index)
{
    if (index < this->sessionsInTrack)
    {
        return &sessions[index];
    }
    else
    {
        cout << "Index out of bound - Track::getSession" << endl;
        exit ( 0 );
    }
}

void Track::setSession(int index, Session session)
{
    if (index < this->sessionsInTrack)
    {
        sessions[index] = session;
    }
    else
    {
        cout << "Index out of bound - Track::setSession" << endl;
        exit ( 0 );
    }
}
