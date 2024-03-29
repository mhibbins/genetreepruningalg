#include <iostream>
#include <algorithm>
#include <regex>
#include <vector>
#include <string>

#include "clade.hpp"

clade::clade(const clade& c, clade* parent, std::function<double(const clade& c)> branchlength_setter) {
    _p_parent = parent;
    _taxon_name = c._taxon_name;
    if (branchlength_setter)
        _branch_length = branchlength_setter(c);
    else
        _branch_length = c._branch_length;
    _descendants.resize(c._descendants.size());
    transform(c._descendants.begin(), c._descendants.end(), _descendants.begin(), [&](const clade* c) { return new clade(*c, this, branchlength_setter);});

    if (is_root())	// for simplicity, we do not calculate descendantss reverse-level order arrays
        update_reverse_level_order();
}

/* Recursive destructor */
clade::~clade() {

  for (auto i : _descendants) {
    delete i; // recursively delete all descendants
  }
}

const clade *clade::get_parent() const {

  return _p_parent; // memory address
}

double clade::get_branch_length() const {

    return _branch_length;
}

/* Adds descendant to vector of descendants */
void clade::add_descendant(clade *p_descendant) {
    
  _descendants.push_back(p_descendant);
  _name_interior_clade();
  if (!is_root()) {
    _p_parent->_name_interior_clade();
  }

  update_reverse_level_order();
}

void clade::remove_descendant(clade *p_descendant) {
    
  _descendants.erase(std::remove(_descendants.begin(), _descendants.end(), p_descendant), _descendants.end());
  _name_interior_clade();
  if (!is_root()) {
    _p_parent->_name_interior_clade();
  }

  update_reverse_level_order();
}

void clade::update_reverse_level_order()
{
    _reverse_level_order.clear();
    _insert_reverse_level_order.clear();
    std::stack<const clade*> stack;
    std::queue<const clade*> q;
    std::stack<clade*> insert_stack;
    std::queue<clade*> insert_q;

    q.push(this);
    insert_q.push(this);

    while (!q.empty())
    {
        /* Dequeue node and make it current */
        auto current = q.front();
        q.pop();
        stack.push(current);

        for (auto i : current->_descendants)
        {
            /* Enqueue child */
            q.push(i);
        }
    }

    while (!insert_q.empty())
    {
        /* Dequeue node and make it current */
        auto current = insert_q.front();
        insert_q.pop();
        insert_stack.push(current);

        for (auto i : current->_descendants)
        {
            /* Enqueue child */
            insert_q.push(i);
        }
    }

    while (!stack.empty())
    {
        auto current = stack.top();
        stack.pop();
        _reverse_level_order.push_back(current);
    }

    while (!insert_stack.empty())
    {
        auto current = insert_stack.top();
        insert_stack.pop();
        _insert_reverse_level_order.push_back(current);
    }

    if (!is_root()) {
        _p_parent->update_reverse_level_order();
    }
}

/* Recursively fills vector of names provided as argument */
void clade::add_leaf_names(std::vector<std::string> &vector_names) {

    if (_descendants.empty()) {
        vector_names.push_back(_taxon_name); // base case (leaf), and it starts returning
    }
    else {
        for (auto desc : _descendants) {
            desc->add_leaf_names(vector_names);
        }
    }
}

/* Recursively finds internal nodes, and returns vector of clades */
std::vector<const clade*> clade::find_internal_nodes() const {

  std::vector<const clade*> internal_nodes;

  /* Base case: returns empty vector */
  if (is_leaf()) { return internal_nodes; }

  else {
    internal_nodes.push_back(this);

    for (auto i : _descendants) {
      auto descendant = i->find_internal_nodes(); // recursion
      if (!descendant.empty()) { internal_nodes.insert(internal_nodes.end(), descendant.begin(), descendant.end()); }
    }

    return internal_nodes;
  }
}


  /* Recursively find pointer to clade with provided taxon name */
const clade *clade::find_descendant(std::string some_taxon_name) const {

    const clade *p_descendant;
    auto descendant_finder = [some_taxon_name, &p_descendant](const clade *clade) {
        if (clade->get_taxon_name() == some_taxon_name)
            p_descendant = clade;
    };

  apply_prefix_order(descendant_finder);

  return p_descendant;
}
  
/* Finds branch length of clade with provided taxon name. Does so by calling find_descendant(), which recursively searches the tree */
double clade::find_branch_length(std::string some_taxon_name) {

  auto clade = find_descendant(some_taxon_name);
  if (clade == NULL || clade->is_root()) { return 0; } // guarding against root query

  std::cout << "Found matching clade";
  return clade->_branch_length;
}

/* Names interior clades, starting from clade of first method call and going up the tree until root */
void clade::_name_interior_clade() {

    std::vector<std::string> descendant_names; // vector of names
    add_leaf_names(descendant_names); // fills vector of names
    sort(descendant_names.begin(), descendant_names.end()); // sorts alphabetically (from std)
    _taxon_name.clear(); // resets whatever taxon_name was
    for (auto name : descendant_names) {
        _taxon_name += name;
    }

    if (_p_parent)
        _p_parent->_name_interior_clade();
}

bool clade::is_leaf() const {

  return _descendants.empty();
}

bool clade::is_root() const {

  return get_parent() == NULL;
}

/* Function print_clade_name() is used in conjunction with apply_reverse_level_order and apply_prefix order for debugging purposes.
   e.g.,
   cout << "Tree " << p_tree->get_taxon_name() << " in reverse order: " << "\n";
   p_tree->apply_reverse_level_order(print_clade_name)   
*/
void print_clade_name(clade *clade) {
   std::cout << clade->get_taxon_name() << " (length of subtending branch: " << clade->get_branch_length() << ")" << "\n";
}

void clade::write_newick(std::ostream& ost, std::function<std::string(const clade *c)> textwriter) const
{
    if (is_leaf()) {
        ost << textwriter(this);
    }
    else {
        ost << '(';

        // some nonsense to supress trailing comma
        for (size_t i = 0; i< _descendants.size() - 1; i++) {
            _descendants[i]->write_newick(ost, textwriter);
            ost << ',';
        }

        _descendants[_descendants.size() - 1]->write_newick(ost, textwriter);
        ost << ')' << textwriter(this);
    }
}

double clade::distance_from_root_to_tip() const
{
    if (is_leaf()) {return 0;}
    std::vector<double> candidates;

    apply_prefix_order([&candidates, this](const clade* c) {
        if (c->is_leaf())
        {
            auto p = c;
            double dist = 0;
            while (p && p != this)
            {
                dist += p->get_branch_length();
                p = p->get_parent();
            }
            candidates.push_back(dist);
        }
        });

    return *std::max_element(candidates.begin(), candidates.end());
}

std::string clade_index_or_name(const clade* node, const cladevector& order)
{
    auto id = distance(order.begin(), find(order.begin(), order.end(), node));
    if (node->is_leaf())
        return node->get_taxon_name() + "<" + std::to_string(id) + ">";
    else
    {
        return "<" + std::to_string(id) + ">";
    }
}

std::vector<double> clade::get_branch_lengths() const
{
    std::vector<double> result;
    auto branch_length_func = [&result](const clade* c) { 
        if (c->get_branch_length() > 0.0)
            result.push_back(c->get_branch_length()); 
    };
    apply_prefix_order(branch_length_func);
    return result;
}

std::set<double> clade::get_speciation_times() const
{
    std::set<double> sp_time_set;

    auto sp_time_func = [&sp_time_set](const clade* c) {

        if (c->is_leaf() == true) {
            sp_time_set.insert(c->get_branch_length());
        };
    };
 
    apply_prefix_order(sp_time_func);

    return sp_time_set;
}

std::set<double> get_branch_intervals(clade* sptree, std::vector<clade*> genetrees) {

    std::set<double> branch_intervals;
    std::set<double> sptree_branches = sptree->get_speciation_times();
    std::set<double> genetrees_branches;

    for (int i=0; i < genetrees.size(); i++) {

        std::set<double> genetree_branches = genetrees[i]->get_speciation_times();

        std::set<double>::iterator it = genetree_branches.begin();

        while (it != genetree_branches.end()) { 
            genetrees_branches.insert(*it);
            branch_intervals.insert(*it); //tip branches in gene trees
            it++;
        }
    }

    std::set<double>::iterator it = sptree_branches.begin();

    while (it != sptree_branches.end()) {

        branch_intervals.insert(*it); //tip branches in species tree

        std::set<double>::iterator it2 = genetrees_branches.begin();

        while (it2 != genetrees_branches.end()) {
            double interval = *it2 - *it; //time slices between species tree and gene tree nodes
            if (interval > 0) {branch_intervals.insert(interval);}
            it2++;
        }
        it++;
    }

    std::set<double>::iterator it3 = sptree_branches.begin();

    while (it3 != sptree_branches.end()) {

        std::set<double>::iterator it4 = sptree_branches.begin();

        while (it4 != sptree_branches.end()) {
            double interval = *it4 - *it3; //species tree internal branches 
            if (interval > 0) {branch_intervals.insert(interval);}
            it4++;
        }
        it3++;
    }

    std::set<double>::iterator it5 = genetrees_branches.begin();

    while (it5 != genetrees_branches.end()) {

        std::set<double>::iterator it6 = genetrees_branches.begin();

        while (it6 != genetrees_branches.end()) {
            double interval = *it6 - *it5; //gene tree internal branches 
            if (interval > 0) {branch_intervals.insert(interval);}
            it6++;
        }
        it5++;
    }

    return branch_intervals;


} //returns time intervals for matrix cache

void clade::apply_to_descendants(const cladefunc& f) const {

    // apply f to direct descendants
    // could replace with apply_prefix_order for functions f that recur through descendants
    //for_each(_descendants.begin(), _descendants.end(), f); // for_each from std
    // for_each apparently passes by value
    for (auto desc : _descendants)
        f(desc);
}

//! apply the functor f to this clade and also to all descendants.
void clade::apply_prefix_order(const cladefunc& f) const {
    std::stack<const clade*> stack;
    stack.push(this);
    while (!stack.empty())
    {
        auto c = stack.top();
        stack.pop();

        // Moving from right to left in the tree because that's what CAFE does
        auto it = c->_descendants.rbegin();
        for (; it != c->_descendants.rend(); ++it)
        {
            stack.push(*it);
        }
        f(c);
    }
}

clade* parse_newick(std::string newick_string) {

    std::regex tokenizer("\\(|\\)|[^\\s\\(\\)\\:\\;\\,]+|\\:[+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?|\\,|\\;");

    auto new_clade = [](clade* p_parent) {
        clade* p_new_clade = new clade();
        if (p_parent != NULL) {
            p_new_clade->_p_parent = p_parent;
        }

        return p_new_clade;
    };

    int lp_count(0), rp_count(0);
    std::sregex_iterator regex_it(newick_string.begin(), newick_string.end(), tokenizer);
    std::sregex_iterator regex_it_end;
    clade* p_root_clade = new_clade(NULL);
    
    clade* p_current_clade = p_root_clade; // current_clade starts as the root

    // The first element below is empty b/c I initialized it in the class body
    for (; regex_it != regex_it_end; regex_it++) {
        /* Checking all regex */
        // cout << regex_it->str() << "\n";

        /* Start new clade */
        if (regex_it->str() == "(") {
            /* Checking '(' regex */
            // cout << "Found (: " << regex_it->str() << "\n";

            p_current_clade = new_clade(p_current_clade); // move down the tree (towards the present)
            p_current_clade->_p_parent->add_descendant(p_current_clade); // can't forget to add the now current clade to its parent's descendants vector
            lp_count++;
        }

        else if (regex_it->str() == ",") {
            /* Checking ',' regex */
            // cout << "Found ,: " << regex_it->str() << "\n";

            /* The if block below is for when the newick notation omits the external parentheses, which is legal */
            if (p_current_clade == p_root_clade) {
                std::cout << "Found root!" << "\n";
                p_root_clade = new_clade(NULL);
                p_current_clade->_p_parent = p_root_clade; // note that get_parent() cannot be used here because get_parent() copies the pointer and it would be the copy that would be assigned p_root_clade... and then the copy would just be thrown away
                p_current_clade->_p_parent->add_descendant(p_current_clade);
            }

            /* Start new clade at same level as the current clade */
            p_current_clade = new_clade(p_current_clade->_p_parent); // move to the side of the tree
            p_current_clade->_p_parent->add_descendant(p_current_clade); // adding current clade as descendant of its parent
        }

        /* Finished current clade */
        else if (regex_it->str() == ")") {
            /* checking ')' regex */
            // cout << "Found ): " << regex_it->str() << "\n";

            p_current_clade = p_current_clade->_p_parent; // move up the tree (into the past)
            rp_count++;
        }

        /* Finished newick string */
        else if (regex_it->str() == ";") {
            /* Checking ';' regex */
            // cout << "Found ;: " << regex_it->str() << "\n";
            break;
        }

        /* Reading branch length */
        else if (regex_it->str()[0] == ':') {
            /* Checking ':' regex */
            // cout << "Found :: " << regex_it->str() << "\n";
            p_current_clade->_branch_length = atof(regex_it->str().substr(1).c_str()); // atof() converts string into float
            }

        /* Reading taxon name */
        else {
            /* Checking species name string regex */
            // cout << "Found species name: " << regex_it->str() << "\n";

            p_current_clade->_taxon_name = regex_it->str();
            clade* p_parent = p_current_clade->_p_parent;
            /* If this species has a parent, we need to update the parent's name */
            if (p_parent != NULL) {
                p_parent->_name_interior_clade(); // update parent's name, _name_interior_clade() is a void method
            }
        }
    }

    return p_root_clade;
}


void clade::insert_between(clade* child, double sptime) {

    double parent_height = this->distance_from_root_to_tip();
    double child_height = child->distance_from_root_to_tip();
    double new_clade_length = parent_height - sptime;
    this->remove_descendant(child); //remove child as descendant of parent 
    clade* c = new clade("c", new_clade_length); //create new clade from timeslice 
    this->add_descendant(c); // add new clade as descendant
    c->_p_parent = this;
    child->_branch_length = sptime - child_height;
    c->add_descendant(child); //add child back as descendant of new node 
    child->_p_parent = c;
    

}

bool clade::insert_between_once(double sptime) {

    for (auto it2 = this->_insert_reverse_level_order.begin(); it2 != this->_insert_reverse_level_order.end(); it2++) {
            
        clade* gt_parent = *it2;
        double parent_height = gt_parent->distance_from_root_to_tip();

        for (auto it3 = gt_parent->_descendants.begin(); it3 != gt_parent->_descendants.end(); it3++) {

            clade* gt_child = *it3;
            double child_height = gt_child->distance_from_root_to_tip(); 

            if ((parent_height > sptime) && (sptime > child_height)) {
                //std::cout << parent_height << " " << sptime << " " << child_height << std::endl;
                gt_parent->insert_between(gt_child, sptime);
                return false;
            }
        }
    }
    return true;
}

void clade::insert_all_between(clade* sptree, clade* genetree) { //function in progress 

    std::set<double> sptimes = sptree->get_speciation_times();
    //std::set<double> sptimes {0.6, 1.2}; //for testing

    for (auto it = sptimes.begin(); it != sptimes.end(); it++) {

        bool done = false;

        while (!done) {
            done = genetree->insert_between_once(*it);
        }
    }
}

void clade::insert_between_all_trees(clade* sptree, std::vector<clade*> genetrees) {

    for (auto it = genetrees.begin(); it != genetrees.end(); it++) {
        insert_all_between(sptree, *it);
    }
}

int count_nodes(clade* p_tree) { //debugging function to count nodes in a tree
    
    int node_counter = 0;

    for (auto it = p_tree->reverse_level_begin(); it != p_tree->reverse_level_end(); it++) {
        node_counter += 1;
    }

    return node_counter;
}

std::vector<int> count_nodes_all_trees(std::vector<clade*> p_trees) {

    std::vector<int> node_counts;

    for (auto it = p_trees.begin(); it != p_trees.end(); it++) {
        node_counts.push_back(count_nodes(*it));
    }

    return node_counts;
}


void print_parent_daughter_nodes(clade* genetree) {

    //Prints parent-daughter relationships for debugging time slicer

    for (auto it = genetree->reverse_level_begin(); it != genetree->reverse_level_end(); it++) {

        const clade* node = *it;

        if (!node->is_leaf()) {
            std::cout << "Parent node is " << node->get_taxon_name() << " (node height: " << node->distance_from_root_to_tip() << ") ";

            for (auto it2 = node->descendant_begin(); it2 != node->descendant_end(); it2++) {
                const clade* desc = *it2;
                std::cout << " Child node is " << desc->get_taxon_name() << " (node height: " << desc->distance_from_root_to_tip() << ") ";
            }

        }

        std::cout << "\n";
    }
}