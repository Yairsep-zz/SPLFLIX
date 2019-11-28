//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//

#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"


using namespace std;
//============================Base Action==================

//============================Rule Of Five==================
//Constructor;
BaseAction::BaseAction() :errorMsg(""),status(PENDING) {

}
BaseAction::~BaseAction() {

}
//============================Getters=================

ActionStatus BaseAction::getStatus() const {
    return status;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


//============================Setters=================
void BaseAction::setStatus(std::string statusOutput, std::string msg) {
    if(statusOutput=="ERROR"){
        status=ERROR;
        errorMsg=msg;
    }else{
        complete();
    }
}

void BaseAction::complete() {
    status = COMPLETED;

}

void BaseAction::error(const std::string &errorMsg1) {
    errorMsg = errorMsg1;
    status = ERROR;
}





//============================Create User==================

void CreateUser::act(Session &sess) {
    string mUserName = sess.getMUserName();
    string type = sess.getMUserType();
    User *user = nullptr;
    if (sess.checkExist(mUserName)) {
        error("User is already existed");
    } else {

        if (type == "rer") {
            user = new RerunRecommenderUser(mUserName);
            complete();
        } else if (type == "len") {
            user = new LengthRecommenderUser(mUserName);
            complete();
        } else if (type == "gen") {
            user = new GenreRecommenderUser(mUserName);
            complete();
        } else {
            cout << "Wrong type of user" << endl;
            error("Wrong type of User");

        }
        sess.addToUserMap(mUserName, user);
        sess.setActiveUser(user);
    }
}

std::string CreateUser::toString() const {
    string output;
    if (getStatus() == COMPLETED) {
        output = "CrateUser COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "CrateUser ERROR:" + getErrorMsg();
    } else {
        output = "CreateUser PENDING";
    }
    return output;
}

BaseAction *CreateUser::clone() {
    BaseAction* newAction=new CreateUser();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;
}


//============================Change Active User==================

void ChangeActiveUser::act(Session &sess) {
    string username=sess.getMUserName();
    std::unordered_map<std::string,User*> usersMap=sess.getUserMap();
    std::unordered_map<std::string,User* >::const_iterator iteratorForUser = usersMap.find (username);
    if (iteratorForUser != sess.getUserMap().end()) {
        User* user=iteratorForUser->second;
        sess.setActiveUser(user);
        complete();
    } else {
        error("Wrong user input");
    }
}

std::string ChangeActiveUser::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "ChangeActiveUser COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "ChangeActiveUser ERROR:" + getErrorMsg() + "\n";
    } else {

    }
    return output;
}

BaseAction *ChangeActiveUser::clone() {
    BaseAction* newAction=new ChangeActiveUser();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;

}



//============================Delete User=================

//Deleting a user from the userMap callling methood from sess class.
void DeleteUser::act(Session &sess) {
    string mUserName = sess.getMUserName();

    if (!sess.checkExist(mUserName)) {
        cout << "ERROR: User doesn't exists";
        error("User Doesn't exists");
    } else {
        sess.deleteFromUserMap(sess.getMUserName());
        complete();
    }
}

std::string DeleteUser::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "DeleteUser COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "DeleteUser ERROR:" + getErrorMsg() + "\n";
    } else {

    }
    return output;
}

BaseAction *DeleteUser::clone() {
    BaseAction* newAction=new DeleteUser();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;

}



//============================Duplicate User=================
void DuplicateUser::act(Session &sess) {
    string name = sess.getMUserName();

    if (!sess.checkExist(name)) {
        cout << "ERROR: User doesn't exists";
        error("User doesn't exists");
    } else if (sess.checkExist(sess.getNewName())) {
        cout << "ERROR: User Name is already taken";
        error("User name is already taken");
    } else {
        std::unordered_map<std::string,User*> usersMap=sess.getUserMap();
        std::unordered_map<std::string,User* >::const_iterator iteratorForUser = usersMap.find (name);
        if (iteratorForUser != sess.getUserMap().end()) {
            User *mTempUser = iteratorForUser->second;
            User *mNewUser = mTempUser->CopyUser(sess.getNewName());
            sess.addToUserMap(sess.getNewName(),mNewUser);
            complete();
        }
    }
}

std::string DuplicateUser::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "DuplicateUser COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "DuplicateUser ERROR:" + getErrorMsg() + "\n";
    } else {

    }
    return output;
}

BaseAction *DuplicateUser::clone() {
    BaseAction* newAction=new DuplicateUser();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;
}


//============================Print Content List=================

//Check about printContent to string
void PrintContentList::act(Session &sess) {
    for (int i=1; i<(unsigned)sess.getContent().size();i++) {
        Watchable* watchable=sess.getContent().at(i);
        cout << watchable->getid() << "." << " " << watchable->toString() << " " << watchable->printContent()
             << "\n";
        complete();
    }

}

std::string PrintContentList::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "PrintContentList COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "PrintContentList ERROR: " + getErrorMsg() + "\n";
    } else {

    }
    return output;
}

BaseAction *PrintContentList::clone() {
    BaseAction* newAction=new PrintContentList();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;
}



//============================Print Watch History================
void PrintWatchHistory::act(Session &sess) {

    cout << "Watch history for " << sess.getActiveUser()->getName() << endl;
    vector<Watchable *> watchHistory = sess.getActiveUser()->get_history();
    for (auto & i : watchHistory) {
        cout << i->toString() << "\n";
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "PrintWatchHistory COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "PrintWatchHistory ERROR:" + getErrorMsg() + "\n";
    } else {

    }
    return output;
}

BaseAction *PrintWatchHistory::clone() {
    BaseAction* newAction=new PrintWatchHistory();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;

}



//============================Watch================

void Watch::act(Session &sess) {
    if (sess.getContentId() < 1 | sess.getContentId() > sess.getContent().size()){
        error("Invalid Input");
    }
    else {
        vector<Watchable *> content = sess.getContent();
        Watchable *currentWatch = content[sess.getContentId()];
        cout << "Watching " << currentWatch->toString() << endl;
        sess.getActiveUser()->addTohistory(currentWatch);
        sess.getActiveUser()->addToTagPrefernce(currentWatch);
        Watchable *nextWatch = currentWatch->getNextWatchable(sess);
        cout << "We recommend watching " << nextWatch->toString() << "[y/n]?\n";
        complete();
        string input;
        getline(cin, input);
        if (input == "y") {
            sess.setContentId(std::stoi(nextWatch->getid()));
            BaseAction *watch = new Watch();
            watch->act(sess);
            sess.addActionLog(watch);
        }
    }
}

std::string Watch::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "Watch COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "Watch ERROR:" + getErrorMsg() + "\n";
    }
    return output;
}

BaseAction *Watch::clone() {
    BaseAction* newAction=new Watch();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;
}


//============================Print Actions log================

void PrintActionsLog::act(Session &sess) {
    std::vector<BaseAction*> actionLog= sess.getActionLog();
//    for (auto & i : actionLog) {
//        cout<< i->toString();
        for (int i = actionLog.size()-1 ; i > 0 ; i-- ){
            cout<< actionLog.at(i)->toString();
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "PrintActionLog COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "PrintActionLog ERROR: " + getErrorMsg() + "\n";
    } else {

    }
    return output;
}

BaseAction *PrintActionsLog::clone() {
    BaseAction* newAction=new PrintActionsLog();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;

}




//============================Exit================

void Exit::act(Session &sess) {
    complete();
}

std::string Exit::toString() const {
    std::string output;
    if (getStatus() == COMPLETED) {
        output = "Exit COMPLETED\n";
    } else if (getStatus() == ERROR) {
        output = "Exit ERROR:" + getErrorMsg() + "\n";
    } else {

    }
    return output;
}

BaseAction *Exit::clone() {
    BaseAction* newAction=new Exit();
    if(getStatus()==COMPLETED) {
        newAction->setStatus("COMPLETED","");

    }else if(getStatus()==ERROR){
        newAction->setStatus("ERROR",getErrorMsg());
    }
    return newAction;
}



