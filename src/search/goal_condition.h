#include <utility>

#ifndef SEARCH_GOAL_CONDITION_H
#define SEARCH_GOAL_CONDITION_H

#include <utility>
#include <unordered_set>
#include <vector>

using namespace std;

/**
 * @brief Define a single atom contained in the goal condition.
 *
 * @var predicate: Index of the predicate symbol of the atom.
 * @var args: Vector containing the indices of the objects instantiating the atom.
 * @var negated: Boolean value indicating whether the atom is negated in the goal.
 *
 */
struct AtomicGoal {
    int predicate;
    std::vector<int> args;
    bool negated;
    AtomicGoal(int predicate, std::vector<int> args, bool negated)
            : predicate(predicate), args(std::move(args)),
              negated (negated) { }
};


/**
 * @brief Represent the goal condition of the task
 *
 * @var goal: Vector of ground atoms (AtomicGoal) in the goal condition.
 * @var positive_nullary_goals: Nullary predicates that appear in the goal.
 * @var negative_nullary_goals: Nullary predicates that appear negated in the goal.
 *
 * @see AtomicGoal (goal_condition.h)
 *
 */
class GoalCondition {
public:
    std::vector<AtomicGoal> goal;
    std::unordered_set<int> positive_nullary_goals;
    std::unordered_set<int> negative_nullary_goals;

    GoalCondition() = default;

    explicit GoalCondition(std::vector<AtomicGoal> goal,
                           std::unordered_set<int> positive_nullary_goals,
                           std::unordered_set<int> negative_nullary_goals) :
            goal(std::move(goal)),
            positive_nullary_goals(std::move(positive_nullary_goals)),
            negative_nullary_goals(std::move(negative_nullary_goals)) {

    }
};


#endif //SEARCH_GOAL_CONDITION_H
