//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

class User;
//TODO where is the destructor?
class BaseAction{
public:

//============================Rule Of Five==================
    BaseAction();
    virtual ~BaseAction();
    virtual BaseAction* clone () =0;

//============================Getters==================

    ActionStatus getStatus() const;

    std::string getErrorMsg() const;

//============================Setters==================
    void  setStatus(std::string,std::string);
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;

protected:
    void complete();
    void error(const std::string& errorMsg);

private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};

class ChangeActiveUser : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};

class DeleteUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};


class DuplicateUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};

class PrintContentList : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};

class PrintWatchHistory : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};


class Watch : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};


class PrintActionsLog : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};

class Exit : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone ();
};
#endif
