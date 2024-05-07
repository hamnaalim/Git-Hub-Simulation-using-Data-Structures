#include "githubsimulation.h"

void Repository::addCommit(const Commit& commit, const string& message) {
    commits.push_back(commit);
    commitMessages.push_back(message); // Store commit message
}

vector<string> Repository::getCommitMessages() const {
    vector<string> messages;
    for (const auto& commit : commits) {
        messages.push_back(commit.getMessage());
    }
    return messages;
}

void Repository::setCommitCount(int count) {
    commitCount = count;
}

const vector<Commit>& Repository::getCommits() const {
    return commits;
}

string Repository::getName() const {
    return name;
}

bool Repository::isPublicRepo() const {
    return isPublic;
}

void Repository::addFile(const File& file) {
    files.push_back(file);
}

void Repository::deleteFile(const string& fileName) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        if (it->getName() == fileName) {
            files.erase(it);
            break;
        }
    }
}

const vector<File>& Repository::getFiles() const {
    return files;
}

void Repository::addCommit(const Commit& commit) {
    commits.push_back(commit);
}

int Repository::getFileCount() const {
    return files.size();
}

int Repository::getCommitCount() const {
    return commits.size();
}

TreeNode::~TreeNode() {
    delete repo;
    for (auto& child : children) {
        delete child.second;
    }
}

TreeNode* TreeNode::addChild(const string& name, Repository* repo) {
    TreeNode* newNode = new TreeNode(repo);
    children[name] = newNode;
    return newNode;
}

Repository* TreeNode::getRepo() const {
    return repo;
}

unordered_map<string, TreeNode*>& TreeNode::getChildren() {
    return children;
}

string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

int User::getFollowers() const {
    return followers;
}

void User::incrementFollowers() {
    followers++;
}

void User::decrementFollowers() {
    followers--;
}

GitSimulation::GitSimulation() {}

void GitSimulation::addFileToRepository(const string& username, const string& repoName, const string& fileName) {
    // Check if the user and repository exist
    if (userRepositories.find(username) == userRepositories.end()) {
        cout << "\t\t\tUser not found!" << endl;
        return;
    }

    TreeNode* root = userRepositories[username];

    if (root->getChildren().find(repoName) == root->getChildren().end()) {
        cout << "\t\t\tRepository not found!" << endl;
        return;
    }

    // Get the repository
    Repository* repo = root->getChildren()[repoName]->getRepo();

    // Add the file to the repository
    File newFile(fileName);
    repo->addFile(newFile);

    // Notify user and write to CSV
    cout << "\t\t\tFile added to repository successfully!" << endl;
    // writeRepositoryDataToCSV(); // Assuming the function to write to CSV is defined elsewhere
}

void GitSimulation::deleteFileFromRepository(const string& username, const string& repoName, const string& fileName) {
    // Check if the user and repository exist
    if (userRepositories.find(username) == userRepositories.end()) 
    {
        cout << "\t\t\tUser not found!" << endl;
        return;
    }

    TreeNode* root = userRepositories[username];

    if (root->getChildren().find(repoName) == root->getChildren().end())
    {
        cout << "\t\t\tRepository not found!" << endl;
        return;
    }

    // Get the repository
    Repository* repo = root->getChildren()[repoName]->getRepo();

    // Delete the file from the repository
    repo->deleteFile(fileName);

    // Notify user and write to CSV
    cout << "\t\t\tFile deleted from repository successfully!" << endl;
    // writeRepositoryDataToCSV(); // Assuming the function to write to CSV is defined elsewhere
}

void GitSimulation::readUserDataFromCSV()
{
    ifstream userFile("users.csv");
    string line;

    while (getline(userFile, line)) 
    {
        stringstream ss(line);
        string username, password;

        getline(ss, username, ',');
        getline(ss, password, ',');

        users[username] = User(username, password);
    }

    userFile.close();
}

void GitSimulation::writeUserDataToCSV() 
{
    ofstream userFile("users.csv");

    for (auto const& pair : users)
    {
        userFile << pair.second.getUsername() << "," << pair.second.getPassword() << "," << pair.second.getFollowers() << endl; // Include followers count
    }

    userFile.close();
}

void GitSimulation::readRepositoryDataFromCSV() {
    ifstream repoFile("repositories.csv");
    string line;

    while (getline(repoFile, line)) {
        stringstream ss(line);
        string username, repoName, isPublicStr, commitCountStr, fileCountStr, filesStr;

        getline(ss, username, ',');
        getline(ss, repoName, ',');
        getline(ss, isPublicStr, ',');
        getline(ss, commitCountStr, ',');
        getline(ss, fileCountStr, ',');
        getline(ss, filesStr, ',');

        // Convert strings to integer with error checking
        bool isPublic;
        int commitCount, fileCount;

        try {
            isPublic = stoi(isPublicStr);
            commitCount = stoi(commitCountStr);
            fileCount = stoi(fileCountStr);
        }
        catch (const std::invalid_argument& e) {
            // Print error message and skip processing this line
            cerr << "\t\t\tInvalid data format in CSV: " << e.what() << endl;
            continue;
        }
        catch (const std::out_of_range& e) {
            // Print error message and skip processing this line
            cerr << "\t\t\tOut of range error during stoi conversion: " << e.what() << endl;
            continue;
        }

        Repository* repo = new Repository(repoName, isPublic);

        for (int i = 0; i < fileCount; ++i) {
            string fileName;
            getline(ss, fileName, ',');
            repo->addFile(File(fileName));
        }

        // Check if the user has a repository root node, create one if not
        if (userRepositories.find(username) == userRepositories.end()) {
            TreeNode* root = new TreeNode(nullptr);
            userRepositories[username] = root;
        }

        // Get the user's repository root node
        TreeNode* root = userRepositories[username];

        // Add the repository to the user's tree node
        root->addChild(repoName, repo);
    }

    repoFile.close();
}

void GitSimulation::writeRepositoryDataToCSV() {
    ofstream repoFile("repositories.csv");

    for (auto const& userPair : userRepositories) {
        for (auto const& repoPair : userPair.second->getChildren()) {
            Repository* repo = repoPair.second->getRepo();
            string username = userPair.first;
            string repoName = repo->getName();
            bool visibility = repo->isPublicRepo();
            int commitCount = repo->getCommitCount();
            int fileCount = repo->getFileCount();
            const vector<File>& files = repo->getFiles();
            const vector<Commit>& commits = repo->getCommits(); // Get commits

            repoFile << username << "," << repoName << "," << visibility << "," << commitCount << "," << fileCount;

            // Add commit messages to the output
            for (const auto& commit : commits) {
                repoFile << "," << commit.getMessage();
            }

            // Add file names to the output
            for (const auto& file : files) {
                repoFile << "," << file.getName();
            }

            repoFile << endl;
        }
    }

    repoFile.close();
}

int GitSimulation::getForkCount(const string& username, const string& repoName)
{
    int count = 0;
    for (auto const& userPair : userRepositories)
    {
        if (userPair.first != username)
        {
            TreeNode* root = userPair.second;
            if (root->getChildren().find(repoName) != root->getChildren().end()) 
            {
                count++;
            }
        }
    }
    return count;
   
}

void GitSimulation::registerUser() {
    string username, password;
    cout << "\t\t\tEnter username: ";
    cin >> username;

    if (users.find(username) != users.end()) {
        cout << "\t\t\tUsername already exists!" << endl;
        return;
    }

    cout << "\t\t\tEnter password: ";
    cin >> password;

    users[username] = User(username, password);
    writeUserDataToCSV();
    cout << "\t\t\tRegistration successful!" << endl;
}

void GitSimulation::login(string& loggedInUser) {
    string username, password;
    cout << "\t\t\tEnter username: ";
    cin >> username;

    if (users.find(username) == users.end()) {
        cout << "\t\t\tUser not found!" << endl;
        return;
    }

    cout << "\t\t\tEnter password: ";
    cin >> password;

    if (users[username].getPassword() == password) {
        cout << "\t\t\tLogin successful!" << endl;
        loggedInUser = username;
    }
    else {
        cout << "\t\t\tIncorrect password!" << endl;
    }
}

void GitSimulation::logout(string& loggedInUser) {
    cout << "\t\t\tLogout successful!" << endl;
    loggedInUser = "";
}

void GitSimulation::viewProfile(const string& usernameToView) {
    if (users.find(usernameToView) == users.end()) {
        cout << "\t\t\tUser not found!" << endl;
        return;
    }

    cout << "\t\t\tUsername: " << users[usernameToView].getUsername() << endl;
    cout << "\t\t\tFollowers: " << users[usernameToView].getFollowers() << endl;
}

void GitSimulation::createRepository(const string& username, const string& repoName, bool isPublic, int commitCount) {
    // Check if the user has a repository root node, create one if not
    if (userRepositories.find(username) == userRepositories.end()) {
        TreeNode* root = new TreeNode(nullptr);
        userRepositories[username] = root;
    }

    // Get the user's repository root node
    TreeNode* root = userRepositories[username];

    // Check if the user already has a node for repositories, create one if not
    if (root->getChildren().find(repoName) == root->getChildren().end()) {
        Repository* repo = new Repository(repoName, isPublic);

        // Add dummy commits to simulate commits read from the CSV file
        /*for (int i = 0; i < commitCount; ++i) {
            Commit commit("Dummy commit");
            repo->addCommit(commit);
        }*/

        root->addChild(repoName, repo);
    }
    else {
        cout << "\t\t\tRepository with the same name already exists!" << endl;
        return;
    }

    // Notify user
    cout << "\t\t\tRepository created successfully!" << endl;
}

void GitSimulation::deleteRepository(const string& username, const string& repoName) {
    // Check if the user exists
    if (userRepositories.find(username) == userRepositories.end()) {
        cout << "\t\t\tUser not found!" << endl;
        return;
    }

    // Get the user's repository root node
    TreeNode* root = userRepositories[username];

    // Check if the repository exists
    if (root->getChildren().find(repoName) == root->getChildren().end()) {
        cout << "\t\t\tRepository not found!" << endl;
        return;
    }

    // Delete the repository node
    delete root->getChildren()[repoName];
    root->getChildren().erase(repoName);

    // Notify user and write to CSV
    cout << "\t\t\tRepository deleted successfully!" << endl;
    writeRepositoryDataToCSV();
}

void GitSimulation::forkRepository(const string& sourceUsername, const string& sourceRepoName, const string& targetUsername) {
    // Check if the source user and repository exist
    if (userRepositories.find(sourceUsername) == userRepositories.end()) {
        cout << "\t\t\tSource user not found!" << endl;
        return;
    }

    TreeNode* sourceRoot = userRepositories[sourceUsername];

    if (sourceRoot->getChildren().find(sourceRepoName) == sourceRoot->getChildren().end()) {
        cout << "\t\t\tSource repository not found!" << endl;
        return;
    }

    // Get the source repository
    Repository* sourceRepo = sourceRoot->getChildren()[sourceRepoName]->getRepo();

    // Output the commit messages of the source repository for debugging
    cout << "\t\t\tCommit messages in source repository before fork:" << endl;
    const vector<Commit>& sourceCommits = sourceRepo->getCommits();
    for (const auto& commit : sourceCommits) {
        cout << "- " << commit.getMessage() << endl;
    }

    // Create a new repository for the target user
    createRepository(targetUsername, sourceRepoName, sourceRepo->isPublicRepo(), sourceRepo->getCommitCount()); // Pass commit count

    // Get the target repository
    TreeNode* targetRoot = userRepositories[targetUsername];
    Repository* targetRepo = targetRoot->getChildren()[sourceRepoName]->getRepo();

    // Copy files from source to target repository
    const vector<File>& sourceFiles = sourceRepo->getFiles();
    for (const auto& file : sourceFiles) {
        targetRepo->addFile(file); // Copy file from source to target
    }

    // Copy commit messages from source to target repository
    for (const auto& commit : sourceCommits) {
        targetRepo->addCommit(commit); // Copy commit from source to target
    }

    // Notify user and write to CSV
    cout << "\t\t\tRepository forked successfully!" << endl;
    writeRepositoryDataToCSV();

    // Update the commit count of the source repository
    /*targetRepo->setCommitCount(sourceRepo->getCommitCount());*/
}

void GitSimulation::commitToRepository(const string& username, const string& repoName, const string& message) {
    // Check if the user and repository exist
    if (userRepositories.find(username) == userRepositories.end()) {
        cout << "\t\t\tUser not found!" << endl;
        return;
    }

    TreeNode* root = userRepositories[username];

    if (root->getChildren().find(repoName) == root->getChildren().end()) {
        cout << "\t\t\tRepository not found!" << endl;
        return;
    }

    // Get the repository
    Repository* repo = root->getChildren()[repoName]->getRepo();

    // Create a new commit
    Commit commit(message);

    // Add the commit to the repository along with the message
    repo->addCommit(commit, message);

    // Notify user and write to CSV
    cout << "\t\t\tCommit added successfully!" << endl;
    writeRepositoryDataToCSV();
}

void GitSimulation::viewRepositoryStats(const string& username, const string& repoName) {
    // Check if the user and repository exist
    if (userRepositories.find(username) == userRepositories.end()) {
        cout << "\t\t\tUser not found!" << endl;
        return;
    }

    TreeNode* root = userRepositories[username];

    if (root->getChildren().find(repoName) == root->getChildren().end()) {
        cout << "\t\t\tRepository not found!" << endl;
        return;
    }

    // Get the repository
    Repository* repo = root->getChildren()[repoName]->getRepo();

    // Calculate the fork count
    int forkCount = getForkCount(username, repoName);

    // Display repository stats
    cout << "\t\t\tRepository Name: " << repo->getName() << endl;
    cout << "\t\t\tFile Count: " << repo->getFileCount() << endl;
    cout << "\t\t\tCommit Count: " << repo->getCommitCount() << endl;
    cout << "\t\t\tFork Count: " << forkCount << endl;

    // Display commit messages
    cout << "\t\t\tCommit Messages:" << endl;
    const vector<string>& commitMessages = repo->getCommitMessages();
    for (size_t i = 0; i < commitMessages.size(); ++i) {
        cout << i + 1 << ". " << commitMessages[i] << endl;
    }
}

void GitSimulation::followUser(const string& follower, const string& following) {
    if (users.find(follower) == users.end() || users.find(following) == users.end()) {
        cout << "\t\t\tInvalid user!" << endl;
        return;
    }

    followGraph[follower].insert(following);
    users[follower].incrementFollowers();
    cout <<"\t\t\t" << follower << " is now following " << following << endl;
}

void GitSimulation::unfollowUser(const string& follower, const string& following) {
    if (users.find(follower) == users.end() || users.find(following) == users.end()) {
        cout << "\t\t\tInvalid user!" << endl;
        return;
    }

    if (followGraph[follower].erase(following) > 0) {
        users[follower].decrementFollowers();
        cout<<"\t\t\t" << follower << " has unfollowed " << following << endl;
    }
    else {
        cout <<"\t\t\t" << follower << " was not following " << following << endl;
    }
}
