//
// Created by idosht@wincs.cs.bgu.ac.il on 20/11/2019.
//

#include "../include/Watchable.h"
#include "../include/Session.h"

//============================Rule Of==================

//Constructor
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length),
                                                                                  tags(tags) {
};

//Destructor
Watchable::~Watchable() {

}

//============================Getters=================

std::string Watchable::getid() {
    return std::to_string(id);
}

std::string Watchable::getLength() {
    return std::to_string(length);
}

std::vector<std::string> Watchable::getTags() {
    return tags;
}

//@TODO check about the const;

//============================Adders=================



//============================Prints=================

//Print all the tags from tags Vector by this pattern("[,]")
std::string Watchable::printTags(std::vector<std::string> &tags) {
    std::string output = "[";
    for (int i = 0; (unsigned) i < tags.size(); ++i) {
        output = output + tags[i];
        if (i < tags.size() - 1) {
            output = output + ",";
        }
    }
    output = output + "]";
    return output;
}

//============================Movie==================

//============================Rule Of==================
//Movie Constructor
Movie::Movie(long id,std::string  name, int length, const std::vector<std::string>& tags) :
        Watchable(id, length, tags), name(std::move(name)){
}



//============================Getters=================\\

Watchable *Movie::getNextWatchable(Session &session) const {
    return session.getActiveUser()->getRecommendation(session);
}

std::string Movie::printContent() {
    //TODO check about this ;
    std::vector<std::string> mytags = this->getTags();
    std::string output = " " + getLength() + " minutes" + printTags(mytags);
    return output;
}

std::string Movie::getName() const {
    return name;
}

Watchable *Movie::clone() {
    Watchable* newWatchable=new Movie(std::stoi( getid() ),name,std::stoi(getLength()),getTags());

    return newWatchable;
}

std::string Movie::toString() const {
    return name;
}


//============================Episode==================\\

//Episode Constructor
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) : Watchable(id, length, tags), seriesName(seriesName),
                                                         season(season), episode(episode) {
}



//============================Getters=================\\

int Episode::getseason() {

    return (this->season);
}

int Episode::getEpisod() {
    return (this->episode);
}

std::string Episode::getName() const {
    return seriesName;
}

Watchable *Episode::getNextWatchable(Session &session) const {
    Watchable *currentWatch = session.getContent().at(session.getContentId());
    if (session.getContent().size() > session.getContentId() + 1) {
        int nextEpisodeId=session.getContentId()+1;
        Watchable *nextWatch = session.getContent().at(nextEpisodeId);
        if (currentWatch->getName() == nextWatch->getName()) {
            session.setContentId(session.getContentId()+1);
            return nextWatch;
        } else{
            return session.getActiveUser()->getRecommendation(session);
        }
    } else {
        return session.getActiveUser()->getRecommendation(session);
    }
}

//============================Clone=================\\

Watchable *Episode::clone() {
    Watchable* newWatchable=new Episode(std::stoi( getid() ),seriesName,std::stoi(getLength()),season,episode,getTags());

    return newWatchable;
}

//============================Prints=================

std::string Episode::printContent() {
    int episode = this->getEpisod();
    int season = this->getseason();
    std::string seasonOutput;
    std::string episodeOutput;
    std::vector<std::string> myTags = this->getTags();
    if (episode < 10) {
        episodeOutput = "0" + std::to_string(episode);
    } else {
        episodeOutput = std::to_string(episode);
    }
    if (season < 10) {
        seasonOutput = "0" + std::to_string(season);
    } else
        seasonOutput = std::to_string(season);

    return "S" + seasonOutput + "E" + episodeOutput + " " + getLength() + " minutes " + printTags(myTags);
}

std::string Episode::toString() const {
    std::string episodeOutput;
    std::string seasonOutput;
    if (this->episode < 10) {
        episodeOutput = "0" + std::to_string(episode);
    } else {
        episodeOutput = std::to_string(episode);
    }
    if (season < 10) {
        seasonOutput = "0" + std::to_string(season);
    } else {
        seasonOutput = std::to_string(season);
    }
    return (seriesName + " S" + seasonOutput + "E" + episodeOutput);
}





