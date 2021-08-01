#include <utility>
#include <vector>

// meal_packages[j] = {n_j meals, some cost f(n_j)} where f is the same throughout
// meal_packages and where n_j is increasing in j and f is increasing in its input (number of meals)
/* this function returns the index of the meal package in meal_packages that has the smallest
   number of meals that is >= number_of_meals
*/ 
int next_meal_package(int number_of_meals, 
std::vector<std::pair<int, int>>& meal_packages = {std::make_pair(20, 142), std::make_pair(40, 276), 
std::make_pair(60, 392), std::make_pair(80, 493)}) {
    int low = 0, high = meal_packages.size() - 1;
    while (low < high) {
        int mid = (low + high) >> 1;
        if (meal_packages[mid + 1].first < number_of_meals) low = mid + 1;
        else high = mid;
    }
    return (low < meal_packages.size())? low : -1;
}

/* returns a vector of pair<int, int> values where the jth entry is 
   {n_j, g(n_j)} where g(n_j) is the optimal number of times the jth meal package is purchased
*/
std::vector<std::pair<int, int>> get_mealplan(int number_of_meals, 
std::vector<std::pair<int, int>>& meal_packages = {std::make_pair(20, 142), std::make_pair(40, 276), 
std::make_pair(60, 392), std::make_pair(80, 493)}) {
    // dp[i] = total cost when one needs number_of_meals meals
    std::vector<int> dp(number_of_meals + 1);

    dp[0] = 0;

    for (int i = 1; i <= number_of_meals; ++i) {
        dp[i] = i * meal_packages.back().second;
        for (int j = 0; j < meal_packages.size(); ++j) {
            dp[i] = (meal_packages[j].first > i)? dp[i] : std::min(dp[i], meal_packages[j].second + 
            dp[i - j]);
        }
        /* if the meal packages j_k with n_j_k < i did not combine together to yield an optimal value
           for i meals, then dp[i] = meal_packages[j] where j is the smallest index such that i < n_j
        */
       int smallest_largest_meal_package = meal_packages[next_meal_package(i, meal_packages)].second;
       if (smallest_largest_meal_package != -1) dp[i] = std::min(dp[i], smallest_largest_meal_package);
    }
    /* time complexity of the above nested for loop is:
       O(number_of_meals * (meal_packages.size() + O(log(meal_packages.size())))) = 
       O(number_of_meals * meal_packages.size())
    */
    // the binary search lowers the size of the hidden constant in the time complexity

    // now backtrack to find the optimal number of purchases of each meal package
    std::vector<std::pair<int, int>> mealplan(meal_packages.size());

    for (int j = 0; j < meal_packages.size(); ++j) {
        mealplan[j] = std::make_pair(meal_packages[j].first, 0);
    }

    int i = number_of_meals;

    while (i > 0) {
        int meal_package_best_index = -1;
        for (int j = 0; j < meal_packages.size() && meal_package_best_index == -1; ++j) {
            if (dp[i] == meal_packages[j].second + dp[i - j]) {
                meal_package_best_index = j;
                i -= meal_packages[j].first;
            }
        }
        if (meal_package_best_index == -1) {
            meal_package_best_index = next_meal_package(i, meal_packages);
            i = 0;
        }
        mealplan[meal_package_best_index].second++;
    }
    return mealplan;
}