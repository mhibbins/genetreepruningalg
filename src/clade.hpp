#ifndef clade_hpp
#define clade_hpp

#include <map>
#include <stack>
#include <queue>
#include <string>
#include <functional>
#include <set>
#include <vector>

/* Forward declaration of newick_parser class, so class clade can see friend */
class newick_parser; // actual declaration in utils.h

class clade;
using cladefunc = std::function<void(const clade*)>;

/*! \brief A Clade represents a node in a tree
*
*  In biology, a clade represents a group of organisms believed to have evolved from a common ancestor.
*  The Clade class has a parent clade, and a list of descendant clades. It can be loaded from a file
*  via the @newick_parser class.
*/
class clade {

    friend clade* parse_newick(std::string newick_string); // allows newick_parser to set parameter values
    //friend std::ostream& operator<<(std::ostream &out, clade* my_clade);

private:
    clade *_p_parent; // needs to be pointer; instance creates infinite loop
    std::string _taxon_name;
    double _branch_length; // or lambda value

    std::vector<clade*> _descendants; // same as above

    /* methods */
    void _name_interior_clade();

    std::vector<const clade*> _reverse_level_order;
    std::vector<clade*> _insert_reverse_level_order;
    void update_reverse_level_order();
public:
    typedef std::vector<const clade*>::const_iterator reverse_level_iterator;
    typedef std::vector<clade*>::const_iterator insert_reverse_level_iterator;
    typedef std::vector<clade*>::const_iterator descendant_iterator;
    
    
    /* methods */
    clade() : _p_parent(nullptr), _branch_length(0) {} // basic constructor

    //! constructor giving taxon name and branch length
    clade(std::string taxon_name, double length) : _taxon_name(taxon_name), _branch_length(length), _p_parent(nullptr) {}

    clade(const clade& c, clade *parent = nullptr, std::function<double(const clade& c)> branchlength_setter = nullptr);

    ~clade(); // destructor

    //! return the parent clade, NULL if there is none
    const clade* get_parent() const;

    //! Add the descendant clade. Used when constructing a tree
    void add_descendant(clade *p_descendant);
    void remove_descendant(clade *p_descendant);

    //!
    void add_leaf_names(std::vector<std::string>& vector_names);

    bool is_leaf() const;

    bool is_root() const;

    double get_branch_length() const;

    double distance_from_root_to_tip() const;

    //! returns descendant nodes of this clade that are not leaves
    std::vector<const clade*> find_internal_nodes() const;

    //! returns a descendant clade by the name
    const clade *find_descendant(std::string some_taxon_name) const;

    double find_branch_length(std::string some_taxon_name);

    std::string get_taxon_name() const { return _taxon_name; }

    void write_newick(std::ostream& ost, std::function<std::string(const clade *c)> textwriter) const;

    //! Return a unique list of all brnach lengths for this clade and its descendants
    std::vector<double> get_branch_lengths() const;
    std::set<double> get_speciation_times() const; //returns tip branch lengths

    //! apply the function f to direct descendants. Does not automatically recurse.
    void apply_to_descendants(const cladefunc& f) const;

    //! apply the function f to this clade and also to all descendants.
    void apply_prefix_order(const cladefunc& f) const;

    reverse_level_iterator reverse_level_begin() const {
        return _reverse_level_order.begin();
    }
    reverse_level_iterator reverse_level_end() const {
        return _reverse_level_order.end();
    }

    descendant_iterator descendant_begin() const {
        return _descendants.begin();
    }
    descendant_iterator descendant_end() const {
        return _descendants.end();
    }

    void insert_between(clade* child, double sptime); 
    bool insert_between_once(double sptimes);
    void insert_all_between(clade* sptree, clade* genetree);
    void insert_between_all_trees(clade* sptree, std::vector<clade*> genetrees);
};

template<typename T>
using clademap = std::map<const clade *, T>;

using cladevector = std::vector<const clade *>;

std::string clade_index_or_name(const clade* node, const cladevector& order);

clade* parse_newick(std::string newick_string);
std::set<double> get_branch_intervals(clade* sptree, std::vector<clade*> genetrees); //returns time intervals for matrix cache
int count_nodes(clade* p_tree);
std::vector<int> count_nodes_all_trees(std::vector<clade*> p_trees);
void print_parent_daughter_nodes(clade* genetree);
//std::map<double, std::vector<double>> get_parent_child_lengths(const clade* genetree);
//inline clade* parse_newick(std::string newick_string) { return parse_newick(newick_string); }

#endif