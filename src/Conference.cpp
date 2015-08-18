/* 
 * File:   Conference.cpp
 * Author: Harishchandra Patidar
 * 
 */

#include "Conference.h"

Conference::Conference() : tracks(NULL)
{
    this->parallelTracks = 0;
    this->sessionsInTrack = 0;
    this->papersInSession = 0;
}

Conference::~Conference()
{
    delete [] tracks;
}

Conference::Conference(int parallelTracks, int sessionsInTrack, int papersInSession)
{
    this->parallelTracks = parallelTracks;
    this->sessionsInTrack = sessionsInTrack;
    this->papersInSession = papersInSession;
    initTracks(parallelTracks, sessionsInTrack, papersInSession);
}

Conference::Conference(const Conference &anotherConference)
{
    this->parallelTracks = anotherConference.parallelTracks;
    this->sessionsInTrack = anotherConference.sessionsInTrack;
    this->papersInSession = anotherConference.papersInSession;

    this->tracks = new Track[this->parallelTracks];

    for(int i=0; i<this->parallelTracks; i++){
        this->tracks[i] = anotherConference.tracks[i];
    }
}

Conference & Conference::operator=(const Conference &anotherConference)
{
    if(this != &anotherConference){
        delete [] tracks;

        this->parallelTracks = anotherConference.parallelTracks;
        this->sessionsInTrack = anotherConference.sessionsInTrack;
        this->papersInSession = anotherConference.papersInSession;

        this->tracks = new Track[this->parallelTracks];

        for(int i=0; i<this->parallelTracks; i++){
            this->tracks[i] = anotherConference.tracks[i];
        }
    }
    return *this;
}

void Conference::initTracks(int parallelTracks, int sessionsInTrack, int papersInSession)
{
    tracks = new Track[parallelTracks];
    for(int i=0; i<parallelTracks; i++)
    {
        Track tempTrack(sessionsInTrack);
        for (int j=0; j<sessionsInTrack; j++)
        {
            Session tempSession(papersInSession);
            tempTrack.setSession(j,tempSession);
        }
        tracks[i] = tempTrack;
    }
}

int Conference::getParallelTracks()
{
    return parallelTracks;
}

int Conference::getSessionsInTrack()
{
    return sessionsInTrack;
}

int Conference::getPapersInSession()
{
    return papersInSession;
}

Track* Conference::getTrack(int index)
{
    if (index < parallelTracks)
    {
        return &tracks[index];
    }
    else
    {
        cout << "Index out of bound - Conference::getTrack" << endl;
        exit ( 0 );
    }
}

void Conference::setPaper(int trackIndex, int sessionIndex, int paperIndex, int paperId)
{
    if (this->parallelTracks > trackIndex)
    {
        tracks[trackIndex].setPaper(sessionIndex, paperIndex, paperId);
    }
    else
    {
        cout << "Index out of bound - Conference::setPaper" << endl;
        exit ( 0 );
    }
}

void Conference::printConference()
{
    for(int i = 0; i < sessionsInTrack; i++)
    {
        for(int j = 0; j < parallelTracks; j++)
        {
            for(int k = 0; k < papersInSession; k++)
            {
                cout<< tracks[j].getSession(i)->getPaper(k) << " ";
            }
            if(j != parallelTracks - 1)
            {
                cout<< "| ";
            }
        }
        cout<< "\n";
    }
}
