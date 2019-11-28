//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//
#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    //============================Rule Of Five==================
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();

    //============================Getters=================
    virtual std::string toString() const = 0;
    virtual std::string printContent() =0;
    std::string printTags(std::vector<std::string> &tags);
    virtual std::string getName() const =0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    std::vector<std::string>getTags();
    std::string getid();
    std::string getLength();
    virtual Watchable* clone () =0;
    //============================Setters==================

private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id,std::string   name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string printContent() ;
    virtual std::string getName() const ;
    virtual Watchable* clone ();

private:
    std::string name;
};


class Episode: public Watchable{
public:

    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string printContent()  ;
    virtual std::string getName() const ;
    int getseason();
    int getEpisod();
    virtual Watchable* clone ();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
