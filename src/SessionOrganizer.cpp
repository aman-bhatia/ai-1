/* 
 * File:   SessionOrganizer.cpp
 * Author: Harishchandra Patidar
 * 
 */

#include "SessionOrganizer.h"
#include "Util.h"

SessionOrganizer::SessionOrganizer() : conference(NULL)
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer(string filename)
{
    readInInputFile(filename);
    conference = new Conference(parallelTracks, sessionsInTrack, papersInSession);
}

void SessionOrganizer::readInInputFile(string filename)
{
    vector<string> lines;
    string line;
    ifstream myfile(filename.c_str());
    if(myfile.is_open())
    {
        while(getline(myfile, line))
        {
            //cout<<"Line read:"<<line<<endl;
            lines.push_back(line);
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open input file" << endl;
        exit(0);
    }

    if (6 > lines.size())
    {
        cout << "Not enough information given, check format of input file";
        exit(0);
    }

    processingTimeInMinutes = atof(lines[0].c_str());
    papersInSession = atoi(lines[1].c_str());
    parallelTracks = atoi(lines[2].c_str());
    sessionsInTrack = atoi(lines[3].c_str());
    tradeoffCoefficient = atof(lines[4].c_str());

    
    int n = lines.size() - 5;
    double ** tempDistanceMatrix = new double*[n];
    for(int i = 0; i < n; ++i)
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for(int i = 0; i < n; i++)
    {
        string tempLine = lines[i+5];
        string elements[n];
        splitString(tempLine, " ", elements, n);
        
        for(int j = 0; j < n; j++)
        {
            tempDistanceMatrix[i][j] = atof(elements[j].c_str());
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if(slots != numberOfPapers)
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit(0);
    }
}

double** SessionOrganizer::getDistanceMatrix()
{
    return distanceMatrix;
}

double SessionOrganizer::scoreOrganization()
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for(int i = 0; i < conference->getParallelTracks(); i++)
    {
        Track *tmpTrack = conference->getTrack(i);
        for(int j = 0; j < tmpTrack->getNumberOfSessions(); j++)
        {
            Session *tmpSession = tmpTrack->getSession(j);
            for(int k = 0; k < tmpSession->getNumberOfPapers(); k++)
            {
                int index1 = tmpSession->getPaper(k);
                for(int l = k + 1; l < tmpSession->getNumberOfPapers(); l++)
                {
                    int index2 = tmpSession->getPaper(l);
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for(int i = 0; i < conference->getParallelTracks(); i++)
    {
        Track *tmpTrack1 = conference->getTrack(i);
        for(int j = 0; j < tmpTrack1->getNumberOfSessions(); j++)
        {
            Session *tmpSession1 = tmpTrack1->getSession(j);
            for(int k = 0; k < tmpSession1->getNumberOfPapers(); k++)
            {
                int index1 = tmpSession1->getPaper(k);

                // Get competing papers.
                for(int l = i + 1; l < conference->getParallelTracks(); l++)
                {
                    Track *tmpTrack2 = conference->getTrack(l);
                    Session *tmpSession2 = tmpTrack2->getSession(j);
                    for(int m = 0; m < tmpSession2->getNumberOfPapers(); m++)
                    {
                        int index2 = tmpSession2->getPaper(m);
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}

void SessionOrganizer::printSessionOrganiser()
{
    conference->printConference();
}

void SessionOrganizer::initialOrganization()
{
    int paperCounter = 0;
    for(int i = 0; i < conference->getSessionsInTrack(); i++)
    {
        for(int j = 0; j < conference->getParallelTracks(); j++)
        {
            for(int k = 0; k < conference->getPapersInSession(); k++)
            {
                conference->setPaper(j, i, k, paperCounter);
                paperCounter++;
            }
        }
    }
}

void SessionOrganizer::swapPapers(int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2) {
    Track *track1 = conference->getTrack(trackIndex1);
    Track *track2 = conference->getTrack(trackIndex2);

    Session *session1 = track1->getSession(sessionIndex1);
    Session *session2 = track2->getSession(sessionIndex2);

    int paper1 = session1->getPaper(paperIndex1);
    int paper2 = session2->getPaper(paperIndex2);

    conference->setPaper(trackIndex1,sessionIndex1,paperIndex1,paper2);
    conference->setPaper(trackIndex2,sessionIndex2,paperIndex2,paper1);
}

void SessionOrganizer::randomizeConference(int numberOfRandomizationSteps) {
    if(numberOfRandomizationSteps == 0) 
        return;

    int trackIndex1 = rand() % parallelTracks ;
    int trackIndex2 = rand() % parallelTracks ;

    int sessionIndex1 = rand() % sessionsInTrack;
    int sessionIndex2 = rand() % sessionsInTrack;

    if(trackIndex1 == trackIndex2) {
        // sessions must be different if both tracks are same
        if(sessionIndex1 == sessionIndex2) {
            if(sessionsInTrack == 1) {
                // change the track
                do { 
                    trackIndex2 = rand() % parallelTracks;
                }while(trackIndex1 == trackIndex2);
            }else {
                // change the session
                do {
                    sessionIndex2 = rand() % sessionsInTrack;
                }while(sessionIndex1 == sessionIndex2);
            }
        }
    }

    int paperIndex1 = rand() % papersInSession;
    int paperIndex2 = rand() % papersInSession;

    swapPapers(trackIndex1, sessionIndex1, paperIndex1, trackIndex2, sessionIndex2, paperIndex2);

    return randomizeConference(numberOfRandomizationSteps-1);
}

bool SessionOrganizer::climbStep() {
    //cout<<"---------------- Climbing Step ---------------------\n";
    double currentMaximumScore = scoreOrganization();
    //cout<<"Current Score = "<<currentMaximumScore<<endl;
    bool increased = false;
    int optimalTrackIndex1,optimalTrackIndex2;
    int optimalSessionIndex1,optimalSessionIndex2;
    int optimalPaperIndex1,optimalPaperIndex2;

    for(int trackIndex1=0; trackIndex1<conference->getParallelTracks(); ++trackIndex1)
    {
        Track *track1 = conference->getTrack(trackIndex1);
        for(int sessionIndex1=0; sessionIndex1<track1->getNumberOfSessions(); ++sessionIndex1)
        {
            Session *session1 = track1->getSession(sessionIndex1);
            for(int paperIndex1=0; paperIndex1<session1->getNumberOfPapers(); ++paperIndex1)
            {
                for(int trackIndex2=trackIndex1; trackIndex2<conference->getParallelTracks(); ++trackIndex2)
                {
                    Track *track2 = conference->getTrack(trackIndex2);
                    for(int sessionIndex2=0; sessionIndex2<track2->getNumberOfSessions(); ++sessionIndex2)
                    {
                        if(trackIndex1 == trackIndex2 && sessionIndex1 == sessionIndex2) {
                            continue;
                        }
                        Session *session2 = track2->getSession(sessionIndex2);
                        for(int paperIndex2=0; paperIndex2<session2->getNumberOfPapers(); ++paperIndex2)
                        {
                            swapPapers(trackIndex1,sessionIndex1,paperIndex1,trackIndex2,sessionIndex2,paperIndex2);
                            double newScore = scoreOrganization();
                            //cout<<"New Score = "<<newScore<<endl;
                            if(newScore > currentMaximumScore) {
                                // save indexes
                                increased = true;
                                currentMaximumScore = newScore;
                                optimalTrackIndex1 = trackIndex1; optimalTrackIndex2 = trackIndex2;
                                optimalSessionIndex1 = sessionIndex1; optimalSessionIndex2 = sessionIndex2;
                                optimalPaperIndex1 = paperIndex1; optimalPaperIndex2 = paperIndex2;
                            }
                            swapPapers(trackIndex1,sessionIndex1,paperIndex1,trackIndex2,sessionIndex2,paperIndex2);
                        }
                    }
                }
            }
        }
    }

    if(increased){
        swapPapers(optimalTrackIndex1,optimalSessionIndex1,optimalPaperIndex1,optimalTrackIndex2,optimalSessionIndex2,optimalPaperIndex2);
    }else{
        ;//cout<<"Reached Local Maxima"<<endl;
    }

    return increased;
}

/**
 * Organize Papers by using Random Restart Hill Climbing Algorithm
 */
void SessionOrganizer::organizePapers()
{
    initialOrganization();

    double maximumScoreSoFar = scoreOrganization();
    Conference optimalConference = *conference;
    
    int numberOfRandomRestarts = 100;
    int numberOfRandomizationSteps = 100;

    double score;

    for(int i=0; i<numberOfRandomRestarts; ++i)
    {
        randomizeConference(numberOfRandomizationSteps);
        bool climbSuccessful;
        do
        {
            climbSuccessful = climbStep();
        }while(climbSuccessful);

        score = scoreOrganization();
        if(score > maximumScoreSoFar){
            cout << endl;
            cout << "Maximum score got increased from " << maximumScoreSoFar << " to " << score << " at random restart " << i << endl;
            cout << "Previous Maximum = " << maximumScoreSoFar << endl;
            cout << "Current Maximum = " << score << endl;
            maximumScoreSoFar = score;
            optimalConference = *conference;
        }
    }
    
    *conference = optimalConference; // make the current conference equal to the optimal conference
}