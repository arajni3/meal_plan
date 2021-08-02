#include <vector>
#include <iostream>

// meal_packages[j] = {n_j meals, some cost f(n_j)} where f is the same throughout
// meal_packages and where n_j is increasing in j and f is increasing in its input (number of meals)
/* this function returns the index of the meal package in meal_packages that has the smallest
   number of meals that is >= number_of_meals
*/
unsigned int next_meal_package_index(unsigned int number_of_meals,
    const std::vector<std::vector<unsigned int>>& meal_packages) {
    unsigned int low = 0, high = meal_packages.size() - 1;
    while (low < high) {
        int mid = (low + high) >> 1;
        if (meal_packages[mid][0] < number_of_meals) low = mid + 1;
        else high = mid;
    }
    return (number_of_meals <= meal_packages[low][0]) ? low : meal_packages.size();
}

/* returns a vector of {unsigned int, unsigned int} values where the jth entry is
   {n_j, g(n_j)} where g(n_j) is the optimal number of times the jth meal package is purchased
*/
std::vector<std::vector<unsigned int>> get_mealplan(unsigned int number_of_meals,
    const std::vector<std::vector<unsigned int>>& meal_packages = {{20, 142}, {40, 276}, {60, 392}, {80, 493}}) {

    // dp[i] = total cost when one needs number_of_meals meals
    std::vector<unsigned int> dp(number_of_meals + 1);

    dp[0] = 0;

    for (unsigned int i = 1; i <= number_of_meals; ++i) {
        dp[i] = i * meal_packages.back()[1];
        for (unsigned int j = 0; j < meal_packages.size(); ++j) {
            dp[i] = (meal_packages[j][0] > i) ? dp[i] : std::min(dp[i], meal_packages[j][1] + dp[i - meal_packages[j][0]]);
        }
        /* if the meal packages j_k with n_j_k < i did not combine together to yield an optimal value
           for i meals, then dp[i] = meal_packages[j] where j is the smallest index such that i < n_j
        */
        unsigned int smallest_largest_meal_package_index = next_meal_package_index(i, meal_packages);
        if (smallest_largest_meal_package_index < meal_packages.size()) {
            dp[i] = std::min(dp[i], meal_packages[smallest_largest_meal_package_index][1]);
        }
    }
    /* time complexity of the above nested for loop is:
       O(number_of_meals * (meal_packages.size() + O(log(meal_packages.size())))) =
       O(number_of_meals * meal_packages.size())
    */
    // the binary search lowers the size of the hidden constant in the time complexity

    // now backtrack to find the optimal number of purchases of each meal package
    std::vector<std::vector<unsigned int>> mealplan(meal_packages.size());

    for (unsigned int j = 0; j < meal_packages.size(); ++j) {
        mealplan[j] = {meal_packages[j][0], 0};
    }

    unsigned int i = number_of_meals;

    while (i > 0) {
        unsigned int best_meal_package_index = meal_packages.size();
        for (unsigned int j = 0; j < meal_packages.size() && best_meal_package_index == meal_packages.size(); ++j) {
            if (i >= meal_packages[j][0] && (dp[i] == meal_packages[j][1] + dp[i - meal_packages[j][0]])) {
                best_meal_package_index = j;
                i -= meal_packages[j][0];
            }
        }
        if (best_meal_package_index == meal_packages.size()) {
            best_meal_package_index = next_meal_package_index(i, meal_packages);
            i = 0;
        }

        // now, best_meal_package_index is always < meal_packages.size()
        mealplan[best_meal_package_index][1]++;
    }

    /* the total time complexity of get_mealplan is O(2 * (number_of_meals * meal_packages.size())
       = O(number_of_meals * meal_packages.size())
    */
    return mealplan;
}

int main() {
    // I need 198 meals for one semester at Purdue Fort Wayne
    auto my_mealplan = get_mealplan(198);
    for (auto purchases : my_mealplan) {
        std::cout << purchases[0] << ": " << purchases[1] << "\n";
    }
    std::endl(std::cout);
}
