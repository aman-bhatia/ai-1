/* 
 * File:   SessionOrganizer.cpp
 * Author: Harishchandra Patidar
 * 
 */

#include <algorithm> // for std::random_shuffle
#include <map>
#include <vector>
#include <limits.h>

#include "SessionOrganizer.h"
#include "Util.h"


SessionOrganizer::SessionOrganizer() : conference(NULL)
{
	parallelTracks = 0;
	papersInSession = 0;
	sessionsInTrack = 0;
	processingTimeInMinutes = 0;
	tradeoffCoefficient = 1.0;
	outputFileName = "output.txt";
	globalMaximumScore = 0.0;
}

SessionOrganizer::SessionOrganizer(string inputFileName, string outputFileName, time_t starting_time)
{
	this->outputFileName = outputFileName;
	this->starting_time = starting_time;
	readInInputFile(inputFileName);
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

double SessionOrganizer::getSimilarityScoreForSession(Conference &conference, int trackIndex, int sessionIndex)
{
	double score = 0.0;
	Session *session = conference.getSession(trackIndex,sessionIndex);

	for(int i=0; i<session->getNumberOfPapers(); i++)
	{
		int paper1 = session->getPaper(i);
		for(int j=i+1; j<session->getNumberOfPapers(); j++)
		{
			int paper2 = session->getPaper(j);
			score += 1.0 - distanceMatrix[paper1][paper2];
		}
	}

	return score;
}

double SessionOrganizer::getParallelScoreBetweenSessions(Conference &conference, int trackIndex1, int sessionIndex1, int trackIndex2, int sessionIndex2)
{
	double score = 0.0;

	Session *session1 = conference.getSession(trackIndex1, sessionIndex1);
	Session *session2 = conference.getSession(trackIndex2, sessionIndex2);

	for(int paperIndex1 = 0; paperIndex1 < session1->getNumberOfPapers(); paperIndex1++)
	{
		int paper1 = session1->getPaper(paperIndex1);
		for(int paperIndex2 = 0; paperIndex2 < session2->getNumberOfPapers(); paperIndex2++)
		{
			int paper2 = session2->getPaper(paperIndex2);
			score += distanceMatrix[paper1][paper2];
		}
	}

	return score;
}

double SessionOrganizer::getParallelScoreForSession(Conference &conference, int trackIndex, int sessionIndex)
{
	double score = 0.0;

	for(int trackIndex2 = 0; trackIndex2 < conference.getParallelTracks(); trackIndex2++)
	{
		if(trackIndex2 != trackIndex){
			score += this->getParallelScoreBetweenSessions(conference,trackIndex,sessionIndex,trackIndex2,sessionIndex);
		}
	}

	return score;
}

double SessionOrganizer::scoreConference(Conference &conference)
{
	// Sum of pairwise similarities per session.
	double score1 = 0.0;
	for(int trackIndex = 0; trackIndex < conference.getParallelTracks(); trackIndex++)
	{
		Track *tmpTrack = conference.getTrack(trackIndex);
		for(int sessionIndex = 0; sessionIndex < tmpTrack->getNumberOfSessions(); sessionIndex++)
		{
			score1 += getSimilarityScoreForSession(conference, trackIndex, sessionIndex);
		}
	}

	// Sum of distances for competing papers.
	double score2 = 0.0;
	for(int trackIndex1 = 0; trackIndex1 < conference.getParallelTracks(); trackIndex1++)
	{
		for(int sessionIndex1 = 0; sessionIndex1 < conference.getSessionsInTrack(); sessionIndex1++)
		{
			for(int trackIndex2 = trackIndex1 + 1; trackIndex2 < conference.getParallelTracks(); trackIndex2++)
			{
				score2 += this->getParallelScoreBetweenSessions(conference,trackIndex1,sessionIndex1,trackIndex2,sessionIndex1);
			}
		}
	}

	double score = score1 + tradeoffCoefficient*score2;
	conference.increaseScore(score - conference.getScore());

	return score;
}

void SessionOrganizer::printSessionOrganiser()
{
	conference->printConference();
}

double SessionOrganizer::getConferenceScore()
{
	return conference->getScore();
}

void SessionOrganizer::initializeOrganization(Conference *conference)
{
	
	int totalNumberOfPapers = parallelTracks * sessionsInTrack * papersInSession;

	vector<int> v;
	for(int i=0; i<totalNumberOfPapers; i++){
		v.push_back(i);
	}

	std::random_shuffle(v.begin(),v.end());

	int paperCounter = 0;
	for(int i = 0; i < conference->getSessionsInTrack(); i++)
	{
		for(int j = 0; j < conference->getParallelTracks(); j++)
		{
			for(int k = 0; k < conference->getPapersInSession(); k++)
			{
				conference->setPaper(j, i, k, v[paperCounter]);
				paperCounter++;
			}
		}
	}

	// update the score
	double score = scoreConference(*conference);
}

void SessionOrganizer::swapPapers(Conference &conference, int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2)
{
	int paper1 = conference.getPaper(trackIndex1, sessionIndex1, paperIndex1);
	int paper2 = conference.getPaper(trackIndex2, sessionIndex2, paperIndex2);
	
	double delta = 0.0;
	delta -= getSimilarityScoreForSession(conference, trackIndex1, sessionIndex1);
	delta -= getSimilarityScoreForSession(conference, trackIndex2, sessionIndex2);
	delta -= tradeoffCoefficient * getParallelScoreForSession(conference, trackIndex1, sessionIndex1);
	delta -= tradeoffCoefficient * getParallelScoreForSession(conference, trackIndex2, sessionIndex2);

	if(sessionIndex1 == sessionIndex2)
	{
		delta += tradeoffCoefficient * getParallelScoreBetweenSessions(conference, trackIndex1, sessionIndex1, trackIndex2, sessionIndex2);
	}

	conference.setPaper(trackIndex1,sessionIndex1,paperIndex1,paper2);
	conference.setPaper(trackIndex2,sessionIndex2,paperIndex2,paper1);

	delta += getSimilarityScoreForSession(conference, trackIndex1, sessionIndex1);
	delta += getSimilarityScoreForSession(conference, trackIndex2, sessionIndex2);
	delta += tradeoffCoefficient * getParallelScoreForSession(conference, trackIndex1, sessionIndex1);
	delta += tradeoffCoefficient * getParallelScoreForSession(conference, trackIndex2, sessionIndex2);
	
	if(sessionIndex1 == sessionIndex2)
	{
		delta -= tradeoffCoefficient * getParallelScoreBetweenSessions(conference, trackIndex1, sessionIndex1, trackIndex2, sessionIndex2);
	}

	conference.increaseScore(delta);
}

void SessionOrganizer::swapTwoRandomPapers(Conference *conference) {
	
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
				} while (trackIndex1 == trackIndex2);
			}else {
				// change the session
				do {
					sessionIndex2 = rand() % sessionsInTrack;
				} while (sessionIndex1 == sessionIndex2);
			}
		}
	}

	int paperIndex1 = rand() % papersInSession;
	int paperIndex2 = rand() % papersInSession;

	swapPapers(*conference, trackIndex1, sessionIndex1, paperIndex1, trackIndex2, sessionIndex2, paperIndex2);
}

double SessionOrganizer::getScoreOnSwapping(Conference &conference, int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2)
{
	// first swap papers
	swapPapers(conference,trackIndex1,sessionIndex1,paperIndex1,trackIndex2,sessionIndex2,paperIndex2);
	
	// calculate the resulting score
	double score = conference.getScore();
	
	// now unswap
	swapPapers(conference,trackIndex1,sessionIndex1,paperIndex1,trackIndex2,sessionIndex2,paperIndex2);
	
	return score;
}

bool SessionOrganizer::greedyStep()
{
	double currentMaximumScore = conference->getScore();
	//cout<<"Current Score = "<<currentMaximumScore<<endl;
	bool increased = false;
	int optimalTrackIndex1,optimalTrackIndex2;
	int optimalSessionIndex1,optimalSessionIndex2;
	int optimalPaperIndex1,optimalPaperIndex2;

	for(int trackIndex1=0; trackIndex1<parallelTracks; ++trackIndex1)
	{

		for(int sessionIndex1=0; sessionIndex1<sessionsInTrack; ++sessionIndex1)
		{
			
			for(int trackIndex2=trackIndex1; trackIndex2<parallelTracks; ++trackIndex2)
			{
				
				int sessionIndex2;
				if(trackIndex1 == trackIndex2){
					sessionIndex2 = sessionIndex1 + 1;
				}else{
					sessionIndex2 = 0;
				}

				for(sessionIndex2 ; sessionIndex2<sessionsInTrack; ++sessionIndex2)
				{
					
					for(int paperIndex1=0; paperIndex1<papersInSession; ++paperIndex1){
						int index1 = paperIndex1;
						//int index1 = rand() % papersInSession;
						int index2 = rand() % papersInSession;

						//for(int index2 = 0; index2<papersInSession; index2++){
							double newScore = getScoreOnSwapping(*conference,trackIndex1,sessionIndex1,index1,trackIndex2,sessionIndex2,index2);
							//cout<<"New Score = "<<newScore<<endl;
							if(newScore > currentMaximumScore) {
								// save indexes
								increased = true;
								currentMaximumScore = newScore;
								optimalTrackIndex1 = trackIndex1; optimalTrackIndex2 = trackIndex2;
								optimalSessionIndex1 = sessionIndex1; optimalSessionIndex2 = sessionIndex2;
								optimalPaperIndex1 = index1; optimalPaperIndex2 = index2;
							}
						//}
					}
				}
			}
		}
	}

	if(increased){
		swapPapers(*conference,optimalTrackIndex1,optimalSessionIndex1,optimalPaperIndex1,optimalTrackIndex2,optimalSessionIndex2,optimalPaperIndex2);
		//cout << conference->getScore() << endl;
	}else{
		;//cout<<"Reached Local Maxima"<<endl;
	}

	return increased;
}

/*
 * Swap papers only in time slot
 */
bool SessionOrganizer::greedyStep2()
{
	double currentMaximumScore = conference->getScore();
	//cout<<"Current Score = "<<currentMaximumScore<<endl;
	bool increased = false;
	int optimalTrackIndex1,optimalTrackIndex2;
	int optimalSessionIndex1;
	int optimalPaperIndex1,optimalPaperIndex2;

	for(int sessionIndex1=0; sessionIndex1<sessionsInTrack; ++sessionIndex1)
	{
		for(int trackIndex1=0; trackIndex1<parallelTracks; ++trackIndex1)
		{

			for(int trackIndex2=trackIndex1+1; trackIndex2<parallelTracks; ++trackIndex2)
			{

				for(int paperIndex1=0; paperIndex1<papersInSession; ++paperIndex1){
					int index1 = paperIndex1;

					for(int index2 = 0; index2<papersInSession; index2++){
						double newScore = getScoreOnSwapping(*conference,trackIndex1,sessionIndex1,index1,trackIndex2,sessionIndex1,index2);
						
						if(newScore > currentMaximumScore) {
							// save indexes
							increased = true;
							currentMaximumScore = newScore;
							optimalTrackIndex1 = trackIndex1; optimalTrackIndex2 = trackIndex2;
							optimalSessionIndex1 = sessionIndex1;
							optimalPaperIndex1 = index1; optimalPaperIndex2 = index2;
						}
					}
				}
			}
		}
	}

	if(increased){
		swapPapers(*conference,optimalTrackIndex1,optimalSessionIndex1,optimalPaperIndex1,optimalTrackIndex2,optimalSessionIndex1,optimalPaperIndex2);
	}else{
		;//cout<<"Reached Local Maxima"<<endl;
	}

	return increased;
}

void SessionOrganizer::randomSwapping(int maximumIterations){

	double maximumScore = conference->getScore();
	int iterations = 0;

	for(; iterations < maximumIterations ; iterations++){
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
					} while (trackIndex1 == trackIndex2);
				}else {
					// change the session
					do {
						sessionIndex2 = rand() % sessionsInTrack;
					} while (sessionIndex1 == sessionIndex2);
				}
			}
		}

		int paperIndex1 = rand() % papersInSession;
		int paperIndex2 = rand() % papersInSession;

		double score = getScoreOnSwapping(*conference, trackIndex1, sessionIndex1, paperIndex1, trackIndex2, sessionIndex2, paperIndex2);

		if(score > maximumScore){
			iterations = 0;
			maximumScore = score;
			swapPapers(*conference, trackIndex1, sessionIndex1, paperIndex1, trackIndex2, sessionIndex2, paperIndex2);
		}
	}
}

void SessionOrganizer::initializeGreedyOrganization()
{
	
	int totalNumberOfPapers = parallelTracks * sessionsInTrack * papersInSession;

	map<int,bool> assigned;
	for(int i=0;i<totalNumberOfPapers;i++)
		assigned[i] = false;

	for(int i = 0; i < conference->getSessionsInTrack(); i++)
	{
		for(int j = 0; j < conference->getParallelTracks(); j++)
		{
			// find the most distant paper
			double maximum_distance = -1;
			int most_distant_paper_index;
			for (int paperIndex1 = 0; paperIndex1 < totalNumberOfPapers; paperIndex1++){
				
				if (!assigned[paperIndex1]){
					
					double temp_distance = 0;
					for (int paperIndex2 = 0; paperIndex2 < totalNumberOfPapers; paperIndex2++){
						
						if (!assigned[paperIndex2])
							temp_distance += distanceMatrix[paperIndex1][paperIndex2];
					}

					if (temp_distance > maximum_distance){
						
						maximum_distance = temp_distance;
						most_distant_paper_index = paperIndex1;
					}
				}
			}

			// set the first paper of the session to be most distant paper
			conference->setPaper(j, i, 0, most_distant_paper_index);
			assigned[most_distant_paper_index] = true;

			for(int k = 1; k < conference->getPapersInSession(); k++)
			{
				// find the nearest paper for the session
				double minimum_distance = INT_MAX;
				int nearest_paper_index;
	
				for (int paperIndex2 = 0; paperIndex2 < totalNumberOfPapers; paperIndex2++){
				
					if (!assigned[paperIndex2]){

						double temp_distance = 0;

						for (int inSessionPaperIndex = 0; inSessionPaperIndex < k; inSessionPaperIndex++){
							temp_distance += distanceMatrix[inSessionPaperIndex][paperIndex2];
						}

						if (temp_distance < minimum_distance){
							minimum_distance = temp_distance;
							nearest_paper_index = paperIndex2;
						}
					}
				}

				conference->setPaper(j, i, k, nearest_paper_index);
				assigned[nearest_paper_index] = true;
			}
		}
	}

	// update the score
	double score = scoreConference(*conference);
}


bool SessionOrganizer::climbStep() {
	//cout << "---------------- Climbing Step ---------------------" << endl;

	int int_max = 1000000;
	long double randomNumber = (long double)(rand() % int_max) / (int_max -1);
	
	//with this probability choose the greedy step else swap two randomly chosen papers
	if(randomNumber <= 1.00)
	{
		return greedyStep();
	}
}

/**
 * Organize Papers by using Random Restart Hill Climbing Algorithm
 */
void SessionOrganizer::organizePapers()
{
	initializeOrganization(this->conference);
	
	globalMaximumScore = conference->getScore();
	writeConference(*conference);

	// check if there is only one track and one session
	if (parallelTracks == 1 && sessionsInTrack == 1)
		return;

	while(true){
		localBeamSearch(100);
	}
}

void SessionOrganizer::writeConference(Conference &conference) {
	ofstream fout(outputFileName);

	for(int sessionIndex = 0; sessionIndex < sessionsInTrack; sessionIndex++)
    {
        for(int trackIndex = 0; trackIndex < parallelTracks; trackIndex++)
        {	
        	Session *currentSession = conference.getSession(trackIndex, sessionIndex);
            for(int paperIndex = 0; paperIndex < papersInSession; paperIndex++)
            {
                fout<< currentSession->getPaper(paperIndex) << " ";
            }
            if(trackIndex != parallelTracks - 1)
            {
                fout<< "| ";
            }
        }
        fout<< "\n";
    }
}


void SessionOrganizer::localBeamSearch(int beamSize) {
	vector<Conference> beam;
	vector<Conference> beamNeighbours;
	int maximumNumberOfIterations = 0;

	initializeOrganization(this->conference);
	double maximumScore = this->conference->getScore();
	
	Conference optimalConference = *(this->conference);

	for(int i=0; i<beamSize; i++){
		Conference newConference = *conference;
		initializeOrganization(&newConference);
		beam.push_back(newConference);
	}

	double time_left = processingTimeInMinutes*60 - difftime(time(0), starting_time);				// time left in seconds
	double max_iteration_time = 0;																	// in seconds

	do {
		time_t iteration_start = time(0);

		cout << endl;
		cout << "Time Left : " << time_left << endl;
		cout << "iteration = " << maximumNumberOfIterations << endl;
		for(int i=0; i<beamSize; i++){
			Conference tmpConference = beam[i];
			beamNeighbours.push_back(tmpConference);

			for(int j=0; j<beamSize; j++){
				Conference neighbourConference = beam[i];
				swapTwoRandomPapers(&neighbourConference);
				beamNeighbours.push_back(neighbourConference);
			}
		}


		sort(beamNeighbours.begin(),beamNeighbours.end());

		int neighboursSize = beamNeighbours.size();

		if(beamNeighbours[neighboursSize-1].getScore() > maximumScore){
			maximumScore = beamNeighbours[neighboursSize-1].getScore();
			optimalConference = beamNeighbours[neighboursSize-1];
			cout << "Increased to " << optimalConference.getScore() << endl;

			time_left = processingTimeInMinutes*60 - difftime(time(0), starting_time);
			
			// don't overwrite the outputfile if time is up
			if(time_left <= 1.0){
				exit(0);
			}

			if(optimalConference.getScore() > globalMaximumScore){
				globalMaximumScore = optimalConference.getScore();
				writeConference(optimalConference);
			}

			maximumNumberOfIterations = 0;
		}else{
			cout << "nhi badh rha h" << endl;
			maximumNumberOfIterations++;
		}

		beam.clear();

		for(int i=0; i<beamSize; i++){
			beam.push_back(beamNeighbours[neighboursSize-i-1]);
		}

		beamNeighbours.clear();

		max_iteration_time = max(max_iteration_time, difftime(time(0), iteration_start));
		time_left = processingTimeInMinutes*60 - difftime(time(0), starting_time);
		
		if (time_left < max_iteration_time) {
			exit(0);
		}

	} while (maximumNumberOfIterations < 100);
}
