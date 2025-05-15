 //BelamyLuchiryFEE3/2552/2024
 #include <iostream>
 #include <vector>
  #include <map>
#include <cmath>
#include   <limits>
 #include <string>
#include <iomanip>
#include <algorithm>

using  namespace std;
struct Rating {
    int itemId;
    double ratescore;
};

class User {
//
public:
    int identifier;
       string name;
    map<int, double> ratings;

    User(int id, const string& name) : identifier(id), name(name) {}

                    void rate(int itemIdentity, double score) {
        ratings[itemIdentity] = score;
    }

  bool hasRated(int itemIdenity) const {
        return ratings.find(itemIdenity) != ratings.end();
    }

    double gettheRating(int itemIdentifier) const {
        auto it = ratings.find(itemIdentifier);
        return (it != ratings.end()) ?it->  second : 0.0;
    }
};
class Item {
public:
int identifier;
     string name;
    map<int, double> ratings;

     Item(int id, const string&   name ) : identifier( id), name (  name ) {

     }

    void addRRating(int userId, double score) {
        ratings [userId] = score;
    }
     
    bool hasbeenRatedBy(int userId) const {
    return ratings.find (userId) != ratings.end();}

    double getRatingBy(int userId) const {
        auto it = ratings.find(userId);
        return (it !=  ratings.end()) ? it->second : 0.0;
    }
};
/*
*/
class   RecommenderSystem {
private:
 map<int, User*> users; 
       map<int, Item*> items;
        map<string,  int> userNameToIdentifier;
    map<string, int> itemNameToIdentifier;
    int     userCounterea = 1;
    int itemCounter = 101;
        const int NEIGHBOR = 10;

public:
//now to add the application user to the system of the recommender
    void addUser   (const string& name) {
        if (  userNameToIdentifier.find(name)     ==userNameToIdentifier.end()) {


                                userNameToIdentifier[name] = userCounterea;
            users[userCounterea] = new User(userCounterea, name);
                ++userCounterea;
        }
    }
    /*adds*/

    void addItem   (const string   & name) {
        if (itemNameToIdentifier.find(name) == itemNameToIdentifier.end()) {


     itemNameToIdentifier[name] = itemCounter;
              items[itemCounter] = new Item(itemCounter, name);
         ++itemCounter;
        }
    }

void    addRating   (const string    &userName, const string& itemName, double score) {

 addUser(userName);
        addItem(itemName);
                 int    userId =userNameToIdentifier   [userName];
          int itemId = itemNameToIdentifier[itemName];
        users[userId]->  rate(itemId, score);

             items[itemId]->addRRating(userId, score);
    }

double    Similarityamongusers    (User*    u1, User*u2) {
        vector<int> common;
        for (auto& r : u1->ratings)
            if (u2->hasRated(r.first))
                common.push_back(r.first);

        int n = common.size();     if (n == 0) return 0.0;

        double sum1= 0,   sum2 =   0, sum1Sq= 0, sum2Sq =0, pSum = 0
        ;
        for (int itemId : common) {
    double r1 = u1->gettheRating(itemId);
            double r2 = u2->gettheRating(itemId);
                 sum1 += r1; sum2 += r2;
            sum1Sq += r1 * r1;
                 sum2Sq += r2 * r2;
                           pSum += r1 * r2;
        }

        double num = pSum - (sum1 * sum2 / n);
            double den= sqrt (   (sum1Sq -sum1 *sum1 / n)* (  sum2Sq - sum2 *sum2 / n) );
        return(den == 0)     ? 0.0   : num /den;
    }

    double theitemSimilarity(Item* i1, Item* i2) {
        vector<int> common;
        for (auto& r : i1->ratings)
            if (i2->hasbeenRatedBy(r.first))
                common.push_back(r.first);

        int n = common.size();
        if (n == 0)                  return 0.0;

        double sum1 = 0, sum2 = 0, sum1Sq = 0, sum2Sq = 0, pSum = 0;
        for (int userId : common) {
               double r1 = i1->getRatingBy(userId);
                 double r2 = i2->getRatingBy(userId);
            sum1 += r1; sum2 += r2;
                     sum1Sq += r1 * r1;
            sum2Sq+= r2 * r2;
                  pSum +=    r1 * r2;
        }

          double num = pSum - (sum1 * sum2/ n);
        double den =sqrt((sum1Sq - sum1 * sum1 / n) * (sum2Sq - sum2 * sum2 / n));
        return (den == 0) ? 0.0 : num / den;
    }
    double predictonUserBased(int userId, int itemId) {
    if (users.find(userId) == users.end()) return 0.0;

    vector<pair<double, double>> sims;

    for (auto& pair : users) {
        User* other = pair.second;
                     if (other->identifier == userId) continue;
         if (!other->hasRated(itemId)) continue;

          double sim = Similarityamongusers(users[userId], other);
        if (sim >0) sims.push_back({sim, other->gettheRating(itemId)});
    }

    sort(sims.rbegin(), sims.rend());
    if (sims.size() > NEIGHBOR) sims.resize(NEIGHBOR);

       double nume = 0.0, denom = 0.0;
    for (auto& s : sims) {
     nume +=   s.first *s.second;
                           denom =denom + s.first;
    }

    return (denom == 0) ? 0.0 : nume / denom;
}

    double predictionItemBased(int userId, int itemId) {
           if (users.find(userId) == users.end()) return 0.0;
                        User* user = users[userId];
 vector<pair<double, double>> sims;
        for (auto& r : user->ratings) {
            if (r.first == itemId) continue;
                 double sim = theitemSimilarity(items[itemId], items[r.first]);
            if (sim > 0) sims.push_back({sim, r.second});


        }

              sort(sims.rbegin(), sims.rend());
        if (sims.size() > NEIGHBOR) sims.resize(NEIGHBOR);

        double num = 0, den = 0;
        for (auto& s : sims) {
            num += s.first * s.second;
            den += s.first;

        }
if(den == 0){ return 0.0;}else return num/den;

    }

    void recommendationofItems(const string& userName, int topN = 5) {
        if (userNameToIdentifier.find(userName) == userNameToIdentifier.end()) {
            cout << "User not found."<<endl;
            return;
        }

        int userId = userNameToIdentifier[userName];
        vector<pair<double, Item*>> results;

        for (auto& pair : items) {
            if (users[userId]->hasRated(pair.first)) continue;
            double predicted = predictionItemBased(userId, pair.first);
            results.push_back({predicted, pair.second});
        }

        sort(results.rbegin(), results.rend());
        cout << "\nRecommendations for " << users[userId]->name << ":\n";
        for (int i = 0; i < topN && i < results.size(); ++i) {
            cout << fixed << setprecision(2)
                 << results[i].second->name << "Predicted: " << results[i].first << " \n";
        }
    }

    void explainRecommendation(const string& userName, const string& itemName) {
        if (userNameToIdentifier.find(userName) == userNameToIdentifier.end() || itemNameToIdentifier.find(itemName) == itemNameToIdentifier.end()) {
            cout << "User or item not found.\n ";
            return;
        }

        int userId = userNameToIdentifier[userName];
        int itemId = itemNameToIdentifier[itemName];

        cout << "\nExplanation for " << itemName << " :\n  ";
        bool found = false;
        for (auto& r : users[userId]->ratings) {
            if (r.first == itemId) continue;
            double sim = theitemSimilarity(items[itemId], items[r.first]);
            if (sim > 0) {
                found = true;
                cout << "-It's Similar to " << items[r.first]->name<< " (you rated " << r.second
            << ", similarity: " << fixed << setprecision(2) << sim << ")\n";
            }
        }
        if (!found) {
            cout << "  No sufficient data to explain the recommendation.\n";
        }
    }

    double evaluateRMSE() {
        double sum = 0.0;
        int count = 0;
        for (auto& pair : users) {
            User* user = pair.second;
            for (auto& r : user->ratings) {
                double predict = predictionItemBased(user->identifier, r.first);
                if (predict > 0.0) {
                    sum += pow(predict - r.second, 2);
                    count++;
                }
            }
        }
        return (count == 0) ? 0.0 : sqrt(sum / count);
    }

    double evaluateMAE() {
        double sum = 0.0;
        int count = 0;
        for (auto& pair : users) {
            User* user = pair.second;
            for (auto& r : user->ratings) {
                double pred = predictionItemBased(user->identifier, r.first);
                if (pred > 0.0) {
                    sum += fabs(pred - r.second);
                    count++;
                }
            }
        }
        if(count ==0) return 0.0;
        else{return sum/count;}
    }

    void Menu() {
        int decision;
        string uname, iname;
        double rating;

        do {
            cout << "\n======= Collaborative Filtering Menu ............\n";
            cout <<"Kindly Take note that lest more items are added by user the available items(movies) in the system are Oppenheimer,Ted,"
            "Moneyheist,Inception,Friends and Familyguy, The users in the system are Belamy,Peter,Luchiry and patrick you're free to add more though\n";
            cout << "1. Add Rating (Name-based)\n";
            cout << "2. Recommend Items\n";
            cout << "3. Explain Recommendation\n";
            cout << "4. Evaluate RMSE\n";
            cout << "5. Evaluate MAE\n";
            cout << "6. Exit\n";
            cout << "7. Add User\n";
            cout << "8. Add Item\n";
            cout << "9. Cold-start Recommend for New User\n";
            cout << "10. Suggest Users for New Item\n";
            cout << "Enter choice: ";
            cin >> decision;

            switch (decision) {
                case 1:
                    cout << "Enter user name:\n";
                    cin >> ws;
                    getline(cin, uname);
                    cout << "Enter item name:\n";
                    getline(cin, iname);
                    cout << "Enter rating:\n";
                    cin >> rating;
                    addRating(uname, iname, rating);
                    break;
                case 2:
                    cout << "Enter user name:"<<endl;
                    cin >> ws;
                    getline(cin, uname);
                    recommendationofItems(uname);
                    break;
                case 3:
                    cout <<   "Enter user name:\n";
                    cin >> ws;
                    getline(cin, uname);
                    cout << "Enter item name:"<<endl;
                    getline(cin, iname);
                    explainRecommendation(uname, iname);
                    break;
                case 4:
                    cout << "RMSE:" << fixed << setprecision(4) << evaluateRMSE() << "\n";
                    break;
                case 5:
                    cout <<"MAE:    " << fixed << setprecision(4) << evaluateMAE() << endl;
                    break;
                case 6:
                    cout<< "Goodbye!"<<endl;
                    break;
                case 7:
                    cout <<"Enter new user name:\n";
                    cin >> ws;
                    getline(cin, uname);
                    addUser(uname);
                    break;
                case 8:
                    cout<< "Enter new item name  :\n";
                    cin >> ws;
                    getline(cin, iname);
                    addItem(iname);
                    break;
            case 9:
                    cout << "Enter user name:\n";
                    cin >>ws;
                        getline(cin, uname);
                    recommendColdStartUser(uname);
                    break;
                        case 10:
                         cout << "Enter item name:\n";
                    cin >>ws;
                    getline(cin, iname);
                    suggestUsersForNewItem(iname);
          break;
          case 11:
          system("cls");
          break;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (decision != 6);
    }
    void recommendColdStartUser(const string& userName, int topN = 5) {
        if (userNameToIdentifier.find(userName) == userNameToIdentifier.end()) {
            cout << "User not found.\n";
            return;
        }

 int userId = userNameToIdentifier[userName];
                      if (!users[userId]->ratings.empty()) {
                 recommendationofItems(userName, topN);
            return;
        }
        vector<pair<double, Item*>> avgRatings;
        for (auto& pair : items) {
            if (pair.second->ratings.empty()) continue;

                double sum = 0;
                     for (auto& r : pair.second->ratings) sum += r.second;
            double avg = sum / pair.second->ratings.size();
                       avgRatings.push_back({avg, pair.second});
        }

        sort(avgRatings.rbegin(), avgRatings.rend());
        cout << "Cold-start recommendations for new user " << userName<< ":\n";
        for (int i = 0; i < topN && i < avgRatings.size(); ++i) {
            cout << fixed << setprecision(2)<<  avgRatings[i].second->name
<< avgRatings[i].second->name << " (Average Rating: " << avgRatings[i].first << ")\n";
        }
    }
 void suggestUsersForNewItem(const string& itemName, int topN = 5) {
        if (itemNameToIdentifier.find(itemName) == itemNameToIdentifier.end()) {
                         cout <<"Item is not found.\n";
            return;}

     int itemId = itemNameToIdentifier[itemName];
        if (!items  [itemId]->ratings.empty()) {
              cout << "sorry This item already has ratings..\n";
            return;
        }
        vector<pair<int, User*>> active;
        for (auto& pair : users) {
            active.push_back({(int)pair.second->ratings.size(), pair.second});
        }

               sort(active.rbegin(), active.rend());
        cout << "\nSuggested users for new item '" <<itemName << "':\n";
        for (int i = 0; i < topN && i < active.size(); ++i) {
            cout <<active[i].second->name << " Rated " <<active[i].first << " items\n";
        }
    }

    ~RecommenderSystem() {
           for (auto& p :users) delete p.second;
        for (auto& p  :  items) delete p.second;
    }
};
int main() {
RecommenderSystem system;

// data in system to use as example
  system.addRating("Belamy", "Friends", 5.0);
system.addRating("Belamy", "Oppenheimer", 4.5);
     system.addRating("Belamy", "Ted", 4.0);
     system.addRating("Belamy", "Moneyheist", 3.5);  ;
           system.addRating("Peter", "Inception", 4.0);
system.addRating("Peter", "Familyguy", 4.5);
     system.addRating("Peter", "Moneyheist", 1.2);  
   system.addRating("Peter", "Friends", 4.0);         
  system.addRating("Patrick", "Friends", 3.0);
 system.addRating("Patrick", "Moneyheist", 2.1);
  system.addRating("Patrick", "Ted", 3.5);
    system.addRating("Patrick", "Familyguy", 4.0);
system.addRating("Luchiry", "Oppenheimer", 5.0);
    system.addRating("Luchiry", "Familyguy", 2.9);     
 system.addRating("Luchiry", "Ted", 4.5);           
   system.addRating("Luchiry", "Friends", 4.5);       
 system.addRating("Luchiry", "Inception", 4.5);
          system.Menu();
                   return 0;
}