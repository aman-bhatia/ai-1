/* 
 * File:   main.cpp
 * Author: Harishchandra Patidar
 *
 */

#include <cstdlib>
#include <time.h>
#include <stdlib.h>
#include <iomanip>

#include "SessionOrganizer.h"

using namespace std;

int main(int argc, char** argv)
{
    // Parse the input.
    if (argc < 1)
    {
        cout << "./a.out <input_filename>";
        exit(0);
    }

    srand(time(NULL));
    setprecision(7);
    
    string inputfilename(argv[1]);

    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer(inputfilename);

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