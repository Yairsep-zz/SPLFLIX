//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//

#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "Action.h"
#include "User.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);

//============================Rule Of Five==================
    ~Session();
    Session(const Session& other);
    Session & operator=(const Session &other);
    Session(Session &&other);
    Session & operator=(Session &&other);

//============================Getters==================
    User*  getActiveUser();
    std::vector<BaseAction *> getActionLog();
    std::unordered_map<std::string,User*> getUserMap();
    const std::string &getMUserName() const;
    const std::string &getMUserType() const;
    std::vector<Watchable*> &getContent();
    std::string getNewName();
    int getContentId();


//============================Setters==================
    void setContentId(int );
    void setActiveUser(User*);
    bool checkExist(const  std::string &);
    void copyAllVectors(const Session &other);
    void shallowCopyVectors(const Session &other);
    void deleteFromUserMap(std::string mUserName);
    void clearAll();

//============================Adders==================
    //check
    void addToUserMap(std::string mUserName, User *user);
    void addActionLog(BaseAction*);
    void start();

//============================Fields==================



private:
    std::string mNewName;
    int mContentId;
    bool mExit;
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string mUserName;
    std::string mUserType;
    std::vector<std::string> splitUserInput(const std::string&, char);


};
#endif
