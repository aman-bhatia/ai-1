/* 
 * File:   Session.cpp
 * Author: Harishchandra Patidar
 * 
 */

#include <errno.h>
#include <vector>

#include "Session.h"

Session::Session() : papers(NULL)
{
    papersInSession = 0;
}

Session::~Session()
{
    delete [] papers;
}

Session::Session(int papersInSession)
{
    this->papersInSession = papersInSession;
    initPapers(papersInSession);
}

Session::Session(const Session &anotherSession)
{
    this->papersInSession = anotherSession.papersInSession;
    this->papers = new int[this->papersInSession];

    for(int i=0; i<this->papersInSession; i++){
        this->papers[i] = anotherSession.papers[i];
    }
}

Session & Session::operator=(const Session &anotherSession)
{
    if(this != &anotherSession){
        delete this->papers;

        this->papersInSession = anotherSession.papersInSession;
        this->papers = new int[this->papersInSession];
        for(int i=0; i<this->papersInSession; i++){
            this->papers[i] = anotherSession.papers[i];
        }
    }
    return *this;
}

void Session::initPapers(int papersInSession)
{
    this->papers = new int[papersInSession];
    for(int i=0; i<papersInSession; i++)
    {
        papers[i] = -1;
    }
}

int Session::getNumberOfPapers()
{
    return papersInSession;
}

int Session::getPaper(int index)
{
    if (index < papersInSession)
    {
        return papers[index];
    }
    else
    {
        cout << "Index out of bound - Session::getPaper" << endl;
        exit(0);
    }
}

void Session::setPaper(int index, int paperId)
{
    if (index < papersInSession)
    {
        papers[index] = paperId;
    }
    else
    {
        cout << "Index out of bound - Session::setPaper" << endl;
        exit(0);
    }
}

void Session::printSession()
{
    for (int i=0; i<papersInSession ; i++)
    {
        cout<<papers[i]<<" ";
    }
    cout<<endl;
}
