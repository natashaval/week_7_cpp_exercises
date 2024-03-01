#include <functional>
#include <ostream>
#include <stdexcept>
#include <vector>

using std::vector;

/**
 * @brief Table used for mapping key value pairs. 
 * Each row is indexed by a hash function on the key. Key-value pairs which map to the same row are stored as an unsorted list.
 * 
 * @tparam keyType 
 * @tparam valType 
 */
template<typename keyType, typename valType>
class Table
{
    struct kv_pair
    {
        keyType key;
        valType val;
    };

    public:
    Table(const int N) : size(N)
    {
        table = vector< vector<kv_pair> >(size);
    }

    // O(n) only looping over row for key placement (unsorted) + O(1) for hashing
    // Example: (k,v) mod 10 = (2,2); (12; 12); (3, 9); (3, 3)
    // Table[2] = [2, 12]
    // Table[3] = [9, 3]
    void insert(const keyType key, const valType val)
    {
        kv_pair kv{key, val};
        auto &row = table[index(key)];
        // Average case if all the rows are empty = O(1) hash + O(1) push_back
        if (row.empty())
        {
            row.push_back(kv);
        }
        // Worst case: if the mod key is already filled = O(1) hash + O(n) check through the loop
        else
        {
            for(auto &row_entry : row)
            {
                if(row_entry.key == key)
                {
                    row_entry.val = val;
                    return;
                }
            }
            row.push_back(kv);
        }
    }

    // O(1) for hashing in table + O(n) for value looping
    valType lookup(const keyType key)
    {
        // Best case O(1) if directly found the value
        const auto &row = table[index(key)];

        // Worse case O(n) because need to look up the vector inside table[key]
        for (auto &row_entry : row)
        {
            if (row_entry.key == key)
            {
                return row_entry.val;
            }
        }
        throw std::runtime_error("Key not found.");
    }

    friend std::ostream &operator<<(std::ostream &out, const Table &h)
    {
        for(int i = 0; i < h.size; i++)
        {
            const auto row = h.table[i];
            if(!row.empty())
            {
                out << i << ": ";
                for(const auto entry : row)
                {
                    out << "(" << entry.key << "," << entry.val << ") ";
                }
                out << std::endl;
            }
        }
        return out;
    }

    private:

    /**
     * @brief Constant time hash function modulo size of table
     * 
     * @param key 
     * @return int 
     */
    int index(keyType key)
    {
        return hash(key) % size;
    }

    std::hash<keyType> hash;
    const int size;
    vector< vector<kv_pair>> table;
};