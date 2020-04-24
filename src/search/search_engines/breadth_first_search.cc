
#include "breadth_first_search.h"

#include "utils.h"
#include "../states/extensional_states.h"
#include "../successor_generators/successor_generator.h"


#include <iostream>
#include <queue>
#include <vector>

using namespace std;


template <class PackedStateT>
int BreadthFirstSearch<PackedStateT>::search(const Task &task,
                                             SuccessorGenerator &generator,
                                             Heuristic &heuristic)
{
    cout << "Starting breadth first search" << endl;
    clock_t timer_start = clock();

    StatePackerT packer(task);
    std::queue<StateID> queue;

    SearchNode& root_node = space.insert_or_get_previous_node(packer.pack_state(task.initial_state), LiftedOperatorId::no_operator, StateID::no_state);
    root_node.open(0);
    statistics.report_f_value_progress(root_node.g);
    queue.emplace(root_node.state_id);


    if (task.is_goal(task.initial_state)) {
        cout << "Initial state is a goal" << endl;
        print_goal_found(generator, timer_start);
        auto plan = space.extract_plan(root_node);
        print_plan(plan, task);
        return SOLVED;
    }


    while (not queue.empty()) {
        StateID sid = queue.front();
        queue.pop();
        SearchNode node = space.get_node(sid);
        if (node.status == SearchNode::Status::CLOSED) {
            continue;
        }
        node.close();
        statistics.report_f_value_progress(node.g);
        statistics.inc_expanded();

        assert(sid.id() >= 0 && (unsigned) sid.id() < space.size());

        auto successors = generator.generate_successors(task.actions, packer.unpack_state(space.get_state(sid)), task.static_info);

        statistics.inc_generated(successors.size());

        for (const auto &successor : successors) {
            const State &s = successor.first;
            const LiftedOperatorId &a = successor.second;

            auto& child_node = space.insert_or_get_previous_node(packer.pack_state(s), a, node.state_id);
            if (child_node.status == SearchNode::Status::NEW) {
                child_node.open(node.g+1);

                if (task.is_goal(s)) {
                    print_goal_found(generator, timer_start);
                    auto plan = space.extract_plan(child_node);
                    print_plan(plan, task);
                    return SOLVED;
                }

                queue.emplace(child_node.state_id);
            }
        }
    }

    print_no_solution_found(timer_start);

    return NOT_SOLVED;
}

template <class PackedStateT>
void BreadthFirstSearch<PackedStateT>::print_statistics() const {
    statistics.print_detailed_statistics();
    space.print_statistics();
}

// explicit template instantiations
template class BreadthFirstSearch<SparsePackedState>;
template class BreadthFirstSearch<ExtensionalPackedState>;