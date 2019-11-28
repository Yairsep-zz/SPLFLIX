#include "../include/Session.h"
//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//
#include "../include/json.hpp"
#include <fstream>
#include <sstream>
#include "vector"
#include "../include/Watchable.h"
#include "../include/User.h"
//============================Read From Json File==================
using namespace std;
using json=nlohmann::json;

Session::Session(const std::string &configFilePath): mNewName(""),mContentId(0),mExit(false),content(std::vector<Watchable*>()),actionsLog(std::vector<BaseAction*>()),userMap(std::unordered_map<std::string,User*>()),activeUser(new LengthRecommenderUser("Default")),mUserName(""),mUserType("") {
    int id = 1;
    std::ifstream k(configFilePath);
    json j;
    k >> j;
    Watchable* w= nullptr;
    content.push_back(w);
    for (auto item = j["movies"].begin(); item != j["movies"].end(); item++) {
        json movie = item.value();
        int length = movie["length"];
        string name = movie["name"];
        vector<std::string> tags = movie["tags"];
        Watchable *w = new Movie(id, name, length, tags);
        id++;
        content.push_back(w);
        tags.clear();
    }
    for (auto item1 = j["tv_series"].begin(); item1 != j["tv_series"].end(); item1++) {
        json episodes = item1.value();
        int mSeasonCounter = 0;
        std::vector<int> seasons = episodes["seasons"];
        for (auto item2 = episodes["seasons"].begin(); item2 != episodes["seasons"].end(); item2++) {
            json season = item2.value();
            string name = episodes["name"];
            int length = episodes["episode_length"];
            vector<std::string> tags = episodes["tags"];

            for (int i = 1; i <= seasons[mSeasonCounter]; i++) {

                Watchable *episode = new Episode(id, name, length, mSeasonCounter + 1, i, tags);
                id++;
                content.push_back(episode);

            }
            tags.clear();
            season.clear();
            mSeasonCounter++;
        }
    }
    userMap.insert(pair<string, User *>("default", activeUser));

}

//============================Rule Of Five==================

//Copy Constructor
Session::Session(const Session &other) {
    copyAllVectors(other);
    std::unordered_map<std::string,User* >::const_iterator iteratorForUser = userMap.find(other.activeUser->getName());
    activeUser= iteratorForUser->second;
//    : mNewName(other.mNewName),mContentId(other.mContentId),mExit(false),content(other.content),actionsLog(other.actionsLog),userMap(other.userMap),activeUser(other.activeUser),mUserName(other.mUserName),mUserType(other.mUserType){

}


//Destructor
Session::~Session() {
    clearAll();
}

//Copy Assignment
Session &Session::operator=(const Session &session) {

    if (this != &session)
    {
        clearAll();
        copyAllVectors(session);
    }
    return *this;
}


//Move Constructor
Session::Session(Session &&other):mNewName(other.mNewName),mContentId(other.mContentId),mExit(false),content(std::vector<Watchable*>()),actionsLog(std::vector<BaseAction*>()),userMap(std::unordered_map<std::string,User*>()),activeUser(other.activeUser),mUserName(other.mUserName),mUserType(other.mUserType) {
    clearAll();
    copyAllVectors(other);
    other.actionsLog.clear();
    other.content.clear();
    other.userMap.clear();
    other.activeUser= nullptr;
}

//Move Assignment Operator
Session &Session::operator=(Session &&other) {

    if (this != &other)
    {
        clearAll();
        content=other.content;
        userMap=other.userMap;
        actionsLog=other.actionsLog;
        mUserName=other.mUserName;
        mNewName=other.mNewName;
        mContentId=other.mContentId;
        shallowCopyVectors(other);
        other.content.clear();
        other.userMap.clear();
        other.actionsLog.clear();
        other.mUserName=nullptr;
        other.mNewName= nullptr;

    }
    return *this;
}

//============================Start==================

void Session::start() {
    cout << "SPLFLIX is now on!" << "\n";
    //Initializng new defaultUser an by LengthReCOMMEND Algorithem
    //TODO Defualt user case
//    if (activeUser == nullptr){
//        User *defaultUser = new LengthRecommenderUser ("defaultUser");
//        addToUserMap("defaultUser" , defaultUser);
//        setActiveUser(defaultUser);
//    }

    mExit= false;
    while (!mExit) {

        string userInput;
        getline(cin, userInput);
        BaseAction *action = nullptr;
        vector<string> mInputHolder = splitUserInput(userInput, ' ');
        //Create User
        if (mInputHolder[0] == "createuser") {
            action = new CreateUser();
            if (mInputHolder.size() !=3){
                action->setStatus("ERROR","invalid Input Size");
            }
            else {
                string user_name = mInputHolder[1];
                //TODO if(mInputHolder[1]!= nullptr& mInputHolder[2]!= nullptr)
                //check if there's value in place 2;
                string recommendation_algo = mInputHolder[2];
                mUserName = mInputHolder[1];
                mUserType = mInputHolder[2];

                //TODO check why pointer, maybe reference?;
                action->act(*this);

            }
            actionsLog.push_back(action);
        }


            //Change User
        else if (mInputHolder[0] == "changeuser") {
            action = new ChangeActiveUser();
            mUserName = mInputHolder[1];
            action->act(*this);
            actionsLog.push_back(action);

        }

            //Delete User
        else if (mInputHolder[0] == "deleteuser") {
            action = new DeleteUser();
            if (mInputHolder.size() !=2){
                action->setStatus("ERROR","invalid Input Size");
            }
            else {
                mUserName = mInputHolder[1];
                action->act(*this);
            }
            actionsLog.push_back(action);
        }
            //Duplicate User
        else if (mInputHolder[0] == "dupuser") {
            action = new DuplicateUser();
            if (mInputHolder.size() == 3) {
            mUserName = mInputHolder[1];
            mNewName = mInputHolder[2];
            action->act(*this);
        }
        else {
            action->setStatus("ERROR","invalid Input");
        }
            actionsLog.push_back(action);
        }

        //Print Content List
        else if (mInputHolder[0] == "content") {
            action = new PrintContentList();
            action->act(*this);
            actionsLog.push_back(action);
        }

            //Print Watch History List
        else if (mInputHolder[0] == "watchhist") {
            action = new PrintWatchHistory();
            action->act(*this);
            actionsLog.push_back(action);
        }
            //Print Actions Log
        else if (mInputHolder[0] == "watch") {
            action = new Watch();
            stringstream mConverter(mInputHolder[1]);
            mConverter >> mContentId;
            action->act(*this);
            actionsLog.push_back(action);

        } else if (mInputHolder[0] == "log") {
            action = new PrintActionsLog();
            action->act(*this);
            delete(action);
        }
        else if (mInputHolder[0] == "exit") {
            action = new Exit();
            action->act(*this);
            mExit=true;
            actionsLog.push_back(action);
            for (int  i= 0; (unsigned) i < mInputHolder.size() ; i++) {
                mInputHolder[i].clear();
            }
        }
        else {
            cout << "Wrong Code\n";
        }
    }

}

//============================Getters==================

//GetUsername
const string &Session::getMUserName() const {
    return mUserName;
}

//GetuserType
const string &Session::getMUserType() const {
    return mUserType;
}

std::vector<Watchable *> &Session::getContent() {
    return content;
}

User *Session::getActiveUser() {
    return activeUser;
}

vector<BaseAction *> Session::getActionLog() {
    return actionsLog;
}

std::unordered_map<std::string, User *> Session::getUserMap() {
    //TODO check if const is neccessry;
    return userMap;
}

//Check if user exist in the user Map
bool Session::checkExist(const std::string &userName) {
    return !(userMap.find(userName) == userMap.end());
}

//============================Adders==================

//Adding user to the userMap vector;
void Session::addToUserMap(string mUserName, User *user) {
    userMap.insert(pair<string, User *>(mUserName, user));
}

void Session::addActionLog(BaseAction * x) {
    actionsLog.push_back(x);
}


//============================Split==================

std::vector<std::string> Session::splitUserInput(const std::string &s, char separator) {
    vector<string> mInputHolder;
    string mWord;
    istringstream mWordStreamer(s);
    while (getline(mWordStreamer, mWord, separator)) {
        mInputHolder.push_back(mWord);
    }
    return mInputHolder;
}

//============================Setters==================

void Session::setActiveUser(User *user) {
    activeUser = user;

}

//============================Clearences==================

void Session::deleteFromUserMap(std::string mUserName) {
    userMap.erase(mUserName);

}

void Session::clearAll() {

    for (int i=0;(unsigned) i<content.size();i++){
        delete content.at(i);

    }
    content.clear();

    for (int j = 0;(unsigned) j <actionsLog.size() ; ++j) {
        delete actionsLog[j];
    }
    actionsLog.clear();
    for (auto user = userMap.begin() ; user!=userMap.end() ; user++){
        delete user->second;
    }
    userMap.clear();



}

//============================Copies==================

void Session::copyAllVectors(const Session &other) {
    //Check if there's no memory leaks
    Watchable* watchable= nullptr;
    content.push_back(watchable);
    for (int j=1;(unsigned)j<other.content.size();j++){
        content.push_back(other.content.at(j)->clone());
    }
    for (int i=0;(unsigned) i<other.actionsLog.size();i++){
        actionsLog.push_back(other.actionsLog[i]->clone());
    }
    for (auto user = other.userMap.begin() ; user!=other.userMap.end() ; user++){
        userMap.insert({user->first , user->second->clone(*this)});
    }

}

void Session::shallowCopyVectors(const Session &other) {
    Watchable* watchable= nullptr;
    content.push_back(watchable);
    for (int j=1;(unsigned)j<other.content.size();j++){
        content.push_back(other.content.at(j));
    }
    for (int i=0; (unsigned)i<other.actionsLog.size();i++){
        actionsLog.push_back(other.actionsLog[i]);
    }
    for (auto user = other.userMap.begin() ; user!=other.userMap.end() ; user++){
        userMap.insert({user->first , user->second});
    }


}

std::string Session::getNewName() {
    return mNewName;
}

int Session::getContentId() {
    return mContentId;
}

void Session::setContentId(int number) {
    mContentId=number;

}









