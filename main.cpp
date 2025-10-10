#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

struct User {
    unordered_map<int, double> ratings;
    double averageRating = 0.0;
};

unordered_map<int, User> userRatings;
unordered_map<int, vector<int>> movieToUsers;
unordered_map<int, unordered_map<int, double>> similarityCache;

void precomputeMovieMapping() {
    for (const auto& userPair : userRatings) {
        int userId = userPair.first;
        const auto& user = userPair.second;

        for (const auto& ratingPair : user.ratings) {
            int movieId = ratingPair.first;
            if (movieToUsers[movieId].empty() || movieToUsers[movieId].back() != userId) {
                movieToUsers[movieId].push_back(userId);
            }
        }
    }
}

double pearsonSimilarity(int userA, int userB) {
    if (userA > userB) swap(userA, userB);
    if (similarityCache[userA].count(userB)) {
        return similarityCache[userA][userB];
    }

    const auto& ratingsA = userRatings[userA].ratings;
    const auto& ratingsB = userRatings[userB].ratings;

    double meanA = userRatings[userA].averageRating;
    double meanB = userRatings[userB].averageRating;

    double numerator = 0.0, denomA = 0.0, denomB = 0.0;

    for (const auto& ratingPair : ratingsA) {
        int movieId = ratingPair.first;
        auto it = ratingsB.find(movieId);
        if (it != ratingsB.end()) {
            double centeredA = ratingPair.second - meanA;
            double centeredB = it->second - meanB;

            numerator += centeredA * centeredB;
            denomA += centeredA * centeredA;
            denomB += centeredB * centeredB;
        }
    }

    if (denomA < 1e-10 || denomB < 1e-10) return 0.0;

    double result = numerator / sqrt(denomA * denomB);
    similarityCache[userA][userB] = result;
    return result;
}

vector<pair<int, double>> findSimilarUsers(int userID, int movieID, int k, double threshold) {
    vector<pair<int, double>> similarities;

    for (int otherUserID : movieToUsers[movieID]) {
        if (userID != otherUserID) {
            double similarity = pearsonSimilarity(userID, otherUserID);
            if (similarity >= threshold) {
                similarities.emplace_back(otherUserID, similarity);
            }
        }
    }

    if (similarities.size() > static_cast<size_t>(k)) {
        partial_sort(similarities.begin(), similarities.begin() + k, similarities.end(),
                     [](const pair<int, double>& a, const pair<int, double>& b) { return a.second > b.second; });
        similarities.resize(k);
    } else {
        sort(similarities.begin(), similarities.end(),
             [](const pair<int, double>& a, const pair<int, double>& b) { return a.second > b.second; });
    }

    return similarities;
}

double predictRating(int userID, int movieID, int k, double threshold) {
    auto similarUsers = findSimilarUsers(userID, movieID, k, threshold);

    double numerator = 0.0, denominator = 0.0;
    for (const auto& similarUser : similarUsers) {
        int similarUserID = similarUser.first;
        double similarity = similarUser.second;

        auto it = userRatings[similarUserID].ratings.find(movieID);
        if (it != userRatings[similarUserID].ratings.end()) {
            numerator += similarity * (it->second - userRatings[similarUserID].averageRating);
            denominator += similarity;
        }
    }

    return (denominator == 0.0) ? userRatings[userID].averageRating
                                : userRatings[userID].averageRating + numerator / denominator;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;

    while (getline(cin, line) && line != "test dataset") {
        stringstream ss(line);
        int userId, movieId;
        double rating;
        ss >> userId >> movieId >> rating;
        userRatings[userId].ratings[movieId] = rating;
    }

    precomputeMovieMapping();

    for (auto& userPair : userRatings) {
        auto& user = userPair.second;
        if (!user.ratings.empty()) {
            user.averageRating = accumulate(user.ratings.begin(), user.ratings.end(), 0.0,
                                            [](double sum, const pair<int, double>& p) { return sum + p.second; }) /
                                 user.ratings.size();
        }
    }

    const int k = 120;
    const double threshold = 0.18;

    while (getline(cin, line)) {
        stringstream ss(line);
        int userId, movieId;
        ss >> userId >> movieId;

        double predictedRating = predictRating(userId, movieId, k, threshold);
        predictedRating = max(1.0, min(predictedRating, 5.0));
        cout << fixed << setprecision(1) << predictedRating << endl;
    }
    return 0;
}