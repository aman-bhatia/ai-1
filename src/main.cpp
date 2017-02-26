/* 
 * File:   main.cpp
 * Author: Harishchandra Patidar
 *
 */

#include <cstdlib>
#include <time.h>
#include <stdlib.h>

#include "SessionOrganizer.h"

using namespace std;

int main(int argc, char** argv)
{
    time_t starting_time = time(0);
    // Parse the input.
    if (argc < 2)
    {
        cout << "./a.out <input_filename> <output_filename>";
        exit(0);
    }

    srand(time(NULL));
    
    string inputFileName(argv[1]);
    string outputFileName(argv[2]);

    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer(inputFileName, outputFileName, starting_time);

    // Organize the papers into tracks based on similarity.
    organizer->organizePapers();
    
    cout << "organization:"<<endl;
    organizer->printSessionOrganiser();

    // Score the organization against the gold standard.
    double score = organizer->getConferenceScore();
    cout<< "score:"<<score<<endl;

    return 0;
}

// 125 papers -> 979.686 for complete swappings
// 125 papers -> 968.638 for taking minimum distance neighbour from each session and swapping with maximum distance neighbour in current session
// 125 papers -> 970.488 for picking random paper from each session for every paper in a given session

// testfile3 -> 479-480 (random neighbour and initializeOrganization())
// testfile2 -> 344-345 (random neighbour and initializeOrganization())