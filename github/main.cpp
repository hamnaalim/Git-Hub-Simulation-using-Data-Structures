#include "githubsimulation.h"
using namespace std;
int main() {
    GitSimulation git;

    git.readUserDataFromCSV();
    git.readRepositoryDataFromCSV();

    int choice;
    string loggedInUser;

    while (true) {
        system("cls");
        if (!loggedInUser.empty()) {
            cout << "\t\t\tLogged in user: " << loggedInUser << endl;
        }
        else {
            cout << "\t\t\tNo user logged in" << endl;
        }

        cout << "\n\t\t\t1. Register\n\t\t\t2. Login\n\t\t\t3. Logout\n\t\t\t4. View Profile\n\t\t\t5. Create Repository\n\t\t\t6. Delete Repository\n\t\t\t7. Fork Repository\n\t\t\t8. Commit to Repository\n\t\t\t9. View Repository Stats\n\t\t\t10. Add File to Repository\n\t\t\t11. Remove File from Repository\n\t\t\t12. Follow User\n\t\t\t13. Unfollow User\n\t\t\t14. Exit\n\t\t\t";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            git.registerUser();
            break;
        case 2:
            git.login(loggedInUser);
            break;
        case 3:
            git.logout(loggedInUser);
            break;
        case 4: {
            string usernameToView;
            cout << "\t\t\tEnter username to view profile: ";
            cin >> usernameToView;
            git.viewProfile(usernameToView);
            break;
        }
        case 5:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string repoName;
                bool isPublic;
                cout << "\t\t\tEnter repository name: ";
                cin >> repoName;
                cout << "\t\t\tEnter visibility (0 for private, 1 for public): ";
                cin >> isPublic;
                // Pass 0 as the commit count for a new repository
                git.createRepository(loggedInUser, repoName, isPublic, 0);
            }
            break;
        case 6:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string repoName;
                cout << "\t\t\tEnter repository name to delete: ";
                cin >> repoName;
                git.deleteRepository(loggedInUser, repoName);
            }
            break;
        case 7:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string sourceUsername, sourceRepoName, targetUsername;
                cout << "\t\t\tEnter source username: ";
                cin >> sourceUsername;
                cout << "\t\t\tEnter source repository name: ";
                cin >> sourceRepoName;
                cout << "\t\t\tEnter target username: ";
                cin >> targetUsername;
                git.forkRepository(sourceUsername, sourceRepoName, targetUsername);
            }
            break;
        case 8:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string repoName, message;
                cout << "\t\t\tEnter repository name to commit: ";
                cin >> repoName;
                cout << "\t\t\tEnter commit message: ";
                cin.ignore(); // Ignore newline character left in the buffer
                getline(cin, message);
                git.commitToRepository(loggedInUser, repoName, message);
            }
            break;
        case 9:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string repoName;
                cout << "\t\t\tEnter repository name to view stats: ";
                cin >> repoName;
                git.viewRepositoryStats(loggedInUser, repoName);
            }
            break;
        case 10:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string repoName, fileName;
                cout << "\t\t\tEnter repository name: ";
                cin >> repoName;
                cout << "\t\t\tEnter file name: ";
                cin >> fileName;
                git.addFileToRepository(loggedInUser, repoName, fileName);
            }
            break;
        case 11:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string repoName, fileName;
                cout << "\t\t\tEnter repository name: ";
                cin >> repoName;
                cout << "\t\t\tEnter file name: ";
                cin >> fileName;
                git.deleteFileFromRepository(loggedInUser, repoName, fileName);
            }
            break;
        case 12:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string following;
                cout << "\t\t\tEnter the username you want to follow: ";
                cin >> following;
                git.followUser(loggedInUser, following);
            }
            break;
        case 13:
            if (loggedInUser.empty()) {
                cout << "\t\t\tPlease login first!" << endl;
            }
            else {
                string unfollowing;
                cout << "\t\t\tEnter the username you want to unfollow: ";
                cin >> unfollowing;
                git.unfollowUser(loggedInUser, unfollowing);
            }
            break;
        case 14:
            git.writeUserDataToCSV();
            git.writeRepositoryDataToCSV();
            return 0;
        default:
            cout << "\t\t\tInvalid choice!" << endl;
            break;
        }
        system("pause");
    }

    return 0;
}
