#pragma once
#ifndef GITSIMULATION_H
#define GITSIMULATION_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib> // for system("cls")
#include <unordered_set>

using namespace std;

class Commit {
private:
    string message;

public:
    Commit(string msg) : message(msg) {}

    string getMessage() const {
        return message;
    }
};

class File {
private:
    string name;

public:
    File(string n) : name(n) {}

    string getName() const {
        return name;
    }
};

class Repository {
private:
    string name;
    bool isPublic;
    vector<File> files;
    vector<Commit> commits;
    vector<string> commitMessages;
    int commitCount;

public:
    Repository(string n, bool pub) : name(n), isPublic(pub) {}

    void addCommit(const Commit& commit, const string& message);

    vector<string> getCommitMessages() const;

    void setCommitCount(int count);

    const vector<Commit>& getCommits() const;

    string getName() const;

    bool isPublicRepo() const;

    void addFile(const File& file);

    void deleteFile(const string& fileName);

    const vector<File>& getFiles() const;

    void addCommit(const Commit& commit);

    int getFileCount() const;

    int getCommitCount() const;
};

class TreeNode {
private:
    Repository* repo;
    unordered_map<string, TreeNode*> children;

public:
    TreeNode(Repository* r) : repo(r) {}

    ~TreeNode();

    TreeNode* addChild(const string& name, Repository* repo);

    Repository* getRepo() const;

    unordered_map<string, TreeNode*>& getChildren();
};

class User {
private:
    string username;
    string password;
    int followers;

public:
    User() : username(""), password(""), followers(0) {}
    User(string uname, string pwd) : username(uname), password(pwd), followers(0) {}

    string getUsername() const;

    string getPassword() const;

    int getFollowers() const;

    void incrementFollowers();

    void decrementFollowers();
};

class GitSimulation {
private:
    unordered_map<string, User> users;
    unordered_map<string, TreeNode*> userRepositories;
    unordered_map<string, unordered_set<string>> followGraph;

public:
    GitSimulation();

    void addFileToRepository(const string& username, const string& repoName, const string& fileName);

    void deleteFileFromRepository(const string& username, const string& repoName, const string& fileName);

    void readUserDataFromCSV();

    void writeUserDataToCSV();

    void readRepositoryDataFromCSV();

    void writeRepositoryDataToCSV();

    int getForkCount(const string& username, const string& repoName);

    void registerUser();

    void login(string& loggedInUser);

    void logout(string& loggedInUser);

    void viewProfile(const string& usernameToView);

    void createRepository(const string& username, const string& repoName, bool isPublic, int commitCount);

    void deleteRepository(const string& username, const string& repoName);

    void forkRepository(const string& sourceUsername, const string& sourceRepoName, const string& targetUsername);

    void commitToRepository(const string& username, const string& repoName, const string& message);

    void viewRepositoryStats(const string& username, const string& repoName);

    void followUser(const string& follower, const string& following);

    void unfollowUser(const string& follower, const string& following);
};

#endif // GITSIMULATION_H

