//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//
#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

class Watchable;
class Session;

class User{
public:
    //============================Rule Of Five==================

    User( const std::string& name);
    virtual User* CopyUser ( const std::string& name)=0;
    virtual  ~User();
    User(User &&other);
    User (const User& other);
    User & operator=(const User &other);
    virtual User* clone (Session& session) =0;
    void deepCopyHistory(User &, User&,Session&);
    User&operator=(User &&other);
    //============================Getters==================
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual std::string getType() const =0;
    std::string getName() const;
    std::vector<Watchable*> copyHistory(std::vector<Watchable*> history);
    std::vector<Watchable*> get_history() const;
    void findMaxTag();
    std::string getMaxTag();
    //============================Setters==================


    void  addTohistory(Watchable* current);
    void addToTagPrefernce (Watchable* current);

    //============================Fields==================
    std::vector <std::string> mNotExistedTag;


protected:
    //============================Fields==================
    std::vector<Watchable*> history;
     std::string name;
    int watchListCounter = 0;
    std::unordered_map <std::string, int > tagPrefernce;
    int maxTagValue=0;
    std::string maxTagName="";
    //============================Functions==================
    void checkLexiMax(std::string mTagName,int mTagCount , std::string maxTagName );
    bool unAvlailableTag(const std::string basicString);
    bool InHistory(Watchable *pWatchable);

};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    ~LengthRecommenderUser();
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getType() const;
    User* CopyUser ( const  std::string& name);
    virtual User* clone (Session& session);

private:
    int i{};
    int getAvarage(Session &session);


};

class RerunRecommenderUser : public User {
public:

    RerunRecommenderUser( const std::string& name);
    ~RerunRecommenderUser();
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getType() const ;
    User* CopyUser (const  std::string& name);
    virtual User* clone (Session& session);

private:

};

class GenreRecommenderUser : public User {
public:

    GenreRecommenderUser( const std::string& name);
    ~GenreRecommenderUser();
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getType() const ;
    User* CopyUser ( const std::string& name);
    virtual User* clone (Session& session);

private:

};

#endif
