//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//


#include <sstream>
#include <climits>
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
using namespace std;

//============================Rule Of Five==================
User::User(const std::string& name) : history(std::vector<Watchable*>()),
    name(name) , watchListCounter(0),tagPrefernce(std::unordered_map <std::string, int >()),
    maxTagValue(0) , maxTagName (""){}

User::~User() {
    history.clear();

}
//Copy Constructor
User::User(const User &other) :name(other.name){
    history=copyHistory(other.history);
}
//copy assignment
User &User::operator=(const User &other) {
    if(this!=&other){
        name =other.name;
        for(int i=0; i<history.size();i++){
            delete(history[i]);
        }
        history.clear();
        history=copyHistory(other.history);
    } else
        return *this;
}

//Move Constructor;
User::User(User &&other):name(other.name),watchListCounter(other.watchListCounter)  {
    history.clear();
    history=copyHistory(other.history);
    tagPrefernce=other.tagPrefernce;
    other.history.clear();
    other.name= nullptr;
}
//Move Assignment opperator
User &User::operator=(User &&other) {
    if (this != &other)
    {
        history.clear();
        name=other.name;
        history=copyHistory(other.history);
        watchListCounter=other.watchListCounter;
        tagPrefernce=other.tagPrefernce;
        other.tagPrefernce.clear();

    }
    return *this;
}




//============================Copies==================

//Copy history from another User (this) return vector that holds the history;
std::vector<Watchable *> User::copyHistory(std::vector<Watchable *> history) {
    for (int i = 0; i < (unsigned) this->history.size(); i++) {
        history.push_back(this->history[i]);
    }
    return history;
}

//============================Getters==================

std::string User::getName() const {
    return name;
};

std::vector<Watchable *> User::get_history() const {
    return history;
}

std::string User::getMaxTag() {
    return maxTagName;
}

//============================Adders==================

void User::addTohistory(Watchable *current) {
    history.push_back(current);

}

void User::addToTagPrefernce(Watchable *current) {
    std::vector<std::string>  generes= current->getTags();
    for (int i = 0; (unsigned) i < generes.size() ; i++ ){
        auto it = tagPrefernce.find(generes[i]);
        if(it!=tagPrefernce.end()){
           tagPrefernce[generes[i]]++;
        }else{
            tagPrefernce.insert(pair<string, int>(generes[i], 1));
        }
    }
}

//============================Finders==================


void User::findMaxTag() {
    unordered_map <string,int > :: iterator it;
    for (it=tagPrefernce.begin() ; it!=tagPrefernce.end(); it++){
        if( it->second >maxTagValue  ) {
            if (!unAvlailableTag(it->first)) {
            maxTagName = it->first;
            maxTagValue = it->second;
        }
        }else if( it->second==maxTagValue)
        {
            checkLexiMax(it->first,it->second,maxTagName);
        }
    }
}


//Check if the Tag is in the unAvaliable content vector (which added when the tag is most popular but there's
//No Content aviable in the list;
bool User::unAvlailableTag(const std::string basicString) {
    for (int i = 0; (unsigned) i < mNotExistedTag.size(); ++i) {
        if(mNotExistedTag[i]==basicString){
            return true;
        }
    }
    return false;
}

//============================RerunRecommenderUser==================

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {

    std::vector<Watchable *> historyList = get_history();
    if (watchListCounter == 0){
        watchListCounter++;
        return historyList[0];
    }
    else{
        watchListCounter++;
        return historyList[(watchListCounter-1)%historyList.size()];

    }
}

RerunRecommenderUser::~RerunRecommenderUser() {

}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {

}

std::string RerunRecommenderUser::getType() const {
    return "ren";
}

User *RerunRecommenderUser::CopyUser(const std::string &name) {
    RerunRecommenderUser *output = new RerunRecommenderUser(name);
    output->history = this->get_history();

    return output;
}

User *RerunRecommenderUser::clone(Session & session) {
    User* clonedUser=new RerunRecommenderUser(*this);
    deepCopyHistory(*clonedUser,*this,session);
    return  clonedUser;

}



//============================LengthRecommenderUser==================


Watchable *LengthRecommenderUser::getRecommendation(Session &s) {

 int avarage= getAvarage(s);
 int temp = INT_MAX;
 int tempId = 0;
    for (int i = 1; i < (unsigned) s.getContent().size(); i++){
        Watchable* Watcher=s.getContent().at(i);
        stringstream mConvertLengthToInt(Watcher->getLength());
        int mCurrentLength;
        mConvertLengthToInt>>mCurrentLength;
        if (abs (mCurrentLength - avarage)<temp){
            if (!InHistory(s.getContent().at(i))){
                temp = abs(mCurrentLength-avarage);
                stringstream mConvertLengthToInt(s.getContent().at(i)->getid());
                mConvertLengthToInt>>tempId;
            }
        }
    }
    return s.getContent().at(tempId);
}

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {

}

std::string LengthRecommenderUser::getType() const {
    return "len";
}

User *LengthRecommenderUser::CopyUser(const std::string &name) {
    LengthRecommenderUser *output = new LengthRecommenderUser(name);
    output->history = this->copyHistory(this->history);
    return output;
}

int LengthRecommenderUser::getAvarage(Session &session) {
    int output=0;
    int lengthOfCurrent;
    vector<Watchable*> historyForAv=session.getActiveUser()->get_history();
    for (int j = 0; (unsigned) j <historyForAv.size() ; ++j) {
        stringstream mCastinToInt(historyForAv.at(j)->getLength());
        mCastinToInt >> lengthOfCurrent;
        output=output+lengthOfCurrent;
    }
    output=output/historyForAv.size();
    return output;
}

User *LengthRecommenderUser::clone(Session& session) {

    User* clonedUser=new LengthRecommenderUser(*this);
    deepCopyHistory(*clonedUser,*this,session);
    return  clonedUser;
}

LengthRecommenderUser::~LengthRecommenderUser() {

}



//============================GenreRecommenderUser==================


Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
this->findMaxTag();
//See if the history is correct;
    for (int k = 1; (unsigned)k <s.getContent().size(); ++k) {
        Watchable * watchable=s.getContent().at(k);
        for (int j = 0; (unsigned) j <watchable->getTags().size() ; ++j) {
            if(watchable->getTags().at(j)==maxTagName){
                if(!InHistory(watchable))
                    return watchable;
            }
        }
    }
    maxTagValue=0;
    mNotExistedTag.push_back(maxTagName);
    getRecommendation(s);

}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {

}

GenreRecommenderUser::~GenreRecommenderUser() {

}

std::string GenreRecommenderUser::getType() const {
    return "gen";
}

User *GenreRecommenderUser::CopyUser(const std::string &name) {
    GenreRecommenderUser *output = new GenreRecommenderUser(name);
    output->history = this->get_history();

    return output;
}

User *GenreRecommenderUser::clone(Session& session) {
    User* clonedUser=new GenreRecommenderUser(*this);
    deepCopyHistory(*clonedUser,*this,session);
    return  clonedUser;
}


bool User::InHistory(Watchable *pWatchable) {
    for (int i = 0; (unsigned) i <history.size() ; ++i) {
        if( pWatchable == history.at(i)){
            return true;
        }
    }
    return false;
}

void User::checkLexiMax(std::string mTagName, int mTagCount, std::string mCurrentMaxName) {
    if(mTagName<maxTagName & !unAvlailableTag(mTagName)){
        maxTagName=mTagName;
        maxTagValue=mTagCount;
    }

}

void User::deepCopyHistory(User &mNewUs, User & mUser, Session & session) {
    mNewUs.history.clear();
    for (int j = 0; (unsigned)j <mUser.history.size() ; ++j) {
        int mId=std::stoi(mUser.history.at(j)->getid());
        mNewUs.history.push_back(session.getContent().at(mId));
    }

}




